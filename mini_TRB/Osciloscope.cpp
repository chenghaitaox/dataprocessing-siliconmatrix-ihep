#!/home/rui/work/root/bin/root
#Filename: Osciloscope.cpp
{
#include "Identifier.h"
#include <vector>
	gROOT->ProcessLine(".L ReadWave.cc");
	char filename[100];
	char crc_char;
	char cal_name[100];
	bool crc_flag=true;
	cout<< "please input the binary file name:" ;
	cin>>filename;
	ReadWave(filename,crc_flag,1,OSC_ID);
}
