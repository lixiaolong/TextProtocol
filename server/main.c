#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#include "std_dbg.h"
#include "socket_common.h"
#include "text_protocol.h"

s32 start_task(s32 sockfd, s32 argc, u8** argv);
s32 stop_task(s32 sockfd, s32 argc, u8** argv);

/* 支持的命令个数 */
#define MAX_CMD_FUNC  3
struct CmdFuncMap const_cmd[MAX_CMD_FUNC] = {
    {"command.start_task", start_task},             /* 1 start_task|seqid|> */
    {"command.stop_task", stop_task},               /* 2 session_exec|session_id|session_uuid|cmdstr|> */
    {"quit", NULL}                                  /* 5 quit|> */
};

s32 start_task(s32 sockfd, s32 argc, u8** argv)
{
    int i = 0;
    
    printf("start_task\n");
    for (i = 0; i < argc; i++)
    {
        printf("%s ", argv[i]);
    }
    printf("\n");

    return 0;
}

s32 stop_task(s32 sockfd, s32 argc, u8** argv)
{
    int i = 0;
    
    printf("stop_task\n");
    for (i = 0; i < argc; i++)
    {
        printf("%s ", argv[i]);
    }
    printf("\n");

    return 0;
}

int main()
{
    int listenfd = -1;
    s32 cmdret = 0;
    fd_set curset;
    struct timeval timeout;
    
    /* 监听unix socket */
    listenfd = std_listen_unix((const s8*)STD_SOCKET_FILE);
    if(listenfd == -1)
    {
        STD_DEBUG_PRINT("listen unix socket failed!\n");
        return -1;
    }
    printf("%d\n", listenfd);

    

    while(1)
    {
        timeout.tv_sec = 0;
        timeout.tv_usec = 500000;

        FD_ZERO(&curset);
        FD_SET(listenfd, &curset);

        cmdret = select(listenfd + 1, &curset, NULL, NULL, &timeout);
        if(cmdret == 0) 
        {
            /* time out */
            continue;
        } 
        else if (cmdret < 0) 
        {
            /* error */
            break;
        } 
        else 
        {
            if(FD_ISSET(listenfd, &curset)) 
            {
                /* 监听的socket上有数据过来 */
                int acceptfd = -1;
                u8* recvdata = NULL;
                
                acceptfd = accept(listenfd, NULL, NULL);
                if(acceptfd > 0)
                {
                    cmdret = std_recv_msg(acceptfd, &recvdata);
                    printf("%s\n", recvdata);
                    
                    std_send_cmd(acceptfd, (u8*)"1> ");
                    cmdret = cmd_callback(acceptfd, recvdata, const_cmd, MAX_CMD_FUNC);
                    
                    free(recvdata);
                    close(acceptfd);
                    
                    if(ERROR_QUIT == cmdret)
                    {
                        // quit
                        break;
                    }
                }
                else
                {
                    STD_DEBUG_PRINT("accept failed!\n");
                }
            }
        }
    }


    close(listenfd);
    
    return 0;
}

