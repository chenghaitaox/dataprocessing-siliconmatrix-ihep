#!/home/rui/work/root/bin/root
#Filename: HitMap.cpp
{
#include "Identifier.h"
	gROOT->ProcessLine(".L ReadRaw.cc");
	gROOT->ProcessLine(".L LinearCal.cc");
	char filename[100];
	char crc_char;
	char cal_name[100];
	bool crc_flag=true;
	cout<< "please input the binary file name(.bin/.dat):" ;
	cin>>filename;
	cout<< "Do you need CRC check?(y/n):" ;
	cin>>crc_char;
	if(crc_char=='y')crc_flag=true;
	else if(crc_char=='n')crc_flag=false;
	else 
	{
		printf("input error!\n");
		exit(0);
	}
	LinearCal(ReadRaw(filename,crc_flag,0,CAL_ID));
}