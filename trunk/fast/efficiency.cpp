{
         //#define vis 0
         double L=25;
         double D=4.5;
         double t=0.5;
         double h=10;
         double y0=2*t+h;
         double y1=0;
         double x0=0;
         double x1=0;
         double U=2*L-2*D;
         int total=1000000;
         bool hit1=0;
         bool hit2=0;
         bool hit3=0;
         int ct1=0;
         int ct2=0;
         int ct3=0;
         int hit_s=0;
         int hit_d=0;
         int er=0;
         double theta=3.1415926/3;
         char name[50];
        TRandom2 *rnd= new TRandom2();
#ifdef vis
        TCanvas *c1= new TCanvas("c1" );
        c1->Range(-2*D,-D,U+D,y0+D);
         //TH1F*start=new TH1F("start","start point",200,0,U);
         //TH1F*end=new TH1F("end","end point",200,0,U);
        TLine *plotline;
        TBox*d1= new TBox(-D,0,L-D,t);
        d1->SetFillColor(9);
        TText*t1= new TText(L/2-D,-1,"1" );
        t1->SetTextColor(12);
        TBox*d2= new TBox(L-2*D,y0-t,U,y0);
        d2->SetFillColor(9);
        TText*t2= new TText(L+D,y0-t+1,"2" );
        t2->SetTextColor(12);
        TBox*d3= new TBox(U-D,0,U-D+L,t);
        d3->SetFillColor(9);
        TText*t3= new TText(U,-1,"3" );
        t3->SetTextColor(12);
        TBox*d4= new TBox(-L,y0-t,0,y0);
        d4->SetFillColor(9);
        d1->Draw();
        d2->Draw();
        d3->Draw();
        d4->Draw();
        t1->Draw();
        t2->Draw();
        t3->Draw();
#endif
         double a=0;
         double b=0;
         for(int i=10;i<total;i++){
                x0=(rnd->Rndm())*U;
                x1=tan((rnd->Rndm()-0.5)*2*theta)*y0+x0;
                 // start->Fill(x0);
                 // end->Fill(x1);
#ifdef vis
                c1->Modified();
                c1->Update();
                sprintf(name, "%d.png",i);
                c1->Print(name);
                 //gSystem->Sleep(1000);
                plotline= new TLine(x0,y0,x1,y1);plotline->Draw();
                plotline->SetLineColor(46);
                plotline->SetLineStyle(4);
                plotline->SetLineWidth(2);
#endif
                 //y=a*x+b
                a=(y0-y1)/(x0-x1);
                b=y0-a*x0;
                 //hit in 1
                 if((((t-b)/a)<(L-D))&&(((t-b)/a)>(-D))){
                         double hitpx=(t-b)/a;
                         double hitpy=t;
                         if((x1<(L-D))&&(x1>(-D)))hit1=true;
                         else if((x1>(L-D))&&(sqrt((L-D-hitpx)*(L-D-hitpx)+(a*(L-D)+b-hitpy)*(a*(L-D)+b-hitpy))>t))hit1= true;
                         else if(sqrt((hitpx+D)*(hitpx+D)+(-a*D+b-hitpy)*(-a*D+b-hitpy)))hit1=true;
                }
                 //hit in 2
                 if(((y0-t-b)/a)>(L-2*D)){
                         double hitpx=(y0-t-b)/a;
                         double hitpy=y0-t;
                         if(((x0)>(L-2*D)))hit2=true ;
                         else if((sqrt((hitpx-L+2*D)*(hitpx-L+2*D)+(hitpy-a*(L-2*D)-b)*(hitpy-a*(L-2*D)-b))>t))hit2=true;
                }
                 //hit in 3
                 if(((t-b)/a>(U-D))){
                         double hitpx=(y0-t-b)/a;
                         double hitpy=y0-t;
                         if((x1>(U-D)))hit3=true ;
                         else if((sqrt((U-D-hitpx)*(U-D-hitpx)+(a*(U-D)+b-hitpy)*(a*(U-D)+b-hitpy))>t))hit3=true;
                }
                 //summary
                 if(hit1&&hit3)er++;
                 else if ((hit1&&hit2)||(hit2&&hit3))hit_d++;
                 else if (hit1||hit2||hit3)hit_s++;
                 if(hit1)ct1++;
                 if(hit2)ct2++;
                 if(hit3)ct3++;
                 //renew
                hit1= false;
                hit2= false;
                hit3= false;
        }
        cout<< "the efficiency is : " <<double(hit_d+hit_s)/total<<endl;
        cout<< "the total events is: " <<total<<endl;
        cout<< "the single hit is : " <<hit_s<<endl;
        cout<< "the double hit is : " <<hit_d<<endl;
        cout<< "the event missed is : " <<total-hit_s-hit_d<<endl;
         //cout<<"the error is: "<<er<<endl;
        cout<< "hit1\t\thit2\t\thit3\n" ;
        cout<<ct1<< "\t\t"<<ct2<<"\t\t" <<ct3<<"\n";
         //start->Draw();
         //getchar();
         //end->Draw();
}
