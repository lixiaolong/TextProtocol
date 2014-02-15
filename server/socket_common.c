#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "socket_common.h"
#include "string_inc.h"
#include "std_dbg.h"



/* listen to an unix domain socket */
s32 std_listen_unix(const s8 *socketfile)
{
    s32 fd, len, err;
    struct sockaddr_un serv;

    if((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        STD_DEBUG_PRINT("socket failed!");
        return ERROR_FAILED;
    }
    unlink(socketfile);

    len = strlen(socketfile);

    if(len > 107)
    {
        STD_DEBUG_PRINT("socket file len is too long!");
        return ERROR_FAILED;
    }

    memset(&serv, 0, sizeof(serv));
    serv.sun_family = AF_UNIX;
    strncpy(serv.sun_path, socketfile, len);

    len += offsetof(struct sockaddr_un, sun_path);

    if(bind(fd, (struct sockaddr *)&serv, len) < 0)
    {
        STD_DEBUG_PRINT("bind failed!");
        err = ERROR_FAILED;
        goto ERROROUT;
    }

    if(listen(fd, 10) < 0)
    {
        STD_DEBUG_PRINT("listen failed!");
        err = ERROR_FAILED;
        goto ERROROUT;
    }
    return (fd);

ERROROUT:
    close(fd);
    return err;
}


/* connect to an unix domain socket */
s32 std_connect_unix(const s8 *socketfile)
{
    s32 sock = 0, ret = 0, opt = 0;
    size_t len = 0;
    struct sockaddr_un srv_sock;
    if( (sock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0 )
    {
        STD_DEBUG_PRINT("socket failed\n");
        return ERROR_FAILED;
    }
    
    len = strlen(socketfile);
    memset(&srv_sock, 0, sizeof(struct sockaddr_un));
    srv_sock.sun_family = AF_UNIX;
    strncpy(srv_sock.sun_path, socketfile, len);
    
    len += offsetof(struct sockaddr_un, sun_path);
    
    if((ret = connect(sock, (struct sockaddr *)&srv_sock, len)) < 0 )
    {
        STD_DEBUG_PRINT("connect failed: %d\n", ret);
        close(sock);
        return ERROR_FAILED;
    }
    
    opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt));
    
    return sock;
}


/* connect to server */
s32 std_connect_socket(u8 *ip, u8 *port)
{
    int sockfd = 0, portnum = 0;
    struct sockaddr_in serv_addr;

    portnum = atoi((const s8*)port);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        STD_DEBUG_PRINT("socket failed!(%d)\n", sockfd);
        return ERROR_FAILED;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr((const s8*)ip);
    serv_addr.sin_port = htons(portnum);

    if(connect(sockfd, (void *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        STD_DEBUG_PRINT("connect failed!\n");
        close(sockfd);
        return ERROR_FAILED;
    }

    return sockfd;
}

/* send cmd */
s32 std_send_cmd(s32 fd, u8 *cmd)
{
    s32 sendlen = 0;
    
    if(fd <= 0)
    {
        STD_DEBUG_PRINT("socket fd error!(%d)\n", fd);
        return ERROR_FAILED;
    }

    sendlen = send(fd, cmd, strlen((const s8*)cmd), 0);
    if(sendlen <= 0)
    {
        STD_DEBUG_PRINT("send error!(%d)\n", fd);
        return ERROR_FAILED;
    }
    
    return ERROR_SUCCESS;
}

/* recv all msg utill ready to send  */
u8* std_recv_allmsg(s32 fd)
{
    s32 recvlen = 0, err = ERROR_FAILED;
    u8 recvbuff[MAX_BUFF_SIZE + 1] = {0};

    u8 *ret_buf = NULL;
    u32 ret_pos = 0, alloc_len = 0;

    NEW_STR(ret_buf, ret_pos, alloc_len);
    memset(ret_buf, 0, alloc_len);

#define ASD(_p3) ADD_STR_DATA(ret_buf, ret_pos, alloc_len, _p3)

    if(fd <= 0)
    {
        STD_DEBUG_PRINT("socket fd error!(%d)\n", fd);
        free(ret_buf);
        return NULL;
    }

    while( ( recvlen = recv(fd, recvbuff, MAX_BUFF_SIZE, 0) ) > 0 )
    {
        if (recvlen == 0) 
        {
            // recv nothing
            STD_DEBUG_PRINT("recv nothing\n");
            err = ERROR_FAILED;
            break;
        } 
        else if (recvlen > 0 && recvlen < MAX_BUFF_SIZE)
        {
            // recv finish
            recvbuff[recvlen] = '\0';
            if(recvbuff[recvlen - 1] == ' ' && recvbuff[recvlen - 2] == '>')
            {
                // recv finish and send ready
                STD_DEBUG_PRINT("recv finish and send ready\n");
                ASD(recvbuff);
                err = ERROR_SUCCESS;
                break;
            }
            else
            {
                // recv finish but may need recv again
                STD_DEBUG_PRINT("recv finish but may need recv again\n");
                STD_DEBUG_PRINT("%s=\n", recvbuff);
                ASD(recvbuff);
                continue;
            }
        }
        else if (recvlen == MAX_BUFF_SIZE)
        {
            // still have data
            recvbuff[MAX_BUFF_SIZE] = '\0';
            STD_DEBUG_PRINT("still have data\n");
            ASD(recvbuff);
            continue;
        }
        else
        {
            // error
            STD_DEBUG_PRINT("recv error!(%d)\n", recvlen);
            err = ERROR_FAILED;
            break;
        }
    }
    
    if(ERROR_SUCCESS == err)
    {
        //STD_DEBUG_PRINT("\n********************\n%s\n", ret_buf);
        return ret_buf;
    }
    else
    {
        STD_DEBUG_PRINT("recv message error!\n");
        free(ret_buf);
        return NULL;
    }   
}

/* recv a message  */
s8 std_recv_msg(s32 fd, u8 **retbuf)
{
    s32 recvlen = 0, err = 0;
    u8 recvbuff[MAX_BUFF_SIZE + 1] = {0};

    u8 *ret_buf = NULL;
    u32 ret_pos = 0, alloc_len = 0;

    NEW_STR(ret_buf, ret_pos, alloc_len);
    memset(ret_buf, 0, alloc_len);

#define ASD(_p3) ADD_STR_DATA(ret_buf, ret_pos, alloc_len, _p3)

    if(fd <= 0)
    {
        STD_DEBUG_PRINT("socket fd error!(%d)\n", fd);
        *retbuf = NULL;
        free(ret_buf);
        return -1;
    }

    while((recvlen = recv(fd, recvbuff, MAX_BUFF_SIZE, 0)))
    {
        if (recvlen == 0) 
        {
            // recv nothing
            STD_DEBUG_PRINT("recv nothing\n");
            err = ERROR_FAILED;
            break;
        } 
        else if (recvlen > 0 && recvlen < MAX_BUFF_SIZE)
        {
            // recv finish
            recvbuff[recvlen] = '\0';
            // recv finish and send ready
            STD_DEBUG_PRINT("recv finish\n");
            ASD(recvbuff);
            err = ERROR_SUCCESS;
            break;
        }
        else if (recvlen == MAX_BUFF_SIZE)
        {
            // still have data
            recvbuff[MAX_BUFF_SIZE] = '\0';
            STD_DEBUG_PRINT("still have data\n");
            ASD(recvbuff);
            continue;
        }
        else
        {
            // error
            STD_DEBUG_PRINT("recv error!(%d)\n", recvlen);
            err = ERROR_FAILED;
            break;
        }
    }
    
    if(ERROR_SUCCESS == err)
    {
        STD_DEBUG_PRINT("\n********************\n%s\n", ret_buf);
        *retbuf = ret_buf;
        return ret_pos;
    }
    else
    {
        free(ret_buf);
        *retbuf = NULL;
        return 0;
    }   
}

u8* std_exec_cmd(s32 fd, u8 *cmd)
{
    u32 ret = 0;
    u8 *recvdata = NULL;
    ret = std_send_cmd(fd, cmd);
    STD_DEBUG_PRINT("\nSEND:%s\n", cmd);
    if(ret != ERROR_SUCCESS)
    {
        return NULL;
    }
    recvdata = std_recv_allmsg(fd);
    STD_DEBUG_PRINT("\nRECV:%s\n", recvdata);
    return recvdata;
}