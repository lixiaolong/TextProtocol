/*******************************************************************************
 
--------------------------------------------------------------------------------
                              text_protocol.h
  Project Code: text_protocol
   Module Name: text_protocol
  Date Created: 2014-2-14
        Author: lixiaolong
   Description: This module provides debug function

--------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------------
  2014-2-14   lixiaolong       init version

*******************************************************************************/

#ifndef _TEXT_PROTOCOL_H_
#define _TEXT_PROTOCOL_H_

#ifdef  __cplusplus
extern "C"{
#endif

/*******************************************************************************
Header file body here
模块内部头文件不允许引用任何其他头文件!!
*******************************************************************************/
typedef s32 (*CMD_CALLBACK_FUNC)(s32, s32, u8**);

struct CmdFuncMap {
    s8* cmdstr;
    CMD_CALLBACK_FUNC func;
};

s32 cmd_callback(s32 sockfd, u8 *cmdstr, struct CmdFuncMap *const_cmd, u32 max_cmd_num);

#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */
#endif  /* end of _TEXT_PROTOCOL_H_ */

