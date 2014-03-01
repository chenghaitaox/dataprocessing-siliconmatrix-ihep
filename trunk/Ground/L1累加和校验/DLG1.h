#pragma once


// CDLG1 对话框

class CDLG1 : public CDialog
{
	DECLARE_DYNAMIC(CDLG1)

public:
	CDLG1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDLG1();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
