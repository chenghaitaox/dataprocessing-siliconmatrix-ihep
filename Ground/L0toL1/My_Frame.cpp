#include "stdafx.h"//必须包含，且必须在其他.h文件之前，否则还是出错。
#include"My_Frame.h"

//#define Error(err) ErrorCode=err;exit(0);

MyClass::MyClass()
{
	buffersize=-1;events=0;
	good=0;
}
unsigned short MyClass::CheckCRC(char* buf,int len)//模仿张飞的程序
{
	int i,j;
	char t;
	memset(crc_reg,1,16);
	for(i=0;i<len;i++)
	{
		for(j=0;j<8;j++)
		{	
			t=crc_reg[15]^((buf[i]>>(7-j))&0x01);//从帧计数开始加入CRC校验
			crc_reg[15]=crc_reg[14];
			crc_reg[14]=crc_reg[13];
			crc_reg[13]=crc_reg[12];
			crc_reg[12]=crc_reg[11]^t;
			crc_reg[11]=crc_reg[10];
			crc_reg[10]=crc_reg[9];
			crc_reg[9]=crc_reg[8];
			crc_reg[8]=crc_reg[7];
			crc_reg[7]=crc_reg[6];
			crc_reg[6]=crc_reg[5];
			crc_reg[5]=crc_reg[4]^t;
			crc_reg[4]=crc_reg[3];
			crc_reg[3]=crc_reg[2];
			crc_reg[2]=crc_reg[1];
			crc_reg[1]=crc_reg[0];
			crc_reg[0]=t;
		}
	}


	unsigned short tt;
	for(i=0;i<16;i++)
	{
		tt|=(crc_reg[i]<<i);
	}
	return tt;
}

void MyClass::OpenFiles(LPVOID params)
{
	CL0toL1Dlg *p_dlg=(CL0toL1Dlg*)params;					//得到窗口句柄
	if(!buffersize)	buffersize=0;							//防止已经读过一遍
	good=0;


	ifile.close();ifile.clear();							//清空输入流
	ifile.open(InputFileName,ios::binary);					//打开L0数据，推荐二进制打开
	if(!ifile.is_open())									//是否成功打开
	{
		p_dlg->Show_Log(my.InputFileName+(CString)"  Not Opened");Sleep(300);
		p_dlg->Show_Log(__T("---------------------------------------------------------------------------------------------------------------------"));
		return ;
	}


	ifile.seekg(0,ios::end);								//找长度必备
	buffersize=ifile.tellg();								//找到长度
	if(buffersize<2)         								//读取字节长度太小
	{
		p_dlg->Show_Log(my.InputFileName+(CString)"  Buffersize too small");
		return ;
	}
	ifile.seekg(0,ios::beg);								//读取指针回到开头
	p_dlg->Show_Log(my.InputFileName+(CString)"  L0 DataFile Successfully Opened");Sleep(500);
	p_dlg->Show_Log(__T("---------------------------------------------------------------------------------------------------------------------"));


	ofile.close();ofile.clear();							//清空输出流
	ofile.open((InputFileName+(CString)"0"),ios::binary);	//新建L1文件
	if(!ofile.is_open())									//是否成功打开
	{
		p_dlg->Show_Log(my.InputFileName+(CString)"0  Not Created");
		return ;
	}


	good=1;
	p_dlg->Show_Log(my.InputFileName+(CString)"0  L1 DataFile Successfully Created");Sleep(500);
	p_dlg->Show_Log(__T("---------------------------------------------------------------------------------------------------------------------"));
}
UINT CheckCRC_Thread( LPVOID params )
{
	CL0toL1Dlg *p_dlg=(CL0toL1Dlg*)params;
	char msg[1000];
	char buffer[2000];							//一个帧的数据
	int readnumber=0;								//总共读入多少数据

//	p_dlg->Show_Log(("######################"));
//	p_dlg->Show_Log("Check Start");

	char buf;
	while(!my.ifile.eof())
	{
		my.ifile.get(buf);readnumber++;
		if(buf==0xEB)
		{
			my.ifile.get(buf);readnumber++;
			if(buf==0x90)									//找到eb90
			{
				my.events++;									//读取科学数据量+1

				my.ifile.read(buffer,4);

				my.counts=int(buffer[0]&0x00ff);				//读取数据帧计数
				my.fee=int(buffer[1]&0x003f);					//读取fee，注意占6bit
				my.length=int((buffer[2]<<8)&0xff00)+int(buffer[3]&0x00ff);//读取fee，注意占6bit
				

				my.ifile.read(buffer+4,my.length-2);				//读取一个帧的科学数据+CRC校验码
				readnumber+=(my.length+2);


				if(my.CheckCRC(buffer,my.length+2))				//CRC校验，长度为数据帧长+2
				{
					sprintf(msg,"CRC Error: FrameCount=%d, FEE=%d, FrameLength=%d，FrameID",my.counts,my.fee,my.length,my.events);
					p_dlg->Show_Log(msg);
				}
				else
				{
					my.ofile.write(buffer+4,my.length-4);			//写入一个帧的科学数据到文件
				}


				if(my.events%10==0)							//每读100个帧打印一次
				{
					sprintf(msg,"Process %04d event",my.events);
					p_dlg->text_packet.SetWindowTextW((CString)msg);
					sprintf(msg,"Processed %04d Bytes --> %3.1f%%",readnumber+1,(readnumber+1)*100.0/my.buffersize);
					p_dlg->text_size.SetWindowTextW((CString)msg);
				}
			}
		}
	}

	//关闭输入输出文件
	my.ifile.close();my.ifile.clear();
	my.ofile.close();my.ofile.clear();

	//CRC校验结束
//	p_dlg->Show_Log(("Check End!"));
//	p_dlg->Show_Log(("######################"));
	sprintf(msg,"Process %04d event",my.events);
	p_dlg->text_packet.SetWindowTextW((CString)msg);
	sprintf(msg,"Processed %04d Bytes --> %3.1f%%",my.buffersize,100);
	p_dlg->text_size.SetWindowTextW((CString)msg);

	return 0;
}
int GenerateEvent(char* ch,int len)
{
	int i=0,j;
	ch[i++]=(char)1;
	ch[i++]=(char)2;
	ch[i++]=(char)3;
	ch[i++]=(char)4;
	ch[i++]=(char)5;
	ch[i++]=(char)6;
	ch[i++]=(char)7;
	ch[i++]=(char)8;
	ch[i++]=(char)9;
	ch[i++]=(char)10;

	ch[i++]=0xeb;
	ch[i++]=0x90;
	ch[i++]=(char)20;//帧计数
	ch[i++]=(char)30;//FEE
	ch[i++]=((len&0xFF00)>>8);//帧长度1
	ch[i++]=(len&0x00FF);//帧长度2

	for(j=0;j<len-4;j++)
	{
		ch[i+j]=(char)j;
	}

	ch[i+len-4]=(char)40;//CRC 1
	ch[i+len-3]=(char)50;//CRC 2

	return i+len-2;
}
void MyClass::GenerateTestData()
{
	ofstream oofile("test.txt",ios::binary);
	char ch[10000];
	int l,i;
	for(i=500;i<600;i++)
	{
		l=GenerateEvent(ch,i*2);	oofile.write(ch,l);
	}
	oofile.close();
}
void MyClass::Test(LPVOID params)
{
	CL0toL1Dlg *p_dlg=(CL0toL1Dlg*)params;

	p_dlg->Show_Log(CString("hahahahahahahahahaha"));
}

MyClass my;//初始化，方便其他文件引用
