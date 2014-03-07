// L0toL1Dlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

// CL0toL1Dlg 对话框
class CL0toL1Dlg : public CDialog
{
// 构造
public:
	CL0toL1Dlg(CWnd* pParent = NULL);	// 标准构造函数
// 对话框数据
	enum { IDD = IDD_L0TOL1_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	//列表框
	CListBox LISTBOX_control;
	afx_msg void OnBnClickedBtn_Clearmsg();
	void Reset_Listbox();
	void Display_Listbox();
	//CRC校验
	afx_msg void OnBnClickedBtn_CRCcheck();
	//进度
	CProgressCtrl PROGRESS1_control;
	CStatic STATIC_PROGRESS_control;
	void Display_Process();
	void Reset_Process();
	//计时器
	CStatic STATIC_TIMER_control;
	long timer_begin;
	int timer_used,timer_remain;
	int minute_used,second_used,minute_remain,second_remain;
	void Display_Timer();
	void Reset_Timer();
	//文件数
	CStatic STATIC_EVENT_control;
	void Display_Event();
	void Reset_Event();
	//显示线程
	friend UINT Thread_Display(LPVOID params);
	//文件
	afx_msg void OnBnClickedBtn_OpenL0Data();
	afx_msg void OnBnClickedBtn_OutputL1Data();
	//退出
	afx_msg void OnBnClicked_Exit();
};
