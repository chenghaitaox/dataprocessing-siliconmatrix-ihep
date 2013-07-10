#!/home/rui/work/root/bin/root
#Filename: Pedestal.cpp
{
#include "Identifier.h"
	gROOT->ProcessLine(".L ReadRaw.cc");
	char filename[100];
	char crc_char;
	bool crc_flag=true;
	cout<< "please input the binary file name:" ;
	cin>>filename;
	ReadRaw(filename,crc_flag,1,COM_ID);
}
