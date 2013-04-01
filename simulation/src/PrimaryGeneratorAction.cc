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
//fan 06.06.09 2:00
//

#include "PrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "Randomize.hh"
#include "G4GeneralParticleSource.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
#ifndef General_Particle_Source
  G4int n_particle = 1;
  G4ParticleGun* fParticleGun = new G4ParticleGun(n_particle);
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
    = particleTable->FindParticle(particleName="mu-");
  fParticleGun->SetParticleDefinition(particle);
   
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,-1.));
  fParticleGun->SetParticleEnergy(4*GeV);
  fParticleGun->SetParticlePosition(G4ThreeVector(0,0,100*cm));
  particleGun = fParticleGun;
  angle=60*deg;
#else   
  particleGun = new G4GeneralParticleSource ();
#endif
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete particleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
#ifndef General_Particle_Source
  G4double x0 = 0;
  G4double y0 = 0;
  G4double z0 = -1;
  G4double tha = angle*G4UniformRand();
  G4double phi = 360*G4UniformRand()*deg;
   z0 = -cos(tha);
   y0 = sin(tha)*sin(phi);
   x0 = sin(tha)*cos(phi);

  G4double x = 0;
  G4double y = 0;
  //x=20*(1-2*G4UniformRand())*cm;
  //y=20*(1-2*G4UniformRand())*cm;
  particleGun->SetParticleMomentumDirection(G4ThreeVector(x0,y0,z0)); 
  particleGun->SetParticlePosition(G4ThreeVector(x,y,10*cm));
#endif
  particleGun->GeneratePrimaryVertex(anEvent); 
  
} 


