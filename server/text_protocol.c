#include <string.h>

#include "stdinc.h"
#include "text_protocol.h"

/* 支持的命令个数 */
#define MAX_CMD_FUNC_DEFAULT  1
struct CmdFuncMap const_cmd_default[MAX_CMD_FUNC_DEFAULT] = {
    {"quit", NULL}                                  /* 5 quit|> */
};

/* 处理命令，注意会改变cmdstr的内容,参数最多为6个  */
s32 cmd_callback(s32 sockfd, u8 *cmdstr, struct CmdFuncMap *const_cmd, u32 max_cmd_num)
{
    s32 ret = ERROR_SUCCESS, i = 0;
    s8 *p = NULL, *cmdflag = NULL;
    u8 *argv[6] = {0};
    s32 argc = 0;

    /* 解析命令行参数 */
    p = strtok((s8*)cmdstr, "|");
    if(p)
    {
        cmdflag = strdup(p);
        for(i = 0; i < 6; i++)
        {
            p = strtok(NULL, "|");
            if(p)
            {
                if(strcmp(p, "> ") == 0)
                {
                    // 命令结束标识
                    argc = i;
                    break;
                }
                else
                {
                    argv[i] = (u8*)strdup(p);
                }
            }
            else
            {
                argc = i;
                break;
            }
        }
    }

    printf("argc(%d) cmd(%s): ", argc, cmdflag);
    for(i = 0; i < argc; i++)
    {
        printf("%s ", argv[i]);
    }
    printf("\n");

    do {
        if(strcmp(cmdflag, "quit") == 0)
        {
            ret = ERROR_QUIT;
            break;
        }
        
        for(i = 0; i < max_cmd_num; i++)
        {
            if(strcmp(cmdflag, const_cmd[i].cmdstr) == 0)
            {
                ret = const_cmd[i].func(sockfd, argc, argv);
                break;
            }
        }
    }while(0);

    if(i == max_cmd_num)
    {
        printf("unknown cmd\n");
    }

    free(cmdflag);
    for(i = 0; i < argc; i++)
    {
        if(argv[i])
        {
            free(argv[i]);
        }
    }

    return ret;
}