#!/home/rui/work/root/bin/root
{
#define CH_NUM 192
         //define
         int adc_data;
         char ch_data;
         int adc;
         int t_adc[CH_NUM];
         int ch;
         int data_q;
         int flength=0;
         float mean[CH_NUM],rms[CH_NUM];
         char filename[100];
         char rootname[100];
         char rmsname[100];
         char Hname[10];
         char Tname[10];
         char dsuffix[10]=".bin" ;
         char csuffix[20]=".bin_RMSE.txt" ;
         char rsuffix[10]="_bin.root" ;
         int mode=0;

         for(int i=0; i< CH_NUM; i++)
        {
                mean[ch]=0;
                rms[ch]=0;
        }
         //mode selection
        cout<< "please input mode: 1 generate rms; 2  read data:" <<endl;
        cin>>mode;
         switch(mode)
        {
         case 1:
                 //initial data file
                cout<< "please input the binary file name(without'.bin'):" ;
                cin>>filename;
                strcpy(rootname,filename);
                strcpy(rmsname,filename);
                strcat(rmsname,csuffix);
                strcat(filename,dsuffix);
                 //initial root file
                strcat(rootname,rsuffix);
                TFile * rootf= new TFile(rootname,"RECREATE" );
                 //convert to histogram;
                TH1F *h[192];
                 for (int i= 0; i< CH_NUM; i++)
                {
                        sprintf(Hname, "Ch%d",i);
                        h[i] = new TH1F(Hname,"spectrum" ,8192,0,16384);
                }
                 //reading
                 int tmpl;
                ifstream f(filename,ifstream::binary);

                 if(f.is_open())
                {
                         while (!f.eof())
                        {
                                f.read(&ch_data,1);
                                 if(((ch_data)&0x00ff)==0xeb)
                                {
                                        f.read(&ch_data,1);
                                         if(((ch_data)&0x00ff)==0x90)
                                        {
                                                f.read(&ch_data,1);
                                                f.read(&ch_data,1);
                                                f.read(&ch_data,1);
                                                data_q= int(ch_data&0x00ff);
                                                f.read(&ch_data,1);
                                                data_q= int((data_q<<8)&0xff00)+int(ch_data&0x00ff);
                                                data_q=(data_q-6)/3;
                                                 for(int i=0;i<data_q;i++)
                                                {
                                                        f.read(&ch_data,1);
                                                        ch= int(ch_data&0x00ff);
                                                         if(ch>191)continue ;
                                                        f.read(( char *)&adc_data,2);
                                                        adc= int((adc_data<<8)&0xff00)+int((adc_data>>8)&0x00ff);
                                                        h[ch]->Fill(adc);
                                                }

                                        }
                                }
                                 else continue ;
                        }
                }
                 else
                {
                        cout << "Error opening file" ;
                }
                f.close();
                rootf->Write();
                ofstream c_f(rmsname);
                 for (int i= 0; i< CH_NUM; i++)
                {
                        c_f<<i<< "\t"<<h[i]->GetMean()<<"\t";
                        c_f<<h[i]->GetRMS()<<endl;
                }
                c_f.close();
                 break;
         case 2:
                 //calibrate
                cout<< "please input the calibrate file name(without'.bin_RMSE.txt'):" ;
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
                        cout<< "Calibration complete!" <<endl;
                }
                 else
                {
                        cout << "Error opening file" <<endl;
                        cout << "Did not Calibrate!" <<endl;
                }
                 //initial data file
                cout<< "please input the binary file name(without'.bin'):" ;
                cin>>filename;
                strcpy(rootname,filename);
                strcat(filename,dsuffix);
                 //initial root file
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
                 //reading
                 int tmpl;
                ifstream f(filename,ifstream::binary);
                 if(f.is_open())
                {
                         while (!f.eof())
                        {
                                f.read(&ch_data,1);
                                 if(((ch_data)&0x00ff)==0xeb)
                                {
                                        f.read(&ch_data,1);
                                         if(((ch_data)&0x00ff)==0x90)
                                        {
                                                f.read(&ch_data,1);
                                                f.read(&ch_data,1);
                                                f.read(&ch_data,1);
                                                data_q= int(ch_data&0x00ff);
                                                f.read(&ch_data,1);
                                                data_q= int((data_q<<8)&0xff00)+int(ch_data&0x00ff);
                                                data_q=(data_q-6)/3;
                                                 for(int i=0;i<data_q;i++)
                                                {
                                                        f.read(&ch_data,1);
                                                        ch= int(ch_data&0x00ff);
                                                         if(ch>191)continue ;
                                                        f.read(( char *)&adc_data,2);
                                                        adc= int((adc_data<<8)&0xff00)+int((adc_data>>8)&0x00ff);
                                                        t_adc[ch]=adc-mean[ch];
                                                        h[ch]->Fill(adc);
                                                }

                                                t->Fill();
                                                 for (int i= 0; i< CH_NUM; i++)
                                                {
                                                        t_adc[i]=0;
                                                }
                                        }
                                }
                                 else continue ;
                        }
                }
                 else
                {
                        cout << "Error opening file" ;
                }
                f.close();
                rootf->Write();
                 break;
         default:
                cout<< "error! the input value unknow!" <<endl;
        }
}






  
