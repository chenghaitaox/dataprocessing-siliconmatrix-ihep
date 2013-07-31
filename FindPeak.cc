Float_t FindPeak(TFile*rootf,int num, int* p,Float_t *slope)
{
	char Hname[10];
	char Pname[10];
	char output[100];
	sprintf(Hname,"Ch%d",num); 
	TH1F*h=(TH1F*)(rootf->Get(Hname));
	Float_t fPositionX[100];
	Float_t fPositionY[100];
    Int_t first = h->GetXaxis()->GetFirst();
    Int_t last  = h->GetXaxis()->GetLast();
    Int_t size = last-first+1;
	Int_t fNPeaks = 0;
	Int_t i,nfound,bin;
	Float_t a;
	Float_t nonlinear=0;
	Double_t xmin  = 0;
	Double_t xmax  = (Double_t)size;
	Float_t * source = new float[size];
	Float_t * dest = new float[size];
    for (i = 0; i < size; i++) source[i] = h->GetBinContent(i + first);
	TSpectrum *s = new TSpectrum();
	nfound = s->SearchHighRes(source, dest, size, 8, 2, kTRUE, 3, kTRUE, 3);
	Float_t *xpeaks = s->GetPositionX();
	for (i = 0; i < nfound; i++)
	{
		a=xpeaks[i];
		bin = 1 + Int_t(a + 0.5);
		fPositionX[i] = h->GetBinCenter(bin);
		fPositionY[i] = h->GetBinContent(bin);
	}
	TPolyMarker * pm = (TPolyMarker*)h->GetListOfFunctions()->FindObject("TPolyMarker");
	if (pm)
	{
		h->GetListOfFunctions()->Remove(pm);
		delete pm;
	}
	pm = new TPolyMarker(nfound, fPositionX, fPositionY);
	h->GetListOfFunctions()->Add(pm);
	pm->SetMarkerStyle(23);
	pm->SetMarkerColor(kRed);
	pm->SetMarkerSize(1.3);
	h->Draw();
	c1->Modified();
	c1->Update();
	c1->Print("results.pdf");
	printf("Found %d candidate peaks\n",nfound);
	*p=nfound;
	for(i=0;i<nfound;i++)printf("posx= %d, posy= %d\n",fPositionX[i], fPositionY[i]);
	QuickSort(fPositionX,0,nfound-1);
	Float_t linY[100];
	for(int ii=0;ii<nfound;ii++)linY[ii]=ii+1;
	TGraph* line=new TGraph(nfound, linY,fPositionX);
	sprintf(Pname,"Ch%d",num);
	line->SetTitle(Pname);
	TF1*f1=new TF1("f1","[0]+[1]*x",0,nfound+5);
	f1->GetXaxis()->SetRangeUser(0,nfound+5);
	line->Fit("f1");
	//Draw the fit line
	line->Draw("AP*");
	c1->Modified();
	c1->Update();
	TPaveText *pt = new TPaveText(0.6,0.2,0.9,0.4,"brNDC");
	sprintf(output,"the intercept is %d",int(f1->GetParameter(0)));
	pt->AddText(output);
	sprintf(output, "the slope is %d",int(f1->GetParameter(1)));
	pt->AddText(output);
	*slope=f1->GetParameter(1);
	pt->Draw();
	c1->Print("results.pdf");
	line->Write();
	//get the nonlinear
	for(i=0;i<nfound;i++)
	{
		a=fabs(fPositionX[i]-(f1->GetParameter(0))-(f1->GetParameter(1)*linY[i]));
		//cout<<a<<endl;
		nonlinear=a>nonlinear?a:nonlinear;
	}
	//cout<<nonlinear<<endl;
	//getchar();
	delete line;
	return nonlinear/fPositionX[nfound-1];
}
