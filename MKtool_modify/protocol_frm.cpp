#include "protocol_frm.h"
#include "protocol_info.h"
#include "stdafx.h"
#include "tool_common.h"
#include "protocol_info.h"
#include "MKtoolDlg.h"

int  protocol::protocolFmt(HI_PBYTE pDeBuf,int inLen,HI_PBYTE pEeBuf,int *outLen,MRS_FRAME_STRU *s,int nPro)
{
	int ret = FUN_NOT_PR;
	int i;
	protocol_Decodefun pDefun = NULL;
	protocol_Encodefun pEnfun = NULL;
	for (i=0;i<MAX_PROTOCOL; i++)
	{
		if ((profun[i].nProtocol&nPro) == 0)
		{
			continue;
		}
		pDefun = profun[i].pDecodefun;
		if (!pDefun)
		{
			return FUN_NOT;
		}
		ret = pDefun(pDeBuf,inLen,s);
		if (ret==HI_ERR_ADDRESS_NOT_MATCH)// 协议解析正确，只是地址不必配
		{
			break;
		}
		if (ret)
		{
			continue;
		}
		
		pEnfun = profun[i].pEncodefun;
		if (pEnfun!=NULL)
		{
			ret = pEnfun(pEeBuf,outLen,s);
		}
		break;
		
	}
	return ret;
}


int protocol::regProtocol(int nProtocol,protocol_Decodefun pDefun,protocol_Encodefun pEnfun)
{
	int i=0;
	for (i=0;i<MAX_PROTOCOL;i++)
	{
		if (profun[i].pDecodefun==NULL)
		{
			profun[i].pDecodefun = pDefun;
			profun[i].pEncodefun = pEnfun;
			profun[i].nProtocol = nProtocol;
			break;
		}
	}
	return (i<MAX_PROTOCOL)?TRUE:FALSE;
}