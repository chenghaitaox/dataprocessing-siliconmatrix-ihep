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
// $Id: Cell.hh,v 1.1.1.1 2013/02/23 13:30:13 rui Exp $
// GEANT4 tag $Name:  $
//

#ifndef Cell_H
#define Cell_H 1
#include "globals.hh"
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
#include "G4VUserDetectorConstruction.hh"
#include "G4Box.hh"

class Cell
{
  public:

    Cell();
    ~Cell();
    G4LogicalVolume* Construct();
    G4double GetUnitX(){return si_length;}
    G4double GetUnitY(){return si_width;}
    G4double GetUnitZ(){return uni_thick;}
	void SetSD(G4int sd_num){cell_num=sd_num;}

  private:
	//SD number
	G4int cell_num;
    // solid volumes
    //
    G4Box* si_box;
    G4Box* pre_box;
    G4Box* cer_box;
    G4Box* glu_box;
    G4Box* alu_box;
    G4Box* sub_box;
    G4Box* uni_box;
    // Logical volumes
    //
    G4LogicalVolume* si_log;
    G4LogicalVolume* pre_log;
    G4LogicalVolume* cer_log;
    G4LogicalVolume* glu_log;
    G4LogicalVolume* alu_log;
    G4LogicalVolume* sub_log;
    G4LogicalVolume* uni_log;
    // Physical volumes
    //
    G4VPhysicalVolume* si_phys;
    G4VPhysicalVolume* pre_phys;
    G4VPhysicalVolume* cer_phys;
    G4VPhysicalVolume* glu_phys;
    G4VPhysicalVolume* alu_phys;
    G4VPhysicalVolume* sub_phys;

//silicon parameters
  G4double si_length;
  G4double si_width;
  G4double si_thick;

//preAmplifier parameters
  G4double pre_length;
 G4double pre_width;
  G4double pre_thick;

//Ceramics parameters
  G4double cer_length;
  G4double cer_width;
  G4double cer_thick;

//Glue parameters
  G4double glu_length;
  G4double glu_width;
  G4double glu_thick;

//Aluminum parameters
  G4double alu_length;
  G4double alu_width;
  G4double alu_thick;

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

