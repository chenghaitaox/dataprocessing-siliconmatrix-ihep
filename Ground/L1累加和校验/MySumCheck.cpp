#include"stdafx.h"

void CMySumCheck::Reset()
{
	//基本
	good=0;
	filelength=0;readlength=0;
	readevents=0;goodevents=0;
	ifile.close();ifile.clear();
	ofile.close();ofile.clear();
	l_msg.RemoveAll();

	//发送消息清空
	l_msg_normal.RemoveAll();
	l_msg_raw.RemoveAll();
	l_msg_cali.RemoveAll();
	l_msg_pds.RemoveAll();
	l_msg_down.RemoveAll();
	l_msg_empty.RemoveAll();
	l_msg_busy.RemoveAll();


	//模式相关
	for(int i=0;i<MODENUM;i++)
	{
		mode_filenum[i]=0;
		mode_num[i]=0;
		error_num[i]=0;
	}


	//数据包
	mode=-99;readmode=-99;


	//累加和校验
	sum=-99;readsum=-99;

}
CMySumCheck::CMySumCheck()
{
	Reset();
}
void CMySumCheck::OpenFile()
{
	CString str;


	Reset();												//全部复位



	ifile.open(InputFileName,ios::binary);					//打开数据，推荐二进制打开
	if(!ifile.is_open())									//是否成功打开
	{
		l_msg.AddTail(__T("文件打开错误"));
		return ;
	}


	ifile.seekg(0,ios::end);								//找长度必备
	filelength=ifile.tellg();								//找到长度
	if(filelength<2)         								//读取字节长度太小
	{
		str.Format(__T("文件长度 %d 太短"),filelength);
		l_msg.AddTail(str);
		return ;
	}
	ifile.seekg(0,ios::beg);								//读取指针回到开头


	good=1;													//标记成功！！！！！！！！！！
}
void CMySumCheck::SetReadMode()
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
void CMySumCheck::NewOutputFile()
{
	//关闭旧文件
	ofile.close();ofile.clear();


	//构造新文件的名字
	CString str;char tempchar[100];
	sprintf(tempchar,"(%d).",mode_filenum[mode]);
	str=OutputFileTitle;
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
		str+=CString("pedestal");
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
UINT thread_SumCheck(LPVOID params)
{
	if(my.good!=1)												//无正确输入文件							
	{
		my.l_msg.AddTail(CString("输入文件不正确"));
		return 0;
	}
	if(my.filelength<1)											//无文件长度
	{
		my.l_msg.AddTail(CString("文件长度太短"));
		return 0;
	}
	if(my.ifile.tellg()<0 || my.ifile.tellg()>=my.filelength)	//已读完
	{
		my.l_msg.AddTail(CString("文件已读完"));
		return 0;
	}


	CString str;
	char ch;
	while(!my.ifile.eof() && my.good==1)
	{
		my.ifile.get(ch);
		if((ch&0xff)==0xEE)
		{
			my.ifile.get(ch);
			if((ch&0xff)==0xBB)
			{
				//找到EEBB-----------------------------------------------
				++my.readevents;								//读取科学数据量+1
				my.ifile.read(my.buffer,4);


				//读取包类型，转换为临时读入模式
				my.SetReadMode();//根据buffer[0]计算readmode
				if(my.readmode==-1)//不可识别的包类型
				{
					//     To   Do  
					my.l_msg.AddTail(CString("不可识别的包类型码"));
					continue;
				}


				//比较这个临时读入模式和上一个包模式是否一致
				if(my.readmode!=my.mode)
				{
					my.mode=my.readmode;//重置包类型
					my.mode_filenum[my.mode]++;//对应的包的文件数目+1
					my.NewOutputFile();//关闭旧文件，打开新的输出文件
				}
				my.mode_num[my.mode]++;//对应包类型的数目+1


				//读取FEE
				my.fee=int((my.buffer[1])&0x003f);


				//读取长度，然后一气读入一个包数据
				my.length=int(((my.buffer[2])<<8)&0xff00)+int((my.buffer[3])&0x00ff);
				if(my.length<6 || my.length>40000)//数据长度越界
				{
					//     To   Do  
					my.l_msg.AddTail(CString("包长度太小"));
					continue;
				}
				my.ifile.read(my.buffer+4,my.length-2);//一次性读取剩余数据


				//触发状态和触发号
				my.trigger_stat=int(((my.buffer[my.length-2])>>4)&0x00ff);//读取触发状态，注意用前4个
				my.trigger_id=int(((my.buffer[my.length-2])<<8)&0x0f00)+int((my.buffer[my.length-1])&0x00ff);//读取触发号，注意用后4个


				//累加和校验
				my.readsum=int(((my.buffer[my.length])<<8)&0xff00)+int((my.buffer[my.length+1])&0x00ff);
				my.sum=0;
				for(int i=0;i<my.length;i++)
				{
					(my.sum)+=int((my.buffer[i])&0xff);//注意有0x00ff
				}
				if(my.sum==my.readsum)//累加和校验通过
				{
					my.ofile.write(my.buffer+4,my.length-6);//输出到文件
				}
				else//累加和校验错误！！！！！！！！
				{
					(my.error_num)[my.mode]++;//对应模式的累加和出错+1
					if((my.error_num)[my.mode]<100)//错误太多就不显示了
					{
						str.Format(__T("序号=%4d， EEBB序号=%8d， FEE=%2d， 包长度=%5d， 触发状态=%3d， 触发号=%5d, 读入累加和=%5d， 计算累加和=%5d")
							,my.error_num[my.mode],my.readevents,my.fee,my.length,my.trigger_stat,my.trigger_id,my.readsum,my.sum);
						switch(my.mode)
						{
						case 0:
							my.l_msg_normal.AddTail(str);
							break;
						case 1:
							my.l_msg_raw.AddTail(str);
							break;
						case 2:
							my.l_msg_cali.AddTail(str);
							break;
						case 3:
							my.l_msg_pds.AddTail(str);
							break;
						case 4:
							my.l_msg_down.AddTail(str);
							break;
						case 5:
							my.l_msg_empty.AddTail(str);
							break;
						case 6:
							my.l_msg_busy.AddTail(str);
							break;
						}
					}
					else
					{
						my.good=2;//标记出错太多，终止程序
					}
				}
			}
		}
		my.readlength=my.ifile.tellg();
	}
	

	//循环结束
	if(my.good==1)
	{
		my.readlength=my.filelength;
	}
	else if(my.good==2)
	{
		my.l_msg.AddTail(CString("出错太多，终止程序!!!"));
	}


	//关闭输入输出文件
	my.ifile.close();my.ifile.clear();
	my.ofile.close();my.ofile.clear();
	
	
	//重置状态
	my.good=0;



	return 0;
}


CMySumCheck my;