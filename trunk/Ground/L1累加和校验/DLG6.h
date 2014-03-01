#pragma once


// CDLG6 对话框

class CDLG6 : public CDialog
{
	DECLARE_DYNAMIC(CDLG6)

public:
	CDLG6(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDLG6();

// 对话框数据
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
