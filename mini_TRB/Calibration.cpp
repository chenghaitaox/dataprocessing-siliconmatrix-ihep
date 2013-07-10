#!/home/rui/work/root/bin/root
#Filename: Calibration.cpp
{
#include "Identifier.h"
	gROOT->ProcessLine(".L ReadRaw.cc");
	gROOT->ProcessLine(".L LinearCal.cc");
	char filename[100];
	char crc_char;
	char cal_name[100];
	bool crc_flag=true;
	cout<< "please input the binary file name:" ;
	cin>>filename;
	LinearCal(ReadRaw(filename,crc_flag,0,CAL_ID));
}
