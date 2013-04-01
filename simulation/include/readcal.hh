#ifndef readcal_hh
#define readcal_hh 1
#include <string>
#include <iostream>
#include <fstream>
using namespace std;
class readcal
{
	public:
		readcal();
		~readcal();
		int Setpars();
		int* Getpars();
	private:
		char filename[100];
		ifstream cal_file;
};
#endif
