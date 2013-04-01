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


#ifndef SiDigi_h
#define SiDigi_h 1

#include "G4VDigi.hh"
#include "G4TDigiCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class SiDigi : public G4VDigi
{

public:
  
  SiDigi();
  ~SiDigi();
  SiDigi(const SiDigi&);
  const SiDigi& operator=(const SiDigi&);
  int operator==(const SiDigi&) const;
  
  inline void* operator new(size_t);
  inline void  operator delete(void*);
  
  void Draw();
  void Print();

private:
  G4int CpNo;   //  the det copyno
  int ADC; // ADC 
  
public:
  
  inline void SetCpNo(G4int num)   {CpNo = num;};
  inline void SetADC(G4double Ene)  {ADC =int((Ene/keV)/1.575)+40;};
  inline G4int GetCh() {return CpNo;};
  inline int GetADC()  {return ADC;};
  

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

typedef G4TDigiCollection<SiDigi> SiDigitsCollection;

extern G4Allocator<SiDigi> SiDigiAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline void* SiDigi::operator new(size_t)
{
  void* aDigi;
  aDigi = (void*) SiDigiAllocator.MallocSingle();
  return aDigi;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline void SiDigi::operator delete(void* aDigi)
{
  SiDigiAllocator.FreeSingle((SiDigi*) aDigi);
}

#endif









