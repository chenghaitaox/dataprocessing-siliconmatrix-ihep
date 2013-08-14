TFile* ReadWave(char* filename, bool crc_flag, bool rms_flag,unsigned int data_mode)
{
	int count=0;
	bool home_run=true;
	char home_suf[4]=".dat";
	int err_time=0;
	int data_q,adc,ch;
	char Hname[10];
	char rsuffix[10]=".root" ;
	char rootname[100];
	char ch_data;
	int seq_check=0;
	char Tname[10];
	int wave_L[50];
	int wave_R[50];
	int wave_T[50];

	ifstream f(filename,ifstream::binary);
	if(f.is_open())
	{  
		home_run=strcmp(home_suf,filename+strlen(filename)-4);
		strcpy(rootname,filename);
		strcat(rootname,rsuffix);
		TFile * rootf= new TFile(rootname,"RECREATE" );
		//initial the point of graph;
		TGraph*Pwave_L;
		TGraph*Pwave_R;

		while (!f.eof())
		{
			f.read(&ch_data,1);
			if(((ch_data)&0x00ff)==0x90)
			{
				f.read(&ch_data,1);
				if(((ch_data)&0x00ff)==0xeb)
				{
					//read the ID
					f.read(&ch_data,1);
					/*if(count&&int(ch_data&0x00ff))
					{
						if((seq_check+1)!=int(ch_data&0x00ff))
						{
							cout<<"The triger count error!"<<endl;
							cout<<"the last triger is "<<seq_check<<endl;
							cout<<"This triger is "<<int(ch_data&0x00ff)<<endl;
						}
						seq_check=int(ch_data&0x00ff);
					}
					else seq_check=int(ch_data&0x00ff);*/
					//read the data model
					f.read(&ch_data,1);
					if(unsigned int((ch_data)&0x00ff)!=data_mode)continue;

					//reading the channels
					for(int i=0;i<50;i++)
					{
						//read the ADC
						f.read(( char *)&adc,2);
						adc= int(adc&0x0000ffff);
						wave_L[i]=adc;
						f.read(( char *)&adc,2);
						adc= int(adc&0x0000ffff);
						wave_R[i]=adc;
						wave_T[i]=i+1;
					}
					for(int ii=0;ii<8;ii++)f.read(&ch_data,1);
					//create 2 graphs;
					Pwave_L=new TGraph(50,wave_T,wave_L);
					Pwave_R=new TGraph(50,wave_T,wave_R);
					Pwave_L->Write();
					Pwave_R->Write();

					count++;
				}
				else continue;			
			}
			else continue ;
		}
		f.close();
		rootf->Write();
		//if(rms_flag)WriteRMS(filename,h);
	}
	else
	{
		cout << "Error opening file" ;
	}
	return rootf;
}
