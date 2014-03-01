#pragma once


// CDLG3 对话框

class CDLG3 : public CDialog
{
	DECLARE_DYNAMIC(CDLG3)

public:
	CDLG3(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDLG3();

// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
