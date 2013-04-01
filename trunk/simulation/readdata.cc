#!/home/rui/work/root/bin/root
{
#define CH_NUM 192
//define
	int adc_data;
	char ch_data;
	int adc;
	int t_adc[CH_NUM];
	int ch;
	int flength=0;
	float mean[CH_NUM],rms[CH_NUM];
	char filename[100];
	char rootname[100];
	char Hname[10];
	char Tname[10];
	char dsuffix[10]=".bin";
	char csuffix[20]=".bin_RMSE.txt";
	char rsuffix[10]="_bin.root";


	for(int i=0; i< CH_NUM; i++)
		{
			mean[ch]=0;
			rms[ch]=0;	
		}

//calibrate
	cout<<"please input the calibrate file name(without'.bin_RMSE.txt'):";
	cin>>filename;
	strcat(filename,csuffix);
 	ifstream c_f(filename);
	if(c_f.is_open())
	{
		for(int i=0; i< CH_NUM; i++)
		{
			c_f>>ch>>mean[ch]>>rms[ch];	
		}
	c_f.close();
	cout<<"Calibration complete!"<<endl;
	}
	else
    {
      	cout << "Error opening file"<<endl;
		cout << "Did not Calibrate!"<<endl;
    }
//initial data file
	cout<<"please input the binary file name(without'.bin'):";
	cin>>filename;
	strcpy(rootname,filename);
	strcat(filename,dsuffix);

//initial root file
	strcat(rootname,rsuffix);
	TFile * rootf=new TFile(rootname,"RECREATE");
	//convert to histogram;
	TH1F *h[192];
	for (int i= 0; i< CH_NUM; i++)
	{
		sprintf(Hname,"Ch%d",i); 
		h[i] = new TH1F(Hname,"spectrum",8192,0,16384);
	}
	//make event analysis
	TTree*t=new TTree("event","data of event");
	for (int i= 0; i< CH_NUM; i++)
	{
		sprintf(Hname,"t_Ch%d",i); 
		sprintf(Tname,"adc%d/I",i); 
		t->Branch(Hname,&t_adc[i],Tname);
	}	

//reading
	int tmpl;
	ifstream f(filename,ifstream::binary);

 	if(f.is_open())
    	{		//for(int ii =0;ii<500;ii++)
				while (!f.eof())
				{
					f.read((char *)&adc_data,4);
					//printf("%x\n",data);
					if((adc_data&0xffff)==0x90eb)
					{
						//printf("good!");
						//f.read((char *)&ch_data,1);
						//printf("%x\n",ch_data);
						//if((ch_data&0xff)==0x90)
						{						
							//printf("good!");
							f.read(&ch_data,1);
							f.read(&ch_data,1);
							for(int i=0;i<180;i++)
							{
								f.read(&ch_data,1);
								ch=int(ch_data&0xff);
								if(ch>191)continue;
								//printf("%d\t",ch);
								f.read((char *)&adc_data,2);
								adc=int((adc_data<<8)&0xff00)+int((adc_data>>8)&0xff);
								//printf("%d\n",adc);
								t_adc[ch]=adc-mean[ch];
								h[ch]->Fill(adc);
							}
							t->Fill();
						}
					}
					else continue;
    			}
		}
  		else
    	{
      		cout << "Error opening file";
    	}
		f.close();
		rootf->Write();
}
