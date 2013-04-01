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
// $Id: DetectorConstruction.cc,v 1.1.1.1 2013/02/23 13:30:13 rui Exp $
// GEANT4 tag $Name:  $
//
#include "DetectorConstruction.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4EqMagElectricField.hh"
#include "G4UniformElectricField.hh"
#include "G4PVPlacement.hh"
#include "G4SubtractionSolid.hh"
#include "G4RotationMatrix.hh"
#include "globals.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4FieldManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "SiSD.hh"
#include "G4NistManager.hh"
#include "TH2F.h"
#include "model.hh"
#ifndef Det_TEST
G4int num_unit=18;
G4int num_ladder=10;
TH1F *total=new TH1F("energy","energy deposited in silicon",8192,0,80000);
TH2F*map_online=new TH2F("the pad viewer","viewer",num_unit,0.5,num_unit+0.5,num_ladder,0.5,num_ladder+0.5);

#endif
using namespace std;

DetectorConstruction::DetectorConstruction()
 :  experimentalHall_log(0), detector_log(0),
    experimentalHall_phys(0),detector_phys(0),
    FEE_log(0)
{ 
acell=new Cell();
aFEE=new FEE();
}

DetectorConstruction::~DetectorConstruction()
{ 
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{ 
printf("Initial Detector construction......");
  G4double a;  // atomic mass
  G4double z;  // atomic number
  G4double density;
  G4Material* Vacuum ;
  Vacuum=new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
                           kStateGas, 3.e-18*pascal, 2.73*kelvin);
  G4NistManager* man = G4NistManager::Instance();
  man->SetVerbose(0);

  //------------------------------ experimental hall (world volume)
  //------------------------------ beam line along z axis
 
  experimentalHall_box
    = new G4Box("expHall_box",1*m,1*m,1*m);
  experimentalHall_log = new G4LogicalVolume(experimentalHall_box,Vacuum
                                             ,"expHall_log",0,0,0);
  experimentalHall_phys = new G4PVPlacement(0,G4ThreeVector(),
                                      experimentalHall_log,"expHall",0,false,0);
//detector construction
#ifdef Det_TEST
//detector test
	detector_log=acell->Construct();
	detector_phys=new G4PVPlacement(0,G4ThreeVector(0,0,0),
                                      detector_log,"det",experimentalHall_log,false,0);
#endif
#ifndef Det_TEST
//place real position
	G4double overlap_unit=2.2*mm;
	G4double overlap_ladder=4.5*mm;
	acell->SetSD(num_unit*num_ladder);
	detector_log=acell->Construct();
	G4double UnitX=acell->GetUnitX();
	G4double UnitY=acell->GetUnitY();
	G4double UnitZ=acell->GetUnitZ();
	G4double p_x=0;
	G4double p_y=0;
	G4double p_z=0;
	char name[100];
	for(int i=1;i<(num_ladder+1);i++)
		{
			for(int j=1;j<(num_unit+1);j++)
				{
					sprintf(name,"det_%d_%d",j,i);
					G4RotationMatrix* rm_x = new G4RotationMatrix();
					if(j%2)
					{
					rm_x->rotateX(0*deg);
					if(i%2)
						{
						p_z=UnitZ/2;
						}
					else
						{
						p_z=-3*UnitZ/2;
						}
					}
					else
					{
					rm_x->rotateX(180*deg);
					rm_x->rotateZ(180*deg);
					if(i%2)
						{
						p_z=3*UnitZ/2;
						}
					else
						{
						p_z=-UnitZ/2;
						}
					}
					p_x=(UnitX-overlap_unit)*(j-num_unit/2+1/2);
					p_y=(UnitY-overlap_ladder)*(i-num_ladder/2+1/2);
					new G4PVPlacement(rm_x,G4ThreeVector(p_x,p_y,p_z),detector_log,name,experimentalHall_log,false,(i-1)*num_unit+j);
				}
		}
	#ifdef FEE_construction
	//FEE construction
	G4double FEE_z=2*UnitZ+3*mm;
	FEE_log=aFEE->Construct();
	new G4PVPlacement(0,G4ThreeVector(0,0,FEE_z),FEE_log,"FEE",experimentalHall_log,false,0);
	G4RotationMatrix* rm_FEE = new G4RotationMatrix();
	rm_FEE->rotateY(180*deg);
	new G4PVPlacement(rm_FEE,G4ThreeVector(0,0,-FEE_z),FEE_log,"FEE",experimentalHall_log,false,0);
	#endif
#endif
experimentalHall_log->SetVisAttributes (G4VisAttributes::Invisible);
  return experimentalHall_phys;
printf("Initial complete!\n");
}


