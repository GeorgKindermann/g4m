//./2getUMaiBm /tmp/datG4mIn.txt /data/tmp/datG4m4Fulvio2018.txt

#include <iostream>
#include <iomanip>
#include <valarray>
#include <cstdlib>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include <iterator>

#include "increment.h"

using namespace std;

int main(int argc, char **argv) {
  g4m::incrementCurvesHf rtShort  //Short rotation time
    (150, 6.55, 0., -1.2, -0.35,
     0., 0.9, -0.8242, -0.4273, -0.4, -1.476, 4.283, -0.3, 3.610, -1.071,
     0.1, 1., -2.0670, -0.3028, 0.5, 1.5, 150, 0.01, 0.5, 0.5,
     0.8, 1./500., 2., 0.01, 0.5, 22.09082, 0.62065,-0.025, 1.50614, -0.25346,
     22.70, 16.56, -0.012, 0.24754, -1.81373, 1.0945, 0.0999, -1.6033,
     1.6, 0.95, 1.5);

  g4m::incrementCurvesHf rtNormal  //Average rotation times
    (150, 8.0, 0., -1.4, -0.3,
     0., 0.9, -0.8242, -0.4273, -0.4, -1.476, 4.283, -0.3, 3.610, -1.071,
     0.1, 1., -2.0670, -0.3028, 0.5, 1.5, 150, 0.01, 0.5, 0.5,
     0.8, 1./500., 2., 0.01, 0.5, 22.09082, 0.62065,-0.02, 1.50614, -0.25346,
     22.70, 16.56, -0.01, 0.24754, -1.81373, 1.0945, 0.0999, -1.6033,
     1.6, 0.95, 1.5);

  g4m::incrementCurvesHf rtLong  //Long rotation times
    (150, 9.0, 0., -1.5, -0.2,
     0., 0.9, -0.8242, -0.4273, -0.4, -1.476, 4.283, -0.3, 3.610, -1.071,
     0.1, 1., -2.0670, -0.3028, 0.5, 1.5, 150, 0.01, 0.5, 0.5,
     0.8, 1./500., 2., 0.01, 0.5, 22.09082, 0.62065,-0.015, 1.50614, -0.25346,
     22.70, 16.56, -0.008, 0.24754, -1.81373, 1.0945, 0.0999, -1.6033,
     1.6, 0.95, 1.5);

  g4m::incrementTab iTabShort(rtShort,30.,0.25,600,1,0.25,1.5,0.25,10.);
  g4m::incrementTab iTabNormal(rtNormal,30.,0.25,600,1,0.25,1.5,0.25,10.);
  g4m::incrementTab iTabLong(rtLong,30.,0.25,600,1,0.25,1.5,0.25,10.);
  g4m::incrementTab* iiTab[3];
  iiTab[0] = &iTabShort;
  iiTab[1] = &iTabNormal;
  iiTab[2] = &iTabLong;

  ifstream fpi;
  string line;
  fpi.open("iso.txt", ios::in);
  getline(fpi, line);
  map<int, string> iso;
  while(getline(fpi, line)) {
    istringstream iss(line);
    vector<string> tokens{istream_iterator<string>{iss}, istream_iterator<string>{}};
    iso[atoi(tokens[0].c_str())] = tokens[1];
  }
  fpi.close();

  ofstream fpo;
  fpo.open(argv[2]);
  vector<double> mul = {0.75, 1., 1.25};
  fpo << "#x y iso forestArea[ha]";
  for(int scen=0; scen<2; ++scen) { //0..maxInc, 1..maxBm
    for(int tab=0; tab<3; ++tab) { //0..short, 1..normal, 2..long
      for(auto &mult : mul) {
	fpo << " u" << scen << "_" << tab << "_" << mult;
	fpo << " bm" << scen << "_" << tab << "_" << mult;
	fpo << " mai" << scen << "_" << tab << "_" << mult;
      }
    }
  }
  fpo << endl;
  fpo << "#u..rotation time[years], bm..average stocking biomass[tC/ha], mai..mean annual incremant[tC/ha/year]" << endl;
  fpo << "#?a_b_c: a..Management regime a=0..maxInc, a=1..maxBm; b..Species class b=0..short, b=1..average, b=2..long rotation time; c..optimal rotation time multiplier" << endl;
  fpi.open(argv[1], ios::in);
  while(getline(fpi, line)) {
    istringstream iss(line);
    double x,y,mmai,forestarea;
    int country;
    iss >> x >> y >> country >> mmai >> forestarea;
    fpo << x << " " << y << " " << iso[country] << " " << forestarea;
    for(int scen=0; scen<2; ++scen) { //0..maxInc, 1..maxBm
      for(int tab=0; tab<3; ++tab) { //0..short, 1..normal, 2..long
       	double uoptt = iiTab[tab]->gTopt(mmai, scen);
	for(auto &mult : mul) {
	  double u = uoptt*mult;
	  fpo << " " << u;
	  fpo << " " << round(iiTab[tab]->gAvgBmt(u, mmai)*100.)/100.;
	  fpo << " " << round(iiTab[tab]->gMait(u, mmai)*100.)/100.;
	}
      }
    }
    fpo << endl;
  }  
  fpo.close();
  fpi.close();
  
  return(0);
}
