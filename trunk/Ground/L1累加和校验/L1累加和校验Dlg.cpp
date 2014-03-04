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
	readlength=0;
	progress.SetWindowText(CString("0 / 0"));
	bar.SetRange(0,100);
	bar.SetPos(0);
	mytimer.SetWindowText(CString(""));


	//输入输出文件
	good=0;
	ifile.close();ifile.clear();
	ofile.close();ofile.clear();
	for(int i=0;i<MODENUM;i++)
	{
		mode_filenum[i]=0;
	}


	//数据包
	mode=-99;readmode=99;


	//累加和校验
	sum=-99;readsum=-99;


	//统计
	for(int i=0;i<MODENUM;i++)
	{
		mode_num[i]=0;
		error_num[i]=0;
		v_modenum[i]->SetWindowText(CString("0"));
		v_errornum[i]->SetWindowText(CString("0"));
	}


	//列表框
	for(int i=0;i<MODENUM;i++)
	{
		v_listbox[i]->ResetContent();
//		v_listbox[i]->AddString(CString("10"));
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

void CSumDlg::DisplayProcess(int p)
{
	char tempchar[100];
	if(p<0||p>100)
	{
		sprintf(tempchar,"%d / %d",readlength,filelength);//构造读取的字符和总字符，主要要加long
		progress.SetWindowText(CString(tempchar));//静态文本框显示
		bar.SetPos(100.*readlength/filelength);//显示进度条
	}
	else
	{
		bar.SetPos(p);//直接设置进度条
	}
}
void CSumDlg::DisplayTimer()
{
	time_now=GetTickCount();
	long deltatime=(time_now-time_start)/1000;
	minute_used=deltatime/60;//计算用了的分钟
	second_used=deltatime-minute_used*60;//用了的秒
	if(readlength>10000)
	{
		long tempremain=double(deltatime)*(filelength-readlength)/readlength;
		minute_remain=tempremain/60;//计算用了的分钟
		second_remain=tempremain-minute_remain*60;//用了的秒
		str_mytimer.Format("已用 %d分%d秒 / 还需 %d分%d秒",minute_used,second_used,minute_remain,second_remain);
	}
	else
	{
		str_mytimer.Format("已用 %d分%d秒 / 还需 99分59秒",minute_used,second_used);
	}
	mytimer.SetWindowText(str_mytimer);
}
void CSumDlg::DisplayModeNum()
{
	CString str;
	for(int i=0;i<MODENUM;i++)
	{
		str.Format(__T("%d"),mode_num[i]);
		v_modenum[i]->SetWindowText(str);
	}
}
void CSumDlg::DisplayErrorNum()
{
	CString str;
	for(int i=0;i<MODENUM;i++)
	{
		str.Format(__T("%d"),error_num[i]);
		v_errornum[i]->SetWindowText(str);
	}
}
void CSumDlg::DisplayErrorMessage()
{
	CString str;
	str.Format(__T("序号=%4d， 位置=%9d， FEE=%2d， 包长度=%5d， 触发状态=%3d， 触发号=%5d, 读入累加和=%5d， 计算累加和=%5d")
		,error_num[mode],readlength,fee,length,trigger_stat,trigger_id,readsum,sum);
	v_listbox[mode]->InsertString(v_listbox[mode]->GetCount(),str);
}
void CSumDlg::NewOutputFile()
{
	//关闭旧文件
	ofile.close();ofile.clear();


	//构造新文件的名字
	CString str;char tempchar[100];
	sprintf(tempchar,"(%d).",mode_filenum[mode]);
	str=ofilename;
	str+=CString(tempchar);
	switch(mode)
	{
	case 0:
		str+=CString("normal");
		break;
	case 1:
		str+=CString("raw");
		break;
	case 2:
		str+=CString("cali");
		break;
	case 3:
		str+=CString("pds");
		break;
	case 4:
		str+=CString("down");
		break;
	case 5:
		str+=CString("empty");
		break;
	case 6:
		str+=CString("busy");
		break;
	default:
		str+=CString("error");
		break;
	}


	//打开新文件
	ofile.open(str,ios::binary);
}
void CSumDlg::SetReadMode()
{
	int tempmode=int((buffer[0]>>4)&0x000f);//计算包类型码，注意检查！！！！！！
	switch(tempmode)
	{
		case 2:	
			readmode=0;break;//0010=正常模式(0)
		case 3:	
			readmode=1;break;//0011=原始模式(1)
		case 4:	
			readmode=2;break;//0100=刻度模式(2)
		case 5:	
			readmode=3;break;//0101=基线更新(3)
		case 6:	
			readmode=4;break;//0110=下传模式(4)
		case 7:	
			readmode=5;break;//1100=空占位包(5)
		case 8:	
			readmode=6;break;//1101=忙占位包(6)
		default:	
			readmode=-1;break;//错误码(-1)
	}
}
CSumDlg::CSumDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSumDlg::IDD, pParent)
	, str_mytimer(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
void CSumDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);


	DDX_Control(pDX,IDC_TAB1,tab);
	DDX_Control(pDX,IDC_STATIC_PROGRESS,progress);
	DDX_Control(pDX,IDC_PROGRESS1,bar);



	DDX_Control(pDX, IDC_STATIC_TIME, mytimer);
	DDX_Text(pDX, IDC_STATIC_TIME, str_mytimer);
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

	
	//设置输出文件名为前缀为时间
	/*
	time_t mytime=time(0);
	char tempchar[100];
	strftime(tempchar,100,"%Y%m%d%H%M%S",localtime(&mytime));
	ofilename.Format(__T("%s"),tempchar);
*/
		
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
	DisplayProcess();//显示进度


	//构造文件夹和文件名前缀
	ofilename=tempdlg.GetPathName();
	int nPos=ofilename.ReverseFind('\\');
	ofilename=ofilename.Left(nPos);
	ofilename+=CString('\\');
	ofilename+=tempdlg.GetFileTitle();
	ofilename+=CString('\\');
	CreateDirectory(ofilename,NULL);
	ofilename+=tempdlg.GetFileTitle();
}
UINT thread_SumCheck(LPVOID params)
{
	CSumDlg* dlg=(CSumDlg*)params;

	if(dlg->good!=1)//输入文件不正确
	{
		dlg->MessageBox(CString("输入文件不正确"));
		 return 0;
	}
	if(dlg->ifile.tellg()<0 || dlg->ifile.tellg()>dlg->filelength)//避免空文件或文件已读完
	{
		dlg->MessageBox(CString("文件已读完"));
		dlg->good=0;
		 return 0;
	}
	


	char ch;
	while(!dlg->ifile.eof() && dlg->good)
	{
		dlg->ifile.get(ch);
		if((ch&0xff)==0xEE)
		{
			dlg->ifile.get(ch);
			if((ch&0xff)==0xBB)
			{
				//找到EEBB-----------------------------------------------


				//读取包类型，转换为临时读入模式
				dlg->ifile.get(dlg->buffer[0]);
				dlg->SetReadMode();//根据buffer[0]计算readmode
				if(dlg->readmode==-1)//不可识别的包类型
				{
					//     To   Do  
					dlg->MessageBox(CString("不可识别的包类型码"));
					continue;
				}


				//比较这个临时读入模式和上一个包模式是否一致
				if(dlg->readmode!=dlg->mode)
				{
					dlg->mode=dlg->readmode;//重置包类型
					dlg->mode_filenum[dlg->mode]++;//对应的包的文件数目+1
					dlg->NewOutputFile();//关闭旧文件，打开新的输出文件
				}
				dlg->mode_num[dlg->mode]++;//对应包类型的数目+1
//				dlg->DisplayModeNum();//在静态文本框显示数目+1


				//读取FEE
				dlg->ifile.get(dlg->buffer[1]);
				dlg->fee=int((dlg->buffer[1])&0x003f);


				//读取长度，然后一气读入一个包数据
				dlg->ifile.get(dlg->buffer[2]);dlg->ifile.get(dlg->buffer[3]);
				dlg->length=int(((dlg->buffer[2])<<8)&0xff00)+int((dlg->buffer[3])&0x00ff);
				if(dlg->length<6 || dlg->length>40000)//数据长度越界
				{
					//     To   Do  
//					dlg->MessageBox(CString("包长度太小"));
					continue;
				}
				dlg->ifile.read(dlg->buffer+4,dlg->length-2);//一次性读取剩余数据


				//触发状态和触发号
				dlg->trigger_stat=int(((dlg->buffer[dlg->length-2])>>4)&0x00ff);//读取触发状态，注意用前4个
				dlg->trigger_id=int(((dlg->buffer[dlg->length-2])<<8)&0x0f00)+int((dlg->buffer[dlg->length-1])&0x00ff);//读取触发号，注意用后4个


				//累加和校验
				dlg->readsum=int(((dlg->buffer[dlg->length])<<8)&0xff00)+int((dlg->buffer[dlg->length+1])&0x00ff);
				dlg->sum=0;
				for(int i=0;i<dlg->length;i++)
				{
					(dlg->sum)+=int((dlg->buffer[i])&0xff);//注意有0x00ff
				}
				if(dlg->sum==dlg->readsum)//累加和校验通过
				{
					dlg->ofile.write(dlg->buffer+4,dlg->length-6);//输出到文件
				}
				else
				{
					(dlg->error_num)[dlg->mode]++;//对应模式的累加和出错+1
					dlg->DisplayErrorNum();//在静态文本框显示数目+1
					if((dlg->error_num)[dlg->mode]<100)//错误太多就不显示了
					{
						dlg->DisplayErrorMessage();//在列表框显示详细信息
					}
				}
			}
		}
		dlg->readlength=dlg->ifile.tellg();
//		dlg->DisplayProcess();//刷新进度条
	}
	

	//重置文件状态
	dlg->ofile.close();dlg->ofile.clear();
	dlg->ifile.close();dlg->ifile.clear();
	dlg->good=0;

	return 0;
}
UINT thread_Show(LPVOID params)
{
	CSumDlg* dlg=(CSumDlg*)params;

	while(dlg->good)
	{
		dlg->DisplayTimer();
		dlg->DisplayErrorNum();
		dlg->DisplayModeNum();
		dlg->DisplayProcess();
		Sleep(1000);
	}

	return 0;
}
void CSumDlg::BTN_Check()
{
	time_start=GetTickCount();
	AfxBeginThread(&thread_SumCheck,this);
	AfxBeginThread(&thread_Show,this);
}

void CSumDlg::TAB_Change(NMHDR *pNMHDR, LRESULT *pResult)
{
	for(int i=0;i<v_subdlg.GetSize();i++)
		v_subdlg[i]->ShowWindow(FALSE);


	v_subdlg[tab.GetCurSel()]->ShowWindow(TRUE);


	*pResult = 0;
}
