TFile* ReadRaw(char* filename, bool crc_flag, bool rms_flag,unsigned int data_mode)
{
	Float_t mean_adc[CH_NUM];//in idensifier.h, CH_NUM=384;
	Float_t nadc_array[CH_NUM];
	Float_t rms_adc[CH_NUM];
	int nADC = 0;


	if(rms_flag)gROOT->ProcessLine(".L WriteRMS.cc");
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
		TH1F *h[CH_NUM];
		for (int i= 0; i< CH_NUM; i++)
		{
			sprintf(Hname, "Ch%d",i);
			h[i] = new TH1F(Hname,"ADC_spectrum" ,8192,0,16384);
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
		t->Branch("trigger",&seq_check,"trigger/I");

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
					if(count!=0)
					{
						for(int i=0;i<CH_NUM;i++)//read for each channel of VA(384 in total)
						{
							//calculate the channel number
							ch= int((i%2)*192+i/2);
							if((ch>(CH_NUM-1)))continue ;
							//read the ADC
							f.read(( char *)&adc,2);
							if(f.eof())break;
							adc= int(adc&0x0000ffff);
							t_adc[ch]=adc;
							h[ch]->Fill(adc);//fill the array of histogram
							


						}

						for(int ii=0;ii<8;ii++)f.read(&ch_data,1);

						t->Fill();
						for (int i= 0; i< CH_NUM; i++)
						{
							t_adc[i]=0;
						}
					}
					count++;
				}
				else continue;			
			}
			else continue ;
		}
		f.close();


		Float_t mini_MEAN,max_MEAN,mini_RMS,max_RMS;

		mini_MEAN=0.0;
		max_MEAN=400.0;
		mini_RMS=0.0;
		max_RMS=100.0;

		TCanvas *c0 = new TCanvas("c0","TMP",200,10,800,700);
		c0->SetFillColor(10);
		c0->SetGrid();	
		gStyle->SetOptFit(1);
		c0->GetFrame()->SetFillColor(21);
		c0->GetFrame()->SetBorderSize(12);
		//c0->SetLogx(1); 
		c0->SetLogy(1); 

		TTimer timer("hsumUpdate()",10000000); 
		timer.TurnOn();

		Int_t speed_sign=1;
		Int_t speed_value=0;

		cout<<"Scanning speed(1: Low, 2: Medium, 3:High):";
		cin>>speed_sign;
		switch(speed_sign)
		{
		case 1:
			speed_value=500000;
			break;
		case 2:
			speed_value=300000;
			break;
		case 3:
			speed_value=0;
			break;
		}

		for(int ii=0;ii<CH_NUM;ii++)
		{
			if (gSystem->ProcessEvents()) break;

			ch= int((ii%2)*192+ii/2);//Calculate the channel number of VA
			mean_adc[ch] = h[ch]->GetMean();//Calculate the Pedestal
			rms_adc[ch]= h[ch]->GetRMS();//Calculate the RMS

			if(mini_MEAN>mean_adc[ch])//to find the minimum value of mean_adc
				mini_MEAN=mean_adc[ch];
			if(max_MEAN<mean_adc[ch])//to find the maximum value of mean_adc
				max_MEAN=mean_adc[ch];

			if(mini_RMS>rms_adc[ch])//to find the minimum value of rms_adc
				mini_RMS=rms_adc[ch];
			if(max_RMS<rms_adc[ch])//to find the maximum value of rms_adc
				max_RMS=rms_adc[ch];

			h[ii]->GetXaxis()->SetRangeUser(0.,8192.); // Set the range

			h[ii]->SetFillColor(kYellow); // Fill fill color to yellow
			h[ii]->SetMarkerStyle(20); 
			h[ii]->SetMarkerColor(kBlue); 
			h[ii]->SetMarkerSize(.6); // Draw histograms with blue markers

			h[ii]->GetXaxis()->SetTitle("ADC bins"); 
			h[ii]->GetYaxis()->SetTitle("Counts"); 


			h[ii]->Draw();

			timer.TurnOff();
			hsumUpdate();

			for(int jj=0;jj<speed_value;jj++)
			{
				Float_t aa=100.9;
				Float_t bb=20.9;
				Float_t cc=aa*bb;
			}


			if(ii<10)
				cout<<"Mean ADC "<<ch<<" "<<mean_adc[ch]<<" "<<rms_adc[ch]<<endl;//screen output the data
			nadc_array[ch] = ch;	
		}


		if(data_mode==0xA0))//only for Pedestal and rms calculation(Pedetal.cpp)

		{
			TCanvas *c1 = new TCanvas("c1","Summary",200,10,800,700);

			c1->SetFillColor(10);
			c1->Divide(1,2);
			c1->SetGrid();
			gStyle->SetOptFit(1);
			c1->GetFrame()->SetFillColor(21);
			c1->GetFrame()->SetBorderSize(12);


			c1->cd(1);

			TGraph *Mean_ADC = new TGraph(CH_NUM, nadc_array,mean_adc);//draw the pedestal for 384 channels
			Mean_ADC->SetMarkerColor(2);
			Mean_ADC->SetLineStyle(15);
			Mean_ADC->SetLineColor(kRed);		
			Mean_ADC->SetMarkerStyle(20);
			Mean_ADC->GetYaxis()->SetRange(mini_MEAN-0.5,max_MEAN-0.5);

			Mean_ADC->SetTitle("Pedestal");
			Mean_ADC->GetXaxis()->SetTitle("Channels of VA chips"); 
			Mean_ADC->GetYaxis()->SetTitle("Mean of ADC value"); 

			Mean_ADC->Draw("AP");

			c1->cd(2);

			TGraph *RMS_ADC = new TGraph(CH_NUM, nadc_array,rms_adc);//draw the rms for 384 channels
			RMS_ADC->SetMarkerColor(2);
			RMS_ADC->SetLineStyle(15);
			RMS_ADC->SetLineColor(kRed);
			RMS_ADC->SetMarkerStyle(20);
			RMS_ADC->GetYaxis()->SetRange(mini_RMS-0.5,max_RMS-0.5);


			RMS_ADC->SetTitle("RMS");
			RMS_ADC->GetXaxis()->SetTitle("Channels of VA chips"); 
			RMS_ADC->GetYaxis()->SetTitle("RMS of ADC value"); 

			RMS_ADC->Draw("AP");
		}

		rootf->Write();
		//if(rms_flag)WriteRMS(filename,h);
	}
	else
	{
		cout << "Error opening file" ;
	}


	return rootf;
}

void hsumUpdate()
{
	// called when Timer times out
	//if (slider) slider->SetRange(0,ratio);
	c0->Modified();
	c0->Update();
}