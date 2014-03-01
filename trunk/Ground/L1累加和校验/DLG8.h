#pragma once


// CDLG8 对话框

class CDLG8 : public CDialog
{
	DECLARE_DYNAMIC(CDLG8)

public:
	CDLG8(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDLG8();

// 对话框数据
	enum { IDD = IDD_DIALOG8 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
