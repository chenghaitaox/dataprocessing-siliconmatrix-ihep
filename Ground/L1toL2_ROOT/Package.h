#ifndef PACKAGE_H
#define PACKAGE_H

#include<fstream>
#include<iostream>
#include<vector>
#include<string>
#include<iomanip>//供setw
using namespace std;
#include"Definition.h"

#include"TGraph.h"
#include"TH1.h"
#include"TH1F.h"
#include"TF1.h"
#include"TFile.h"
#include"TCanvas.h"
#include"TPolyMarker.h"
#include"TSpectrum.h"


//L1整个科学数据包
class Package
{
public:
	Package();
public://模式
	static int SetMode;								//要处理的模式
	static void CinMode();							//屏幕输入工作模式，然后定义派生类对象
public://重要基类变量
	long filelength;								//记录要顺序打开的文件长度
	long readlength;
	ifstream ifile;	ofstream ofile;
	int good;										//代表运行状态，重新执行一遍OpenFile才能刷新
public://与派生类无关
	TH1F *hist[DET_NUM];
	int mode,fee,length,trigger_stat,trigger_id,sum;//包类型码，FEE编号，数据包长，触发状态，触发号，累加和
	char buffer[30000];								//一个触发里面的科学数据
	void OpenFile();								//确认要打开的文件名，存到vector<string> v_filename里面，并尝试打开文件读取长度。
	void ReadOneTrigger();							//寻找EEBB,读取特征和数据，确认包类型码正确，然后把数据存入buffer里面
	void ReadFile();								//循环读取所有文件，调用ReadOneTrigger()和FillHist()
	TCanvas *c1;
public://需要派生类具体设置
	virtual void FillHist()=0;						//把buffer[30000]数据填入hist[DET_NUM]
	virtual void GenerateTestHist(int,int){};		//产生测试谱，共Analysis和Output
	virtual void Analysis()=0;						//根据hist[DET_NUM]，得到统计数据结果。
	virtual void Output()=0;						//根据统计数据，画图和输出文件
};
//原始模式
class Package_Raw:public Package
{
public:
	Package_Raw();
	int RMS[DET_NUM],PDS[DET_NUM];					//通过高斯拟合得到PDS/RMS
	TGraph *gr_RMS,*gr_PDS;							//PDS/RMS统计图

	virtual void FillHist();
	virtual void Analysis();						//拟合Hist，得到PDS/RMS数组
	virtual void Output();
	virtual void GenerateTestHist(int,int);			//给所有Hist填充测试数据，第一个参量是平均值、第二个参量是sigma
};
//下传模式
class Package_Down:public Package_Raw
{
public:
	Package_Down();
	int RMS_read[DET_NUM],PDS_read[DET_NUM];		//读入的数据，记得把PDS左移一位
	TGraph *gr_RMS_read,*gr_PDS_read;				//读入的PDS/RMS统计图
	virtual void FillHist();
	virtual void Output();
};
//刻度模式
class Package_Cal:public Package
{
public:
	Package_Cal();
	int PeakNum[DET_NUM];								//拟合的峰的数目
	double Slope[DET_NUM];								//拟合的斜率
#ifdef TSPECTRUM_Search
	int Search(TH1F *S_hist,float* S_peaksx,float* S_peaksy,double S_sigma,double S_threshold);//Search寻峰
#endif
#ifdef TSPECTRUM_SearchHighRes
	int SearchHighRes(TH1F *SHR_hist,float* SHR_peaksx,float* SHR_peaksy,float SHR_sigma,double SHR_threshold,bool SHR_backgroundRemove,int SHR_deconlterations,bool SHR_markov,int SHR_averWindow);//SearchHighRes寻峰
#endif
	virtual void FillHist();
	virtual void Analysis();
	virtual void Output();
	virtual void GenerateTestHist(int,int);			//给所有Hist填充测试数据，第一个参量是峰的数目、第二个参量是sigma
};

//全局的变量
Package *mypackage;
int Package::SetMode=-1;
//Package类的具体实现
Package::Package()
{
	char ch[100];
	for(int i=0;i<DET_NUM;i++)//初始化TH1F
	{
		sprintf(ch,"Hist%d",i+1);
		hist[i]=new TH1F(ch,ch,TH1F_CH,0,TH1F_MAX);
		hist[i]->SetStats(0);
	}
	c1=new TCanvas("c1","c1",800,800);

	//初始化为非正常运行状态
	good=0;
	filelength=0;
	readlength=0;
}
void Package::CinMode()
{
	while(Package::SetMode!=MODE_RAW &&Package::SetMode!=MODE_CAL &&Package::SetMode!=MODE_DOWN)
	{
		cout<<"请输入数据处理模式"<<endl;
		cout<<"原始模式("<<MODE_RAW<<") 、 刻度模式("<<MODE_CAL<<") 、下传模式("<<MODE_DOWN<<")"<<endl;
		cin>>Package::SetMode;
		switch(Package::SetMode)
		{
		case MODE_RAW:
			cout<<"你选择了原始模式"<<endl;
			mypackage=new Package_Raw();
			break;
		case MODE_CAL:
			cout<<"你选择了刻度模式"<<endl;
			mypackage=new Package_Cal();
			break;
		case MODE_DOWN:
			cout<<"你选择了下传模式"<<endl;
			mypackage=new Package_Down();
			break;
		default:
			cout<<"输入模式有误"<<endl;
			break;
		}
	}
	cout<<"cinmode"<<endl;
}
void Package::OpenFile()							
{
	good=0;
	filelength=0;
	readlength=0;


	ifile.close();ifile.clear();ifile.open(L1_FILENAME,ios::binary);
	if(ifile.is_open())
	{
		ifile.seekg(0,ios::beg);ifile.seekg(0,ios::end);
		filelength=ifile.tellg();
		good=1;
		cout<<"L1文件打开成功，总长度 "<<filelength<<endl;
	}
	else//打开不成功
	{
		cout<<"L1文件打开失败"<<endl;
		filelength=0;
	}
}
void Package::ReadOneTrigger()												///寻找EEBB,读取特征和数据，确认包类型码正确
{
	char ch,ch1,ch2;
	while(!ifile.eof())
	{
		ifile.get(ch);++readlength;
		if(ch==0xEE)
		{
			ifile.get(ch);++readlength;
			if(ch==0xBB)
			{
				//找到EEBB-----------------------------------------------
				ifile.get(ch);mode=int(ch&0x00ff);//读取包类型
				if(mode!=Package::SetMode)//包类型码不对
				{
					cout<<"读取的包类型码 "<<mode<<" 与预置的类型码 "<<Package::SetMode<<" 不同"<<endl;
					good=0;	return;
				}
				ifile.get(ch);fee=int(ch&0x00ff);//读取FEE
				ifile.get(ch);ifile.get(ch1);length=int((ch<<8)&0xff00)+int(ch1&0x00ff);//读取包长度


				ifile.read(buffer,length-6);//一次性读取一个触发内的科学数据


				ifile.get(ch);ifile.get(ch1);ch2=ch;
				trigger_stat=int((ch2>>4)&0x00ff);//读取触发状态，注意用前4个
				trigger_id=int((ch<<8)&0x0f00)+int(ch1&0x00ff);//读取触发号，注意用后4个
				ifile.get(ch);ifile.get(ch1);
				sum=int((ch<<8)&0xff00)+int(ch1&0x00ff);//读取累加和


				readlength+=(length+2);


				break;//跳出循环结束文件读取，由virtual void ReadFile()处理buffer数据
			}
		}
	}
}

void Package::ReadFile()
{
	if(!good)
	{
		cout<<"输入文件不正确，停止执行ReadFile"<<endl;
		return;
	}
	if(readlength>=filelength)
	{
		cout<<"文件已经读取完毕"<<endl;
		good=0;
		return;
	}
	//开始读取内容
	ifile.seekg(0,ios::beg);
	while(!ifile.eof()&&good)
	{
		ReadOneTrigger();
		FillHist();
		cout<<right<<setw(11)<<filelength<<" / "<<left<<setw(11)<<readlength<<right<<setw(8)<<100.*readlength/filelength<<"%"<<endl;
	}
}
//Package_Raw类的具体实现
Package_Raw::Package_Raw(){}
void Package_Raw::FillHist()
{
	//--------------- TO DO -----------------------
}
void Package_Raw::Analysis()
{
	//拟合，得到每个TH1F的PDS和RMS（认为是高斯拟合的sigma），事件数，以及拟合误差
	TF1 *gausfit=new TF1("gausfit","[0]*exp(-0.5*pow(((x-[1])/[2]),2))",0,TH1F_MAX);
	for(int i=0;i<DET_NUM;i++)
	{
		gausfit->SetParameter(1,hist[i]->GetMean());
		gausfit->SetParameter(2,hist[i]->GetRMS());
		hist[i]->GetXaxis()->SetRangeUser(hist[i]->GetMean()-hist[i]->GetRMS()*5,hist[i]->GetMean()+hist[i]->GetRMS()*5);
		hist[i]->Fit("gausfit","Q");
		PDS[i]=gausfit->GetParameter(1);
		RMS[i]=gausfit->GetParameter(2);


		if((i+1)%10==0) cout<<"Fitting "<<i+1<<" / "<<DET_NUM<<endl;
	}
}
void Package_Raw::Output()
{
	//得到整体的PDS/RMS统计图
	int xaxis[DET_NUM];
	for(int i=0;i<DET_NUM;i++)
	{
		xaxis[i]=i+1;
	}
	TGraph *gr_RMS=new TGraph(DET_NUM,xaxis,RMS);
	gr_RMS->SetTitle("RMS Distribution");
	gr_RMS->GetXaxis()->SetTitle("Channel");
	gr_RMS->GetXaxis()->CenterTitle(1);
	gr_RMS->GetYaxis()->SetTitle("RMS");
	gr_RMS->GetYaxis()->CenterTitle(1);
//	gr_RMS->GetYaxis()->SetRangeUser(0,250);
	TGraph *gr_PDS=new TGraph(DET_NUM,xaxis,PDS);
	gr_PDS->SetTitle("Pedestal Distribution");
	gr_PDS->GetXaxis()->SetTitle("Channel");
	gr_PDS->GetXaxis()->CenterTitle(1);
	gr_PDS->GetYaxis()->SetTitle("PDS");
	gr_PDS->GetYaxis()->CenterTitle(1);
//	gr_PDS->GetYaxis()->SetRangeUser(0,500);


	//把所有结果存为图像PDF
	char tempchar[100];
	sprintf(tempchar,"%s%s",RAW_PDFNAME,"[");c1->Print(tempchar);//开启PDF文件

	gr_RMS->Draw("AL");c1->Print(RAW_PDFNAME);
	gr_PDS->Draw("AL");c1->Print(RAW_PDFNAME);
	for(int i=0;i<DET_NUM;i++)
	{
		hist[i]->Draw();c1->Print(RAW_PDFNAME);
	}
	sprintf(tempchar,"%s%s",RAW_PDFNAME,"]");c1->Print(tempchar);//结束PDF文件


	//输出RMS/PDS注入包，RMS占1个字节，PDS右移1位占1个字节
	ofstream oRMS(RAW_RMSNAME,ios::binary);
	ofstream oPDS(RAW_PDSNAME,ios::binary);
	char ch_RMS[DET_NUM],ch_PDS[DET_NUM];
	for(int i=0;i<DET_NUM;i++)
	{
		ch_RMS[i]=int(RMS[i]&0x00ff);
		ch_PDS[i]=int((PDS[i]>>1)&0x00ff);//右移1位
	}
	oRMS.write(ch_RMS,DET_NUM);
	oPDS.write(ch_PDS,DET_NUM);
	oRMS.close();oRMS.clear();
	oPDS.close();oPDS.clear();
}
void Package_Raw::GenerateTestHist(int mean,int sigma)
{
	TF1 *gausfit=new TF1("gausfit","[0]*exp(-0.5*pow(((x-[1])/[2]),2))",0,TH1F_MAX);
	gausfit->SetParameter(0,1);
	gausfit->SetParameter(1,mean);
	gausfit->SetParameter(2,sigma);


	hist[0]->FillRandom("gausfit",10000);
	char tempchar[100];
	for(int i=1;i<DET_NUM;i++)
	{
		strcpy(tempchar,hist[i]->GetName());
		delete hist[i];
		hist[i]=(TH1F*)hist[0]->Clone(tempchar);
		hist[i]->SetTitle(tempchar);
	}
}
//Package_Down类的具体实现
Package_Down::Package_Down(){}
void Package_Down::FillHist()
{
	//--------------- TO DO -----------------------
}
void Package_Down::Output()
{
	//得到整体的PDS/RMS统计图
	int xaxis[DET_NUM];
	for(int i=0;i<DET_NUM;i++)
	{
		xaxis[i]=i+1;
	}
	TGraph *gr_RMS=new TGraph(DET_NUM,xaxis,RMS);
	gr_RMS->SetTitle("RMS Distribution");
	gr_RMS->GetXaxis()->SetTitle("Channel");
	gr_RMS->GetXaxis()->CenterTitle(1);
	gr_RMS->GetYaxis()->SetTitle("RMS");
	gr_RMS->GetYaxis()->CenterTitle(1);
	gr_RMS->GetYaxis()->SetRangeUser(0,250);
	TGraph *gr_PDS=new TGraph(DET_NUM,xaxis,PDS);
	gr_PDS->SetTitle("Pedestal Distribution");
	gr_PDS->GetXaxis()->SetTitle("Channel");
	gr_PDS->GetXaxis()->CenterTitle(1);
	gr_PDS->GetYaxis()->SetTitle("PDS");
	gr_PDS->GetYaxis()->CenterTitle(1);
	gr_PDS->GetYaxis()->SetRangeUser(0,500);


	//读入RMS/PDS注入包，RMS占1个字节，记得把PDS左移1位
	ifstream iRMS(RAW_RMSNAME,ios::binary);
	ifstream iPDS(RAW_PDSNAME,ios::binary);
	char ch_RMS[DET_NUM],ch_PDS[DET_NUM];
	iRMS.read(ch_RMS,DET_NUM);
	iPDS.read(ch_PDS,DET_NUM);
	for(int i=0;i<DET_NUM;i++)
	{
		RMS_read[i]=int(ch_RMS[i]);
		PDS_read[i]=int(ch_PDS[i]<<1);
	}
	iRMS.close();iRMS.clear();
	iPDS.close();iPDS.clear();


	//得到地面的RMS/PDS统计图
	TGraph *gr_RMS_read=new TGraph(DET_NUM,xaxis,RMS_read);
	gr_RMS_read->SetTitle("RMS_read Distribution Comparison");
	gr_RMS_read->GetXaxis()->SetTitle("Channel");
	gr_RMS_read->GetXaxis()->CenterTitle(1);
	gr_RMS_read->GetYaxis()->SetTitle("RMS_read");
	gr_RMS_read->GetYaxis()->CenterTitle(1);
	gr_RMS_read->GetYaxis()->SetRangeUser(0,250);
	gr_RMS_read->SetLineColor(2);//地面的是红色曲线
	TGraph *gr_PDS_read=new TGraph(DET_NUM,xaxis,PDS_read);
	gr_PDS_read->SetTitle("Pedestal Distribution Comparison");
	gr_PDS_read->GetXaxis()->SetTitle("Channel");
	gr_PDS_read->GetXaxis()->CenterTitle(1);
	gr_PDS_read->GetYaxis()->SetTitle("PDS_read");
	gr_PDS_read->GetYaxis()->CenterTitle(1);
	gr_PDS_read->GetYaxis()->SetRangeUser(0,500);
	gr_PDS_read->SetLineColor(2);//地面的是红色曲线


	//把所有结果存为图像PDF
	char tempchar[100];
	sprintf(tempchar,"%s%s",DOWN_PDFNAME,"[");c1->Print(tempchar);//开启PDF文件

	gr_RMS_read->Draw("AL");gr_RMS->Draw("L");c1->Print(DOWN_PDFNAME);
	gr_PDS_read->Draw("AL");gr_PDS->Draw("L");c1->Print(DOWN_PDFNAME);
	for(int i=0;i<DET_NUM;i++)
	{
		hist[i]->Draw();c1->Print(DOWN_PDFNAME);
	}
	sprintf(tempchar,"%s%s",DOWN_PDFNAME,"]");c1->Print(tempchar);//结束PDF文件
}
//Package_Cal类的具体实现
Package_Cal::Package_Cal(){}
void Package_Cal::FillHist()
{
	//--------------- TO DO -----------------------
}

#ifdef TSPECTRUM_Search//Search寻峰
int Package_Cal::Search(TH1F *S_hist,float* S_peaksx,float* S_peaksy,double S_sigma,double S_threshold)
{
	float *PeakArray;

	//新建一个TSpectrum
	TSpectrum *s = new TSpectrum();

	//寻峰
	int number=s->Search(S_hist,S_sigma,"",S_threshold);

	//根据拟合结果得到X、Y位置
	PeakArray = s->GetPositionX();//返回的是TH1F的横坐标！！！！！！！
	for(int i=0;i<number&&i<100;i++)//认为最多100个峰
	{
			S_peaksx[i]=PeakArray[i];
			S_peaksy[i]=S_hist->GetBinContent(S_hist->FindBin(S_peaksx[i]));
	}

	//产生Marker标注峰位置，并绑定到TH1F
	TPolyMarker* pm=new TPolyMarker(number,S_peaksx,S_peaksy);
	pm->SetMarkerStyle(23);
	pm->SetMarkerColor(kRed);
	pm->SetMarkerSize(1.3);
	S_hist->GetListOfFunctions()->Add(pm);

	return number;
}
#endif
#ifdef TSPECTRUM_SearchHighRes//SearchHighRes寻峰
int Package_Cal::SearchHighRes(TH1F *SHR_hist,float* SHR_peaksx,float* SHR_peaksy,float SHR_sigma,double SHR_threshold,bool SHR_backgroundRemove,int SHR_deconlterations,bool SHR_markov,int SHR_averWindow)
{
	float dest[100], *PeakArray;

	//新建一个TSpectrum
	TSpectrum *s = new TSpectrum();

	//得到TH1F的每道的数据
	float *source;
	source=SHR_hist->GetArray()+1;

	//寻峰
	int number=s->SearchHighRes(source,dest,SHR_hist->GetNbinsX(),SHR_sigma,SHR_threshold,SHR_backgroundRemove,SHR_deconlterations,SHR_markov,SHR_averWindow);
	cout<<"寻峰数目 "<<number<<endl;

	//根据拟合结果得到X、Y位置
	PeakArray = s->GetPositionX();//返回的是峰值是source数组中的第几个，而不是横坐标！！！！！！！
	for(int i=0;i<number&&i<100;i++)//认为最多100个峰
	{
		SHR_peaksy[i]=(int(PeakArray[i]+1)-PeakArray[i]) * source[int(PeakArray[i])]   +   source[int(PeakArray[i]+1)] * (PeakArray[i]-int(PeakArray[i]));
		SHR_peaksx[i]=(int(PeakArray[i]+1)-PeakArray[i]) * SHR_hist->GetBinCenter(int(PeakArray[i]+1))    +    SHR_hist->GetBinCenter(int(PeakArray[i]+2)) * (PeakArray[i]-int(PeakArray[i]));
	}

	//产生Marker标注峰位置，并绑定到TH1F
	TPolyMarker* pm=new TPolyMarker(number,SHR_peaksx,SHR_peaksy);
	pm->SetMarkerStyle(23);
	pm->SetMarkerColor(kRed);
	pm->SetMarkerSize(1.3);
	SHR_hist->GetListOfFunctions()->Add(pm);

	return number;
}
#endif
void Package_Cal::Analysis()
{
	float Peak_X[100],Peak_Y[100];						//储存寻峰结果
	float Peak_Xmax,Peak_Xmin;							//储存峰位最大、最小值，用于得到斜率
	//循环寻峰每个探测器的刻度谱，并得到斜率和峰数目
	for(int i=0;i<DET_NUM;i++)
	{
		//寻峰
#ifdef TSPECTRUM_Search//Search寻峰
		PeakNum[i]=Search(hist[i],Peak_X,Peak_Y,CALFIT_SIGMA,CALFIT_THRE);
#endif
#ifdef TSPECTRUM_SearchHighRes//SearchHighRes寻峰
		PeakNum[i]=SearchHighRes(hist[i],Peak_X,Peak_Y,CALFIT_SIGMA,CALFIT_THRE,CALFIT_BGRM,CALFIT_DECON,CALFIT_MARK,CALFIT_AVER);
#endif

		//得到斜率，计数不足3个则返回斜率为1
		if(PeakNum[i]<3){
			Slope[i]=1;
		} else {
			//开始找最大最小值
			Peak_Xmax=Peak_X[0];Peak_Xmin=Peak_X[0];
			for(int j=1;j<PeakNum[i];j++)
			{
				if(Peak_X[j]<Peak_Xmin)
				{
					Peak_Xmin=Peak_X[j];//重新确定最小值
				}
				else if(Peak_X[j]>Peak_Xmax)
				{
					Peak_Xmax=Peak_X[j];//重新确定最大值
				}
			}
			Slope[i]=(Peak_Xmax-Peak_Xmin) / (PeakNum[i]-1);//斜率
		}


		cout<<"Searching Peak "<<i+1<<" / "<<DET_NUM<<endl;
	}
}
void Package_Cal::Output()
{


	//产生统计图:斜率vs通道、斜率vs寻峰数
	double ch[DET_NUM];
	for(int i=0;i<DET_NUM;i++)
	{
		ch[i]=i+1;//通道id为从1~DET_NUM
	}
	TGraph *gr_SlopeVsChannel=new TGraph(DET_NUM,ch,Slope);
	gr_SlopeVsChannel->SetTitle("Slope vs Channel");
	gr_SlopeVsChannel->GetXaxis()->SetTitle("Channel");gr_SlopeVsChannel->GetXaxis()->CenterTitle(1);
	gr_SlopeVsChannel->GetYaxis()->SetTitle("Slope");gr_SlopeVsChannel->GetYaxis()->CenterTitle(1);
	for(int i=0;i<DET_NUM;i++)
	{
		ch[i]=PeakNum[i];//通道id为从1~DET_NUM
	}
	TGraph *gr_SlopeVsNumber=new TGraph(DET_NUM,ch,Slope);
	gr_SlopeVsNumber->SetTitle("Slope vs Number");
	gr_SlopeVsNumber->GetXaxis()->SetTitle("Number");gr_SlopeVsNumber->GetXaxis()->CenterTitle(1);
	gr_SlopeVsNumber->GetYaxis()->SetTitle("Slope");gr_SlopeVsNumber->GetYaxis()->CenterTitle(1);


	//把所有结果存为图像PDF
	char tempchar[100];
	sprintf(tempchar,"%s%s",CAL_PDFNAME,"[");
	c1->Print(tempchar);
	gr_SlopeVsChannel->Draw("AL");c1->Print(CAL_PDFNAME);
	gr_SlopeVsNumber->Draw("A*");c1->Print(CAL_PDFNAME);
	for(int i=0;i<DET_NUM;i++)
	{
		hist[i]->Draw();c1->Print(CAL_PDFNAME);
	}
	sprintf(tempchar,"%s%s",CAL_PDFNAME,"]");
	c1->Print(tempchar);
}
void Package_Cal::GenerateTestHist(int peaknumber,int sigma)
{
	TF1 *gausfit=new TF1("gausfit","[0]*exp(-0.5*pow(((x-[1])/[2]),2))",0,TH1F_MAX);
	gausfit->SetParameter(0,1);
	gausfit->SetParameter(2,sigma);
//	c1->Print("test.pdf[");
	for(int i=0;i<DET_NUM;i++)
	{
		for(int j=0;j<peaknumber;j++)
		{
			gausfit->SetParameter(1,double(TH1F_MAX)/peaknumber*(j+0.5));
			hist[i]->FillRandom("gausfit",10000);
		}
		hist[i]->Draw();
//		c1->Print("test.pdf");
	}
//	c1->Print("test.pdf]");

}






#endif