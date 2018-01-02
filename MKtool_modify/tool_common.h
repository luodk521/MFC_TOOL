#ifndef _TOOL_PROTOCOL_H_
#define _TOOL_PROTOCOL_H_

#define SUPPORT_PROTOCOL_645_07 (0x01<<0)
#define SUPPORT_PROTOCOL_645_97 (0x01<<1)
#define SUPPORT_PROTOCOL_188 (0x01<<2)
#define SUPPORT_PROTOCOL_698 (0x01<<3)
#define SUPPORT_PROTOCOL_WRIELESS (0x01<<4)
#define MAX_LEN 256

typedef unsigned int           HI_U32;
typedef int                    HI_S32;
typedef unsigned short         HI_U16;
typedef signed   short         HI_S16;
typedef unsigned char          HI_U8;
typedef unsigned char			BYTE;
typedef signed char            HI_S8;
typedef void                   HI_VOID;
typedef char                   HI_CHAR;
typedef unsigned char          HI_UCHAR;
typedef HI_U8                  HI_BOOL;
typedef void*                  HI_PVOID;
typedef HI_U8                  HI_BYTE;
typedef HI_PVOID               HI_HANDLE;
typedef HI_BYTE*               HI_PBYTE;
typedef HI_S32                 HI_SOCKET;
typedef float                  HI_FLOAT;
typedef double                 HI_DOUBLE;
typedef unsigned int			UINT32;
typedef unsigned short u16;

BYTE const OSUnMapTbl[256] = {
	0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* 0x00 to 0x0F */
		4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* 0x10 to 0x1F */
		5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* 0x20 to 0x2F */
		4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* 0x30 to 0x3F */
		6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* 0x40 to 0x4F */
		4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* 0x50 to 0x5F */
		5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* 0x60 to 0x6F */
		4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* 0x70 to 0x7F */
		7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* 0x80 to 0x8F */
		4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* 0x90 to 0x9F */
		5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* 0xA0 to 0xAF */
		4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* 0xB0 to 0xBF */
		6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* 0xC0 to 0xCF */
		4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* 0xD0 to 0xDF */
		5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* 0xE0 to 0xEF */
		4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0 /* 0xF0 to 0xFF */
}; 
int reverse(BYTE ch[],int n);
BYTE charToHex(char ch);
BYTE strToHex(char str[],BYTE byte[]);
void strRev(char *s);
HI_U32 IntToBcd(HI_U32 data,HI_U8 bcd[],HI_U8 len);
HI_U32 IntToHex(HI_U32 data);
HI_U32 ByteToHex(BYTE byte[],int n);

u16 pppfcs16(u16 fcs, register unsigned char *cp,int len);
u16 tryfcs16(unsigned char *cp, int len);
#endif