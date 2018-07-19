//time ./2g4gm /data/tmp/g4mInData.txt >/data/tmp/g4mOutBm.txt

const int firstYear = 2010;
const int decades = 10;

#include <iostream>
#include <fstream>
#include <sstream>
#include <valarray>
#include <vector>
#include <cmath>
#include <map>
#include <algorithm>

#include "/home/georg/prg/g4m/increment/ageStruct.h"
#include "/home/georg/prg/g4m/increment/mai.h"
#include "/home/georg/prg/g4m/increment/misc.h"

using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct siteData {
  string lat;
  string lon;
  float maiMax;
  float maiMaxIrri;
};

int main(int argc, char **argv) {

  //Country number to ISO code
  map<int, string> countryCode;
  {
    ifstream fi;
    fi.open("/home/georg/13/g4mData/country/countrynames.csv");
    std::string line;
    getline(fi,line);
    while(getline(fi,line)) {
      stringstream ss(line);
      string item;
      std::vector<string> elem;
      while (getline(ss, item, ',')) {
	elem.push_back(item);
      }
      countryCode[atoi(elem[0].c_str())] = elem[1];
    }
    fi.close();
  }

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

  g4m::incrementTab iTabShort(rtShort,15.,0.25,600,1,0.25,1.5,0.25,10.);
  g4m::incrementTab iTabNormal(rtNormal,15.,0.25,600,1,0.25,1.5,0.25,10.);
  g4m::incrementTab iTabLong(rtLong,15.,0.25,600,1,0.25,1.5,0.25,10.);
  g4m::incrementTab* iTab[3];
  iTab[0] = &iTabShort;
  iTab[1] = &iTabNormal;
  iTab[2] = &iTabLong;

  cout << "lat lon stockShort[tC/ha] stockNormal[tC/ha] stockLong[tC/ha] stockShortIrri[tC/ha] stockNormalIrri[tC/ha] stockLongIrri[tC/ha]" << endl;

  siteData site;
  ifstream IN(argv[1]);
  do {
    IN >> site.lat >> site.lon >> site.maiMax >> site.maiMaxIrri;
    cout << site.lat << " " << site.lon;
    for(int i=0; i<3; ++i) {
      if(site.maiMax > 0.) {
	cout << " " << iTab[i]->gAvgBmt(iTab[i]->gTopt(site.maiMax, 0), site.maiMax); } else {cout << " 0";}
    }
    for(int i=0; i<3; ++i) {
      if(site.maiMaxIrri > 0.) {
	cout << " " << iTab[i]->gAvgBmt(iTab[i]->gTopt(site.maiMaxIrri, 0), site.maiMaxIrri); } else {cout << " 0";}
    }
    cout << endl;
  } while(IN.good());
  return(0);
}

