#ifndef __STRING_INC_H__
#define __STRING_INC_H__

#include <stdio.h>
#include <string.h>


#define NEW_STR(_buf_ptr, _buf_len, _alloc_len) do { \
    (_buf_ptr) = malloc(1024); \
    (_buf_len) = 0; \
    (_alloc_len) = 1024; \
  } while (0)


#define ADD_STR_DATA(_buf_ptr, _buf_len, _alloc_len, _str) do { \
    u32 _sl = strlen((char*)_str); \
    ADD_DATA(_buf_ptr, _buf_len, _alloc_len, _str, _sl);\
  } while (0)


#define ADD_DATA(_buf_ptr, _buf_len, _alloc_len, _data, _sl) do { \
    if ((_buf_len) + (_sl) + 1 > (_alloc_len)) { \
      u32 _nsiz = ((_buf_len) + (_sl) + 1024) >> 10 << 10; \
      (_buf_ptr)  = realloc(_buf_ptr, _nsiz); \
      (_alloc_len) = _nsiz; \
    } \
    memcpy((_buf_ptr) + (_buf_len), (_data), (_sl) + 1); \
    (_buf_len) += (_sl); \
  }while(0)


#endif /* __STRING_INC_H__ */