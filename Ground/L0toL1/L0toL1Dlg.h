// L0toL1Dlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "My_Frame.h"

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
	afx_msg void OnBnClickedBtn_Clearmsg();
	afx_msg void OnBnClickedBtn_CRCcheck();
	afx_msg void OnBnClickedBtn_OpenL0Data();
	afx_msg void OnBnClickedBtn_OutputL1Data();
	afx_msg void OnBnClicked_Exit();
public:
	CListBox m_list_log;
	CStatic text_packet,text_size,text_allsize;
	void Show_Log(CString s);
	void Show_Log(char* s);
	afx_msg void OnBnClickedBtn_GenerateTest();
};
