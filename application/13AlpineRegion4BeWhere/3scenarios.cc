#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>

#include "/home/georg/prg/g4m/increment/increment.h"

using namespace std;

int main(int argc, char **argv) {
  g4m::incrementTab itabspruceAssmann (-0.25,-2.2232,-1.0051,0.5819,168.882,80.586,4.478,-1.387,0.753,0,0,0,-0.1951*2.,0,0,0,0,0,1./6.,0,0,0,0.5,-0.266,150,0.000254,2.496e-09,8.205,0.4364,1./500.,2.169,0.01,2.178,24.62079,0.46573,-0.02426,2.58884,-0.30364,700,0,-0.0005094,0.9633471,-0.886505,2,1.2122679,0.1034592,1.6,0.95, 10.,0.1,450.,5.,0.1,1.3,0.1,5.);

  ifstream ifile("/tmp/g4mAlps.csv");
  ofstream ofile("/tmp/g4mAlpsScen.csv");
  string line;
  getline(ifile, line);
  //  ofile << line << ",conif_maxInc_increment,conif_maxInc_stock,conif_maxInc_rotationtime,conif_maxBm_increment,conif_maxBm_stock,conif_maxBm_rotationtime,nonconif_maxInc_increment,nonconif_maxInc_stock,nonconif_maxInc_rotationtime,nonconif_maxBm_increment,nonconif_maxBm_stock,nonconif_maxBm_rotationtime" << endl;
  ofile << line << ",conif_maxInc_increment,conif_maxInc_stock,conif_maxInc_rotationtime,conif_maxBm_increment,conif_maxBm_stock,conif_maxBm_rotationtime,conif_cur_increment,conif_cur_stock,conif_cur_rotationtime,nonconif_maxInc_increment,nonconif_maxInc_stock,nonconif_maxInc_rotationtime,nonconif_maxBm_increment,nonconif_maxBm_stock,nonconif_maxBm_rotationtime,nonconif_cur_increment,nonconif_cur_stock,nonconif_cur_rotationtime" << endl;

  while(getline(ifile, line)) {
    vector<double> token;
    stringstream lineStream(line);
    string cell;
    while(getline(lineStream,cell,',')) {token.push_back(atof(cell.c_str()));}

    ofile << line;

    for(int speciesgroup=0; speciesgroup<2; ++speciesgroup) {
      double mai=0.;
      switch(speciesgroup) {
      case 0: mai = token[7]; //conif
	break;
      case 1: mai = token[8]; //non conif
	break;
	//case 2: double sharesum = token[5] + token[6];
	//if(sharesum > 0.) {mai=(token[7]*token[5] + token[8]*token[6])/sharesum;}
	//break;
      }
      for(int scenario=0; scenario<3; ++scenario) {
	double topt = -1.;
	switch(scenario) {
	case 0: topt = itabspruceAssmann.gToptt(mai, 0)+1; //maxInc
	  break;
	case 1: topt = itabspruceAssmann.gToptt(mai, 1); //maxStockingBm
	  break;
	case 2: topt = itabspruceAssmann.gUt(token[16], mai); //Current rotation time
	}
	double rotationTime=0.;
	double stockingBm=0.;
	double increment=0.;
	if(mai > 0. && topt>0.) {
	  rotationTime = topt ;
	  stockingBm = itabspruceAssmann.gAvgBmt(rotationTime, mai);
	  increment = itabspruceAssmann.gMait(rotationTime, mai);
	}
	ofile << "," << increment << "," << stockingBm << "," << rotationTime;
      }
    }
    ofile << endl;
  }

  return(0);
}
