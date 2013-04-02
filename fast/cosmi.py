#!/usr/bin/python
# Filename : helloworld.py
from ROOT import TH1F,TRandom,TFile
import math
r0 = TRandom()
nonlinear=0.25
atom=8
h1= TH1F("test1" ,"test spectrum",8192,0,70)
h2= TH1F("test2" ,"test spectrum",8192,0,70)
h3= TH1F("test3" ,"test spectrum",8192,0,70)
energy1= TH1F("charge1" ,"total charge",8192,0,4096)
energy2= TH1F("charge2" ,"total charge",8192,0,4096)
f= TFile("results.root","RECREATE")
cosmic=[540,26,0.4,0.4,0.4,2.2,2.2,2.2,0.3,0.3,0.22,0.22,0.19,0.19,0.03,0.03,0.01,0.01,0.02,0.02,0.05,0.05,0.05,0.05,0.05,0.12,0.12,0.12]
for i in range(1,28):
     num=int(cosmic[i-1]*100000)
     for j in range(0,num):
          qq1=0
          for l in range(0,6):
               qq1=qq1+i*4*r0.Gaus(0,0.4)
          qq1=qq1/6
          x=-1
          while x<0:
               qq2=i*4*r0.Gaus(0,0.2)
               x=qq2+i*i*4
          h2.Fill(math.sqrt(x))
          energy2.Fill(qq2+i*i*4)
          if i<(atom+1):
               x=qq1+i*i*4
               energy1.Fill(qq1+i*i*4)
               h1.Fill(math.sqrt(x))
               x=qq2+i*i*4
               h3.Fill(math.sqrt(x))
          else:
               x=qq1+(i*i-atom*atom)*4*nonlinear+atom*atom*4
               h1.Fill(math.sqrt(x))
               x=qq2+(i*i-atom*atom)*4*nonlinear+atom*atom*4
               h3.Fill(math.sqrt(x))
               energy1.Fill(qq1+(i*i-atom*atom)*4*nonlinear+atom*atom*4)

h1.SetLineColor(1);
h1.GetXaxis().SetTitle( "cluster charge1/2" )
h2.SetLineColor(2);
h2.GetXaxis().SetTitle( "cluster charge1/2" )
h3.SetLineColor(6);
h3.GetXaxis().SetTitle( "cluster charge1/2" )
energy1.SetLineColor(3);
energy2.SetLineColor(4);
h1.Write()
h2.Write()
h3.Write()
energy1.Write()
energy2.Write()
f.Close()
