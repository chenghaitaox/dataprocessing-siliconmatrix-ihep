void LinearCal(TFile*calname)
{		
	gROOT->ProcessLine(".L QuickSort.cc");
	gROOT->ProcessLine(".L FindPeak.cc");
	gROOT->ProcessLine(".L FloatCheck.cc");

	char rootname[100]="linearCal.root";
	Float_t linear_fit[CH_NUM];
	Float_t non_linear[CH_NUM];
	float just_number[CH_NUM];
	bool funflag=true;
	int point_c[CH_NUM];
	double slope_rms;
	double slope_mean;
	int point_mean;
	//initial data file
	TFile * outf=new TFile(rootname,"RECREATE");
    TCanvas *c1= new TCanvas("c1" );
	c1->Print("results.pdf[");
	//gStyle->SetOptFit();
	TH1F* slope_h=new TH1F("slope","Statistics of slope",200,0,1000)	;
	TH1I* point_h=new TH1I("point","Statistics of points",100,0,100)	;
	TH2F* total2=new TH2F("total2","Slope vs points",200,0,1000,200,0,100);
	for (int i= 0; i< CH_NUM; i++)
	{
		if(FloatCheck(i))
		{
			non_linear[i]=FindPeak(calname,i,&point_c[i],&linear_fit[i]);
			slope_h->Fill(linear_fit[i]);
			point_h->Fill(point_c[i]);
			total2->Fill(linear_fit[i],point_c[i]);
			just_number[i]=i+1;
			//cout<<just_number[i]<<"\t"<<non_linear[i]<<endl;
			//getchar();
		}
	}

	slope_h->Draw();
	c1->Print("results.pdf");
	c1->Modified();
	c1->Update();
	total2->SetMarkerStyle(3);
	total2->Draw();
	c1->Print("results.pdf");
	TGraph*stic_nonlinear=new TGraph(CH_NUM,just_number,non_linear);
	stic_nonlinear->GetXaxis()->SetTitle("Channel");
	stic_nonlinear->GetYaxis()->SetTitle("Nonlinearity");
	stic_nonlinear->SetFillColor(4);
	stic_nonlinear->Draw("AB");
	c1->Print("results.pdf");
	c1->Print("results.pdf]");

	slope_rms=slope_h->GetMean();
	slope_mean=slope_h->GetRMS();
	point_mean=point_h->GetMaximumBin()-1;
	cout<<"the following channel has a bad response to calibration"<<endl;
	cout<<"channel\tslope\tpoints\n";
	for (int i= 0; i< CH_NUM; i++)
	{
		if(FloatCheck(i))
		{
			if(!((point_c[i]==point_mean)&&((linear_fit[i]>(slope_mean-3*slope_rms))||(linear_fit[i]<(slope_mean+3*slope_rms)))))
			cout<<i<<"\t"<<linear_fit[i]<<"\t"<<point_c[i]<<"\n";
		}
	}
	outf->cd();
	slope_h->Write();
	point_h->Write();
	total2->Write();
	stic_nonlinear->Write();
	calname->Write();
}
