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
// $Id: FEE.cc,v 1.1.1.1 2013/02/23 13:30:13 rui Exp $
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
#include "G4NistManager.hh"
#include "FEE.hh"
using namespace std;
FEE::FEE()
 :  VA32_log(0), FPGA_log(0), sub_log(0), uni_log(0)
{ 
//VA32 parameters
  VA32_length=2.1*cm;
  VA32_width=2.1*cm;
  VA32_thick=2*mm;

//FPGA parameters
  FPGA_length=2.79*cm;
  FPGA_width=2.79*cm;
  FPGA_thick=2*mm;

//AD parameters
  AD_length=1.5*cm;
  AD_width=1.5*cm;
  AD_thick=2*mm;

//OP parameters
  OP_length=2*cm;
  OP_width=1*cm;
  OP_thick=2*mm;

//FR4 parameters
  FR4_length=80*cm;
  FR4_width=80*cm;
  FR4_thick=1.5*mm;

//Cu parameters
  Cu_length=FR4_length;
  Cu_width=FR4_width;
  Cu_thick=0.5*mm;

//substrate parameters
  sub_length=FR4_length;
  sub_width=FR4_width;
  sub_thick=FR4_thick+Cu_thick;

//virtual unit parameters
  uni_length=sub_length;
  uni_width=sub_width;
  uni_thick=sub_thick+4*mm;
}

FEE::~FEE()
{ 
}

G4LogicalVolume* FEE::Construct()
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
  G4Material* Cu= man->FindOrBuildMaterial("G4_Cu");
//construction FR4
  z      = 6.;
  a      = 12.011*g/mole;
  G4Element* C = new G4Element("Element_Carbon",  "C", z, a);

  z      = 1.;
  a      = 1.00794*g/mole;
  G4Element* H = new G4Element("Element_Hydrogen", "H", z, a);

  z      = 8.;
  a      = 16.00*g/mole;
  G4Element* O = new G4Element( "Element_Oxygen", "O", z, a);

  z      = 14.;
  a      = 28.09*g/mole;
  G4Element* Si  = new G4Element("Element_Silicon", "Si", z, a);

  density     = 1.70*g/cm3;
  G4int ncomponents = 4;
  G4int natoms;
  G4Material* FR4 = new G4Material("FR4",density , ncomponents);
	      FR4 -> AddElement(Si, natoms=1);
	      FR4 -> AddElement(O , natoms=2);
	      FR4 -> AddElement(C , natoms=3);
	      FR4 -> AddElement(H , natoms=3);
//construction
  uni_box=new G4Box("uni_box",uni_length/2,uni_width/2,uni_thick/2);
  uni_log=new G4LogicalVolume(uni_box,Vacuum,"uni_log");

  sub_box=new G4Box("sub_box",sub_length/2,sub_width/2,sub_thick/2);
  sub_log=new G4LogicalVolume(sub_box,Vacuum,"sub_log");
  sub_phys=new G4PVPlacement(0,G4ThreeVector(0,0,sub_thick/2-uni_thick/2),
                                      sub_log,"sub",uni_log,false,0);

  FR4_box=new G4Box("FR4_box",FR4_length/2,FR4_width/2,FR4_thick/2);
  FR4_log=new G4LogicalVolume(FR4_box,FR4,"FR4_log");
  FR4_phys=new G4PVPlacement(0,G4ThreeVector(0,0,FR4_thick/2-sub_thick/2),
                                      FR4_log,"FR4",sub_log,false,0);
  
  Cu_box=new G4Box("Cu_box",Cu_length/2,Cu_width/2,Cu_thick/2);
  Cu_log=new G4LogicalVolume(Cu_box,Cu,"Cu_log");
  Cu_phys=new G4PVPlacement(0,G4ThreeVector(0,0,-Cu_thick/2),
                                      Cu_log,"Cu",sub_log,false,0);

  VA32_log=VA32Construct();
for(int i=0;i<6;i++) 
{
  for(int j=0;j<5;j++)new G4PVPlacement(0,G4ThreeVector(-30*cm+j*15*cm,-25*cm+i*10*cm,VA32_thick/2),
                                      VA32_log,"VA32",uni_log,false,i*6+j);
}

  FPGA_log=FPGAConstruct();
  for(int j=0;j<3;j++)new G4PVPlacement(0,G4ThreeVector(0,-20*cm+j*20*cm,FPGA_thick/2),
                                      FPGA_log,"FPGA",uni_log,false,j);


  AD_log=ADConstruct();
for(int i=0;i<2;i++) 
{
  for(int j=0;j<3;j++)new G4PVPlacement(0,G4ThreeVector(-20*cm+i*40*cm,-20*cm+j*20*cm,AD_thick/2),
                                      AD_log,"AD",uni_log,false,i*2+j);
}

  OP_log=OPConstruct();
for(int i=0;i<9;i++) 
{
  for(int j=0;j<5;j++)
				{
				G4double OP_y=-1;
				if(i%2)OP_y=1;
				new G4PVPlacement(0,G4ThreeVector(-32*cm+i*8*cm,-20*cm+j*10*cm+OP_y*2.5*cm,AD_thick/2),AD_log,"AD",uni_log,false,i*9+j);
				}
}
//set colour
  G4VisAttributes* VisAtt1= new G4VisAttributes(G4Colour(0.5,0.5,0.5));
  G4VisAttributes* VisAtt2= new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  G4VisAttributes* VisAtt3= new G4VisAttributes(G4Colour(0.0,1.0,0.0));
  Cu_log->SetVisAttributes(VisAtt1);
  VA32_log->SetVisAttributes(VisAtt2);
  FPGA_log->SetVisAttributes(VisAtt2);
  AD_log->SetVisAttributes(VisAtt2);
  FR4_log->SetVisAttributes(VisAtt3);
  uni_log->SetVisAttributes (G4VisAttributes::Invisible);
  sub_log->SetVisAttributes (G4VisAttributes::Invisible);

  return uni_log;
}

G4LogicalVolume* FEE::VA32Construct()
{
  G4double a;  // atomic mass
  G4double z;  // atomic number
  G4double density;
  G4Material* Vacuum ;
  Vacuum=new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
                           kStateGas, 3.e-18*pascal, 2.73*kelvin);
  G4NistManager* man = G4NistManager::Instance();
  man->SetVerbose(0);
  G4Material* Si= man->FindOrBuildMaterial("G4_Si");
  G4Material* Cer=man->FindOrBuildMaterial("G4_ALUMINUM_OXIDE");
  
//construction
//
  G4double VA32_Si_thick=0.3*mm;
  G4Box* VA32_box=new G4Box("VA32_box",VA32_length/2,VA32_width/2,VA32_thick/2);
  G4LogicalVolume *VA32=new G4LogicalVolume(VA32_box,Vacuum,"VA32");

  G4Box* VA32_Cer_box=new G4Box("VA32_Cer_box",VA32_length/2,VA32_width/2,VA32_thick/2-VA32_Si_thick/2);
  G4LogicalVolume *VA32_Cer=new G4LogicalVolume(VA32_Cer_box,Cer,"VA32_Cer");
  new G4PVPlacement(0,G4ThreeVector(0,0,VA32_Si_thick/2),VA32_Cer,"VA32_Cer",VA32,false,0);

  G4Box* VA32_Si_box=new G4Box("VA32_box",VA32_length/2,VA32_width/2,VA32_Si_thick/2);
  G4LogicalVolume *VA32_Si=new G4LogicalVolume(VA32_Si_box,Si,"VA32_Si");
  new G4PVPlacement(0,G4ThreeVector(0,0,VA32_Si_thick/2-VA32_thick/2),VA32_Cer,"VA32_Cer",VA32,false,0);

  return VA32;
}
G4LogicalVolume* FEE::FPGAConstruct()
{
  G4double a;  // atomic mass
  G4double z;  // atomic number
  G4double density;
  G4Material* Vacuum ;
  Vacuum=new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
                           kStateGas, 3.e-18*pascal, 2.73*kelvin);
  G4NistManager* man = G4NistManager::Instance();
  man->SetVerbose(0);
  G4Material* Si= man->FindOrBuildMaterial("G4_Si");
  G4Material* Cer=man->FindOrBuildMaterial("G4_ALUMINUM_OXIDE");
//construction
  G4double FPGA_Si_thick=0.3*mm;
  G4Box* FPGA_box=new G4Box("FPGA_box",FPGA_length/2,FPGA_width/2,FPGA_thick/2);
  G4LogicalVolume *FPGA=new G4LogicalVolume(FPGA_box,Vacuum,"FPGA");
  
  G4Box* FPGA_Cer_box=new G4Box("FPGA_Cer_box",FPGA_length/2,FPGA_width/2,FPGA_thick/2-FPGA_Si_thick/2);
  G4LogicalVolume *FPGA_Cer=new G4LogicalVolume(FPGA_Cer_box,Cer,"FPGA_Cer");
  new G4PVPlacement(0,G4ThreeVector(0,0,FPGA_Si_thick/2),FPGA_Cer,"FPGA_Cer",FPGA,false,0);

  G4Box* FPGA_Si_box=new G4Box("FPGA_box",FPGA_length/2,FPGA_width/2,FPGA_Si_thick/2);
  G4LogicalVolume *FPGA_Si=new G4LogicalVolume(FPGA_Si_box,Si,"FPGA_Si");
  new G4PVPlacement(0,G4ThreeVector(0,0,FPGA_Si_thick/2-FPGA_thick/2),FPGA_Cer,"FPGA_Cer",FPGA,false,0);
return FPGA;
}
G4LogicalVolume* FEE::ADConstruct()
{
  G4double a;  // atomic mass
  G4double z;  // atomic number
  G4double density;
  G4Material* Vacuum ;
  Vacuum=new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
                           kStateGas, 3.e-18*pascal, 2.73*kelvin);
  G4NistManager* man = G4NistManager::Instance();
  man->SetVerbose(0);
  G4Material* Si= man->FindOrBuildMaterial("G4_Si");
  G4Material* Cer=man->FindOrBuildMaterial("G4_ALUMINUM_OXIDE");
//construction
  G4double AD_Si_thick=0.3*mm;
  G4Box* AD_box=new G4Box("AD_box",AD_length/2,AD_width/2,AD_thick/2);
  G4LogicalVolume *AD=new G4LogicalVolume(AD_box,Vacuum,"AD");
 
  G4Box* AD_Cer_box=new G4Box("AD_Cer_box",AD_length/2,AD_width/2,AD_thick/2-AD_Si_thick/2);
  G4LogicalVolume *AD_Cer=new G4LogicalVolume(AD_Cer_box,Cer,"AD_Cer");
  new G4PVPlacement(0,G4ThreeVector(0,0,AD_Si_thick/2),AD_Cer,"AD_Cer",AD,false,0);

  G4Box* AD_Si_box=new G4Box("AD_box",AD_length/2,AD_width/2,AD_Si_thick/2);
  G4LogicalVolume *AD_Si=new G4LogicalVolume(AD_Si_box,Si,"AD_Si");
  new G4PVPlacement(0,G4ThreeVector(0,0,AD_Si_thick/2-AD_thick/2),AD_Cer,"AD_Cer",AD,false,0);
return AD;
}

G4LogicalVolume* FEE::OPConstruct()
{
  G4double a;  // atomic mass
  G4double z;  // atomic number
  G4double density;
  G4Material* Vacuum ;
  Vacuum=new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
                           kStateGas, 3.e-18*pascal, 2.73*kelvin);
  G4NistManager* man = G4NistManager::Instance();
  man->SetVerbose(0);
  G4Material* Si= man->FindOrBuildMaterial("G4_Si");
  G4Material* Cer=man->FindOrBuildMaterial("G4_ALUMINUM_OXIDE");
//construction
  G4double OP_Si_thick=0.3*mm;
  G4Box* OP_box=new G4Box("OP_box",OP_length/2,OP_width/2,OP_thick/2);
  G4LogicalVolume *OP=new G4LogicalVolume(OP_box,Vacuum,"OP");
 
  G4Box* OP_Cer_box=new G4Box("OP_Cer_box",OP_length/2,OP_width/2,OP_thick/2-OP_Si_thick/2);
  G4LogicalVolume *OP_Cer=new G4LogicalVolume(OP_Cer_box,Cer,"OP_Cer");
  new G4PVPlacement(0,G4ThreeVector(0,0,OP_Si_thick/2),OP_Cer,"OP_Cer",OP,false,0);

  G4Box* OP_Si_box=new G4Box("OP_box",OP_length/2,OP_width/2,OP_Si_thick/2);
  G4LogicalVolume *OP_Si=new G4LogicalVolume(OP_Si_box,Si,"OP_Si");
  new G4PVPlacement(0,G4ThreeVector(0,0,OP_Si_thick/2-OP_thick/2),OP_Cer,"OP_Cer",OP,false,0);
return OP;
}
