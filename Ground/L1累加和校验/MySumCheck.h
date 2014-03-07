#ifndef MYSUMCHECK_H
#define MYSUMCHECK_H

#include<fstream>
using namespace std;


const int MODENUM=7;


class CMySumCheck
{
public:
	CMySumCheck();


	//文件
	ifstream ifile;
	ofstream ofile;
	int good;
	int filelength,readlength;
	int readevents,goodevents;
	CString InputFileName;
	CString OutputFileName;
	void OpenFile();


	//数据包
	int mode,readmode;//0正常模式，1原始模式，2刻度模式，3基线更新，4下传模式，5空占位包，6忙占位包
	void SetReadMode();//buffer[0]读入包类型码(0010~~1101)，然后计算对应的模式(0~~6)，赋值到readmode
	int fee,length,trigger_stat,trigger_id;

	//每个模式有多少个L1.5数据包文件
	int mode_filenum[MODENUM];
	void NewOutputFile();//关闭输出文件。然后根据mode，新建输出文件
	CString OutputFileTitle;


	//累加和校验
	int sum,readsum;//累加和
	char buffer[50000];//找到EEBB后，一次读取整个包的数据
	friend UINT thread_SumCheck(LPVOID params);//线程


	//统计
	int mode_num[MODENUM],error_num[MODENUM];

	
	//系统消息
	CList<CString,CString> l_msg;
	//每种模式的消息
	CList<CString,CString> l_msg_normal,l_msg_raw,l_msg_cali,l_msg_pds,l_msg_down,l_msg_empty,l_msg_busy;


	void Reset();

};
extern CMySumCheck my;


#endif