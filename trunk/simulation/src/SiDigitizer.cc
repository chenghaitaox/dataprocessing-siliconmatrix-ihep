//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//


#include "SiDigitizer.hh"
#include "SiDigi.hh"

#include "SiHit.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4DigiManager.hh"
#include "G4ios.hh"
#include "model.hh"
//#include "G4CollectionNameVector.hh"
#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

SiDigitizer::SiDigitizer(G4String name)
  :G4VDigitizerModule(name)
{
	G4String colName = "DigitsCollection";
	collectionName.push_back(colName);
	Energy_Threshold = 0.*keV;
	TotalEnergy = 0.;
	ACDThreshold = 15*keV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

SiDigitizer::~SiDigitizer()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void SiDigitizer::Digitize()
{
	DigitsCollection = new SiDigitsCollection
	("SiDigitizer","DigitsCollection"); // to create the Digi Collection
  
	G4DigiManager* DigiMan = G4DigiManager::GetDMpointer();
  
	G4int THCID; // HitCollection


	TotalEnergy = 0.; // at each event
  
// silicon Hits collection
  
	THCID = DigiMan->GetHitsCollectionID("SiCollection");  
	SiHitsCollection* THC = 0; 
	THC = (SiHitsCollection*)(DigiMan->GetHitsCollection(THCID));

#ifndef Det_TEST

	if (THC)
	{
		G4int n_hit = THC->entries();
		G4double ESil=0;
		G4int NDet=0;
		for (G4int i=0;i<n_hit;i++)
		{
			ESil = (*THC)[i]->GetEdep();
			NDet = i+1;
// digi generation only if energy deposit greater than threshold
			//if (ESil>Energy_Threshold)
	    	{
				SiDigi* Digi = new SiDigi();
				Digi->SetCpNo(NDet);		  
	      		//Digi->SetSiX(((NDet%num_unit)?NDet%num_unit:num_unit));
	      		//Digi->SetSiY(1+int((NDet-1)/num_unit));
	      		Digi->SetADC(G4RandGauss::shoot(0,10)*keV+G4RandGauss::shoot(1,0.1)*ESil/keV*keV);
	      		DigitsCollection->insert(Digi);
	    	} 
		}
	}
 
	StoreDigiCollection(DigitsCollection);
#endif  
	G4int DCID = -1;
	if(DCID<0)
	{ 
		//DigiMan->List();
		DCID = DigiMan->GetDigiCollectionID("SiDigitizer/DigitsCollection");
    }
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....










