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

#include "SiHit.hh"
#include "G4ios.hh"
#include "G4VVisManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"

G4Allocator<SiHit> SiHitAllocator;

SiHit::SiHit()
{
}


SiHit::~SiHit()
{;}

SiHit::SiHit(const SiHit &right)
  : G4VHit()
{
  edep = right.edep;
}
const SiHit& SiHit::operator=(const SiHit &right)
{
  edep = right.edep;
  return *this;
}

G4int SiHit::operator==(const SiHit &right) const
{
  return (this==&right) ? 1 : 0;
}

void SiHit::Draw()
{
 
}

void SiHit::Print()
{;}


