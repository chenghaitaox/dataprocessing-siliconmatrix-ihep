#ifndef DataBase_h
#define DataBase_h 1
#include "globals.hh"
#include "G4UnitsTable.hh"

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TTree.h"
#include "viewer.hh"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;
#ifndef Det_TEST
extern TH2F*map_online;
extern G4int num_unit;
extern G4int num_ladder;

class DataBase
{
public:
	DataBase(int);
	~DataBase();

	void FillTrueth(double,int);
	void FillData(int,int);
	void FillOnline(int); 
	void Fill2DOnline(int,int,int); 
	void SaveData(int);
	void SaveTrueth();
	void ResetOnline();
private: 
	int det_num;
	TTree*t;
	TTree*d;
	char Hname[100];
	TFile* hfile;
	viewer*iview;
	double *energy;
	int *D_energy;
	int Evn_number;
	ofstream bin_file;
};
#endif
#endif /*DataBase_h*/

