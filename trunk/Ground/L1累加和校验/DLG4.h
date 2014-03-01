#pragma once


// CDLG4 对话框

class CDLG4 : public CDialog
{
	DECLARE_DYNAMIC(CDLG4)

public:
	CDLG4(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDLG4();

// 对话框数据
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
