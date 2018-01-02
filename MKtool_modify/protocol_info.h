#ifndef _PROTOCOL_INFO_H_
#define _PROTOCOL_INFO_H_
#define HI_IN 
#define HI_OUT

#include "tool_common.h"

#define  HI_NULL NULL

#define  HI_ERR_SUCCESS                                   (0)
#define  HI_ERR_INVALID_PARAMETER                         (1)
#define  HI_ERR_INVALID_ID                                (2)
#define  HI_ERR_NO_INITILIZATION                          (3) 
#define  HI_ERR_NOT_FOUND                                 (4)
#define  HI_ERR_FULL                                      (5)
#define  HI_ERR_BAD_DATA								(6)
#define  HI_ERR_FAILURE									(7)
#define  HI_ERR_ADDRESS_NOT_MATCH						(8)	// 地址不匹配
#define  HI_ERR_FAIL									(9)								
#define  HI_ERR_PROTOCOL_NOT_MATCH						(10) //协议不匹配
#define  HI_ERR_CONTROL_NOT_MATCH						(11)

#define PROTO_DATAGRAM_DATA_LEN_MAX     2048

#define PROTO645_DATAGRAM_DATA_LEN_MAX     256
#define PROTO645_DATAGRAM_HEADER_LEN_MAX   12
#define PROTO645_DATAGRAM_LEN_MAX   (PROTO645_DATAGRAM_DATA_LEN_MAX+PROTO645_DATAGRAM_HEADER_LEN_MAX)

#define PROTO698_HEADER_LEN_MAX   11
#define PROTO698_LEN_MAX     2048

#define PROTO188_DATAGRAM_DATA_LEN_MAX     256
#define PROTO188_DATAGRAM_HEADER_LEN_MAX   13
#define PROTO188_DATAGRAM_LEN_MAX   (PROTO188_DATAGRAM_DATA_LEN_MAX+PROTO188_DATAGRAM_HEADER_LEN_MAX)

#define MRS_PROTO645_VER_UNKNOWN	0
#define MRS_PROTO645_VER_1997		1
#define MRS_PROTO645_VER_2007		2
#define MRS_PROTO188_DOT			3
#define MRS_PROTO698_DOT_45			4

#define MRS_COLLECTOR_07_PROTOCOL_CONTROL         0x03   //特定控制字，属于645-2007规约
#define MRS_COLLECTOR_BOARDCAST_TIMEING_CONTROL   0x08   //广播校时报文中的控制字
#define MRS_MAXIMUM_DEMAND_CLEAR_CONTROL          0x10   //645-97规约中最大需量清零控制字

#define P645_ADDRESS_LEN 6
#define ADRESS_LEN   16

#define BROADCAST_ADD "999999999999"
#define WILDCARD 0xAA

typedef struct 
{
    HI_U8 ucFn        :5; // 请求及应答功能码    
    HI_U8 ucFrameFlag :1; // 后续帧标志 0-无后续数据帧 1-有后续数据帧
    HI_U8 ucSlaveFlag :1; // 从站异常标志 0-从站正确应答 1-从站对异常信息的应答
    HI_U8 ucDir       :1; // 传送方向 0-由主站发出的命令帧 1-由从站发出的应答帧
}MRS_645_CTRL_STRU;

typedef struct 
{
    HI_U8 ucFn        :6; // 请求及应答功能码    
    HI_U8 ucSlaveFlag :1; // 从站异常标志 0-从站正确应答 1-从站对异常信息的应答
    HI_U8 ucCom       :1; // 通信是否正常0 正常 1 异常
}MRS_188_CTRL_STRU;

// 645 帧头信息结构体
typedef struct 
{
	HI_U8 nBroadcast;
	HI_U8 ucCtrl;						// 控制码
	HI_U8 ucErrorResFlag;				// 需要错误回复数据，主要用于搜表时有多个表匹配的搜表
	BYTE nProType;						// 协议类型
	HI_U8 ucAddr[ADRESS_LEN];           // 表地址
	int ucDataRealmLen;				// 数据域大小必须小于2048字节, 0表示无数据域
    HI_U8 ucDataRealm[PROTO_DATAGRAM_DATA_LEN_MAX];	// 数据域 
	int nIndex; // 电表在电表数组中的索引
	int ucDataSendLen;				// 数据域, 0表示无数据域
    HI_U8 ucDataSend[PROTO_DATAGRAM_DATA_LEN_MAX];	// 数据域 

    HI_U8 ucVer;						// 协议版本,取值为 MRS_PROTO645_VER_XXX 
	MRS_645_CTRL_STRU st645Ctrl;		// 控制码

	HI_U8 uc188Type;					// 操作设备类型
	// 698	协议相关的处理

	HI_U8 uc698AdrssLen; // 地址长度和单播还是广播



    
}MRS_FRAME_STRU;
#define MAETER_EVENT_COUNT 4
typedef struct
{
	int nValid;
	BYTE address[16];
	BYTE protocol;
	BYTE nAddrLen;
	BYTE IsSupportEventState;
	int nEvent[MAETER_EVENT_COUNT];
}Meter;
#define MAX_METER_COUNT 64
typedef struct
{
	BYTE numMeter;
	Meter sMeter[MAX_METER_COUNT];
}MeterArray;

#define PROTOCOL_DI_COUNT 32
typedef struct
{
	BYTE isValid;
	unsigned int nDI;
	int len;
	BYTE nPro;
	BYTE nFunID;
	BYTE DIData[128];
}ProDI;

typedef struct
{
	ProDI DI[PROTOCOL_DI_COUNT];
	int nCount;
}ProDIData;

//数据标识码处理函数
typedef HI_U32 (*PROTOCOL_DI_FUN)(MRS_FRAME_STRU* pstFrame);

typedef struct
{
    HI_U32              ulMinDI;
    HI_U32              ulMaxDI;
    PROTOCOL_DI_FUN   func;
} MRS_IRDA_DI_PAIR_STRU;

PROTOCOL_DI_FUN mrs645_Control(MRS_FRAME_STRU* pstFrame);

MeterArray *getMeterArray(void);
ProDIData *getProDIData(void);

void RefreshMeter(void);

HI_U32 mrs188_All_DItest(MRS_FRAME_STRU* pstFrame);
HI_U32 mrsIr645_11H_DI33FFFFFF(MRS_FRAME_STRU* pstFrame);
HI_U32 mrsIr645_11H_DI34FFFFFF(MRS_FRAME_STRU* pstFrame);

HI_U32 mrsDI(MRS_FRAME_STRU* pstFrame,BYTE nPro,HI_U32 nDI);

HI_U32 mrsIr645_11H_07_SM(MRS_FRAME_STRU* pstFrame);
HI_U32 mrsIr645_11H_DI37334834(MRS_FRAME_STRU* pstFrame);
HI_U32 mrsIr645_11H_DI38393334(MRS_FRAME_STRU* pstFrame);
HI_U32 mrsIr645_11H_DI38393434(MRS_FRAME_STRU* pstFrame);
HI_U32 mrsIr645_11H_DI37334834(MRS_FRAME_STRU* pstFrame);
HI_U32 mrsIr645_11H_DI33343233(MRS_FRAME_STRU* pstFrame);
HI_U32 mrsIr645_11H_DI33333333(MRS_FRAME_STRU* pstFrame);

HI_U32 mrsIr645_11H_DI4C343334(MRS_FRAME_STRU* pstFrame);
HI_U32 mrsIr645_11H_DI4C353334(MRS_FRAME_STRU* pstFrame);
HI_U32 mrsIr645_11H_DI4C363334(MRS_FRAME_STRU* pstFrame);

HI_U32 mrsIr645_11H_DI36633433(MRS_FRAME_STRU* pstFrame);
HI_U32 mrsIr645_11H_DI36634033(MRS_FRAME_STRU* pstFrame);
HI_U32 mrsIr645_11H_DI33343333(MRS_FRAME_STRU* pstFrame);
HI_U32 mrsIr645_11H_DI37333435(MRS_FRAME_STRU* pstFrame); // 查询时间
HI_U32 mrsIr645_11H_DI37333434(MRS_FRAME_STRU* pstFrame); // 查询日期星期

HI_U32 mrsIr645_11H_DI37333834(MRS_FRAME_STRU* pstFrame); // 查询电池电压运行情况

HI_U32 mrsIr645_11H_DI37334536(MRS_FRAME_STRU* pstFrame); // 查询日冻结时间

HI_U32 mrsIr645_11H_DI35343433(MRS_FRAME_STRU* pstFrame); // 查询A相电压
HI_U32 mrsIr645_11H_DI35353433(MRS_FRAME_STRU* pstFrame); // 查询A相电流

HI_U32 mrsIr645_12H_ALL(MRS_FRAME_STRU* pstFrame);

HI_U32 mrsIr645_13H_00(MRS_FRAME_STRU* pstFrame);
HI_U32 mrsIr645_13H_3E3D3E3D(MRS_FRAME_STRU* pstFrame); //I采 启动搜表
HI_U32 mrsIr645_13H_3E3F3E3F(MRS_FRAME_STRU* pstFrame); // I采 停止搜表
HI_U32 mrsIr645_13H_3E403E40(MRS_FRAME_STRU* pstFrame); // 读取搜表结果
HI_U32 mrsIr645_13H_3E3D3F33(MRS_FRAME_STRU* pstFrame); // I采集器版本

HI_U32 mrsIr645_1EH_03(MRS_FRAME_STRU* pstFrame); //I采数据

HI_U32 mrs64507_All_DenyFrame(MRS_FRAME_STRU* pstFrame);
HI_U32 mrsIr645_11H_DI36443333(MRS_FRAME_STRU* pstFrame);

HI_U32 mrsIr645_11H_97_SM(MRS_FRAME_STRU* pstFrame);
HI_U32 mrsIr645_11H_97_C343(MRS_FRAME_STRU* pstFrame);
HI_U32 mrsIr645_11H_97_D343(MRS_FRAME_STRU* pstFrame);
HI_U32 mrs64597_All_DItest(MRS_FRAME_STRU* pstFrame);

HI_U32 Check698Frame(HI_PBYTE ptr,HI_U16 len);
HI_U16 CalcFcs16Ex(HI_U8 *cp, HI_U16 len);
HI_U32 mrsIr698_SM(MRS_FRAME_STRU* pstFrame);

HI_U32 Find645Frame(HI_IN HI_PBYTE in_buf, HI_IN HI_S16 in_len, HI_OUT HI_U16 *start, HI_OUT HI_U16 *out_len);
HI_U32 Find698Frame(HI_IN HI_PBYTE in_buf, HI_IN HI_S16 in_len, HI_OUT HI_U16 *start, HI_OUT HI_U16 *out_len);
HI_U32 Find188Frame(HI_IN HI_PBYTE in_buf, HI_IN HI_S16 in_len, HI_OUT HI_U16 *start, HI_OUT HI_U16 *out_len);

int MRS_Proto645Dec( HI_IN HI_PBYTE pucDatagram, int usDatagramSize, HI_OUT MRS_FRAME_STRU* pstFrame );
int MRS_Proto188Dec( HI_IN HI_PBYTE pucDatagram, int usDatagramSize, HI_OUT MRS_FRAME_STRU* pstFrame);
int MRS_Proto698Dec( HI_IN HI_PBYTE pucDatagram, int usDatagramSize, HI_OUT MRS_FRAME_STRU* pstFrame );

int MRS_Proto188Enc( HI_IN HI_PBYTE pucDatagram, int *pusDatagramSize, HI_OUT MRS_FRAME_STRU* pstFrame);
int MRS_Proto645Enc( HI_IN HI_PBYTE pucDatagram, int *pusDatagramSize, HI_OUT MRS_FRAME_STRU* pstFrame);
int MRS_Proto698Enc( HI_IN HI_PBYTE pucDatagram, int *pusDatagramSize, HI_OUT MRS_FRAME_STRU* pstFrame);

int MRS_ProtoWirelessDec( HI_IN HI_PBYTE pucDatagram, int usDatagramSize, HI_OUT MRS_FRAME_STRU* pstFrame);
int MRS_ProtoWirelessEnc( HI_IN HI_PBYTE pucDatagram, int *pusDatagramSize, HI_OUT MRS_FRAME_STRU* pstFrame);

UINT32 get64507SmDI(void);
UINT32 get64597SmDI(void);

void set64507SmDI(UINT32 n);
void set64597SmDI(UINT32 n);
#endif