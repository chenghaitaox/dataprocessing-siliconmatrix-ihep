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
// $Id: DetectorConstruction.hh,v 1.1.1.1 2013/02/23 13:30:13 rui Exp $
// GEANT4 tag $Name:  $
//

#ifndef DetectorConstruction_H
#define DetectorConstruction_H 1
#include "globals.hh"
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
#include "G4VUserDetectorConstruction.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "Cell.hh"
#include "FEE.hh"

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:

    DetectorConstruction();
    ~DetectorConstruction();

    G4VPhysicalVolume* Construct();
 
  private:
 
    // solid volumes
    //
    G4Box* experimentalHall_box;

    // Logical volumes
    //
    G4LogicalVolume* experimentalHall_log;
    G4LogicalVolume* detector_log;
    G4LogicalVolume* FEE_log;
    Cell *acell;
    FEE *aFEE;
    // Physical volumes
    //
    G4VPhysicalVolume* experimentalHall_phys;
    G4VPhysicalVolume* detector_phys;

};

#endif

