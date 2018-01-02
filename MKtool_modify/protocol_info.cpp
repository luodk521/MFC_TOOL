
#include "stdafx.h"
#include <string.h>

#include "protocol_info.h"
#define PROTOCOL_HEAD_BYTE 0x68
#define PROTOCOL_TAIL_BYTE 0x16

MeterArray sMeterArray;
UINT32 n64507SmDI;
UINT32 n64597SmDI;


MRS_IRDA_DI_PAIR_STRU PROTOCOL_64507_11H_DI_PAIR_MAP[] =
{
	{n64507SmDI,n64507SmDI,mrsIr645_11H_07_SM}, // 设置n64507SmDI时会重新设置
	{0x38393334,0x38393334,mrsIr645_11H_DI38393334}, // 日冻结时间YYMMDDHHmm
	{0x38393434,0x38393434,mrsIr645_11H_DI38393434}, // 日冻结正向有功数据 4*N
	{0x33343333,0x33343333,mrsIr645_11H_DI33343333}, // 组合有功功率1电能
	{0x33343233,0x33343233,mrsIr645_11H_DI33343233}, // 正向有功
	{0x37333435,0x37333435,mrsIr645_11H_DI37333435}, // 获取时间
	{0x37333434,0x37333434,mrsIr645_11H_DI37333434}, // 获取日期星期
	{0x37334536,0x37334536,mrsIr645_11H_DI37334536}, // 日冻结时间
	{0x35343433,0x35343433,mrsIr645_11H_DI35343433}, // 查询A相电压
	{0x35353433,0x35353433,mrsIr645_11H_DI35353433}, // 查询A相电流
	{0x37333834,0x37333834,mrsIr645_11H_DI37333834}, // 查询电池电压运行情况
	{0x37334834,0x37334834,mrsIr645_11H_DI37334834}, //  事件总状态
	{0x36443333,0x36443333,mrsIr645_11H_DI36443333}, //事件 掉电总次数
	{0x36634033,0x36634033,mrsIr645_11H_DI36634033}, // 查询事件3 开盖总次数
	{0x36633433,0x36633433,mrsIr645_11H_DI36633433}, // 查询事件4 电表清零总次数

	{0x4C343334,0x4C343334,mrsIr645_11H_DI4C343334}, // 查询事件2 A 相过流总次数
	{0x4C353334,0x4C353334,mrsIr645_11H_DI4C353334}, // 查询事件2 A 相过流总次数
	{0x4C363334,0x4C363334,mrsIr645_11H_DI4C363334}, // 查询事件2 A 相过流总次数

	{0x33000000,0x33FFFFFF,mrsIr645_11H_DI33FFFFFF},  // 电能数据标识 ,回复数据是4字节
	{0x34000000,0x34FFFFFF,mrsIr645_11H_DI34FFFFFF},  // 最大需量及发生时间数据标识,回复数据都是8字节
	{0,0xFFFFFFFF,mrs64507_All_DenyFrame}, // 回复否认帧
};

MRS_IRDA_DI_PAIR_STRU PROTOCOL_64597_11H_DI_PAIR_MAP[] =
{
	{n64597SmDI,n64597SmDI,mrsIr645_11H_97_SM},
	{0xC343,0xC3CC,mrsIr645_11H_97_C343},
	
	{0xD343,0xD3CC,mrsIr645_11H_97_D343},
	{0x00,0xFFFF,mrs64507_All_DenyFrame},

};

MRS_IRDA_DI_PAIR_STRU PROTOCOL_64507_12H_DI_PAIR_MAP[]=
{
	{0,0xFFFFFFFF,mrsIr645_12H_ALL},
};

MRS_IRDA_DI_PAIR_STRU PROTOCOL_64507_13H_DI_PAIR_MAP[] =
{
	{0,0x0,mrsIr645_13H_00},
	{0x3E3D3E3D,0x3E3D3E3D,mrsIr645_13H_3E3D3E3D},//I采 启动搜表
	{0x3E3F3E3F,0x3E3F3E3F,mrsIr645_13H_3E3F3E3F},// I采 停止搜表
	{0x3E403E40,0x3E403E40,mrsIr645_13H_3E403E40},// 读取搜表结果
	{0x3E3D3F33,0x3E3D3F33,mrsIr645_13H_3E3D3F33},// 确认版本
};

MRS_IRDA_DI_PAIR_STRU PROTOCOL_64507_1EH_DI_PAIR_MAP[] =
{
	{0x03,0x03,mrsIr645_1EH_03},//I采 启动搜表
	
};

MRS_IRDA_DI_PAIR_STRU PROTOCOL_188_DI_PAIR_MAP[] =
{
	{0,0xFFFFFFFF,mrs188_All_DItest},
};

MeterArray *getMeterArray(void)
{
	return &sMeterArray;
}

UINT32 get64507SmDI(void)
{
	return n64507SmDI;
}
UINT32 get64597SmDI(void)
{
	return n64597SmDI;
}

void set64507SmDI(UINT32 n)
{
	n64507SmDI = n;
	PROTOCOL_64507_11H_DI_PAIR_MAP[0].ulMinDI = n64507SmDI;
	PROTOCOL_64507_11H_DI_PAIR_MAP[0].ulMaxDI = n64507SmDI;
}
void set64597SmDI(UINT32 n)
{
	n64597SmDI= n;
	PROTOCOL_64597_11H_DI_PAIR_MAP[0].ulMinDI = n64597SmDI;
	PROTOCOL_64597_11H_DI_PAIR_MAP[0].ulMaxDI = n64597SmDI;
}

void RefreshMeter(void)
{
	int i,n,num = sMeterArray.numMeter;
	n=0;
	for (i=0;i<num;i++)
	{
		if (sMeterArray.sMeter[i].nValid == 1)
		{
			if (n!=i)
			{
				memcpy(&(sMeterArray.sMeter[n]),&(sMeterArray.sMeter[i]),sizeof(Meter));
			}
			n++;
		}
	}
	sMeterArray.numMeter  = n;
}
ProDIData nProDIData;
ProDIData *getProDIData(void)
{
	return &nProDIData;
}

HI_PBYTE StreamFindProtocolHeadByte(HI_PBYTE ptr,HI_U16 len,HI_U8 v)
{
    HI_U16 i = 0;
    
    if (ptr != HI_NULL)
    {
        for (i = 0; i < len; i++)
        {
            if (ptr[i] == v)
            {
                return ptr + i;
            }
        }
    }
	
    return HI_NULL;    
}

HI_U16 CalcCheckSum(HI_PBYTE ptr,HI_U16 len)
{
    HI_U16 CheckSum = 0;
	
    if ((ptr != HI_NULL) && (len > 0))  
    {
        HI_U16 i = 0;
        for(i = 0; i < len; i++)
        {
            CheckSum += ptr[i];
        }
    }
    return CheckSum;
}

HI_U32 Check698Frame(HI_PBYTE ptr,HI_U16 len)
{
    HI_U16 checkSum, HcheckSum;
    HI_U8 Addr = 0; 
    HI_U8 offset = 0, uclen = 0;  
	
    //帧长检测
    if ((!ptr) || (len < PROTO698_HEADER_LEN_MAX))
    {
        return HI_ERR_BAD_DATA;
    }
    
    //帧标志检查
    if ((ptr[0] != PROTOCOL_HEAD_BYTE) || (ptr[len-1] != PROTOCOL_TAIL_BYTE)) 
    {
        return HI_ERR_BAD_DATA;
    }
    memcpy(&Addr, ptr + 4, sizeof(HI_U8));
    uclen = (Addr & 0x0F) + 1;
    offset = uclen + 6;//1byte head, 2bytes len, 1byte ctrl, 1byte addrinfo, 1byte srcaddr 
	
    HcheckSum = CalcFcs16Ex(ptr + 1, offset - 1); //find Hcs, u do not need head.
    if (memcmp(&HcheckSum, ptr + offset, sizeof(HI_U16)))
    {
        return HI_ERR_BAD_DATA;
    }
	
    //校验和检查
    checkSum = CalcFcs16Ex(ptr + 1,(HI_U16)(len-4));// 2 bytes CS, 1byte head, 1byte tail.
    if (memcmp(&checkSum, ptr + len - 3, sizeof(HI_U16)))
    {
        return HI_ERR_BAD_DATA;
    }
    
    return HI_ERR_SUCCESS;
}

HI_U32 Check188Frame(HI_PBYTE ptr,HI_U16 len)
{
    HI_U8 checkSum;
	
    //帧长检测
    if ((!ptr) || 
        (len > PROTO188_DATAGRAM_LEN_MAX) || 
        (len < PROTO188_DATAGRAM_HEADER_LEN_MAX))
    {
        return HI_ERR_BAD_DATA;
    }
    
    //帧标志检查
    if ((ptr[0] != PROTOCOL_HEAD_BYTE) || (ptr[len-1] != PROTOCOL_TAIL_BYTE)) 
    {
        return HI_ERR_BAD_DATA;
    }
	//校验和检查
    checkSum = (HI_U8)CalcCheckSum(ptr,(HI_U16)(len-2));
    if (checkSum != ptr[len - 2])
    {
        return HI_ERR_BAD_DATA;
    }
    
    return HI_ERR_SUCCESS;
}

HI_U32 Check645Frame(HI_PBYTE ptr,HI_U16 len)
{
    HI_U8 checkSum;
	
    //帧长检测
    if ((!ptr) || 
        (len > PROTO645_DATAGRAM_LEN_MAX) || 
        (len < PROTO645_DATAGRAM_HEADER_LEN_MAX))
    {
        return HI_ERR_BAD_DATA;
    }
    
    //帧标志检查
    if ((ptr[0] != PROTOCOL_HEAD_BYTE) || (ptr[7] != PROTOCOL_HEAD_BYTE) || (ptr[len-1] != PROTOCOL_TAIL_BYTE)) 
    {
        return HI_ERR_BAD_DATA;
    }
    
    //校验和检查
    checkSum = (HI_U8)CalcCheckSum(ptr,(HI_U16)(len-2));
    if (checkSum != ptr[len - 2])
    {
        return HI_ERR_BAD_DATA;
    }
    
    return HI_ERR_SUCCESS;
}
/*
* FCS lookup table as calculated by the table generator.
*/
static HI_U16 fcstab[256]={
	0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
		0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
		0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
		0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
		0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
		0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
		0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
		0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
		0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
		0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
		0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
		0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
		0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
		0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
		0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
		0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
		0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
		0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
		0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
		0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
		0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
		0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
		0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
		0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
		0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
		0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
		0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
		0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
		0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
		0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
		0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
		0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};
#define PPPINITFCS16 0xffff /* Initial FCS value */
#define PPPGOODFCS16 0xf0b8 /* Good final FCS value */
/*
* Calculate a new fcs given the current fcs and the new data.
*/
HI_U16 CalcFcs16(HI_U8 *cp, HI_U16 len)
{
    HI_U8 *p = HI_NULL;
    HI_U16 fcs = PPPINITFCS16;
    p = cp;
    while (len--)
        fcs=(fcs >> 8) ^ fcstab[(fcs ^ *(p++)) & 0xff];
    return (fcs);
}

HI_U16 CalcFcs16Ex(HI_U8 *cp, HI_U16 len)
{
	HI_U16 CS = CalcFcs16(cp,len);
	CS ^= 0xFFFF;
	return CS;
}

HI_U32 Find645Frame(HI_IN HI_PBYTE in_buf, HI_IN HI_S16 in_len, 
					HI_OUT HI_U16 *start, HI_OUT HI_U16 *out_len)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_PBYTE buf = in_buf;
	
    if((!in_buf) || (!start) || (!out_len))
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    
    while (in_len > 0)
    {
        HI_PBYTE head = HI_NULL;
        head = (HI_PBYTE)StreamFindProtocolHeadByte(buf,(HI_U16)in_len,PROTOCOL_HEAD_BYTE);
        
        if (head != HI_NULL) 
        {//找到帧
            HI_U16 frame_len = 0;
            
            in_len -= (HI_S16)(head - buf);
            buf = head;
            
            if (in_len < 12) 
            {
                ret = HI_ERR_BAD_DATA;
                break;
            }
			
            frame_len = (HI_U16)(buf[9] + 12);
			
            //超长帧
            if (frame_len > PROTO645_DATAGRAM_LEN_MAX) 
            {
                buf++;
                in_len--;
                continue;
            }
			
            if(in_len >= frame_len)
            {
                if(HI_ERR_SUCCESS == Check645Frame(buf,frame_len))
                {
                    *start = (HI_U16)(buf - in_buf);
                    *out_len = frame_len;
					
                    //mrsProtoPrint0(MRS_LOG_INFO,"找到了一个完整645帧!");
                    ret = HI_ERR_SUCCESS;
                    break;
                }
                else
                {
                    buf++;
                    in_len--;
                    continue;
                }
            }
            else
            {
                buf++;
                in_len--;
                continue;
            }
			
        }
        else 
        {
            ret = HI_ERR_BAD_DATA;
            break;
        }
    }
	
    return ret;
}

HI_U32 Find188Frame(HI_IN HI_PBYTE in_buf, HI_IN HI_S16 in_len, 
					HI_OUT HI_U16 *start, HI_OUT HI_U16 *out_len)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_PBYTE buf = in_buf;
	
    if((!in_buf) || (!start) || (!out_len))
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    
    while (in_len > 0)
    {
        HI_PBYTE head = HI_NULL;
        head = (HI_PBYTE)StreamFindProtocolHeadByte(buf,(HI_U16)in_len,PROTOCOL_HEAD_BYTE);
        
        if (head != HI_NULL) 
        {//找到帧
            HI_U16 frame_len = 0;
            
            in_len -= (HI_S16)(head - buf);
            buf = head;
            
            if (in_len < 13) //cjt188 min is 13.
            {
                ret = HI_ERR_BAD_DATA;
                break;
            }
			
            frame_len = (HI_U16)(buf[10] + 13);
			
            //超长帧
            if (frame_len > PROTO188_DATAGRAM_LEN_MAX) 
            {
                buf++;
                in_len--;
                continue;
            }
			
            if(in_len >= frame_len)
            {
                if(HI_ERR_SUCCESS == Check188Frame(buf,frame_len))
                {
                    *start = (HI_U16)(buf - in_buf);
                    *out_len = frame_len;
				    ret = HI_ERR_SUCCESS;
                    break;
                }
                else
                {
                    buf++;
                    in_len--;
                    continue;
                }
            }
            else
            {
                buf++;
                in_len--;
                continue;
            }
			
        }
        else 
        {
            ret = HI_ERR_BAD_DATA;
            break;
        }
    }
	
    return ret;
}

HI_U32 Find698Frame(HI_IN HI_PBYTE in_buf, HI_IN HI_S16 in_len, HI_OUT HI_U16 *start, HI_OUT HI_U16 *out_len)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_PBYTE buf = in_buf;
	
    if((!in_buf) || (!start) || (!out_len))
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    
    while (in_len > 0)
    {
        HI_PBYTE head = HI_NULL;
        head = (HI_PBYTE)StreamFindProtocolHeadByte(buf,(HI_U16)in_len,PROTOCOL_HEAD_BYTE);
        
        if (head != HI_NULL) 
        {//找到帧
            HI_U16 frame_len = 0;
            
            in_len -= (HI_S16)(head - buf);
            buf = head;
            
            if (in_len < PROTO698_HEADER_LEN_MAX) // 1byte head, 2bytes len, 1byte Control, 1byte srcaddr, 1byte dstaddr, 2byte Hcs, 2byte Cs, 1byte tail 
            {
                ret = HI_ERR_BAD_DATA;
                break;
            }
			
            memcpy(&frame_len,buf + 1, sizeof(HI_U16));
            frame_len &= 0x3FFF;// len without head, and tail
            frame_len += 2; // now we get all.
			
            //超长帧
            if (frame_len > PROTO698_LEN_MAX) 
            {
                buf++;
                in_len--;
                continue;
            }
			
            if(in_len >= frame_len)
            {
                if(HI_ERR_SUCCESS == Check698Frame(buf,frame_len))
                {
                    *start = (HI_U16)(buf - in_buf);
                    *out_len = frame_len;
					
                    //mrsProtoPrint0(MRS_LOG_INFO,"找到了一个完整698帧!");
                    ret = HI_ERR_SUCCESS;
                    break;
                }
                else
                {
                    buf++;
                    in_len--;
                    continue;
                }
            }
            else
            {
                buf++;
                in_len--;
                continue;
            }
			
        }
        else 
        {
            ret = HI_ERR_BAD_DATA;
            break;
        }
    }
	
    return ret;
}
HI_U32 mrsFindWireLessProFrame(HI_IN HI_PBYTE in_buf, HI_IN HI_S16 in_len, 
							   HI_OUT HI_U16 *start, HI_OUT HI_U16 *out_len)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_PBYTE buf = in_buf;
	
    if((!in_buf) || (!start) || (!out_len))
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    while (in_len > 0)
    {
        HI_PBYTE head = HI_NULL;
        head = (HI_PBYTE)StreamFindProtocolHeadByte(buf,(HI_U16)in_len,0x68);
        
        if (head != HI_NULL) 
        {//找到帧
            HI_U16 frame_len = 0;
            
            in_len -= (HI_S16)(head - buf);
            buf = head;
            
            if (in_len < 18) 
            {
                ret = HI_ERR_BAD_DATA;
                break;
            }
			
            frame_len = (HI_U16)(buf[2] + 18);
			
            //超长帧
            if (frame_len > 128) 
            {
                buf++;
                in_len--;
                continue;
            }
			
            if(in_len >= frame_len)
            {
                //校验和检查
                if(HI_ERR_SUCCESS == Check188Frame(buf,frame_len))
                {
                    *start = (HI_U16)(buf - in_buf);
                    *out_len = frame_len;
					 ret = HI_ERR_SUCCESS;
                    break;
                }
                else
                {
                    buf++;
                    in_len--;
                    continue;
                }
            }
            else
            {
                buf++;
                in_len--;
                continue;
            }
			
        }
        else 
        {
            ret = HI_ERR_BAD_DATA;
            break;
        }
    }
	
    return ret;
}
HI_U8 mrsToolsGet645Ver(HI_U8 ucFn)
{
    HI_U8 ret = SUPPORT_PROTOCOL_645_07;
    
    if ((!(ucFn & 0x10) && (ucFn != MRS_COLLECTOR_07_PROTOCOL_CONTROL) 
        && (ucFn != MRS_COLLECTOR_BOARDCAST_TIMEING_CONTROL)) 
        || (MRS_MAXIMUM_DEMAND_CLEAR_CONTROL == (ucFn & 0x1F)))
    {
        ret = SUPPORT_PROTOCOL_645_97;
    }
	
    return ret;
}

int MRS_Proto645Dec( HI_IN HI_PBYTE pucDatagram, int usDatagramSize, HI_OUT MRS_FRAME_STRU* pstFrame )
{
    int ret = HI_ERR_SUCCESS;
    HI_PBYTE pInBuffer = pucDatagram;
    HI_U8    ucDataRealmLen = 0;
    HI_U16 pos = 0;
    HI_U16 frame_len = 0;
	int i=0,j=0;
	int nMatchAddrCount = 0,nMatchAddr=0;

	PROTOCOL_DI_FUN pFunc = NULL;
	
    if ((pucDatagram == HI_NULL) || (pstFrame == HI_NULL)) 
    {
        return HI_ERR_BAD_DATA;
    }
	if (usDatagramSize>PROTO645_DATAGRAM_DATA_LEN_MAX)
	{
		return HI_ERR_BAD_DATA;
	}

    ret = Find645Frame(pucDatagram, (HI_S16)usDatagramSize, &pos, &frame_len);
    if (ret != HI_ERR_SUCCESS) 
    {
        return HI_ERR_BAD_DATA;
    }
    pInBuffer = pucDatagram + pos;

	//控制域
    pstFrame->st645Ctrl.ucDir = (HI_U8)((pInBuffer[8] & 0x80) >> 7);
    pstFrame->st645Ctrl.ucSlaveFlag = (HI_U8)((pInBuffer[8] & 0x40) >> 6);
    pstFrame->st645Ctrl.ucFrameFlag = (HI_U8)((pInBuffer[8] & 0x20) >> 5);
    pstFrame->st645Ctrl.ucFn = (HI_U8)(pInBuffer[8] & 0x1F);

	//获取版本号
    pstFrame->ucVer = mrsToolsGet645Ver(pstFrame->st645Ctrl.ucFn);
	pstFrame->nProType = SUPPORT_PROTOCOL_645_07;
	
    //地址域6个字节
    memcpy(pstFrame->ucAddr,(HI_PBYTE)pInBuffer + 1,P645_ADDRESS_LEN);

	MeterArray *pM=getMeterArray();
	for (i=0;i<pM->numMeter;i++)
	{
		for (j=0;j<P645_ADDRESS_LEN;j++)
		{
			if (!(
				(((pstFrame->ucAddr[j] & 0xF0) == (pM->sMeter[i].address[j]&0xF0)) &&((pstFrame->ucAddr[j] & 0x0F) ==0x0A))
				|| (((pstFrame->ucAddr[j] & 0x0F) == (pM->sMeter[i].address[j]&0x0F)) && ((pstFrame->ucAddr[j] & 0xF0) ==0xA0))
				|| (pstFrame->ucAddr[j] == 0xAA) || (pstFrame->ucAddr[j] == pM->sMeter[i].address[j])))
			{
				pstFrame->nBroadcast = 1;
				break;
			}
		}
		if (j>=P645_ADDRESS_LEN)
		{
			if (pstFrame->ucVer == SUPPORT_PROTOCOL_645_07)
			{
			}
			if(pM->sMeter[i].nValid &&
				(((pstFrame->ucVer == SUPPORT_PROTOCOL_645_07)&&(pM->sMeter[i].protocol &SUPPORT_PROTOCOL_645_07))
				||((pstFrame->ucVer == SUPPORT_PROTOCOL_645_97)&&(pM->sMeter[i].protocol &SUPPORT_PROTOCOL_645_97)))
			)
			{
				nMatchAddr = i;
				nMatchAddrCount++;
			}
			
		}
	}
	if (nMatchAddrCount ==0 )
	{
		return HI_ERR_ADDRESS_NOT_MATCH;
	}
	//if (!(pM->sMeter[nMatchAddr].protocol&(SUPPORT_PROTOCOL_645_07|SUPPORT_PROTOCOL_645_97)))
	if ((pstFrame->ucVer & pM->sMeter[nMatchAddr].protocol)==0)
	{
		return HI_ERR_PROTOCOL_NOT_MATCH;
	}

	if (nMatchAddrCount > 1)
	{ // 有多个表匹配，置为需要错误应答
		pstFrame->ucErrorResFlag =0x55;
	}

	pstFrame->nIndex  = nMatchAddr;
	memcpy(pstFrame->ucAddr,pM->sMeter[nMatchAddr].address,P645_ADDRESS_LEN);
	
	
   // 控制域
	
    //长度
    pInBuffer += 9;
    pstFrame->ucDataRealmLen = *pInBuffer++;
    ucDataRealmLen = pstFrame->ucDataRealmLen;
	
    if (ucDataRealmLen > sizeof(pstFrame->ucDataRealm))
    {
        return HI_ERR_BAD_DATA;
    }
    
    //应用程序数据
    if (ucDataRealmLen != 0) 
    {
        memcpy(pstFrame->ucDataRealm, pInBuffer, ucDataRealmLen);
    }

	// 根据控制命令，查找处理函数
	pFunc = mrs645_Control(pstFrame);
	
	if (pFunc!=NULL)
	{
		ret=pFunc(pstFrame);
	}
    return ret;
}
int MRS_ProtoWirelessDec( HI_IN HI_PBYTE pucDatagram, int usDatagramSize, HI_OUT MRS_FRAME_STRU* pstFrame)
{
	int ret = HI_ERR_SUCCESS;
    HI_PBYTE pInBuffer = pucDatagram;
    HI_U8    ucDataRealmLen = 0;
    HI_U16 pos = 0;
    HI_U16 frame_len = 0;
	HI_U32 di = 0;
	PROTOCOL_DI_FUN pFunc = NULL;
	
    if ((pucDatagram == HI_NULL) || (pstFrame == HI_NULL)) 
    {
        return HI_ERR_BAD_DATA;
    }
	if (usDatagramSize>PROTO188_DATAGRAM_DATA_LEN_MAX)
	{
		return HI_ERR_BAD_DATA;
	}
    ret = mrsFindWireLessProFrame(pucDatagram, (HI_S16)usDatagramSize, &pos, &frame_len);
    if (ret != HI_ERR_SUCCESS) 
    {
        return HI_ERR_BAD_DATA;
    }
/*	查找电表，电表是否支持188协议，暂时没做判断，只要收到188协议就回复
	if (SUPPORT_PROTOCOL_188)
	{
	}
*/
    pInBuffer = pucDatagram + pos;

    ucDataRealmLen = (HI_U8)frame_len;
	pstFrame->ucDataSendLen = frame_len;
    pstFrame->nProType = SUPPORT_PROTOCOL_WRIELESS;
    //应用程序数据
    if (ucDataRealmLen != 0) 
    {
        memcpy(pstFrame->ucDataSend, pInBuffer, ucDataRealmLen);
    }
    return ret;
}
int MRS_ProtoWirelessEnc( HI_IN HI_PBYTE pucDatagram, int *pusDatagramSize, HI_OUT MRS_FRAME_STRU* pstFrame)
{
	int ret = HI_ERR_SUCCESS;
    HI_PBYTE pOutBuffer = pucDatagram;
    HI_U8    ucDataRealmLen = 0;
	int i;
	// 暂时只做了收到，原包回复
    if ((pusDatagramSize == HI_NULL) || (pstFrame == HI_NULL)||(pstFrame->ucDataSendLen==0))
    {
        return HI_ERR_BAD_DATA;
    }
	pucDatagram[0] = 0xFE;
	memcpy(pucDatagram+1,pstFrame->ucDataSend,pstFrame->ucDataSendLen);
	*pusDatagramSize =35;
	pucDatagram[2] = pucDatagram[2]|(0x80);
	pucDatagram[3] = 0x10;
	for (i=0;i<0x10;i++)
	{
		pucDatagram[17+i]=i;
	}
	pucDatagram[33] = (HI_U8)CalcCheckSum(pucDatagram+1,32);
	pucDatagram[34] =0x16;
	 return ret;
}
int MRS_Proto188Dec( HI_IN HI_PBYTE pucDatagram, int usDatagramSize, HI_OUT MRS_FRAME_STRU* pstFrame)
{
    int ret = HI_ERR_SUCCESS;
    HI_PBYTE pInBuffer = pucDatagram;
    HI_U8    ucDataRealmLen = 0;
    HI_U16 pos = 0;
    HI_U16 frame_len = 0;
	HI_U32 di = 0;
	int index,nCount;
	PROTOCOL_DI_FUN pFunc = NULL;
	
    if ((pucDatagram == HI_NULL) || (pstFrame == HI_NULL)) 
    {
        return HI_ERR_BAD_DATA;
    }
	if (usDatagramSize>PROTO188_DATAGRAM_DATA_LEN_MAX)
	{
		return HI_ERR_BAD_DATA;
	}
    ret = Find188Frame(pucDatagram, (HI_S16)usDatagramSize, &pos, &frame_len);
    if (ret != HI_ERR_SUCCESS) 
    {
        return HI_ERR_BAD_DATA;
    }
/*	查找电表，电表是否支持188协议，暂时没做判断，只要收到188协议就回复
	if (SUPPORT_PROTOCOL_188)
	{
	}
*/
    pInBuffer = pucDatagram + pos;
	
	pstFrame->uc188Type = pInBuffer[1];
    //地址域6个字节
    memcpy(pstFrame->ucAddr,(HI_PBYTE)pInBuffer + 2,7);
	
    //控制域
    pstFrame->ucCtrl = pInBuffer[9];
	
    //长度
    pInBuffer += 10;
    pstFrame->ucDataRealmLen = *pInBuffer++;
    ucDataRealmLen = pstFrame->ucDataRealmLen;
	
    if (ucDataRealmLen > sizeof(pstFrame->ucDataRealm))
    {
        return HI_ERR_BAD_DATA;
    }
    pstFrame->nProType = SUPPORT_PROTOCOL_188;
    //应用程序数据
    if (ucDataRealmLen != 0) 
    {
        memcpy(pstFrame->ucDataRealm, pInBuffer, ucDataRealmLen);
    }

	for (index = 3; index >= 0; index--)
    {
        di <<= 8;
        di |= pstFrame->ucDataRealm[index];
    }
	
	nCount = sizeof(PROTOCOL_188_DI_PAIR_MAP)/sizeof(MRS_IRDA_DI_PAIR_STRU);
    for (index = 0; index < nCount; index++)
    {
        if ((di >= PROTOCOL_188_DI_PAIR_MAP[index].ulMinDI) && (di <= PROTOCOL_188_DI_PAIR_MAP[index].ulMaxDI))
        {
            pFunc = PROTOCOL_188_DI_PAIR_MAP[index].func;
			break;
        }
    }
	if (pFunc!=NULL)
	{
		ret = pFunc(pstFrame);
	}
	
    return ret;
}

int MRS_Proto698Dec( HI_IN HI_PBYTE pucDatagram, int usDatagramSize, HI_OUT MRS_FRAME_STRU* pstFrame )
{
	int ret = HI_ERR_SUCCESS;
    HI_PBYTE pInBuffer = pucDatagram;
    HI_U8    ucDataRealmLen = 0;
    HI_U16 pos = 0;
    HI_U16 frame_len = 0;
	int i=0;
	if (usDatagramSize>PROTO_DATAGRAM_DATA_LEN_MAX)
	{
		return HI_ERR_BAD_DATA;
	}
	// 暂时只做了收到698协议的包，原包回复
	ret = Find698Frame(pucDatagram, (HI_S16)usDatagramSize, &pos, &frame_len);
    if (ret != HI_ERR_SUCCESS) 
    {
        return HI_ERR_BAD_DATA;
    }

	pInBuffer = pucDatagram + pos;
	pstFrame->ucCtrl = *(pInBuffer+3);
	if (!((pstFrame->ucCtrl & 0x40) && !(pstFrame->ucCtrl & 0x80)))
	{
		return HI_ERR_CONTROL_NOT_MATCH;
	}
    //地址域6个字节
    memcpy(pstFrame->ucAddr,(HI_PBYTE)pInBuffer + 5,P645_ADDRESS_LEN);
	MeterArray *pM=getMeterArray();
	do 
	{
		if (!memcmp(pstFrame->ucAddr,BROADCAST_ADD,P645_ADDRESS_LEN))
		{
			pstFrame->nBroadcast = 1;
			break;
		}
		for (i=0;i<P645_ADDRESS_LEN;i++)
		{
			if (((pstFrame->ucAddr[i] & 0xF0) == 0xA0) || ((pstFrame->ucAddr[i] & 0x0F) == 0x0A) || (pstFrame->ucAddr[i] == 0xAA))
			{
				pstFrame->nBroadcast = 1;
				break;
			}
		}
		if (i < P645_ADDRESS_LEN)
		{
			break;
		}
		for (i=0;i<pM->numMeter;i++)
		{
			if (!memcmp(pM->sMeter[i].address,pstFrame->ucAddr,P645_ADDRESS_LEN))
			{
				break;
			}
		}
		if (i>=pM->numMeter)
		{
			return HI_ERR_ADDRESS_NOT_MATCH;
		}
		
	} while (0);
	pstFrame->nProType = SUPPORT_PROTOCOL_698;
	if ((pstFrame->nBroadcast ==0) && !(pM->sMeter[i].protocol&(SUPPORT_PROTOCOL_698)))
	{
		return HI_ERR_PROTOCOL_NOT_MATCH;
	}
	pstFrame->nIndex = i;
	if (pstFrame->nBroadcast ==0)
	{
		memcpy(pstFrame->ucAddr,pM->sMeter[i].address,P645_ADDRESS_LEN);
	}
	

    //长度
    pstFrame->ucDataRealmLen = usDatagramSize;
   
    //应用程序数据
  /*  if (pstFrame->ucDataRealmLen != 0) 
    {
        memcpy(pstFrame->ucDataRealm, pucDatagram, usDatagramSize);
    }
	*/
	//HI_U32 nDI = 0xFF;

	//ret = mrsDI(pstFrame,SUPPORT_PROTOCOL_698,nDI);
//	if (ret == 0)
//	{
//		return ret;
//	}

	memcpy(pstFrame->ucDataSend,pInBuffer,frame_len);
	pstFrame->ucDataSendLen = frame_len;
	return mrsIr698_SM(pstFrame);
//	return ret;

}
int fun698(unsigned char *p698,int n)
{
	int nOffent =0;
	int nhead;
	HI_U16 hcs=0;
	
	nhead = 12;
	
	nOffent=n-3;
	p698[1] = (nOffent+1)&0xFF;
	p698[2] = ((nOffent+1)&0xFF00) >>8;
	
	
	hcs = tryfcs16(p698+1,nhead-1);
	p698[nhead++] = hcs&0xFF;
	p698[nhead++] = (hcs&0xFF00)>>8;
	
	hcs = tryfcs16(p698+1,nOffent-1);
	
	p698[nOffent++] = hcs&0xFF;
	p698[nOffent++] = (hcs&0xFF00)>>8;

	return nOffent+1;
}
int MRS_Proto698Enc( HI_IN HI_PBYTE pucDatagram, int *pusDatagramSize, HI_OUT MRS_FRAME_STRU* pstFrame)
{
	int ret = HI_ERR_SUCCESS;
    HI_PBYTE pOutBuffer = pucDatagram;
    HI_U8    ucDataRealmLen = 0;
	// 暂时只做了收到698协议的包，原包回复
    if ((pusDatagramSize == HI_NULL) || (pstFrame == HI_NULL)||(pstFrame->ucDataSendLen==0))
    {
        return HI_ERR_BAD_DATA;
    }
	memcpy(pucDatagram,pstFrame->ucDataSend,pstFrame->ucDataSendLen);
	 *pusDatagramSize =pstFrame->ucDataSendLen;
	 pucDatagram[3] = pucDatagram[3]|(0x80|0x40);
	 memcpy(pucDatagram+5,pstFrame->ucAddr,6);
	 fun698(pucDatagram,pstFrame->ucDataSendLen);
	 if (pstFrame->ucErrorResFlag == 0x55)
	 {
		 pucDatagram[pstFrame->ucDataSendLen-3] = 0xFF;
		 pstFrame->ucErrorResFlag = 0;
	}
	 return ret;
}
int MRS_Proto188Enc( HI_IN HI_PBYTE pucDatagram, int *pusDatagramSize, HI_OUT MRS_FRAME_STRU* pstFrame)
{
	int ret = HI_ERR_SUCCESS;
    HI_PBYTE pOutBuffer = pucDatagram;
    HI_U8    ucDataRealmLen = 0;
	
    if ((pusDatagramSize == HI_NULL) || (pstFrame == HI_NULL)||(pstFrame->ucDataSendLen==0))
    {
        return HI_ERR_BAD_DATA;
    }
    
    //长度
    ucDataRealmLen = pstFrame->ucDataSendLen;
    
    //帧头
    *pOutBuffer++ = PROTOCOL_HEAD_BYTE;
	*pOutBuffer++ = pstFrame->uc188Type;
	
    //地址域6个字节
    memcpy(pOutBuffer,pstFrame->ucAddr,7);
    pOutBuffer += 7;
	
    
    //控制域
    *pOutBuffer++ = (HI_U8)(pstFrame->ucCtrl|0x80);
    //长度
    *pOutBuffer++ = ucDataRealmLen;
    *pusDatagramSize = (13 + ucDataRealmLen);
	
    //数据域
    if (ucDataRealmLen != 0) 
    {
        memcpy(pOutBuffer,pstFrame->ucDataSend,ucDataRealmLen);
        pOutBuffer += ucDataRealmLen;
    }
	
    //校验码
	
	if (pstFrame->ucErrorResFlag == 0x55)
	{
		*pOutBuffer++ = 0xFF;
		pstFrame->ucErrorResFlag = 0;
	}
	else
	{
		*pOutBuffer++ = (HI_U8)CalcCheckSum(pucDatagram,(HI_U16)((*pusDatagramSize)-2));
		
	}
	
    //帧尾
    *pOutBuffer++ = PROTOCOL_TAIL_BYTE;
	
    return ret;
}
int MRS_Proto645Enc( HI_IN HI_PBYTE pucDatagram, int *pusDatagramSize, HI_OUT MRS_FRAME_STRU* pstFrame)
{
	int ret = HI_ERR_SUCCESS;
    HI_PBYTE pOutBuffer = pucDatagram;
    HI_U8    ucDataRealmLen = 0;
	
    if ((pusDatagramSize == HI_NULL) || (pstFrame == HI_NULL)||(pstFrame->ucDataSendLen==0))
    {
        return HI_ERR_BAD_DATA;
    }
    
    //长度
    ucDataRealmLen = pstFrame->ucDataSendLen;
    
    //帧头
    *pOutBuffer++ = PROTOCOL_HEAD_BYTE;
	
    //地址域6个字节
    memcpy(pOutBuffer,pstFrame->ucAddr,6);
    pOutBuffer += 6;
	
    //帧头
    *pOutBuffer++ = PROTOCOL_HEAD_BYTE;
    
    //控制域
    *pOutBuffer++ = (HI_U8)(((pstFrame->st645Ctrl.ucDir | 0x01) << 7)
		+ ((pstFrame->st645Ctrl.ucSlaveFlag & 0x01) << 6)
		+ ((pstFrame->st645Ctrl.ucFrameFlag & 0x01) << 5)
		+ ((pstFrame->st645Ctrl.ucFn & 0x1F)));
    //长度
    *pOutBuffer++ = ucDataRealmLen;
    *pusDatagramSize = (12 + ucDataRealmLen);
	
    //数据域
    if (ucDataRealmLen != 0) 
    {
        memcpy(pOutBuffer,pstFrame->ucDataSend,ucDataRealmLen);
        pOutBuffer += ucDataRealmLen;
    }
	
    //校验码

	if (pstFrame->ucErrorResFlag == 0x55)
	{
		*pOutBuffer++ = 0xFF;
		pstFrame->ucErrorResFlag = 0;
	}
	else
	{
		*pOutBuffer++ = (HI_U8)CalcCheckSum(pucDatagram,(HI_U16)((*pusDatagramSize) - 2));

	}
	
    //帧尾
    *pOutBuffer++ = PROTOCOL_TAIL_BYTE;
	
    return ret;
}

HI_U32 mrsDI(MRS_FRAME_STRU* pstFrame,BYTE nPro,HI_U32 nDI)
{
	HI_U32 di = 0;
	int i,j,index;
	ProDIData *nProDIData = getProDIData();
	for (i=0;i<nProDIData->nCount;i++)
	{
		if (nProDIData->DI[i].isValid == 0)
		{
			continue;
		}
		if (nProDIData->DI[i].nFunID!= pstFrame->st645Ctrl.ucFn)
		{
			continue;
		}
		if ((nPro&(nProDIData->DI[i].nPro)) == 0)
		{
			continue;
		}
	
		if (nProDIData->DI[i].nDI != nDI)
		{
			continue;
		}
		index = 0;
		if (pstFrame->nProType == SUPPORT_PROTOCOL_645_07 && pstFrame->ucVer == SUPPORT_PROTOCOL_645_97)
		{
			
			pstFrame->ucDataSend[index++] = pstFrame->ucDataRealm[0];
			pstFrame->ucDataSend[index++] = pstFrame->ucDataRealm[1];
			
		}
		else
		{
			for (j=0;j<4;j++)
			{
				pstFrame->ucDataSend[index++] = pstFrame->ucDataRealm[j];
			}
		}
	
	
		for (j=0;j<nProDIData->DI[i].len;j++)
		{
			pstFrame->ucDataSend[index++] = nProDIData->DI[i].DIData[j];
		}
		pstFrame->ucDataSendLen = index;
		return 0;
	}
	return HI_ERR_FAIL;
}

	

PROTOCOL_DI_FUN mrs645_Control(MRS_FRAME_STRU* pstFrame)
{
	HI_U32 di = 0;
	int index,nCount;
	int nNum=3;
	PROTOCOL_DI_FUN pFunc = NULL;
	if (pstFrame->ucVer==SUPPORT_PROTOCOL_645_97 )
	{
		nNum = 1;
	}

	if (pstFrame->ucDataRealmLen>=nNum)
	{
		for (index = nNum; index >= 0; index--)
		{
			di <<= 8;
			di |= pstFrame->ucDataRealm[index];
		}
	}
	if (pstFrame->st645Ctrl.ucFn == 0x1E)
	{
		di = pstFrame->ucDataRealm[0];
	}
	switch(pstFrame->st645Ctrl.ucFn)
	{
	case 0x08://广播校时
		break;
	case 0x01:// 97表读数据
	case 0x02:
		if (pstFrame->ucVer==SUPPORT_PROTOCOL_645_97 )
		{
			nCount = sizeof(PROTOCOL_64597_11H_DI_PAIR_MAP)/sizeof(MRS_IRDA_DI_PAIR_STRU);
			for (index = 0; index < nCount; index++)
			{
				if ((di >= PROTOCOL_64597_11H_DI_PAIR_MAP[index].ulMinDI) && (di <= PROTOCOL_64597_11H_DI_PAIR_MAP[index].ulMaxDI))
				{
					pFunc = PROTOCOL_64597_11H_DI_PAIR_MAP[index].func;
					break;
				}
			}
		}
		break;
	case 0x11://读数据	
		{
			nCount = sizeof(PROTOCOL_64507_11H_DI_PAIR_MAP)/sizeof(MRS_IRDA_DI_PAIR_STRU);
			for (index = 0; index < nCount; index++)
			{
				if ((di >= PROTOCOL_64507_11H_DI_PAIR_MAP[index].ulMinDI) && (di <= PROTOCOL_64507_11H_DI_PAIR_MAP[index].ulMaxDI))
				{
					pFunc = PROTOCOL_64507_11H_DI_PAIR_MAP[index].func;
					break;
				}
			}
		}
		
		break;
	case 0x12://读后续数据
		if (pstFrame->ucVer==SUPPORT_PROTOCOL_645_07 )
		{
			nCount = sizeof(PROTOCOL_64507_12H_DI_PAIR_MAP)/sizeof(MRS_IRDA_DI_PAIR_STRU);
			for (index = 0; index < nCount; index++)
			{
				if ((di >= PROTOCOL_64507_12H_DI_PAIR_MAP[index].ulMinDI) && (di <= PROTOCOL_64507_12H_DI_PAIR_MAP[index].ulMaxDI))
				{
					pFunc = PROTOCOL_64507_12H_DI_PAIR_MAP[index].func;
					break;
				}
			}
		}
		break;
	case 0x13://读通信地址
		nCount = sizeof(PROTOCOL_64507_13H_DI_PAIR_MAP)/sizeof(MRS_IRDA_DI_PAIR_STRU);
		for (index = 0; index < nCount; index++)
		{
			if ((di >= PROTOCOL_64507_13H_DI_PAIR_MAP[index].ulMinDI) && (di <= PROTOCOL_64507_13H_DI_PAIR_MAP[index].ulMaxDI))
			{
				pFunc = PROTOCOL_64507_13H_DI_PAIR_MAP[index].func;
				break;
			}
		}
		break;
	case 0x14:// 写通信地址
		break;
	case 0x15:// 冻结命令
		break;
	case 0x16://更改通信速率
		break;
	case 0x17:// 更改密码
		break;
	case 0x18://最大需求清零
		break;
	case 0x19:// 电量清零
		break;
	case 0x1A:// 事件清零
		break;
	case 0x1E:// 事件清零
		nCount = sizeof(PROTOCOL_64507_1EH_DI_PAIR_MAP)/sizeof(MRS_IRDA_DI_PAIR_STRU);
		for (index = 0; index < nCount; index++)
		{
			if ((di >= PROTOCOL_64507_1EH_DI_PAIR_MAP[index].ulMinDI) && (di <= PROTOCOL_64507_1EH_DI_PAIR_MAP[index].ulMaxDI))
			{
				pFunc = PROTOCOL_64507_1EH_DI_PAIR_MAP[index].func;
				break;
			}
		}
		break;
	default:
		break;
	}
	return pFunc;
}


HI_U32 mrs188_All_DItest(MRS_FRAME_STRU* pstFrame)
{
	int i=0,nIndex=0;
	for (i=0;i<4;i++)
	{
		pstFrame->ucDataSend[nIndex++] = pstFrame->ucDataRealm[i];
	}
	for (i=0;i<4;i++)
	{
		pstFrame->ucDataSend[nIndex++] = i+0x33;
	}
	pstFrame->ucDataSendLen = nIndex;
    return 0;
}
HI_U32 mrsIr645_11H_DI33343333(MRS_FRAME_STRU* pstFrame)
{
	// 组合有功功率1电能
	int i=0,nIndex=0;
	for (i=0;i<4;i++)
	{
		pstFrame->ucDataSend[nIndex++] = pstFrame->ucDataRealm[i];
	}
	for (i=0;i<4;i++)
	{
		pstFrame->ucDataSend[nIndex++] = 0+0x33;
	}
	pstFrame->ucDataSendLen = nIndex;
	return 0;
}
HI_U32 mrsIr645_11H_DI37333435(MRS_FRAME_STRU* pstFrame)
{
	int i=0,nIndex=0;
	SYSTEMTIME st;
	GetLocalTime(&st);
	for (i=0;i<4;i++)
	{
		pstFrame->ucDataSend[nIndex++] = pstFrame->ucDataRealm[i];
	}
	
	
	pstFrame->ucDataSend[nIndex++] = IntToHex(st.wSecond)+0x33;
	pstFrame->ucDataSend[nIndex++] = IntToHex(st.wMinute)+0x33;
	pstFrame->ucDataSend[nIndex++] = IntToHex(st.wHour)+0x33;
	
	
	pstFrame->ucDataSendLen = nIndex;
	return 0;
}
HI_U32 mrsIr645_11H_DI37333434(MRS_FRAME_STRU* pstFrame)
{
	int i=0,nIndex=0;
	COleDateTime t;
	t=COleDateTime::GetCurrentTime();
	for (i=0;i<4;i++)
	{
		pstFrame->ucDataSend[nIndex++] = pstFrame->ucDataRealm[i];
	}
	pstFrame->ucDataSend[nIndex++] = (t.GetDayOfWeek()-1)+0x33;
	pstFrame->ucDataSend[nIndex++] = IntToHex(t.GetDay())+0x33;
	pstFrame->ucDataSend[nIndex++] = IntToHex(t.GetMonth())+0x33;
	pstFrame->ucDataSend[nIndex++] = ((IntToHex(t.GetYear()))&0xFF)+0x33;
	
	pstFrame->ucDataSendLen = nIndex;
	return 0;
}
HI_U32 mrsIr645_11H_DI37333834(MRS_FRAME_STRU* pstFrame)
{
	int i=0,nIndex=0;
	COleDateTime t;
	t=COleDateTime::GetCurrentTime();
	for (i=0;i<4;i++)
	{
		pstFrame->ucDataSend[nIndex++] = pstFrame->ucDataRealm[i];
	}

	pstFrame->ucDataSend[nIndex++] = 0+0x33;
	pstFrame->ucDataSend[nIndex++] = 0+0x33;
	
	pstFrame->ucDataSendLen = nIndex;
	return 0;
}
HI_U32 mrsIr645_11H_DI33FFFFFF(MRS_FRAME_STRU* pstFrame)
{
	int i=0,nIndex=0;
	for (i=0;i<4;i++)
	{
		pstFrame->ucDataSend[nIndex++] = pstFrame->ucDataRealm[i];
	}
	for (i=0;i<4;i++)
	{
		pstFrame->ucDataSend[nIndex++] = i+0x33;
	}
	pstFrame->ucDataSendLen = nIndex;
	return 0;
}
HI_U32 mrsIr645_11H_DI34FFFFFF(MRS_FRAME_STRU* pstFrame)
{
	int i=0,nIndex=0;
	SYSTEMTIME st;
	GetLocalTime(&st);
	for (i=0;i<4;i++)
	{
		pstFrame->ucDataSend[nIndex++] = pstFrame->ucDataRealm[i];
	}
	// 最大需求量xx.xxxx YYMMDDHHmm
	for (i=0;i<3;i++)
	{
		pstFrame->ucDataSend[nIndex++] = i+0x33;
	}
	pstFrame->ucDataSend[nIndex++] = st.wYear%100+0x33;
	pstFrame->ucDataSend[nIndex++] = st.wMonth+0x33;
	pstFrame->ucDataSend[nIndex++] = st.wDay+0x33;
	pstFrame->ucDataSend[nIndex++] = st.wHour+0x33;
	pstFrame->ucDataSend[nIndex++] = st.wMinute+0x33;
	pstFrame->ucDataSendLen = nIndex;
	return 0;
}
HI_U32 mrsIr645_11H_DI33343233(MRS_FRAME_STRU* pstFrame)
{
	int i=0,nIndex=0;
	for (i=0;i<4;i++)
	{
		pstFrame->ucDataSend[nIndex++] = pstFrame->ucDataRealm[i];
	}
	// 日冻结正向有功数据 4*N
	for (i=0;i<5;i++)
	{
		pstFrame->ucDataSend[nIndex++] = 0+0x33;
		pstFrame->ucDataSend[nIndex++] = 0+0x33;
		pstFrame->ucDataSend[nIndex++] = 0+0x33;
		pstFrame->ucDataSend[nIndex++] = 0+0x33;
	}
	
	pstFrame->ucDataSendLen = nIndex;
	return 0;
}
HI_U32 mrsIr645_11H_DI37334536(MRS_FRAME_STRU* pstFrame)
{
	int i=0,nIndex=0;
	for (i=0;i<4;i++)
	{
		pstFrame->ucDataSend[nIndex++] = pstFrame->ucDataRealm[i];
	}
	// 日冻结时间
	
	pstFrame->ucDataSend[nIndex++] = 0+0x33;
	pstFrame->ucDataSend[nIndex++] = 0+0x33;
	pstFrame->ucDataSendLen = nIndex;
	return 0;
}
HI_U32 mrsIr645_11H_DI35343433(MRS_FRAME_STRU* pstFrame) // 查询A相电压
{
	int i=0,nIndex=0;
	for (i=0;i<4;i++)
	{
		pstFrame->ucDataSend[nIndex++] = pstFrame->ucDataRealm[i];
	}
	
	pstFrame->ucDataSend[nIndex++] = 0+0x33;
	pstFrame->ucDataSend[nIndex++] = 0x22+0x33;
	pstFrame->ucDataSendLen = nIndex;
	return 0;
}
HI_U32 mrsIr645_11H_DI35353433(MRS_FRAME_STRU* pstFrame) // 查询A相电流
{
	int i=0,nIndex=0;
	for (i=0;i<4;i++)
	{
		pstFrame->ucDataSend[nIndex++] = pstFrame->ucDataRealm[i];
	}
	
	pstFrame->ucDataSend[nIndex++] = 0+0x33;
	pstFrame->ucDataSend[nIndex++] = 0+0x33;
	pstFrame->ucDataSend[nIndex++] = 0+0x33;
	pstFrame->ucDataSendLen = nIndex;
	return 0;
}
HI_U32 mrsIr645_11H_DI38393434(MRS_FRAME_STRU* pstFrame)
{
	int i=0,nIndex=0;
	for (i=0;i<4;i++)
	{
		pstFrame->ucDataSend[nIndex++] = pstFrame->ucDataRealm[i];
	}
	// 日冻结正向有功数据 4*N
	for (i=0;i<5;i++)
	{
		pstFrame->ucDataSend[nIndex++] = 0+0x33;
		pstFrame->ucDataSend[nIndex++] = 0+0x33;
		pstFrame->ucDataSend[nIndex++] = 0+0x33;
		pstFrame->ucDataSend[nIndex++] = 0+0x33;
	}

	pstFrame->ucDataSendLen = nIndex;
	return 0;
}
HI_U32 mrsIr645_11H_DI38393334(MRS_FRAME_STRU* pstFrame)
{
	int i=0,nIndex=0;
	SYSTEMTIME st;

	GetLocalTime(&st);
	for (i=0;i<4;i++)
	{
		pstFrame->ucDataSend[nIndex++] = pstFrame->ucDataRealm[i];
	}
	// 日冻结时间YYMMDDHHmm

	pstFrame->ucDataSend[nIndex++] = 0+0x33;
	pstFrame->ucDataSend[nIndex++] = 0+0x33;
//	pstFrame->ucDataSend[nIndex++] = 9+0x33;
//	pstFrame->ucDataSend[nIndex++] = 6+0x33;
//	pstFrame->ucDataSend[nIndex++] = 0x17+0x33;
	pstFrame->ucDataSend[nIndex++] = IntToHex(st.wDay)+0x33;
	pstFrame->ucDataSend[nIndex++] = IntToHex(st.wMonth)+0x33;
	pstFrame->ucDataSend[nIndex++] = IntToHex(st.wYear%100)+0x33;
	pstFrame->ucDataSendLen = nIndex;
	return 0;
}
HI_U32 mrsIr645_11H_DI37334834(MRS_FRAME_STRU* pstFrame)
{
	int i=0,nIndex=0;
	int nEvent;
	MeterArray *PmeterArray=getMeterArray();

	if(PmeterArray->sMeter[pstFrame->nIndex].IsSupportEventState == 0)
	{
		//pstFrame->ucDataSend[nIndex++] = 1;
		
		pstFrame->ucDataSend[nIndex++] = 2+0x33;
		pstFrame->st645Ctrl.ucSlaveFlag =1;
		pstFrame->ucDataSendLen = nIndex;
		return 0;
	}
	for (i=0;i<4;i++)
	{
		pstFrame->ucDataSend[nIndex++] = pstFrame->ucDataRealm[i];
	}
	// 查询事件1 状态
	for (i=0;i<4;i++)
	{
		nEvent = PmeterArray->sMeter[pstFrame->nIndex].nEvent[i];
		pstFrame->ucDataSend[nIndex++] = (nEvent%100) + 0x33;
		pstFrame->ucDataSend[nIndex++] = ((nEvent/100)%100)+0x33;
		pstFrame->ucDataSend[nIndex++] = ((nEvent/10000)%100)+0x33;
	}
	pstFrame->ucDataSend[nIndex++] = 0xAA+0x33;
	pstFrame->ucDataSend[nIndex++] = 0xAA+0x33;
/*	for(i=0;i<11;i++)
	{
		pstFrame->ucDataSend[nIndex++] = 0+0x33;
	}
	pstFrame->ucDataSend[nIndex++] = 0x0+0x33;
	pstFrame->ucDataSend[nIndex++] = PmeterArray->sMeter[pstFrame->nIndex].nEvent[0]+0x33;
	pstFrame->ucDataSend[nIndex++] = 0x0+0x33;
	*/
	pstFrame->ucDataSendLen = nIndex;
	return 0;
}
HI_U32 mrsIr645_11H_DI4C343334(MRS_FRAME_STRU* pstFrame)
{
	int i=0,nIndex=0;
	int nEvent;
	MeterArray *PmeterArray=getMeterArray();
	for (i=0;i<4;i++)
	{
		pstFrame->ucDataSend[nIndex++] = pstFrame->ucDataRealm[i];
	}
	// 查询事件2
	nEvent = PmeterArray->sMeter[pstFrame->nIndex].nEvent[1];
	pstFrame->ucDataSend[nIndex++] = (nEvent%100) + 0x33;
	pstFrame->ucDataSend[nIndex++] = ((nEvent/100)%100)+0x33;
	pstFrame->ucDataSend[nIndex++] = ((nEvent/10000)%100)+0x33;
	pstFrame->ucDataSendLen = nIndex;
	return 0;
}
HI_U32 mrsIr645_11H_DI4C353334(MRS_FRAME_STRU* pstFrame)
{
	int i=0,nIndex=0;
	int nEvent;
	MeterArray *PmeterArray=getMeterArray();
	for (i=0;i<4;i++)
	{
		pstFrame->ucDataSend[nIndex++] = pstFrame->ucDataRealm[i];
	}
	// 查询事件2
	nEvent = PmeterArray->sMeter[pstFrame->nIndex].nEvent[1];
	pstFrame->ucDataSend[nIndex++] = (nEvent%100) + 0x33;
	pstFrame->ucDataSend[nIndex++] = ((nEvent/100)%100)+0x33;
	pstFrame->ucDataSend[nIndex++] = ((nEvent/10000)%100)+0x33;
	pstFrame->ucDataSendLen = nIndex;
	return 0;
}
HI_U32 mrsIr645_11H_DI4C363334(MRS_FRAME_STRU* pstFrame)
{
	int i=0,nIndex=0;
	int nEvent;
	MeterArray *PmeterArray=getMeterArray();
	for (i=0;i<4;i++)
	{
		pstFrame->ucDataSend[nIndex++] = pstFrame->ucDataRealm[i];
	}
	// 查询事件2
	nEvent = PmeterArray->sMeter[pstFrame->nIndex].nEvent[1];
	pstFrame->ucDataSend[nIndex++] = (nEvent%100) + 0x33;
	pstFrame->ucDataSend[nIndex++] = ((nEvent/100)%100)+0x33;
	pstFrame->ucDataSend[nIndex++] = ((nEvent/10000)%100)+0x33;
	pstFrame->ucDataSendLen = nIndex;
	return 0;
}
HI_U32 mrsIr645_11H_DI36443333(MRS_FRAME_STRU* pstFrame)
{
	int i=0,nIndex=0;
	MeterArray *PmeterArray=getMeterArray();
	int nEvent;
	for (i=0;i<4;i++)
	{
		pstFrame->ucDataSend[nIndex++] = pstFrame->ucDataRealm[i];
	}
	
	// 掉电总次数
	nEvent = PmeterArray->sMeter[pstFrame->nIndex].nEvent[0];
	
	pstFrame->ucDataSend[nIndex++] = (nEvent%100) + 0x33;
	pstFrame->ucDataSend[nIndex++] = ((nEvent/100)%100)+0x33;
	pstFrame->ucDataSend[nIndex++] = ((nEvent/10000)%100)+0x33;
	
	pstFrame->ucDataSendLen = nIndex;
	return 0;
}
HI_U32 mrsIr645_11H_DI36634033(MRS_FRAME_STRU* pstFrame)
{
	int i=0,nIndex=0;
	MeterArray *PmeterArray=getMeterArray();
	int nEvent;
	for (i=0;i<4;i++)
	{
		pstFrame->ucDataSend[nIndex++] = pstFrame->ucDataRealm[i];
	}

	// 查询事件3
	nEvent = PmeterArray->sMeter[pstFrame->nIndex].nEvent[2];

	pstFrame->ucDataSend[nIndex++] = (nEvent%100) + 0x33;
	pstFrame->ucDataSend[nIndex++] = ((nEvent/100)%100)+0x33;
	pstFrame->ucDataSend[nIndex++] = ((nEvent/10000)%100)+0x33;

	pstFrame->ucDataSendLen = nIndex;
	return 0;
}
HI_U32 mrsIr645_11H_DI36633433(MRS_FRAME_STRU* pstFrame)
{
	int i=0,nIndex=0;
	int nEvent;
	MeterArray *PmeterArray=getMeterArray();
	for (i=0;i<4;i++)
	{
		pstFrame->ucDataSend[nIndex++] = pstFrame->ucDataRealm[i];
	}
	// 查询事件4
	nEvent = PmeterArray->sMeter[pstFrame->nIndex].nEvent[3];
	pstFrame->ucDataSend[nIndex++] = (nEvent%100) + 0x33;
	pstFrame->ucDataSend[nIndex++] = ((nEvent/100)%100)+0x33;
	pstFrame->ucDataSend[nIndex++] = ((nEvent/10000)%100)+0x33;

	pstFrame->ucDataSendLen = nIndex;
	return 0;
}


HI_U32 mrsIr645_11H_DI33333333(MRS_FRAME_STRU* pstFrame)
{
	int i=0,nIndex=0;
	MeterArray *pM=getMeterArray();

	for (i=0;i<4;i++)
	{
		pstFrame->ucDataSend[nIndex++] = pstFrame->ucDataRealm[i];
	}
	for (i=0;i<4;i++)
	{
		pstFrame->ucDataSend[nIndex++] =0+0x33;
	}
	pstFrame->ucDataSendLen = nIndex;
	
	return 0;
}

HI_U32 mrsIr645_11H_07_SM(MRS_FRAME_STRU* pstFrame)
{
	int i=0,nIndex=0;
	MeterArray *pM=getMeterArray();
	
	for (i=0;i<4;i++)
	{
		pstFrame->ucDataSend[nIndex++] = pstFrame->ucDataRealm[i];
	}
	for (i=0;i<6;i++)
	{
		pstFrame->ucDataSend[nIndex++] =pM->sMeter[pstFrame->nIndex].address[i]+0x33;
	}
	pstFrame->ucDataSendLen = nIndex;

	return 0;
}
HI_U32 mrsIr698_SM(MRS_FRAME_STRU* pstFrame)
{
	int i=0,j=0,nIndex=0;
	int nMatchAddrCount = 0,nMatchAddr=0;
	CString strmp;
	MeterArray *pM=getMeterArray();
	for (i=0;i<pM->numMeter;i++)
	{
		for (j=0;j<P645_ADDRESS_LEN;j++)
		{
			if (!((pstFrame->ucAddr[j] == WILDCARD) || (pstFrame->ucAddr[j] == pM->sMeter[i].address[j]) ||
				(((pstFrame->ucAddr[j] & 0xF0) == 0xA0)&& ((pstFrame->ucAddr[j] & 0x0F) == (pM->sMeter[i].address[j] & 0x0F)))
			|| (((pstFrame->ucAddr[j] & 0x0F) == 0x0A)&& ((pstFrame->ucAddr[j] & 0xF0) ==( pM->sMeter[i].address[j] & 0xF0)))))
			{
				break;
			}
		}
		if (j>=P645_ADDRESS_LEN)
		{
			if(pM->sMeter[i].nValid &&
				(pM->sMeter[i].protocol &SUPPORT_PROTOCOL_698))
			{
				nMatchAddr = i;
				nMatchAddrCount++;
			}
		}
	}
	if (nMatchAddrCount ==0 )
	{
		return HI_ERR_ADDRESS_NOT_MATCH;
	}
	
	for (i=0;i<P645_ADDRESS_LEN;i++)
	{
		pstFrame->ucAddr[i] =pM->sMeter[nMatchAddr].address[i];
	}
	
	if (nMatchAddrCount > 1)
	{ // 有多个表匹配，置为需要错误应答
		pstFrame->ucErrorResFlag =0x55;
	}
	return 0;
}

HI_U32 mrsIr645_13H_00(MRS_FRAME_STRU* pstFrame)
{
	int i=0,nIndex=0;
	MeterArray *PmeterArray=getMeterArray();
	if (PmeterArray->numMeter>0)
	{
		for (i=0;i<P645_ADDRESS_LEN;i++)
		{
			pstFrame->ucDataSend[nIndex++] = 0xCC;
		}
	}
	pstFrame->ucErrorResFlag = 0;
	pstFrame->ucDataSendLen = nIndex;
	return 0;
}
HI_U32 mrsIr645_13H_3E3D3E3D(MRS_FRAME_STRU* pstFrame) //I采 启动搜表
{
	int i=0,nIndex=0;

	for (i=0;i<4;i++)
	{
		pstFrame->ucDataSend[nIndex++] = pstFrame->ucDataRealm[i];
	}
	pstFrame->ucDataSendLen = nIndex;
	pstFrame->ucErrorResFlag = 0;
	return 0;
}
HI_U32 mrsIr645_13H_3E3F3E3F(MRS_FRAME_STRU* pstFrame) // I采 停止搜表
{
	int i=0,nIndex=0;
	
	for (i=0;i<4;i++)
	{
		pstFrame->ucDataSend[nIndex++] = pstFrame->ucDataRealm[i];
	}
	pstFrame->ucDataSendLen = nIndex;
	pstFrame->ucErrorResFlag = 0;
	return 0;
}
HI_U32 mrsIr645_13H_3E403E40(MRS_FRAME_STRU* pstFrame) // 读取搜表结果
{
	int i=0,nIndex=0;
	MeterArray *PmeterArray=getMeterArray();
	if (pstFrame->ucDataRealmLen!=6)
    {
		mrs64507_All_DenyFrame(pstFrame);
		return 0;
    }
	int nStart = pstFrame->ucDataRealm[4]-0x33;
	int nNum = pstFrame->ucDataRealm[5]-0x33;

	for (i=0;i<4;i++)
	{
		pstFrame->ucDataSend[nIndex++] = pstFrame->ucDataRealm[i];
	}
	pstFrame->ucDataSend[nIndex++] = 0x33+0;
	pstFrame->ucDataSend[nIndex++] = 0x33+PmeterArray->numMeter;
	int noffent=nIndex,number=0;
	nIndex++;
    for (i=((nStart-1)>-1?(nStart-1):0);i<(nStart+nNum) && i< PmeterArray->numMeter;i++)
	{
		number++;
		pstFrame->ucDataSend[nIndex++] = PmeterArray->sMeter[i].address[0]+0x33;
		pstFrame->ucDataSend[nIndex++] = PmeterArray->sMeter[i].address[1]+0x33;
		pstFrame->ucDataSend[nIndex++] = PmeterArray->sMeter[i].address[2]+0x33;
		pstFrame->ucDataSend[nIndex++] = PmeterArray->sMeter[i].address[3]+0x33;
		pstFrame->ucDataSend[nIndex++] = PmeterArray->sMeter[i].address[4]+0x33;
		pstFrame->ucDataSend[nIndex++] = PmeterArray->sMeter[i].address[5]+0x33;
		pstFrame->ucDataSend[nIndex++] = 0x33+((PmeterArray->sMeter[i].protocol&0x01)>0?2:3);
	}
    pstFrame->ucDataSend[noffent] = 0x33+number;
	pstFrame->ucDataSendLen = nIndex;
	pstFrame->ucErrorResFlag = 0;

	return 0;
}
HI_U32 mrsIr645_13H_3E3D3F33(MRS_FRAME_STRU* pstFrame) // I采集器版本
{
	int i=0,nIndex=0;
	MeterArray *PmeterArray=getMeterArray();
	if (pstFrame->ucDataRealmLen!=10)
    {
		mrs64507_All_DenyFrame(pstFrame);
		return 0;
    }
	// 输出接收到版本
	
	for (i=0;i<4;i++)
	{
		pstFrame->ucDataSend[nIndex++] = pstFrame->ucDataRealm[i];
	}
	pstFrame->ucDataSendLen = nIndex;
	pstFrame->ucErrorResFlag = 0;
	
	return 0;

}
HI_U32 mrsIr645_11H_97_SM(MRS_FRAME_STRU* pstFrame)
{
	int i=0,nIndex=0;
	MeterArray *pM=getMeterArray();
	
	for (i=0;i<2;i++)
	{
		pstFrame->ucDataSend[nIndex++] = pstFrame->ucDataRealm[i];
	}
	for (i=0;i<6;i++)
	{
		pstFrame->ucDataSend[nIndex++] =pM->sMeter[pstFrame->nIndex].address[i]+0x33;
	}
	pstFrame->ucDataSendLen = nIndex;
	
	return 0;
}
HI_U32 mrsIr645_11H_97_C343(MRS_FRAME_STRU* pstFrame)
{
	int i=0,nIndex=0;
	MeterArray *pM=getMeterArray();
	
	for (i=0;i<2;i++)
	{
		pstFrame->ucDataSend[nIndex++] = pstFrame->ucDataRealm[i];
	}
	for (i=0;i<4;i++)
	{
		pstFrame->ucDataSend[nIndex++] =0+0x33;
	}
	pstFrame->ucDataSendLen = nIndex;
	
	return 0;
}
HI_U32 mrsIr645_11H_97_D343(MRS_FRAME_STRU* pstFrame)
{
	int i=0,nIndex=0;
	MeterArray *pM=getMeterArray();
	
	for (i=0;i<2;i++)
	{
		pstFrame->ucDataSend[nIndex++] = pstFrame->ucDataRealm[i];
	}
	for (i=0;i<3;i++)
	{
		pstFrame->ucDataSend[nIndex++] =0+0x33;
	}
	pstFrame->ucDataSendLen = nIndex;
	
	return 0;
}
HI_U32 mrs64597_All_DItest(MRS_FRAME_STRU* pstFrame)
{
	int i=0,nIndex=0;
	if (pstFrame->ucDataRealmLen<2)
	{
		mrs64507_All_DenyFrame(pstFrame);
		return 0;
	}
	for (i=0;i<2;i++)
	{
		pstFrame->ucDataSend[nIndex++] = pstFrame->ucDataRealm[i];
	}
	for (i=0;i<4;i++)
	{
		pstFrame->ucDataSend[nIndex++] = i+0x33;
	}
	pstFrame->ucDataSendLen = nIndex;
    return 0;
}

HI_U32 mrs64507_All_DenyFrame(MRS_FRAME_STRU* pstFrame)
{
	int nIndex=0;
	
//	pstFrame->ucDataSend[nIndex++] = 1;
	
	
	pstFrame->ucDataSend[nIndex++] = 2+0x33;
	pstFrame->st645Ctrl.ucSlaveFlag =1;
	pstFrame->ucDataSendLen = nIndex;
    return 0;
}

HI_U32 mrsIr645_12H_ALL(MRS_FRAME_STRU* pstFrame)
{
	if (pstFrame->ucDataRealmLen<5)
	{
		mrs64507_All_DenyFrame(pstFrame);
		return 0;
	}
	int i=0,nIndex=0;
	for (i=0;i<4;i++)
	{
		pstFrame->ucDataSend[nIndex++] = pstFrame->ucDataRealm[i];
	}
	// 读取后续 4*N
	for (i=0;i<4;i++)
	{
		pstFrame->ucDataSend[nIndex++] = 1+0x33;
	}
	pstFrame->ucDataSend[nIndex++] = pstFrame->ucDataRealm[4];
	pstFrame->ucDataSendLen = nIndex;
	return 0;
}

HI_U32 mrsIr645_1EH_03(MRS_FRAME_STRU* pstFrame)
{
	int i=0,nIndex=0;
	MeterArray *PmeterArray=getMeterArray();
	if (pstFrame->ucDataRealmLen!=2)
    {
		mrs64507_All_DenyFrame(pstFrame);
		return 0;
    }

	pstFrame->ucDataSend[nIndex++] = pstFrame->ucDataRealm[0];
	pstFrame->ucDataSend[nIndex++] = pstFrame->ucDataRealm[1];
	for (i=0;i<16;i++)
	{
		pstFrame->ucDataSend[nIndex++] = 0x33+i;
	}
	pstFrame->ucDataSendLen = nIndex;
	pstFrame->ucErrorResFlag = 0;
	
	return 0;

}