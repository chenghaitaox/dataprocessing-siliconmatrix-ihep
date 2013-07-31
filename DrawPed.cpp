#!/home/rui/work/root/bin/root
#Filename: DrawPed.cpp
{
#include "Identifier.h"
	char filename[100];
	Float_t rms[CH_NUM];
	Float_t mean[CH_NUM];
	float ch[CH_NUM];
	float c2q=0.035;
	cout<< "please input the RMS file name(.txt):" ;
	cin>>filename;
	ifstream c_f(filename);
	if(c_f.is_open())
	{
		for(int i=0; i< CH_NUM; i++)
		{
			c_f>>ch[i]>>mean[i]>>rms[i];
			rms[i]=rms[i]*c2q;
			cout<<ch[i]<<mean[i]<<rms[i]<<endl;
		}
		c_f.close();
	}
	else cout<<"Can't open file!"<<endl;
    TCanvas *c1= new TCanvas("c1" );
	c1->Print("results.pdf[");

	TGraph*stic_rms=new TGraph(CH_NUM,ch,rms);
	stic_rms->GetXaxis()->SetTitle("Channel");
	stic_rms->GetYaxis()->SetTitle("rms/fC");
	stic_rms->SetFillColor(4);
	stic_rms->Draw("AB");
	c1->Print("results.pdf");

	TGraph*stic_mean=new TGraph(CH_NUM,ch,mean);
	stic_mean->GetXaxis()->SetTitle("Channel");
	stic_mean->GetYaxis()->SetTitle("mean/ADC");
	stic_mean->SetFillColor(4);
	stic_mean->Draw("AB");
	c1->Print("results.pdf");
	c1->Print("results.pdf]");
}