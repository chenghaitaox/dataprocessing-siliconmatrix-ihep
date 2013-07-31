void WriteRMS(char* rmsname,TH1F**h)
{
	char filename[100];
	char suffix[20]="_RMSE.txt";
	char b_name[20];
	int rms;
	int tmp_num=1;
	int size=192;
	int th=3;
	strcpy(filename,rmsname);
	strcat(filename,suffix);
	ofstream c_f(filename);
	ofstream b_f;
	float mean_arr[CH_NUM],rms_arr[CH_NUM];

	for (int i= 0; i< CH_NUM; i++)
		{
			h[i]->Fit("gaus");
			c_f<<i<< "\t"<<gaus->GetParameter(1)<<"\t";
			c_f<<gaus->GetParameter(2)<<endl;
			if(size==192)
			{
				if(b_f.is_open())b_f.close();
				sprintf(b_name,"%d.gate",tmp_num);
				b_f.open(b_name,ios_base::binary);
				tmp_num++;
				size=0;
			}
			rms=0;
			b_f.write((char*)(&rms),1);
			size++;
			rms=0x10;
			b_f.write((char*)(&rms),1);
			size++;
			rms=i;		
			b_f.write((char*)(&rms),1);
			size++;
			rms=int(th*h[i]->GetRMS()+h[i]->GetMean())/16;			
			b_f.write((char*)(&rms),1);
			size++;
		}
	c_f.close();
	b_f.close();
}
