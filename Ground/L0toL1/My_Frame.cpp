#include"stdafx.h"

void MyClass::Reset()
{
	good=0;
	filelength=0;readlength=0;
	readevents=0;goodevents=0;
	ifile.close();ifile.clear();
	ofile.close();ofile.clear();
	l_msg.RemoveAll();	
}
MyClass::MyClass()
{
	Reset();
}
unsigned short MyClass::CheckCRC(char* buf,int len)//模仿张飞的程序
{
	int i,j;
	unsigned char t;
	memset(crc_reg,1,16);
	for(i=0;i<len;i++)
	{
		for(j=0;j<8;j++)
		{	
			t=crc_reg[15]^(((buf[i]&0x00ff)>>(7-j))&0x01);//从帧计数开始加入CRC校验
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


	unsigned short tt=0;
	for(i=0;i<16;i++)
	{
		tt|=(crc_reg[i]<<i);
	}
	return tt;
}

void MyClass::OpenFile()
{
	CString str;


	Reset();												//全部复位


	ifile.open(InputFileName,ios::binary);					//打开L0数据，推荐二进制打开
	if(!ifile.is_open())									//是否成功打开
	{
		str=InputFileName;
		str+=CString(" Not Opened");
		l_msg.AddTail(str);
		return ;
	}


	ifile.seekg(0,ios::end);								//找长度必备
	filelength=ifile.tellg();								//找到长度
	if(filelength<2)         								//读取字节长度太小
	{
		str=InputFileName;
		str+=CString(" Buffersize too small");
		l_msg.AddTail(str);
		return ;
	}


	ifile.seekg(0,ios::beg);								//读取指针回到开头
	str=InputFileName;
	str+=CString(" Successfully Opened");
	l_msg.AddTail(str);


	OutputFileName=InputFileName;OutputFileName+=CString("0");//构造输出文件名
	ofile.open(OutputFileName,ios::binary);					//新建L1文件
	if(!ofile.is_open())									//是否成功打开
	{
		str=OutputFileName;
		str+=CString(" Not Opened");
		l_msg.AddTail(str);
		return ;
	}
	else
	{
		str=OutputFileName;
		str+=CString(" Successfully Created");
		l_msg.AddTail(str);
	}


	good=1;													//标记成功！！！！！！！！！！
}
UINT CheckCRC_Thread( LPVOID params )
{
	if(my.good!=1) return 0;									//无正确输入文件
	if(my.filelength<1)return 0;								//无文件长度
	if(my.ifile.tellg()<0 || my.ifile.tellg()>my.filelength)return 0;//已读完
	my.l_msg.AddTail(CString("######################"));
	my.l_msg.AddTail(CString("CRC Check Start"));


	CString str;
	char ch;
	while(!my.ifile.eof() && my.good==1)
	{
		my.ifile.get(ch);
		if((ch&0xff)==0xEB)
		{
			my.ifile.get(ch);
			if((ch&0xff)==0x90)								
			{
				//找到eb90------------------------------------------------------------
				++my.readevents;								//读取科学数据量+1
				my.ifile.read(my.buffer,4);


				my.counts=int(my.buffer[0]&0x00ff);				//读取数据帧计数
				my.fee=int(my.buffer[1]&0x003f);					//读取fee，注意占6bit
				my.length=int((my.buffer[2]<<8)&0xff00)+int(my.buffer[3]&0x00ff);//读取长度
//				if(my.length<16 || my.length>1996)//数据长度越界
				if(my.length<1 || my.length>1996)//数据长度越界
				{
					//     To   Do  
					str.Format(__T("包长度越界，长度为%d，位置%d"),my.length,int(my.ifile.tellg()));
					my.l_msg.AddTail(str);
					continue;
				}


				my.ifile.read(my.buffer+4,my.length-2);				//读取一个帧的科学数据+CRC校验码


				if(my.CheckCRC(my.buffer,my.length+2))				//CRC校验，长度为数据帧长+2
				{
					str.Format(__T("CRC Error: EventID(从1开始)=%d,FrameCount=%d, FEE=%d, Length=%d")
						,my.readevents,my.counts,my.fee,my.length);
					my.l_msg.AddTail(str);
					continue;
				}
				else
				{
					++my.goodevents;
					my.ofile.write(my.buffer+4,my.length-4);			//写入一个帧的科学数据到文件
				}
			}
		}
		my.readlength=my.ifile.tellg();
	}


	//CRC校验结束
	if(my.good==1)
	{
		my.readlength=my.filelength;
		my.l_msg.AddTail(CString("Check End!"));
	}
	else if(my.good==2)
	{
		my.l_msg.AddTail(CString("Too Much Error, Program Break!!!"));
	}
	else if(my.good==3)
	{
		my.l_msg.AddTail(CString("BUG Exist, Program Break!!!"));
	}
	my.l_msg.AddTail(CString("######################"));


	//关闭输入输出文件
	my.ifile.close();my.ifile.clear();
	my.ofile.close();my.ofile.clear();
	
	
	//重置状态
	my.good=0;


	return 0;
}
MyClass my;//初始化，方便其他文件引用
