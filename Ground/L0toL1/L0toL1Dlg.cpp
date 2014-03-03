// L0toL1Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "L0toL1.h"
#include "L0toL1Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

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


// CL0toL1Dlg 对话框




CL0toL1Dlg::CL0toL1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CL0toL1Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CL0toL1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_LIST_MSG,m_list_log);
	DDX_Control(pDX,IDC_STATIC_PACKET,text_packet);
	DDX_Control(pDX,IDC_STATIC_ALREADYSIZE,text_size);
	DDX_Control(pDX,IDC_STATIC_TOTALFILESIZE,text_allsize);
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
	ON_BN_CLICKED(IDC_GenerateTest, &CL0toL1Dlg::OnBnClickedBtn_GenerateTest)
END_MESSAGE_MAP()


// CL0toL1Dlg 消息处理程序

BOOL CL0toL1Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CL0toL1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CL0toL1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


SYSTEMTIME CurTime;
CString now_time;
void CL0toL1Dlg::Show_Log(CString s)
{
	GetLocalTime(&CurTime);
	now_time.Format(__T("(%02d:%02d:%02d)> "),CurTime.wHour,CurTime.wMinute,CurTime.wSecond);
	m_list_log.AddString(now_time+s);
//	m_list_log.SetCurSel(m_list_log.GetCount()-1);
}
void CL0toL1Dlg::Show_Log(char* s)
{
	GetLocalTime(&CurTime);
	now_time.Format(__T("(%02d:%02d:%02d)> "),CurTime.wHour,CurTime.wMinute,CurTime.wSecond);
	m_list_log.AddString(now_time+(CString)s);
//	m_list_log.SetCurSel(m_list_log.GetCount()-1);
}
void CL0toL1Dlg::OnBnClickedBtn_OpenL0Data()
{
	CString testname;
	// TODO: 在此添加控件通知处理程序代码
	char buf[100];
	char szFileFilter[]=
		"Txt File(*.txt)|*.txt|"
		"All File(*.*)|*.*||";
	//打开输入L0数据
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY,(CString)szFileFilter);
	dlg.m_ofn.lpstrInitialDir=__T("."); //设置默认路径
	if (dlg.DoModal()==IDOK)
	{
		my.InputFileName=dlg.GetPathName();
		Show_Log(__T("---------------------------------------------------------------------------------------------------------------------"));
		Sleep(500);
		//读取文件属性到到WIN32_FIND_DATA结构中去,获取文件大小
		WIN32_FIND_DATA fileInfo; 
		long filesize;
		HANDLE hFind = FindFirstFile(my.InputFileName,&fileInfo); 
		if(hFind!=INVALID_HANDLE_VALUE) 
			filesize = fileInfo.nFileSizeLow; 
		FindClose(hFind); 

		sprintf(buf,"Total file size %04d(0x%08X) Bytes",filesize,filesize);
		text_allsize.SetWindowText((CString)buf);
	}

	
	//调用MyClass::OpenFiles()
	my.OpenFiles(this);
}
typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int U32;
#define BIT_OF_WORD(w,i) ((w>>(i))&0x01) 
U8 data_buf[2048];
U16 make_u16(U8* buf)
{
	U16 t=0;
	U8 i;
	
	for(i=0;i<16;i++)
		t|=(buf[i]<<i);

	return t;
}
U16 do_crc(U8 *buf,U32 len)
{
	U32 i,j;
	U8 crc_reg[16];
	U8 t;

	memset(crc_reg,1,16);

	for(i=0;i<len;i++)
	{
		for(j=0;j<8;j++)
		{	
			t=crc_reg[15]^BIT_OF_WORD(buf[i],7-j);
			crc_reg[15]=crc_reg[14];
			crc_reg[14]=crc_reg[13];
			crc_reg[13]=crc_reg[12];
			crc_reg[12]=crc_reg[11]^t;
			crc_reg[11]=crc_reg[10];
			crc_reg[10]=crc_reg[9];
			crc_reg[9]=crc_reg[8];
			crc_reg[8]=crc_reg[7];
			crc_reg[7]=crc_reg[6];
			crc_reg[6]=crc_reg[5];
			crc_reg[5]=crc_reg[4]^t;
			crc_reg[4]=crc_reg[3];
			crc_reg[3]=crc_reg[2];
			crc_reg[2]=crc_reg[1];
			crc_reg[1]=crc_reg[0];
			crc_reg[0]=t;
		}
	}
	return make_u16(crc_reg);
}
void CL0toL1Dlg::OnBnClickedBtn_CRCcheck()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxBeginThread(&CheckCRC_Thread, this);	
}

void CL0toL1Dlg::OnBnClicked_Exit()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}
void CL0toL1Dlg::OnBnClickedBtn_Clearmsg()
{
	// TODO: 在此添加控件通知处理程序代码
	m_list_log.ResetContent();
}

void CL0toL1Dlg::OnBnClickedBtn_GenerateTest()
{
	// TODO: 在此添加控件通知处理程序代码
	my.GenerateTestData();
}
