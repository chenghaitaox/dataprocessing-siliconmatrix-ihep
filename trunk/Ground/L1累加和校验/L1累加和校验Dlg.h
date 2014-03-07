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
#include<time.h>
#include<stdio.h>
#include "afxwin.h"
using namespace std;


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
	afx_msg void BTN_Open();
	afx_msg void BTN_Check();
	//选项卡
	CTabCtrl tab;
	afx_msg void TAB_Change(NMHDR *pNMHDR, LRESULT *pResult);
	void Reset_Tab();


	//进度条
	CProgressCtrl PROGRESS1_control;
	CStatic STATIC_PROGRESS_control;
	void Display_Process();
	void Reset_Process();


	//时间
	CStatic STATIC_TIMER_control;
	long timer_begin;
	int timer_used,timer_remain;
	int minute_used,second_used,minute_remain,second_remain;
	void Display_Timer();
	void Reset_Timer();


	//子对话框
	CArray<CDialog*,CDialog*> v_subdlg;
	CArray<CString,CString> v_subdlgname;
	CArray<int,int> v_subdlgIDD;
	void InitialSubDialog();


	//列表框显示出错位置
	CArray<CListBox*,CListBox*> v_listbox;
	CArray<int,int> v_listboxIDC;
	void InitialListBox();
	void Display_ErrorMessage();//在列表框显示累加和校验出错的信息
	void Reset_ErrorMessage();


	//编辑框显示处理的包数目和累加和出错数目
	CArray<CStatic*,CStatic*> v_modenum;
	CArray<int,int> v_modenumIDC;
	CArray<CStatic*,CStatic*> v_errornum;
	CArray<int,int> v_errornumIDC;
	void InitialStatic();//初始化容器
	void Reset_Statistic();
	void Display_Statistic();//显示某一个模式数目的数目和累加和出错的数目


	//重置
	void ResetAll();


	//直接显示消息
	void Display_SystemMessage();


	//对话框的线程
	void DisplayAll();
	friend UINT Thread_Display(LPVOID params);
};
