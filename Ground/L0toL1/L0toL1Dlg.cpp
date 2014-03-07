// L0toL1Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "L0toL1.h"
#include "L0toL1Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void CL0toL1Dlg::Reset_Listbox()
{
	LISTBOX_control.ResetContent();
}
void CL0toL1Dlg::Display_Listbox()
{
	while(!my.l_msg.IsEmpty())
	{
		if(my.l_msg.GetCount()>100)//错误太多啦
		{
			my.good=2;
		}
		LISTBOX_control.InsertString(LISTBOX_control.GetCount(),my.l_msg.RemoveHead());
	}
}
void CL0toL1Dlg::Reset_Process()
{
	PROGRESS1_control.SetRange(0,100);
	PROGRESS1_control.SetPos(0);
	STATIC_PROGRESS_control.SetWindowText(__T("已处理(MB) 0.0  /  总长度(MB) 0.0"));
}
void CL0toL1Dlg::Display_Process()
{
	if(my.filelength<10)
	{
		my.l_msg.AddTail(CString("BUG: L0 Data Length <10"));
		my.good=3;
		return;
	}
	if(my.readlength<0)
	{
		my.l_msg.AddTail(CString("BUG: readlength <0"));
		my.good=3;
		return;
	}
	if(my.readlength>my.filelength)
	{
		my.l_msg.AddTail(CString("BUG: readlength > filelength"));
		my.good=3;
		return;
	}


	timer_used=(GetTickCount()-timer_begin)/1000;
	if(timer_used<0)
	{
		my.l_msg.AddTail(CString("BUG: timer_used <0"));
		my.good=3;
		return;
	}


	CString str;

	str.Format(__T("已处理(MB) %.1lf  /  总长度(MB) %.1lf"),my.readlength/1000000.,my.filelength/1000000.);
	STATIC_PROGRESS_control.SetWindowText(str);

	PROGRESS1_control.SetPos(int(100.*my.readlength/my.filelength));

}

void CL0toL1Dlg::Reset_Timer()
{
	STATIC_TIMER_control.SetWindowText(__T("用时 0分0秒  /  仍需 0分0秒"));
}
void CL0toL1Dlg::Display_Timer()
{
	if(my.filelength<10)
	{
		my.l_msg.AddTail(CString("BUG: L0 Data Length <10"));
		my.good=3;
		return;
	}
	if(my.readlength<0)
	{
		my.l_msg.AddTail(CString("BUG: readlength <0"));
		my.good=3;
		return;
	}
	if(my.readlength>my.filelength)
	{
		my.l_msg.AddTail(CString("BUG: readlength > filelength"));
		my.good=3;
		return;
	}


	CString str;
	minute_used=timer_used/60;
	second_used=timer_used-60*minute_used;
	if(my.readlength<100)
	{
		str.Format(__T("用时 %2d分%2d秒  /  仍需 99分00秒"),minute_used,second_used);
	}
	else
	{
		timer_remain=timer_used*(my.filelength-my.readlength)/(my.readlength);
		minute_remain=timer_remain/60;
		second_remain=timer_remain-60*minute_remain;
		str.Format(__T("用时 %3d分%2d秒  /  仍需 %3d分%2d秒"),minute_used,second_used,minute_remain,second_remain);
	}
	STATIC_TIMER_control.SetWindowText(str);
}
void CL0toL1Dlg::Reset_Event()
{
	STATIC_EVENT_control.SetWindowText(__T("总事件 0  /  好事件 0"));
}
void CL0toL1Dlg::Display_Event()
{
	if(my.readevents<0)
	{
		my.l_msg.AddTail(CString("BUG: readevents <0"));
		my.good=3;
		return;
	}
	if(my.goodevents<0)
	{
		my.l_msg.AddTail(CString("BUG: goodevents <0"));
		my.good=3;
		return;
	}


	CString str;
	str.Format(__T("总事件 %.1d  /  好事件 %.1d"),my.readevents,my.goodevents);
	STATIC_EVENT_control.SetWindowText(str);
}
UINT Thread_Display(LPVOID params)
{
	CL0toL1Dlg *dlg=(CL0toL1Dlg*)params;
	while(my.good==1)
	{
		dlg->Display_Listbox();
		dlg->Display_Process();
		dlg->Display_Event();
		dlg->Display_Timer();
		Sleep(500);
	}	
	dlg->Display_Process();
	dlg->Display_Event();
	dlg->Display_Timer();
	dlg->Display_Listbox();

	return 0;
}
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()
CL0toL1Dlg::CL0toL1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CL0toL1Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CL0toL1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_LIST_MSG,LISTBOX_control);
	DDX_Control(pDX, IDC_PROGRESS, PROGRESS1_control);
	DDX_Control(pDX, IDC_STATIC_PROGRESS, STATIC_PROGRESS_control);
	DDX_Control(pDX, IDC_STATIC_TIMER, STATIC_TIMER_control);
	DDX_Control(pDX, IDC_STATIC_EVENT, STATIC_EVENT_control);
}

BEGIN_MESSAGE_MAP(CL0toL1Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_CLEARMSG, &CL0toL1Dlg::OnBnClickedBtn_Clearmsg)
	ON_BN_CLICKED(IDC_CheckCRC, &CL0toL1Dlg::OnBnClickedBtn_CRCcheck)
	ON_BN_CLICKED(IDC_OpenL0Data, &CL0toL1Dlg::OnBnClickedBtn_OpenL0Data)
	ON_BN_CLICKED(IDCANCEL, &CL0toL1Dlg::OnBnClicked_Exit)
END_MESSAGE_MAP()
BOOL CL0toL1Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);


	//初始化界面
	Reset_Listbox();
	Reset_Process();
	Reset_Timer();
	Reset_Event();


	return TRUE;
}

void CL0toL1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}
void CL0toL1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CL0toL1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CL0toL1Dlg::OnBnClickedBtn_OpenL0Data()
{
	//全部初始化
	Reset_Listbox();
	Reset_Process();
	Reset_Event();
	Reset_Timer();
	my.Reset();


	//得到入L0数据文件名
	char szFileFilter[]=
		"Dat File(*.dat)|*.dat|"
		"All File(*.*)|*.*||";
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY,(CString)szFileFilter);
	if (dlg.DoModal()==IDOK)
	{
		my.InputFileName=dlg.GetPathName();
	}

	
	//调用MyClass::OpenFile()
	my.OpenFile();
	if(my.good!=1)
	{
		Display_Listbox();
		return; 
	}


	//屏幕显示
	Display_Listbox();
	Display_Process();
}
void CL0toL1Dlg::OnBnClickedBtn_CRCcheck()
{
	if(my.good!=1)
	{
		MessageBox(CString("Status Not Ready, Please Open File"));
		return ;
	}


	//计时器
	timer_begin=GetTickCount();


	//开启线程
	AfxBeginThread(&CheckCRC_Thread, this);	
	AfxBeginThread(&Thread_Display, this);	
}

void CL0toL1Dlg::OnBnClicked_Exit()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}
void CL0toL1Dlg::OnBnClickedBtn_Clearmsg()
{
	// TODO: 在此添加控件通知处理程序代码
	LISTBOX_control.ResetContent();
}
