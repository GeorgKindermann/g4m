//./3makeCalibMap |sort -g -k 1,1 -k 2,2 >/tmp/res.txt

#include <fstream>
#include <string>
#include <map>
#include <cmath>
#include <vector>
#include <sstream>
#include <iostream>

using namespace std;

struct calcCoef {
  double expo;
  double irri;
  double multi;
};

int main() {
  ifstream fi;
  fi.open("calibFactors.csv");
  //fi.open("calibFactors2000.csv");
  map<int, calcCoef> coef;
  std::string line;
  getline(fi,line);
  while(getline(fi,line)) {
    stringstream ss(line);
    string item;
    std::vector<string> elem;
    while (getline(ss, item, ',')) {
      elem.push_back(item);
    }
    calcCoef tmp;
    int country = atoi(elem[3].c_str());
    tmp.expo = atof(elem[4].c_str());
    tmp.irri = atof(elem[7].c_str());
    tmp.multi = atof(elem[9].c_str());
    coef[country] = tmp;
    //cout << country << " " << coef[country].expo << " " << coef[country].irri << " " << coef[country].multi << endl;
  }
  fi.close();
  double forestCoverG4mBase, forestCoverG4mCut, gridarea, waterLandModis30Sec, irrigation, nppG4m, nppG4mMaxIrri;
  int countryGadmV1, g4mcol, g4mrow;
  fi.open("/tmp/forestShareCountryGridsizeLandshare.txt");
  calcCoef defCoef;
  defCoef.expo = 1.;
  defCoef.irri = 0.;
  defCoef.multi = 1.;
  do {
    fi >> forestCoverG4mBase >> forestCoverG4mCut >> countryGadmV1 >> gridarea >> waterLandModis30Sec >> irrigation >> nppG4m >> nppG4mMaxIrri >> g4mcol >> g4mrow;
    map<int, calcCoef>::iterator it = coef.find(countryGadmV1);
    calcCoef tmp = defCoef;;
    if(it != coef.end()) {tmp = it->second;}
    gridarea /= 10000.;
    double landarea = (1. - waterLandModis30Sec/16.) * gridarea;
    double forShare = forestCoverG4mCut / 100.;
    double area = pow(forShare * gridarea / landarea, tmp.expo) * landarea * tmp.multi;
    double irri = 0.;
    double irriPlus = irrigation + 100. * waterLandModis30Sec / 4.;
    if(irriPlus > 100.) {irriPlus = 100.;}
    if(nppG4m <= 0. && nppG4mMaxIrri>0.) {irri = forestCoverG4mBase/100. * landarea * irriPlus/100. * tmp.irri * tmp.multi;}
    area /= gridarea / 100.;
    irri /= gridarea / 100.;
    int iarea = 0.5 + area;
    int iirri = 0.5 + irri;
    cout << g4mrow << "\t" << g4mcol << "\t" << iarea << "\t" << iirri << endl;
    //cout << g4mrow << "\t" << g4mcol << "\t" << area << "\t" << irri << endl;
  } while (fi.good());

  return(0);
}
