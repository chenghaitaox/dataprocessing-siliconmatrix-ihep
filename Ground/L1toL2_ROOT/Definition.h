//定义主要参数
#ifndef DEFINITION_H
#define DEFINITION_H


//定义数据处理模式在数据包中的包类型码
const int MODE_RAW=1;			//原始模式
const int MODE_CAL=2;			//刻度模式
const int MODE_DOWN=3;			//下传模式

//定义数据处理相关
const int DET_NUM=10;			//探测器数目
const int TH1F_CH=8192;			//TH1F分道数目
const int TH1F_MAX=16384;		//TH1F最大道数

//要打开的L1科学数据
const char L1_FILENAME[]="1.txt";



//原始模式的输出文件名
const char RAW_PDFNAME[100]="PDF_Raw.pdf";
const char RAW_RMSNAME[100]="Package_RMS.txt";//RMS注入包
const char RAW_PDSNAME[100]="Package_PDS.txt";//PDS注入包
//刻度模式的输出文件名
const char CAL_PDFNAME[100]="PDF_Cal.pdf";
//下传模式的输出文件名
const char DOWN_PDFNAME[100]="PDF_Down.pdf";



//刻度模式的寻峰参数，首先选择Search寻峰还是SearchHighRes寻峰
//#define TSPECTRUM_SearchHighRes
#define TSPECTRUM_Search
	#ifdef TSPECTRUM_Search
	//Search的参数
		const double	CALFIT_SIGMA=8;//Search的sigma
		const double	CALFIT_THRE=0.1;//Search的阈值
	#endif
	#ifdef TSPECTRUM_SearchHighRes
	//SearchHighRes的参数
		const float		CALFIT_SIGMA=8;//SearchHighRes的sigma
		const double	CALFIT_THRE=2;//SearchHighRes的阈值
		const bool		CALFIT_BGRM=true;//SearchHighRes的去除本底
		const int		CALFIT_DECON=3;//SearchHighRes的deconlteration
		const bool		CALFIT_MARK=true;//SearchHighRes的markov
		const int		CALFIT_AVER=3;//SearchHighRes的averwindow
	#endif


#endif
