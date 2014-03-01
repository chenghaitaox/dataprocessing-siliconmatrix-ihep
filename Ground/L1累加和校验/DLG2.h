#pragma once


// CDLG2 对话框

class CDLG2 : public CDialog
{
	DECLARE_DYNAMIC(CDLG2)

public:
	CDLG2(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDLG2();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
