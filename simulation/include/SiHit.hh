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

#ifndef SiHit_h
#define SiHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4String.hh"
class SiHit : public G4VHit
{
  public:

      SiHit();
      ~SiHit();
      SiHit(const SiHit &right);
      const SiHit& operator=(const SiHit &right);
      G4int operator==(const SiHit &right) const;

      inline void *operator new(size_t);
      inline void operator delete(void *aHit);

      void Draw();
      void Print();

  private:
       G4double edep;
  public:
      inline void SetEdep(G4double de)
      { edep =de;}
      inline void AddEdep(G4double de)
        { edep +=de;}
        inline G4double GetEdep()
      { return edep; }

};

typedef G4THitsCollection<SiHit> SiHitsCollection;

extern G4Allocator<SiHit> SiHitAllocator;

inline void* SiHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) SiHitAllocator.MallocSingle();
  return aHit;
}

inline void SiHit::operator delete(void *aHit)
{
  SiHitAllocator.FreeSingle((SiHit*) aHit);
}

#endif


