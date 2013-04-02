{
        TH1F *h1= new TH1F("test1" ,"test spectrum",8192,0,70);//nonlinear with correction
        TH1F *h2= new TH1F("test2" ,"test spectrum",8192,0,70);//linear
        TH1F *h3= new TH1F("test3" ,"test spectrum",8192,0,70);//nonlinear without correction
        TH1F *energy1= new TH1F("charge1" ,"total charge",8192,0,1500); //nonlinear with correction
        TH1F *energy2= new TH1F("charge2" ,"total charge",8192,0,4096); //linear
        TRandom *r0 = new TRandom();
         float nonlinear=0.25;//75% charge collection by mid strip
         int atom=8;//8th nuclei
         double x=0;
         double qq1=0;
         double qq2=0;
         //the cosmic ray ratio
         float cosmic[28]={540,26,0.4,0.4,0.4,2.2,2.2,2.2,0.3,0.3,0.22,0.22,0.19,0.19,0.03,0.03,0.01,0.01,0.02,0.02,0.05,0.05,0.05,0.05,0.05,0.12,0.12,0.12};
         for(int i=1;i<28;i++)
        {
                 int num=cosmic[i-1]*100000;
                 for(int j=0;j<num;j++)
                {
                        qq1=0;
                         for(int l=0;l<6;l++)qq1+=i*4*r0->Gaus(0,0.4);
                        qq1=qq1/6;
                        qq2=i*4*r0->Gaus(0,0.4);
                        x=sqrt(qq2+i*i*4);
                        h2->Fill(x);
                        energy2->Fill(qq2+i*i*4);
                         if(i<(atom+1))
                        {
                                x=sqrt(qq1+i*i*4);
                                energy1->Fill(qq1+i*i*4);
                                h1->Fill(x);
                                h3->Fill(sqrt(qq2+i*i*4));
                        }
                         else
                        {
                                x=sqrt(qq1+(i*i-atom*atom)*4*nonlinear+atom*atom*4);
                                h1->Fill(x);
                                h3->Fill(sqrt(qq2+(i*i-atom*atom)*4*nonlinear+atom*atom*4));
                                energy1->Fill(qq1+(i*i-atom*atom)*4*nonlinear+atom*atom*4);
                        }
                }
        }
        h1->SetLineColor(1);
        h1->GetXaxis()->SetTitle( "cluster charge1/2" );
        h2->SetLineColor(2);
        h2->GetXaxis()->SetTitle( "cluster charge1/2" );
        h3->SetLineColor(6);
        h3->GetXaxis()->SetTitle( "cluster charge1/2" );
        energy1->SetLineColor(3);
        energy2->SetLineColor(4);
        energy2->Draw();
}
