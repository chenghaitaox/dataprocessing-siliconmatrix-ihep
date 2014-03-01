#pragma once


// CDLG5 对话框

class CDLG5 : public CDialog
{
	DECLARE_DYNAMIC(CDLG5)

public:
	CDLG5(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDLG5();

// 对话框数据
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
