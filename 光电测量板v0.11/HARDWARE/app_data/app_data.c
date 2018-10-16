#include "app_data.h"
#include "scan.h"

static u8 JudgeFrame(const u8* data,u16 len);
static sFrame RecFrame,SedFrame;

static u8 JudgeFrame(const u8* outdata,u16 len,u8* data);
static void FrameStructure(u8* indata, u16 inlen,u8* outdata, u8* outlen);
static u8 JudgementFixationFrame(u8* indata,u16 inlen);
//void InitAppData(void)
//{
//	Frame.header =  FRAME_HEX_HEADER;
//	Frame.tail = FRAME_HEX_TAIL;
//}
void AppDataDeal(const u8* data,u16 len)
{
	u8 i,ret = 0xff;
	u8 rec_ret; //���ջظ���0������ظ���1����Ҫ�ظ�
	u8 ret_buf[50];
	u16 ret_len = 50;
	//*******���ղ���*******//
	if(data[0] == FRAME_HEX_HEADER)
	{
		//��֡Ϊ0x68
		ret = JudgeFrame(data,len);
	}
	else
	{
		//��֡�͵�һ���ֽڶ�Ӧ���ϣ�������������֡
		for(i=0;i<len;i++)
		{
			if(data[i] == FRAME_HEX_HEADER)
			{
				//�ҵ���֡
				ret=JudgeFrame(&data[i],len-i);
				break;
			}
		}
	}
	if(ret == 0xff)
	{
		//�������û���ҵ�֡ͷ
	}
	else if(ret == 0)
	{
		//���ճɹ�
	}
	else
	{
		//���մ��� 
		if(ret == 1)
		{
			//֡������
		}
		else if(ret == 2)
		{
			//֡����С��4
		}
		else
		{
			//���� �����ܿ�
		}
	}
	//********���Ͳ���*********//
	if(rec_ret)
	{
		rec_ret = 0;
		if(RecFrame.type == HANDSHAKE_FRAMETYPE)
		{	//�ظ�����
			
			
			FrameStructure(SedFrame,ret_buf,&ret_len);//�ӹ��������ظ���֡ ������ʽ���
		}
	}
	
}
/* 	֡�жϺ��� ��⵽֡ͷִ�У�����֡��Ӧ����ṹ���� 
	���أ�0 ��ȷ���գ� 1 ֡������ 2 ֡����С��4*/
static u8 JudgeFrame(const u8* outdata,u16 len,u8* data)	//len ���Ƴ��� �������󳤶� 
{
	u16 tem_len,tem_hard;
	u8 i,ret=0;
	
	RecFrame.header = outdata[0];
	
	tem_len |= outdata[1]<<8;
	tem_len |= outdata[2];
	RecFrame.len = tem_len;
	
	RecFrame.type = outdata[3];
	
	RecFrame.version = outdata[4];
	
	tem_hard |= outdata[5]<<8;
	tem_hard |= outdata[6];
	RecFrame.hard = tem_hard;
	
	//����֤֡�����ԣ��鿴֡β�Ƿ���ȷ
	if(outdata[tem_hard+3] == FRAME_HEX_TAIL) //3 = 1 + 2 ֡ͷ һ���ֽ� ���� �����ֽ�
	{
		if(tem_hard<4)
		{
			RecFrame.FrameData.len = tem_hard - 4;//���ݳ��ȵ��� �ܳ��ȼ�4 2���ֽڵ�Ӳ���� 1���ֽڰ汾�� ��1�����ӽ�֡����
			//��ȷ,����֡����
			if(RecFrame.FrameData.len == 0) 
			{	//������λ ����Ϊ ���ֵ�Э��
				
			}
			else
			{
				for(i=0;i<RecFrame.FrameData.len;i++)
				{
					RecFrame.FrameData.data[i] = outdata[i+7];
				}
			}
			//֡��ȷ ret = 0
		}
		else
		{
			ret = 2; //tem_hard < 4 ��ʱû��Э��֡����С��4
		}
	}
	else
	{
		ret = 1;	//֡������
	}
	return ret;
}
//��װ���� �����͵�Э���װ
static void FrameStructure(u8* indata, u16 inlen,u8* outdata, u8* outlen)
{
	u16 len = FIXATION_LEN+inlen;
	
	outdata[0] = FRAME_HEX_HEADER;
	outdata[1] = (u8)(len>>8);
	outdata[2] = (u8)len;
	outdata[3] = 
	outdata[inlen+FIXATION_LEN] = FRAME_HEX_TAIL;
	
}
//�ж�֡�Ĺ̶�֡ ���� 1:�ɹ� 0:����汾��Ӧ���� 2:Ӳ���汾��Ӧ����
//ps:��Ҫ�Ƚ����������ͨ��֡�������ж�
static u8 JudgementFixationFrame(u8* indata,u16 inlen)
{
	u8 ret;
	u8 version;
	u16 hard;
	
	version = indata[4];
	hard |= ((u16)indata[5])<<8;
	hard |= (u16)indata[6];
	
	if(version == SOFTWARE_VERSION)
	{
		//����汾�ܶ�Ӧ��
		if(hard == LGJType)	
		{
			//Ӳ���汾�ܶ�Ӧ��	ps:��ʱ��֧��ֱ��Э���޸�Ӳ���汾��֮�������� ���ΪӲ������Ϊ����
			ret = 1;
		}
		else
		{
			ret = 2;
		}
	}
	else
	{
		ret = 0;
	}
	return ret;
}
static void FrameProperty(sFrame* frame,u8 type)
{
	//����֡���ض����� ��������
	frame->header = FRAME_HEX_HEADER;
	frame->tail = FRAME_HEX_TAIL;
	frame->version = FRAME_SOFTWARE;
	frame->hard = LGJType;
	switch(type)
	{
		case HANDSHAKE_FRAMETYPE:
		{//����
			frame->type = HANDSHAKE_FRAMETYPE;
			frame->FrameData.len = 2;
			frame->len = frame->FrameData.len+4;//֡���� Ϊ����֡ͷ ֡β�ĳ��� 4Ϊ֡���Ͱ汾�Ż���Ӳ���汾�� 
			frame->FrameData.data
			break;
		}
		case 
	}
			
}

//��װ���� �����͵�Э���װ
static void FrameStructure(sFrame frame,u8* outdata, u16* outlen)
{
	u8 i;
	
	outdata[0] = frame.header;
	outdata[1] = (u8)frame.len>>8;
	outdata[2] = (u8)frame.len;
	outdata[3] = frame.type;
	outdata[4] = frame.version;
	outdata[5] = (u8)frame.hard>>8;
	outdata[6] = (u8)frame.hard;
	if((frame.FrameData.len>0)&&(frame.FrameData.len<*outlen))
	{
		for(i=0;i<frame.FrameData.len;i++)
		{
			outdata[7+i] = frame.FrameData.data[i];
		}
	}
	outdata[frame.FrameData.len+FIXATION_LEN] = frame.tail;
	*outlen = FIXATION_LEN+frame.FrameData.len;
}
