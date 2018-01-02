#ifndef _PROTOCOL_FRM_H_
#define _PROTOCOL_FRM_H_

#include "protocol_info.h"

#define MAX_PROTOCOL 8
#include "tool_common.h"
#define FUN_NOT_PR 1
#define FUN_NOT 2

#define PROTOCOL_FRM_645 1
#define PROROCOL_FRM_188 2
#define PROROCOL_FRM_698 3

typedef int (*protocol_Decodefun)(HI_PBYTE buf,int nlen, MRS_FRAME_STRU *s);
typedef int (*protocol_Encodefun)(HI_PBYTE buf,int *nlen, MRS_FRAME_STRU *s);
typedef int (*protocol_IsReply)(MRS_FRAME_STRU *s);

typedef struct s
{
	int nProtocol;
	protocol_Decodefun pDecodefun;

	protocol_Encodefun pEncodefun;
}proFun;

class protocol
{
public:
	proFun profun[MAX_PROTOCOL];
	int nFunCount;

public:
	int protocolFmt(HI_PBYTE pDeBuf,int inLen,HI_PBYTE pEeBuf,int *outLen,MRS_FRAME_STRU *s,int nPro);
	int regProtocol(int nProtocol,protocol_Decodefun pDefun,protocol_Encodefun pEnfun);
};

#endif