#include "protocol_frm.h"
#include "stdafx.h"

int  protocol::protocolFmt(BYTE buf[],int nlen,void *s)
{
	int ret = FUN_NOT_PR;
	int i;
	protocol_fun pfun = NULL;
	for (i=0;i<MAX_PROTOCOL; i++)
	{
		pfun = profun[i].pfun;
		if (!pfun)
		{
			return FUN_NOT;
		}
		ret = pfun(buf,nlen,s);
		if (!ret)
		{
			break;
		}
	}
	return ret;
}

int protocol::regProtocol(int nProtocol,protocol_fun pfun)
{
	int i=0;
	for (i=0;i<MAX_PROTOCOL;i++)
	{
		if (profun[i].pfun==NULL)
		{
			profun[i].pfun = pfun;
			break;
		}
	}
	return (i<MAX_PROTOCOL)?TRUE:FALSE;
}