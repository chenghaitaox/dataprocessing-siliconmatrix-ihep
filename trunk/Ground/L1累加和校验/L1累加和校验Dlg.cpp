// L1累加和校验Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "L1累加和校验.h"
#include "L1累加和校验Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSumDlg 对话框

void CSumDlg::ResetAll()
{
	//选项卡
	tab.SetCurSel(0);
	for(int i=0;i<v_subdlg.GetSize();i++)
		v_subdlg[i]->ShowWindow(FALSE);
	v_subdlg[0]->ShowWindow(TRUE);


	//进度
	filelength=0;
	progress.SetWindowText(CString("1 / 1"));
	bar.SetRange(0,100);
	bar.SetPos(50);


	//输入输出文件
	good=0;
	ifile.close();ifile.clear();
	ofile.close();ofile.clear();


	//累加和校验
	sum=-1;readsum=-1;
	mode=-1;readmode=-1;


	//统计
	for(int i=0;i<MODENUM;i++)
	{
		mode_num[i]=0;
		error_num[i]=0;
		v_modenum[i]->SetWindowText(CString("1"));
		v_errornum[i]->SetWindowText(CString("1"));
	}


	//列表框
	for(int i=0;i<MODENUM;i++)
	{
		v_listbox[i]->ResetContent();
		v_listbox[i]->AddString(CString("10"));
	}
}
void CSumDlg::InitialListBox()
{
	for(int i=0;i<MODENUM;i++)
	{
		v_listboxIDC.Add(IDC_LIST1+i);
		v_listbox.Add((CListBox*)v_subdlg[i+1]->GetDlgItem(v_listboxIDC[i]));//v_listboxIDC[i]
	}
}
void CSumDlg::InitialStatic()
{
	for(int i=0;i<MODENUM;i++)
	{
		v_modenumIDC.Add(IDC_STATIC11+i);
		v_errornumIDC.Add(IDC_STATIC12+i);
		v_modenum.Add((CStatic*)v_subdlg[0]->GetDlgItem(v_modenumIDC[i]));
		v_errornum.Add((CStatic*)v_subdlg[0]->GetDlgItem(v_errornumIDC[i]));
	}
}
void CSumDlg::InitialSubDialog()
{
	//子对话框的类
	v_subdlg.Add(new CDLG1); 
	v_subdlg.Add(new CDLG2); 
	v_subdlg.Add(new CDLG3); 
	v_subdlg.Add(new CDLG4); 
	v_subdlg.Add(new CDLG5); 
	v_subdlg.Add(new CDLG6); 
	v_subdlg.Add(new CDLG7); 
	v_subdlg.Add(new CDLG8); 


	//子对话框的名字
	v_subdlgname.Add(CString("统计数据"));
	v_subdlgname.Add(CString("正常模式"));
	v_subdlgname.Add(CString("原始模式"));
	v_subdlgname.Add(CString("刻度模式"));
	v_subdlgname.Add(CString("基线更新"));
	v_subdlgname.Add(CString("下传模式"));
	v_subdlgname.Add(CString("空占位包"));
	v_subdlgname.Add(CString("忙占位包"));


	//子对话框的IDD
	v_subdlgIDD.Add(IDD_DIALOG1);
	v_subdlgIDD.Add(IDD_DIALOG2);
	v_subdlgIDD.Add(IDD_DIALOG3);
	v_subdlgIDD.Add(IDD_DIALOG4);
	v_subdlgIDD.Add(IDD_DIALOG5);
	v_subdlgIDD.Add(IDD_DIALOG6);
	v_subdlgIDD.Add(IDD_DIALOG7);
	v_subdlgIDD.Add(IDD_DIALOG8);
}

void CSumDlg::ShowProcess(int p)
{
	char tempchar[100];
	if(p<0||p>100)
	{
		sprintf(tempchar,"%d / %d",int(ifile.tellg()),filelength);//构造读取的字符和总字符，主要要加long
		progress.SetWindowText(CString(tempchar));//静态文本框显示
		bar.SetPos(100.*int(ifile.tellg())/filelength);//显示进度条
	}
	else
	{
		bar.SetPos(p);//直接设置进度条
	}
}
CSumDlg::CSumDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSumDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSumDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);


	DDX_Control(pDX,IDC_TAB1,tab);
	DDX_Control(pDX,IDC_STATIC_PROGRESS,progress);
	DDX_Control(pDX,IDC_PROGRESS1,bar);



}

BEGIN_MESSAGE_MAP(CSumDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CSumDlg::BTN_Open)
	ON_BN_CLICKED(IDC_BUTTON2, &CSumDlg::BTN_Check)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CSumDlg::TAB_Change)
END_MESSAGE_MAP()


// CSumDlg 消息处理程序

BOOL CSumDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标



	//初始化子对话框
	InitialSubDialog();
	for(int i=0;i<v_subdlg.GetSize();i++)
	{
		v_subdlg[i]->Create(v_subdlgIDD[i],&tab);
		tab.InsertItem(i,v_subdlgname[i]);
//		v_subdlg[i]->MoveWindow(rs);
		v_subdlg[i]->ShowWindow(FALSE);
	}
		

	//初始化静态文本框
	InitialStatic();
	

	//初始化列表框
	InitialListBox();


	//重置
	ResetAll();
		
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSumDlg::OnPaint()
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
HCURSOR CSumDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSumDlg::BTN_Open()
{
	ResetAll();


	CFileDialog tempdlg(TRUE);//打开文件对话框
	if(tempdlg.DoModal()!=IDOK) return;


	ifile.open(tempdlg.GetPathName(),ios::binary);//打开文件
	if(!ifile.is_open()) return;//确保文件打开


	ifile.seekg(0,ios::beg);ifile.seekg(0,ios::end);
	filelength=ifile.tellg();ifile.seekg(0,ios::beg);//得到文件长度
	if(filelength<1) return;//文件长度太小


	good=1;
	ShowProcess();//显示进度
}
UINT thread_SumCheck(LPVOID params)
{
	CSumDlg* dlg=(CSumDlg*)params;
	if(dlg->good!=1) return 0;//输入文件不正确


	if(dlg->ifile.tellg()<0 || dlg->ifile.tellg()>dlg->filelength) return 0;//避免空文件或文件已读完
	


	char ch,ch1,ch2;
	while(!dlg->ifile.eof())
	{
		dlg->ifile.get(ch);
		if(ch==0xEE)
		{
			dlg->ifile.get(ch);
			if(ch==0xBB)
			{
				//找到EEBB-----------------------------------------------
				dlg->ifile.get(ch);dlg->readmode=int((ch>>4)&0x000f);//读取包类型
				if(mode!=Package::SetMode)//包类型码不对
				{
					cout<<"读取的包类型码 "<<mode<<" 与预置的类型码 "<<Package::SetMode<<" 不同"<<endl;
					good=0;	return;
				}
				dlg->ifile.get(ch);fee=int(ch&0x00ff);//读取FEE
				dlg->ifile.get(ch);dlg->ifile.get(ch1);length=int((ch<<8)&0xff00)+int(ch1&0x00ff);//读取包长度


				dlg->ifile.read(buffer,length-6);//一次性读取一个触发内的科学数据


				dlg->ifile.get(ch);dlg->ifile.get(ch1);ch2=ch;
				trigger_stat=int((ch2>>4)&0x00ff);//读取触发状态，注意用前4个
				trigger_id=int((ch<<8)&0x0f00)+int(ch1&0x00ff);//读取触发号，注意用后4个
				dlg->ifile.get(ch);dlg->ifile.get(ch1);
				sum=int((ch<<8)&0xff00)+int(ch1&0x00ff);//读取累加和


				readlength+=(length+2);


				break;//跳出循环结束文件读取，由virtual void ReadFile()处理buffer数据
			}
		}
	}


	return 0;
}
void CSumDlg::BTN_Check()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CSumDlg::TAB_Change(NMHDR *pNMHDR, LRESULT *pResult)
{
	for(int i=0;i<v_subdlg.GetSize();i++)
		v_subdlg[i]->ShowWindow(FALSE);


	v_subdlg[tab.GetCurSel()]->ShowWindow(TRUE);


	*pResult = 0;
}
