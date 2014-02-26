//记录一个科学数据帧的所有信息-------------------------------------------
#ifndef _MY_FRAME_H
#define _MY_FRAME_H

#include<fstream>
#include<iostream>
#include <cstring>
using namespace std;
#include"L0toL1.h"
#include"L0toL1Dlg.h"
#include"Resource.h"


class MyClass
{
public:
	MyClass();
	int good;									//是否正常，通过打开文件刷新good
public://科学数据帧特征量
	int counts;									//读取数据帧计数
	int fee;									//FEE编号
	int length;									//数据帧长
	int events;									//第几个科学数据
public://CRC校验
	unsigned short CheckCRC(char* buf,int len);//把数据帧放入CRC校验
	unsigned char crc_reg[16];
	friend UINT CheckCRC_Thread( LPVOID params );
public://文件操作
	CString InputFileName;
	CString OutputFileName;
	ofstream ofile;
	ifstream ifile;
	void OpenFiles(LPVOID params);				//打开L0数据，新建L1文件（文件名后+0）
	long buffersize;							//一整个文件的长度
public://产生测试数据
	void GenerateTestData();
	void Test(LPVOID params);
};

extern MyClass my;//方便其他源程序引用---------------------


#endif	//#ifndef _MY_FRAME_H
