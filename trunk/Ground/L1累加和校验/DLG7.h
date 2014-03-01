#pragma once


// CDLG7 对话框

class CDLG7 : public CDialog
{
	DECLARE_DYNAMIC(CDLG7)

public:
	CDLG7(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDLG7();

// 对话框数据
	enum { IDD = IDD_DIALOG7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
