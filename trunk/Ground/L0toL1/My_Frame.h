//记录一个科学数据帧的所有信息-------------------------------------------
#ifndef _MY_FRAME_H
#define _MY_FRAME_H

#include<fstream>
using namespace std;


class MyClass
{
public:
	MyClass();
	int good;									//是否正常，通过打开文件刷新good
public://科学数据帧特征量
	int counts;									//读取数据帧计数
	int fee;									//FEE编号
	int length;									//数据帧长
	int readevents,goodevents;					//第几个科学数据
	char buffer[2000];							//一个帧的数据
public://CRC校验
	unsigned short CheckCRC(char* buf,int len);//把数据帧放入CRC校验
	unsigned char crc_reg[16];
	friend UINT CheckCRC_Thread( LPVOID params );
public://文件操作
	CString InputFileName;
	CString OutputFileName;
	ofstream ofile;
	ifstream ifile;
	void OpenFile();							//打开L0数据，新建L1文件（文件名后+0）
	int filelength;								//一整个文件的长度
	int readlength;								//已读取文件的长度
public://信息
	CList<CString,CString> l_msg;				//信息

	void Reset();
};

extern MyClass my;//方便其他源程序引用---------------------


#endif	//#ifndef _MY_FRAME_H
