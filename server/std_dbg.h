/*******************************************************************************

--------------------------------------------------------------------------------
                              std_dbg.h
  Project Code: Std
   Module Name: std_dbg
  Date Created: 2013-12-21
        Author: lixiaolong
   Description: This module provides debug function

--------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------------
  2013-12-21  lixiaolong       init version

*******************************************************************************/

#ifndef _STD_DBG_H_
#define _STD_DBG_H_

#ifdef  __cplusplus
extern "C"{
#endif


/*******************************************************************************
Header file body here
模块内部头文件不允许引用任何其他头文件!!
*******************************************************************************/
#include <stdio.h>

#define DEBUG_FILENAME "/tmp/std_main_dbg.log"

#define STD_DBG_PRINT(fmt,...)  \
do \
{ \
    FILE * ___fp = fopen(DEBUG_FILENAME,"a+"); \
    if(___fp) \
    { \
        fprintf(___fp, "[%s:%d:%s]  " fmt, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__); \
        fclose(___fp); \
    } \
}while(0)

#define DEBUG

#ifdef DEBUG
#define STD_DEBUG_PRINT STD_DBG_PRINT
#else
#define STD_DEBUG_PRINT 
#endif

#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */
#endif  /* end of _STD_DBG_H_ */
