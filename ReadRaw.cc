TFile* ReadRaw(char* filename, bool crc_flag, bool rms_flag,unsigned int data_mode)
{
if(crc_flag)
	{
		gROOT->ProcessLine(".L EnableCRC.cc");
		gROOT->ProcessLine(".L LoopCRC.cc");
		gROOT->ProcessLine(".L CheckCRC.cc");
	}

	if(rms_flag)gROOT->ProcessLine(".L WriteRMS.cc");
	gROOT->ProcessLine(".L DecodeTime.cc");

	int count=0;
	bool home_run=true;
	char home_suf[4]=".dat";
	int err_time=0;
	int data_q,adc,ch;
	char Hname[10];
	char rsuffix[10]=".root" ;
	char rootname[100];
	char ch_data;
	char time_data[8];
	int seq_check=0;
	TTimeStamp *tree_time=0;
#ifdef CRC_DEF
	unsigned char crc[16];
#endif
	int t_adc[CH_NUM];
	char Tname[10];
	ifstream f(filename,ifstream::binary);
		if(f.is_open())
		{   
			home_run=strcmp(home_suf,filename+strlen(filename)-4);
			strcpy(rootname,filename);
            strcat(rootname,rsuffix);
			TFile * rootf= new TFile(rootname,"RECREATE" );
            //convert to histogram;
            TH1F *h[192];
            for (int i= 0; i< CH_NUM; i++)
            {
				sprintf(Hname, "Ch%d",i);
				h[i] = new TH1F(Hname,"spectrum" ,8192,0,16384);
			}
			//make event analysis
			TTree*t= new TTree("event" ,"data of event");
			for (int i= 0; i< CH_NUM; i++)
                {
					sprintf(Hname, "t_Ch%d",i);
					sprintf(Tname, "adc%d/I",i);
					t->Branch(Hname,&t_adc[i],Tname);
					t_adc[i]=0;
                }
			t->Branch("event_time","TTimeStamp",&tree_time);
			while (!f.eof())
			{
				f.read(&ch_data,1);
				if(((ch_data)&0x00ff)==0xeb)
				{
					f.read(&ch_data,1);
					if(((ch_data)&0x00ff)==0x90)
					{
					#ifdef CRC_DEF
						memset(crc,1,16);
					#endif						
						//read the triger number
						f.read(&ch_data,1);
						if(count&&int(ch_data&0x00ff))
						{
							if((seq_check+1)!=int(ch_data&0x00ff))
							{
								cout<<"The triger count error!"<<endl;
								cout<<"the last triger is "<<seq_check<<endl;
								cout<<"This triger is "<<int(ch_data&0x00ff)<<endl;
							}
							seq_check=int(ch_data&0x00ff);
						}
						else seq_check=int(ch_data&0x00ff);
					#ifdef CRC_DEF
						LoopCRC(ch_data,crc);
					#endif						
						//read the data model
						f.read(&ch_data,1);
						if(unsigned int((ch_data)&0x00ff)!=data_mode)continue;
					#ifdef CRC_DEF
						LoopCRC(ch_data,crc);
					#endif
						//read the file length
						f.read(&ch_data,1);
					#ifdef CRC_DEF
						LoopCRC(ch_data,crc);
					#endif
						data_q= int(ch_data&0x00ff);
						f.read(&ch_data,1);
					#ifdef CRC_DEF
						LoopCRC(ch_data,crc);
					#endif
						data_q= int((data_q<<8)&0xff00)+int(ch_data&0x00ff);
	
						//reading the channels
						for(int i=0;i<(data_q-6)/3;i++)
						{
							//read the channel number
							f.read(&ch_data,1);
						#ifdef CRC_DEF
							LoopCRC(ch_data,crc);
						#endif
							ch= int(ch_data&0x00ff);
							if((ch>191))continue ;
							//read the ADC
							f.read(( char *)&ch_data,1);
						#ifdef CRC_DEF
							LoopCRC(ch_data,crc);
						#endif
							adc= int(ch_data&0x00ff);
							f.read(( char *)&ch_data,1);
						#ifdef CRC_DEF
							LoopCRC(ch_data,crc);
						#endif
							adc= int((adc<<8)&0xff00)+int(ch_data&0x00ff);
							t_adc[ch]=adc;						
							h[ch]->Fill(adc);
						}

					#ifdef CRC_DEF
					if((data_q-6)%3)
					{
						f.read(&ch_data,1);
						LoopCRC(ch_data,crc);
					}
					#endif

					f.read(&ch_data,1);
					#ifdef CRC_DEF
						LoopCRC(ch_data,crc);
					#endif

					f.read(&ch_data,1);
					#ifdef CRC_DEF
						LoopCRC(ch_data,crc);
					#endif

					f.read(&ch_data,1);
					#ifdef CRC_DEF
						LoopCRC(ch_data,crc);
					#endif

					f.read(&ch_data,1);
					#ifdef CRC_DEF
						LoopCRC(ch_data,crc);
					#endif

					count++;

					#ifdef CRC_DEF				
					if(crc_flag&&CheckCRC(crc))
					{
						err_time++;
						printf("%d CRC error checked in %d!\n",err_time,count);
						continue;
					}
					#endif 

					if(!home_run)
					{
						f.read(&ch_data,1);
						if(unsigned int((ch_data)&0x00ff)==0xe2) f.read(&ch_data,1);
						else continue;
						if(unsigned int((ch_data)&0x00ff)==0x25)
						{
						//read more six bytes
						f.read(&ch_data,1);
						f.read(&ch_data,1);
						f.read(&ch_data,1);
						f.read(&ch_data,1);
						f.read(&ch_data,1);
						f.read(&ch_data,1);
						//decode time
						for(int iii=0;iii<8;iii++)f.read(&time_data[7-iii],1);
						tree_time=DecodeTime(time_data);
					}
					t->Fill();
					for (int i= 0; i< CH_NUM; i++)
						{
							t_adc[i]=0;
						}
					}
					else continue;			
				}
			}
			else continue ;
		}
	f.close();
	rootf->Write();
	if(rms_flag)WriteRMS(filename,h);
	}
	else
	{
		cout << "Error opening file" ;
	}
	return rootf;
}
