//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                   *
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
// $Id: FEE.hh,v 1.1.1.1 2013/02/23 13:30:13 rui Exp $
// GEANT4 tag $Name:  $
//

#ifndef FEE_H
#define FEE_H 1
#include "globals.hh"
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
#include "G4VUserDetectorConstruction.hh"
#include "G4Box.hh"

class FEE
{
  public:

    FEE();
    ~FEE();
    G4LogicalVolume* Construct();
    G4LogicalVolume* VA32Construct();
    G4LogicalVolume* FPGAConstruct();
    G4LogicalVolume* ADConstruct();
    G4LogicalVolume* OPConstruct();
  private:
    // solid volumes
    //
    G4Box* VA32_box;
    G4Box* FPGA_box;
    G4Box* AD_box;
    G4Box* FR4_box;
    G4Box* Cu_box;
	G4Box* OP_box;
    G4Box* sub_box;
    G4Box* uni_box;
    // Logical volumes
    //
    G4LogicalVolume* VA32_log;
    G4LogicalVolume* FPGA_log;
    G4LogicalVolume* AD_log;
    G4LogicalVolume* FR4_log;
    G4LogicalVolume* Cu_log;
    G4LogicalVolume* OP_log;
    G4LogicalVolume* sub_log;
    G4LogicalVolume* uni_log;
    // Physical volumes
    //
    G4VPhysicalVolume* FR4_phys;
    G4VPhysicalVolume* Cu_phys;
    G4VPhysicalVolume* sub_phys;

//VA32 parameters
  G4double VA32_length;
  G4double VA32_width;
  G4double VA32_thick;

//FPGA parameters
  G4double FPGA_length;
  G4double FPGA_width;
  G4double FPGA_thick;

//AD parameters
  G4double AD_length;
  G4double AD_width;
  G4double AD_thick;

//OP parameters
  G4double OP_length;
  G4double OP_width;
  G4double OP_thick;

//FR4 parameters
  G4double FR4_length;
  G4double FR4_width;
  G4double FR4_thick;

//Copper parameters
  G4double Cu_length;
  G4double Cu_width;
  G4double Cu_thick;

//substrate parameters
  G4double sub_length;
  G4double sub_width;
  G4double sub_thick;

//virtual unit parameters
  G4double uni_length;
  G4double uni_width;
  G4double uni_thick;
};

#endif

