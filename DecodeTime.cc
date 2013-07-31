TTimeStamp *DecodeTime(char*buff)
{
	long long time=0;
	memcpy(&time,buff,8);
	int month,day,hour,min,sec,msec;
	msec=time&0x00000000000000ff;
	time=(time>>16)&0x00ffffffffffffff;
	sec = time%60;
 	min = (time%3600)/60;
	hour= (time%(3600*24))/3600;
	day = (time%(3600*24*30))/(3600*24)+1;
	month =time/(3600*24*30)+1;
	TTimeStamp *datatime=new TTimeStamp(2013,month,day,hour,min,sec,msec*1000000,true,0);	
	//datatime->Print();
	return datatime;
}
