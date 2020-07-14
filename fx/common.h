/*
 * Common.h
 *
 * Created: 7/11/2020
 *  Author: Mohamed
 */ 

#ifndef __COMMON_H__
#define __COMMON_H__

#define MAX_BYTE 0xFF
#define VALUE_MASK          0b110000
#define PACK_VALUE(v)       (VALUE_MASK | v)
#define UNPACK_VALUE(v)     (v & ~VALUE_MASK)
#define CHECK_VALUE(v)      (v & VALUE_MASK)

/*
 * Comment to disable debugging code (as long as you are only using DBG_* macros)
 */
#define __DBG_ENABLED__ 1

#endif  //__COMMON_H__