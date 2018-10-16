#ifndef APP_DADA_H__
#define APP_DADA_H__
#include "sys.h"

#define FRAME_HEX_HEADER 	0x68
#define FRAME_HEX_TAIL		0x7f

#define FRAME_SOFTWARE		0x00


//帧类型
#define HANDSHAKE_FRAMETYPE		0x01	

#define HEARTBEAT_FRAMETYPE		0x02

#define DETECTION_FRAMETYPE		0x03

#define MEASURING_FRAMETYPE		0x04

#define END_FRAMETYPE			0x05

#define DATA_FRAMETYPE			0x06
#define INQUIRE_FRAMETYPE		0x06

#define FIXATION_LEN			0x07		//一帧的固定长度 由帧头 长度 类型 版本号 硬件号 组成
#define FIXATION_FRAME_LEN		FIXATION_LEN+1 //固定长度加上帧尾

typedef struct
{
	u8 len;
	u8 data[50];
}sFrameData;

typedef struct 
{
	u8 	header;	//帧头
	u16 len;	//帧长度
	u8 	type;	//帧类型
	u8	version;//版本号
	u16 hard;	//硬件
	sFrameData FrameData;
	u8	tail;	//帧尾
}sFrame;

void AppDataDeal(const u8 *data,u16 len);

#endif