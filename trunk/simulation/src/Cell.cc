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
// $Id: Cell.cc,v 1.1.1.1 2013/02/23 13:30:13 rui Exp $
// GEANT4 tag $Name:  $
//
#include "DetectorConstruction.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4Box.hh"
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
#include "Cell.hh"
using namespace std;
//#define PRE 1
Cell::Cell()
 :  si_log(0), cer_log(0), sub_log(0), uni_log(0),
    si_phys(0),cer_phys(0), sub_phys(0),cell_num(1)
{ 
//silicon parameters
  si_length=2*cm;
  si_width=2.5*cm;
  si_thick=0.5*mm;

//Ceramics parameters
  cer_length=2.3*cm;
  cer_width=2.8*cm;
  cer_thick=0.8*mm;

//Glue parameters
  glu_length=2.5*cm;
  glu_width=3.2*cm;
  glu_thick=2*mm;

//Aluminum parameters
  alu_length=2.7*cm;
  alu_width=4.1*cm;
  alu_thick=3*mm;

#ifdef PRE
//preAmplifier parameters
  pre_length=0.8*cm;
  pre_width=3*cm;
  pre_thick=2*mm;

//substrate parameters
  sub_length=alu_length+pre_length;
  sub_width=alu_width;
  sub_thick=2*mm;
#else
//substrate parameters
  sub_length=alu_length;
  sub_width=alu_width;
  sub_thick=2*mm;
#endif

//virtual unit parameters
  uni_length=sub_length;
  uni_width=sub_width;
  uni_thick=sub_thick+alu_thick;
}

Cell::~Cell()
{ 
}

G4LogicalVolume* Cell::Construct()
{ 
//materials
  G4double a;  // atomic mass
  G4double z;  // atomic number
  G4double density;
  G4Material* Vacuum ;
  Vacuum=new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
                           kStateGas, 3.e-18*pascal, 2.73*kelvin);
  G4NistManager* man = G4NistManager::Instance();
  man->SetVerbose(0);
  G4Material* Si= man->FindOrBuildMaterial("G4_Si");
  G4Material* Al= man->FindOrBuildMaterial("G4_Al");
  G4Material* Cer=man->FindOrBuildMaterial("G4_ALUMINUM_OXIDE");
//construction FR4
  z      = 6.;
  a      = 12.011*g/mole;
  G4Element* elC = new G4Element("Element_Carbon",  "C", z, a);

  z      = 1.;
  a      = 1.00794*g/mole;
  G4Element* elH = new G4Element("Element_Hydrogen", "H", z, a);

  z      = 8.;
  a      = 16.00*g/mole;
  G4Element* elO = new G4Element( "Element_Oxygen", "O", z, a);

  z      = 14.;
  a      = 28.09*g/mole;
  G4Element* elSi  = new G4Element("Element_Silicon", "Si", z, a);

  density     = 1.70*g/cm3;
  G4int ncomponents = 4;
  G4int natoms;
  G4double fractionmass;
  G4Material* FR4 = new G4Material("FR4",density , ncomponents);
	      FR4 -> AddElement(elSi, natoms=1);
	      FR4 -> AddElement(elO , natoms=2);
	      FR4 -> AddElement(elC , natoms=3);
	      FR4 -> AddElement(elH , natoms=3);
//construct kovar
  G4Material *Ni =man->FindOrBuildMaterial("G4_Ni");
  G4Material *Co =man->FindOrBuildMaterial("G4_Co");
  G4Material *Fe =man->FindOrBuildMaterial("G4_Fe");
  G4Material *Kovar =
    new G4Material ("Kovar", 8.8 * g / cm3, ncomponents = 3);
  Kovar->AddMaterial (Ni, fractionmass = 0.29);
  Kovar->AddMaterial (Co, fractionmass = 0.18);
  Kovar->AddMaterial (Fe, fractionmass = 0.53);
//detector construction
  uni_box=new G4Box("uni_box",uni_length/2,uni_width/2,uni_thick/2);
  uni_log=new G4LogicalVolume(uni_box,Vacuum,"uni_log");
 
  alu_box=new G4Box("alu_box",alu_length/2,alu_width/2,alu_thick/2);
  alu_log=new G4LogicalVolume(alu_box,Kovar,"alu_log");
  alu_phys=new G4PVPlacement(0,G4ThreeVector(-pre_length/2,0,uni_thick/2-alu_thick/2),
                                      alu_log,"alu",uni_log,false,0);

  glu_box=new G4Box("glu_box",glu_length/2,glu_width/2,glu_thick/2);
  glu_log=new G4LogicalVolume(glu_box,Si,"glu_log");
  glu_phys=new G4PVPlacement(0,G4ThreeVector(0,0,alu_thick/2-glu_thick/2),
                                      glu_log,"glu",alu_log,false,0);

  cer_box=new G4Box("cer_box",cer_length/2,cer_width/2,cer_thick/2);
  cer_log=new G4LogicalVolume(cer_box,Cer,"cer_log");
  cer_phys=new G4PVPlacement(0,G4ThreeVector(0,1.5*mm,cer_thick/2-glu_thick/2),cer_log,"cer",glu_log,false,0);

  si_box=new G4Box("si_box",si_length/2,si_width/2,si_thick/2);
  si_log=new G4LogicalVolume(si_box,Si,"si_log");
  si_phys=new G4PVPlacement(0,G4ThreeVector(0,1.5*mm,si_thick/2+cer_thick-glu_thick/2),si_log,"si",glu_log,false,0);

#ifdef PRE
  pre_box=new G4Box("pre_box",pre_length/2,pre_width/2,pre_thick/2);
  pre_log=new G4LogicalVolume(pre_box,Si,"pre_log");
  pre_phys=new G4PVPlacement(0,G4ThreeVector(alu_length/2,0,pre_thick/2+sub_thick-uni_thick/2),
                                      pre_log,"pre",uni_log,false,0);
#endif

  sub_box=new G4Box("sub_box",sub_length/2,sub_width/2,sub_thick/2);
  sub_log=new G4LogicalVolume(sub_box,FR4,"sub_log");
  sub_phys=new G4PVPlacement(0,G4ThreeVector(0,0,sub_thick/2-uni_thick/2),
                                      sub_log,"sub",uni_log,false,0);

//set colour
  G4VisAttributes* VisAtt1= new G4VisAttributes(G4Colour(0.5,0.5,0.5));
  G4VisAttributes* VisAtt2= new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  G4VisAttributes* VisAtt3= new G4VisAttributes(G4Colour(0.0,1.0,0.0));
  G4VisAttributes* VisAtt4= new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  G4VisAttributes* VisAtt5= new G4VisAttributes(G4Colour(0.0,1.0,0.5));
  alu_log->SetVisAttributes(VisAtt1);
#ifdef PRE
  pre_log->SetVisAttributes(VisAtt2);
#endif
  sub_log->SetVisAttributes(VisAtt3);
  si_log->SetVisAttributes(VisAtt4);
  cer_log->SetVisAttributes(VisAtt5);

//set SD
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4String Dname = "/SiPIN";
  SiSD * detSD = new SiSD(Dname,cell_num);
  SDman->AddNewDetector(detSD);
  si_log->SetSensitiveDetector(detSD);

  uni_log->SetVisAttributes (G4VisAttributes::Invisible);
  glu_log->SetVisAttributes (G4VisAttributes::Invisible);

  return uni_log;
}
