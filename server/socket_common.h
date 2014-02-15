#ifndef __SOCKET_COMMON_H__
#define __SOCKET_COMMON_H__

#include "stdinc.h"

#define STD_SOCKET_FILE   "/tmp/manager.socket"
#define STD_LISTEN_IP     "127.0.0.1"
#define STD_LISTEN_PORT   "55554"

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((int)&((TYPE *)0)->MEMBER)
#endif

s32 std_listen_unix(const s8 *socketfile);
s32 std_connect_unix(const s8 *socketfile);
s32 std_connect_socket(u8 *ip, u8 *port);
u8* std_recv_allmsg(s32 fd);
s8  std_recv_msg(s32 fd, u8 **retbuf);
s32 std_send_cmd(s32 fd, u8 *cmd);
u8* std_exec_cmd(s32 fd, u8 *cmd);


#endif /* __SOCKET_COMMON_H__ */