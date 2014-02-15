#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#include "std_dbg.h"
#include "socket_common.h"

int main(int argc, char **argv)
{
    int acceptfd = -1;
    u8* recvdata = NULL;
    s32 cmdret = 0;

    acceptfd = std_connect_unix((const s8*)STD_SOCKET_FILE);
    if(acceptfd == -1)
    {
        STD_DEBUG_PRINT("listen unix socket failed!\n");
        return -1;
    }
    printf("%d\n", acceptfd);

    if (2 == argc)
    {
        std_send_cmd(acceptfd, argv[1]);
    }
    else
    {
        std_send_cmd(acceptfd, "test");
    }
    

    cmdret = std_recv_msg(acceptfd, &recvdata);

    printf("%s\n", recvdata);

    close(acceptfd);
    
    return 0;
}

