/*----------------------------------------------------------------------------
 * Copyright(c)  :  NPC CORPORTAION All Rights Reserved                       
 * FileName      :  NPC_TypeDef.h
 * Version       :  1.1
 * Author        :  CCH
 * DateTime      :  2010-05-06
 * Description   :  NPC system data type definition
 *----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 |  Define header file
 *----------------------------------------------------------------------------*/
#ifndef __NPC_TYPEDEF_H
#define __NPC_TYPEDEF_H

#ifdef _WIN32
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
#include <winsock2.h>
#include <windows.h>
#include "t_sprintf.h"
#else

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <netdb.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "t_sprintf.h"
#ifdef __APPLE__
#include <sys/sysctl.h>
#include <net/if_dl.h>
#else
#include <net/if_arp.h>
#endif

#endif

#define NPC_SOCKET_DATA_AUDIO_TIME      1   //Timestamp of sent audio data
#define NPC_SOCKET_DATA_AUDIO_TIMEOUT   5   //Timeout of send audio data 

#define NPC_SYSTEM_VERSION          "2.7.6"

//-------------------------------------------------------------------------------
#define NPC_DEBUG

#define NPC_TRUE			1
#define NPC_FALSE			0

#ifndef TRUE
#define TRUE				NPC_TRUE
#endif

#ifndef FALSE
#define FALSE				NPC_FALSE
#endif

#define NPC_NULL			0

#ifndef NULL
#define NULL				NPC_NULL
#endif

typedef	unsigned int		NPC_BOOL, *PNPC_BOOL;
typedef	unsigned char		NPC_BOOLEAN, *PNPC_BOOLEAN;
typedef char				NPC_CHAR, *PNPC_CHAR;
typedef unsigned char		NPC_BYTE, *PNPC_BYTE;
typedef short 				NPC_SHORT, *PNPC_SHORT;
typedef unsigned short 		NPC_USHORT, *PNPC_USHORT;
typedef long 				NPC_LONG, *PNPC_LONG;
typedef unsigned long 		NPC_ULONG, *PNPC_ULONG;
typedef int					NPC_INT, *PNPC_INT;
typedef unsigned int		NPC_UINT, *PNPC_UINT;
#ifdef WIN32
typedef __int64				NPC_LONGLONG, *PNPC_LONGLONG;
#else
typedef unsigned long long	NPC_LONGLONG, *PNPC_LONGLONG;
#endif
typedef float				NPC_FLOAT, *PNPC_FLOAT;
typedef double				NPC_DOUBLE, *PNPC_DOUBLE;

typedef NPC_USHORT			NPC_WORD, *PNPC_WORD;
typedef NPC_UINT			NPC_DWORD, *PNPC_DWORD;

#define NPC_VOID			void
#define PNPC_VOID			void*

typedef char				NPC_INT8, *PNPC_INT8;
typedef short 				NPC_INT16, *PNPC_INT16;
typedef int					NPC_INT32, *PNPC_INT32;
#ifdef WIN32
typedef __int64				NPC_INT64, *PNPC_INT64;
#else
typedef unsigned long long	NPC_INT64, *PNPC_INT64;
#endif
typedef unsigned char		NPC_UINT8, *PNPC_UINT8;
typedef unsigned short 		NPC_UINT16, *PNPC_UINT16;
typedef unsigned int		NPC_UINT32, *PNPC_UINT32;

typedef PNPC_VOID 			NPC_HANDLE, *PNPC_HANDLE;

#ifndef HANDLE
typedef PNPC_VOID 			HANDLE;
#endif

#ifndef NPC_HWND
typedef PNPC_VOID 			NPC_HWND;
#endif

#ifdef WIN32
typedef time_t 				NPC_TIME_T;
#else
typedef time_t 				NPC_TIME_T;
#endif

typedef NPC_INT8			NPC_S1, *PNPC_S1;
typedef NPC_INT16			NPC_S2, *PNPC_S2;
typedef NPC_INT32			NPC_S4, *PNPC_S4;

typedef NPC_UINT8			NPC_U1, *PNPC_U1;
typedef NPC_UINT16			NPC_U2, *PNPC_U2;
typedef NPC_UINT32			NPC_U4, *PNPC_U4;

typedef NPC_INT8			NPC_S8, *PNPC_S8;
typedef NPC_INT16			NPC_S16, *PNPC_S16;
typedef NPC_INT32			NPC_S32, *PNPC_S32;

typedef NPC_UINT8			NPC_U8, *PNPC_U8;
typedef NPC_UINT16			NPC_U16, *PNPC_U16;
typedef NPC_UINT32			NPC_U32, *PNPC_U32;

typedef NPC_UINT32			NPC_RESULT;

#ifdef _WIN32
typedef SOCKET				NPC_SOCKET;
#else
typedef NPC_INT				NPC_SOCKET;
#define MAX_PATH			256
#endif

/**********************************************************************************/
/* Swap bytes of a 16 bits data type (Constant) */
#define NPC_D_SWAP_BYTES_16BITS(_16X)		((((_16X) & 0x00FF) << 8) | (((_16X) & 0xFF00) >> 8))

/* Swap bytes of a 32 bits data type (Constant) */
#define NPC_D_SWAP_BYTES_32BITS(_32X)	\
  ((((_32X) & 0x000000FF) << 24) |		\
   (((_32X) & 0x0000FF00) << 8) |		\
   (((_32X) & 0x00FF0000) >> 8) |		\
   (((_32X) & 0xFF000000) >> 24))

/**********************************************************************************/
#ifdef WIN32

/* In Win32, the bottom is first, the high is last, and the high of the CLASSFILE is first, and the bottom is later, you need to convert */
#define NPC_D_ADDDATA_TO_UINT16(_ADDR) (NPC_D_SWAP_BYTES_16BITS(*(NPC_UINT16*)_ADDR))
#define NPC_D_ADDDATA_TO_UINT32(_ADDR) (NPC_D_SWAP_BYTES_32BITS(*(NPC_UINT32*)_ADDR))

#else

#define NPC_D_ADDDATA_TO_UINT16(_ADDR) (NPC_D_SWAP_BYTES_16BITS(*(NPC_UINT16*)_ADDR))
#define NPC_D_ADDDATA_TO_UINT32(_ADDR) (NPC_D_SWAP_BYTES_32BITS(*(NPC_UINT32*)_ADDR))

#endif


/* Function input and output parameter modifiers */
#define		IN
#define		OUT
#define		INOUT

/* Process after malloc function fails */
#define NPC_D_MALLOC_FAIL_EXCEPTION(_RET) \
{\
	if ( (_RET) == NULL ){\
		exit(0);\
	}\
}


#ifdef _WIN32
	#ifdef _NPCLIB_EXPORTS
		#define NPC_API_INTER __declspec(dllexport)
	#else
		#ifdef _NPCLIB_IMPORT
			#define NPC_API_INTER __declspec(dllimport)
		#else
			#define NPC_API_INTER
		#endif
	#endif
#else
	#define NPC_API_INTER
#endif


#define _NPC_DEBUG


#define NPC_D_SMP_COM_TIMEOUT   30000

//Common constant definitions
#define NPC_D_FC_IPADDR_BUFSIZE									40			//IP address buffer size
#define NPC_D_FC_USERID_BUFSIZE									64			//User ID buffer size
#define NPC_D_FC_USERNAME_BUFSIZE								64			//User name buffer size
#define NPC_D_FC_FILENAME_BUFSIZE								128			//File name buffer size
#define NPC_D_FC_DOMAIN_IPADDR_BUFSIZE							128			//Domain or IP address buffer size


//-------------------------------------------------------------------------------------
//Date time definition
typedef struct _NPC_S_TD_DATE_TIME
{
	NPC_USHORT									usYear;						//year
	NPC_USHORT									usMonth;					//month
	NPC_BYTE									ucDay;						//day
	NPC_BYTE									ucHour;						//hour
	NPC_BYTE									ucMinute;					//minute
	NPC_BYTE									ucSecond;					//second
	NPC_DWORD									dwMinsecond;				//millisecond

} NPC_S_TD_DATE_TIME, *PNPC_S_TD_DATE_TIME;

//Date definition
typedef struct _NPC_S_TD_DATE
{
	NPC_USHORT									usYear;						//year
	NPC_BYTE									ucMonth;					//month
	NPC_BYTE									ucDay;						//day

} NPC_S_TD_DATE, *PNPC_S_TD_DATE;

//Time definition
typedef struct _NPC_S_TD_TIME
{
	NPC_BYTE									ucHour;						//hour
	NPC_BYTE									ucMin;						//minute
	NPC_BYTE									ucSec;						//second
	NPC_BYTE									ucReserve;					//backup

} NPC_S_TD_TIME, *PNPC_S_TD_TIME;



//--------------------------------------------------------------------------------------
#endif

