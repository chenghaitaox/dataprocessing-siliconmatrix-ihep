// L1累加和校验Dlg.h : 头文件
//

#pragma once
#include"DLG1.h"
#include"DLG2.h"
#include"DLG3.h"
#include"DLG4.h"
#include"DLG5.h"
#include"DLG6.h"
#include"DLG7.h"
#include"DLG8.h"
#include<fstream>
using namespace std;
const int MODENUM=7;


// CSumDlg 对话框
class CSumDlg : public CDialog
{
// 构造
public:
	CSumDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_L1_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	//选项卡
	CTabCtrl tab;
	afx_msg void TAB_Change(NMHDR *pNMHDR, LRESULT *pResult);


	//进度条
	int filelength;
	CProgressCtrl bar;
	CStatic progress;
	void ShowProcess(int p=-1);


	//文件
	afx_msg void BTN_Open();
	ifstream ifile;
	ofstream ofile;
	int good;


	//累加和校验
	int sum,readsum;//累加和
	int mode,readmode;//0正常模式，1原始模式，2刻度模式，3基线更新，4下传模式，5空占位包，6忙占位包
	char buffer[40000];//找到EEBB后，一次读取整个包的数据
	friend UINT thread_SumCheck(LPVOID params);//线程
	afx_msg void BTN_Check();


	//统计
	int mode_num[MODENUM],error_num[MODENUM];
	CArray<CStatic*,CStatic*> v_modenum;
	CArray<int,int> v_modenumIDC;
	CArray<CStatic*,CStatic*> v_errornum;
	CArray<int,int> v_errornumIDC;
	void InitialStatic();


	//子对话框
	CArray<CDialog*,CDialog*> v_subdlg;
	CArray<CString,CString> v_subdlgname;
	CArray<int,int> v_subdlgIDD;
	void InitialSubDialog();


	//列表框显示出错位置
	CArray<CListBox*,CListBox*> v_listbox;
	CArray<int,int> v_listboxIDC;
	void InitialListBox();



	//重置
	void ResetAll();
};
