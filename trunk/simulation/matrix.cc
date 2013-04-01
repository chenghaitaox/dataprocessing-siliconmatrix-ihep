//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// Description: Test of DAMPE silicon matrix 
//              with a ROOT based monitor to get Energy and position
// 
// Version:     1.0
// Created:     2012-04-10
// Author:      henry
// mail:        fanrr@ihep.ac.cn

// Updated:
//     
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#include "G4ios.hh"
#include <stdlib.h>
#include "TThread.h"

#include "DetectorConstruction.hh"
#include "QGSP_BERT_HP.hh"
#include "QGSP_BERT.hh"
#include "PrimaryGeneratorAction.hh"
#include "EventAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"
#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif
#include "model.hh"
#include "G4TransportationManager.hh"
#include <stdexcept>

#ifndef Det_TEST
// a global canvas for the drawing of histograms
TCanvas *c1;
// a pad for the histogram, will be used by thread mhs
TPad *pad1;
// a pad for the histogram, will be used by thread mhs2
TPad *pad2;
TThread *th1;
TThread *thp ;
extern TH1F *total;
extern TH2F *map_online;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void *g4(void *){
#else
	void g4(){
#endif
  	G4RunManager* runManager = new G4RunManager;

// UserInitialization classes - mandatory
	runManager-> SetUserInitialization(new DetectorConstruction);
	runManager-> SetUserInitialization(new QGSP_BERT);

#ifdef G4VIS_USE
// visualization manager
	G4VisManager* visManager = new G4VisExecutive;
	visManager->Initialize();
#endif
	// UserAction classes 

	runManager->SetUserAction(new PrimaryGeneratorAction);
	RunAction * run_action=new RunAction();
	runManager->SetUserAction(run_action);
	EventAction* event_action = new EventAction(run_action);
	runManager->SetUserAction(event_action);
	SteppingAction* stepping_action =new   SteppingAction( event_action);
	runManager->SetUserAction(stepping_action);

//Initialize G4 kernel
	runManager->Initialize();
// get the pointer to the User Interface manager 
	G4UImanager* UI = G4UImanager::GetUIpointer(); 
   
	G4UIsession* session = 0;
	session = new G4UIterminal(new G4UItcsh);      
	//UI->ApplyCommand("/control/execute vis.mac");
	session->SessionStart();
	delete session;
    
// job termination
#ifdef G4VIS_USE
	delete visManager;
#endif
	delete runManager;  
	printf("NOTE: Use the ROOT menu quit to exit.\n");


}
#ifndef Det_TEST
void *draw(void*)
{
	c1->cd();
	c1->SetGrid();
	pad1->Draw();
	pad2->Draw();
	total->SetXTitle("energy/keV");
	map_online->GetZaxis()->SetRangeUser(0,150);
	while(1)
	{
		if(total!=0)
		{
			pad1->cd();
			th1->Lock();
			total->Draw();
			th1->UnLock();
    		pad1->Modified();
			pad1->Update();
			pad2->cd();
			th1->Lock();
			map_online->Draw("LEGO2");
			th1->UnLock();
			pad2->Modified();
			pad2->Update();
			c1->Modified();
			c1->Update();
			c1->Update();
			sleep(1);
		}
	}
}
#endif

int main(int argc,char** argv)
{
	printf("************************************************\n");
	printf("*      this program is based on ROOT 5.28      *\n");
	printf("*              and GEANT4.9.4.p02              *\n");
	printf("*     which are recommend for the program      *\n");
	printf("*    if you have questions, please mail to     *\n");
	printf("*             fanrr@ihep.ac.cn                 *\n");
	printf("************************************************\n");
	printf("press Enter to continue........\n");
	getchar();
#ifndef Det_TEST
	TApplication theApp("App", &argc, argv);   
	c1 = new TCanvas("c1","Online monitor",800,400);
	pad1 = new TPad("pad1","the energy deposited",0.02,0.02,0.48,0.98,33);
	pad2 = new TPad("pad2","the hit map",0.52,0.02,0.98,0.98,33);
	th1 = new TThread("draw",draw);
	thp = new TThread("g4",g4);
	thp->Run();
	sleep(1);
	th1->Run();
	sleep(1);
	theApp.Run(kTRUE);
	thp->SetCancelAsynchronous();
	th1->SetCancelAsynchronous();
	thp->Kill();
	th1->Kill();
#else
	g4();
#endif
	printf("The END...\n");
	return 0;
}


