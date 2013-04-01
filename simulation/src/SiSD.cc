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
// $Id: SiSD.cc,v 1.1.1.1 2013/02/23 13:30:13 rui Exp $
// GEANT4 tag $Name:  $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "SiSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"
#include "G4ParticleTypes.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SiSD::SiSD(G4String name,G4int nCells)
:G4VSensitiveDetector(name),numberOfCells(nCells),HCID(-1)
{
  G4String HCname;
  collectionName.insert(HCname="SiCollection");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SiSD::~SiSD()
{
  
 }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SiSD::Initialize(G4HCofThisEvent* HCE)
{
  SiCollection = new SiHitsCollection
                        (SensitiveDetectorName,collectionName[0]); 
 if(HCID<0)
  { HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); }
  HCE->AddHitsCollection( HCID, SiCollection );  
       for(G4int j=0;j<numberOfCells;j++)
  {
     SiHit* newHit = new SiHit();
 	 newHit->SetEdep(0);
  	 SiCollection->insert( newHit );
    }
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool SiSD::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  e = aStep->GetTotalEnergyDeposit();
  if(e==0.) return false;

  G4int copyID =aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(3);
 (*SiCollection)[copyID-1]->AddEdep(e);
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SiSD::EndOfEvent(G4HCofThisEvent*HCE)
{

} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

