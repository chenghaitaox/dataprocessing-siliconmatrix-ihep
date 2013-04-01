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
//
// $Id: EventAction.cc,v 1.1.1.1 2013/02/23 13:30:13 rui Exp $
// GEANT4 tag $Name:  $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "model.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SiHit.hh"
#include "SiSD.hh"
#include "SiDigi.hh"
#include "SiDigitizer.hh"
#include "Randomize.hh"  
#include "G4Event.hh"
#include "G4TrajectoryContainer.hh"
#include "G4VTrajectory.hh"
#include "G4VVisManager.hh"
#include "G4UnitsTable.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4DigiManager.hh"
#include "Randomize.hh"
#include "DataBase.hh"
#include <iomanip>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;
#ifndef Det_TEST
extern G4int num_unit;
#endif
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//class StackingAction
EventAction::EventAction(RunAction*rct)
:drawFlag("all"),printModulo(1),id(-1),runact(rct)
{
	G4DigiManager * fDM = G4DigiManager::GetDMpointer();
	SiDigitizer * myDM = new SiDigitizer( "SiDigitizer" );
	fDM->AddNewModule(myDM);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{
 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void EventAction::BeginOfEventAction(const G4Event* evt)
{ 
	G4SDManager * SDman = G4SDManager::GetSDMpointer(); 
 
	if(id<0)
	{
		G4String colNam;
		id = SDman->GetCollectionID(colNam="SiCollection");
	}


	G4int evtNb = evt->GetEventID();
	G4cout << "\n---> Begin of event: " << evtNb << G4endl;
	EnergyAbs=0; 
 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* evt)
{
	G4int evtNb = evt->GetEventID();
#ifndef Det_TEST
	G4DigiManager * fDM = G4DigiManager::GetDMpointer();
	DataBase*newdata=runact->GetDataBase();
	newdata->ResetOnline();
	G4int P_id=0;
	G4double Ede=0;
	if(id<0) return;
	G4HCofThisEvent * HCE = evt->GetHCofThisEvent();
	SiHitsCollection* PHC = 0;

	if(HCE||PHC)
	{
		PHC= (SiHitsCollection*)(HCE->GetHC(id));
	} 
	if(PHC)
	{
		int n_hit = PHC->entries();
		for(int i=0;i<n_hit;i++)
		{  
			Ede=(*PHC)[i]->GetEdep();
			newdata->FillTrueth(((*PHC)[i]->GetEdep())/keV,i);
		}
		newdata->SaveTrueth();
	}
	SiDigitizer * myDM =
	(SiDigitizer*)fDM->FindDigitizerModule( "SiDigitizer" );
	myDM->Digitize();

	G4int myDigiCollID = fDM->GetDigiCollectionID("DigitsCollection");

	SiDigitsCollection * DC = (SiDigitsCollection*)fDM->GetDigiCollection( myDigiCollID );
	if(DC)
	{
		G4int n_digi =  DC->entries();
		int NDet = 0;
		for (G4int i=0;i<n_digi;i++)
		{
			NDet=i+1;
			newdata->FillData((*DC)[i]->GetADC(),NDet-1);
			newdata->FillOnline( (*DC)[i]->GetADC());
			newdata->Fill2DOnline((*DC)[i]->GetADC(),((NDet%num_unit)?NDet%num_unit:num_unit),(1+int((NDet-1)/num_unit)));
		}
			newdata->SaveData(evtNb);
//		printf("Complete!\n");
	}

#endif
	G4cout << "---> End of event: " << evtNb << G4endl;	
	if(!(evtNb%27))sleep(1);
	G4TrajectoryContainer* trajectoryContainer = evt->GetTrajectoryContainer();
	G4int n_trajectories = 0;
	if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();

}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
