#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <map>
#include "/home/georg/prg/g4m/increment/increment.h"

using namespace std;

struct ab {
  ab() : area(0.), areaManNoirri(0.), areaUmanNoirri(0.), areaManIrri(0.), areaUmanIrri(0.), bm(0.), bmManNoirri(0.), bmUmanNoirri(0.), bmManIrri(0.), bmUmanIrri(0.), bmMax(0.) {}
  double area;
  double areaManNoirri;
  double areaUmanNoirri;
  double areaManIrri;
  double areaUmanIrri;
  double bm;
  double bmManNoirri;
  double bmUmanNoirri;
  double bmManIrri;
  double bmUmanIrri;
  double bmMax;
};

int main(int argc, char **argv) {
  g4m::incrementCurvesHf rtNormal  //Average rotation times
    (150, 8.0, 0., -1.4, -0.3,
     0., 0.9, -0.8242, -0.4273, -0.4, -1.476, 4.283, -0.3, 3.610, -1.071,
     0.1, 1., -2.0670, -0.3028, 0.5, 1.5, 150, 0.01, 0.5, 0.5,
     0.8, 1./500., 2., 0.01, 0.5, 22.09082, 0.62065,-0.02, 1.50614, -0.25346,
     22.70, 16.56, -0.01, 0.24754, -1.81373, 1.0945, 0.0999, -1.6033,
     1.6, 0.95, 1.5);

  g4m::incrementTab iTab(rtNormal,
     15.,      //maiMax
     0.25,   //maiStep
     600,    //tMax
     1,      //tStep
     0.25,   //sdNatStep
     1.5,    //sdTabMax
     0.25,   //sdTabStep
     10.);    //timeframe

  // g4m::incrementTab tabPine
  //   (-0.3835, -0.2416, -1.7576, 1.1638,         //k
  //    170, 114.343, -2.804, 1.044,               //tMax
  //    0., 0.9, -0.8242, -0.4273,                     //maxDens0
  //    -0.4, -1.476, 4.283, -0.3, 3.610, -1.071,  //maxDens1
  //    0.1, 1., -2.0670, -0.3028,                 //maxDens2
  //    0.5,                                       //maxDens3
  //    1.5, 150, 0.01, 0.5, 0.5,                  //imul0
  //    0.8, 1./500., 2., 0.01, 0.5,               //imul0
  //    22.09082, 0.62065, -0.01965, 1.50614, -0.25346, //h
  //    22.70, 16.56,                              //d0
  //    -0.01068, 0.24754, -1.81373,               //d1
  //    1.0945, 0.0999, -1.6033,                   //d2
  //    1.6,                                       //dmul
  //    0.95,                                      //iCrit
  //    5.,      //maiMax
  //    0.25,   //maiStep
  //    600,    //tMax
  //    1,      //tStep
  //    0.25,   //sdNatStep
  //    1.5,    //sdTabMax
  //    0.25,   //sdTabStep
  //    10.);    //timeframe
  double slpMult[] = {1., 102./98., 134./98., 158./98., 111./98.};

  map<int, double> fcalib;
  {
    ifstream fp;
    fp.open("calibFactor.txt", ios::in);
    int country;
    double f;
    while(fp >> country >> f) {
      fcalib[country] = f;
    }
  }

  map<int, double[10]> fcalib2;
  {
    ifstream fp;
    fp.open("calibFactor2.txt", ios::in);
    int country;
    double f[10];
    while(fp >> country) {
      for(int i=0; i<10; ++i) {
	fp >> f[i];
	fcalib2[country][i] = f[i];
      }
    }
  }

  ifstream fp[12];
  int val[10];
  double dval[2];
  fp[0].open("/data/tmp/country.bin", ios::in | ios::binary);
  fp[1].open("/data/tmp/forest.bin", ios::in | ios::binary);
  fp[2].open("/data/tmp/forestIrri.bin", ios::in | ios::binary);
  fp[3].open("/data/tmp/managedForest.bin", ios::in | ios::binary);
  fp[4].open("/data/tmp/slope1t6.bin", ios::in | ios::binary);
  fp[5].open("/data/tmp/slope7t8.bin", ios::in | ios::binary);
  fp[6].open("/data/tmp/slope1t9.bin", ios::in | ios::binary);
  fp[7].open("/data/tmp/slope1t10.bin", ios::in | ios::binary);
  fp[8].open("/data/tmp/slope1t11.bin", ios::in | ios::binary);
  fp[9].open("/data/tmp/landarea.bin", ios::in | ios::binary);  //4
  fp[10].open("/data/tmp/mai.bin", ios::in | ios::binary);  //4
  fp[11].open("/data/tmp/maiIrri.bin", ios::in | ios::binary);  //4
  ofstream fpo[24];
  fpo[0].open("/data/tmp/cStem_ha.bin", ios::out | ios::binary);
  fpo[1].open("/data/tmp/cStemManNoirri_ha.bin", ios::out | ios::binary);
  fpo[2].open("/data/tmp/cStemUmanNoirri_ha.bin", ios::out | ios::binary);
  fpo[3].open("/data/tmp/cStemManIrri_ha.bin", ios::out | ios::binary);
  fpo[4].open("/data/tmp/cStemUmanIrri_ha.bin", ios::out | ios::binary);
  fpo[5].open("/data/tmp/cStemMax_ha.bin", ios::out | ios::binary);
  fpo[6].open("/data/tmp/stock.bin", ios::out | ios::binary);
  fpo[7].open("/data/tmp/stock_ha.bin", ios::out | ios::binary);
  fpo[8].open("/data/tmp/bmAg.bin", ios::out | ios::binary);
  fpo[9].open("/data/tmp/bmAg_ha.bin", ios::out | ios::binary);
  fpo[10].open("/data/tmp/bmBg.bin", ios::out | ios::binary);
  fpo[11].open("/data/tmp/bmBg_ha.bin", ios::out | ios::binary);
  fpo[12].open("/data/tmp/bmDe.bin", ios::out | ios::binary);
  fpo[13].open("/data/tmp/bmDe_ha.bin", ios::out | ios::binary);
  fpo[14].open("/data/tmp/cAg.bin", ios::out | ios::binary);
  fpo[15].open("/data/tmp/cAg_ha.bin", ios::out | ios::binary);
  fpo[16].open("/data/tmp/cBg.bin", ios::out | ios::binary);
  fpo[17].open("/data/tmp/cBg_ha.bin", ios::out | ios::binary);
  fpo[18].open("/data/tmp/cDe.bin", ios::out | ios::binary);
  fpo[19].open("/data/tmp/cDe_ha.bin", ios::out | ios::binary);
  fpo[20].open("/data/tmp/cLi.bin", ios::out | ios::binary);
  fpo[21].open("/data/tmp/cLi_ha.bin", ios::out | ios::binary);
  fpo[22].open("/data/tmp/cLv.bin", ios::out | ios::binary);
  fpo[23].open("/data/tmp/cLv_ha.bin", ios::out | ios::binary);


  map<int, ab> total;
  while (fp[0].good()) {
    union {
      float f;
      int i;
      char c[4];
    } b2f;
    for(int i=0; i<9; ++i) {val[i] = fp[i].get();}
    for(int i=0; i<4; ++i) {b2f.c[i] = fp[9].get();}
    val[9] = b2f.i;
    for(int j=0; j<2; ++j) {
      for(int i=0; i<4; ++i) {b2f.c[i] = fp[j+10].get();}
      dval[j] = b2f.f;
    }
    double bm = 0.;
    double bmManNoirri = 0.;
    double bmUmanNoirri = 0.;
    double bmManIrri = 0.;
    double bmUmanIrri = 0.;
    double shareIrri = 1.;
    double bmMax = 0.;
    double bmDef = 0.;
    if(val[1]>0 && val[2]>0) {shareIrri = val[1] / (val[1] + val[2]);}
    for(int i=0; i<2; ++i) { //0..mai, 1..maiIrrigated
      double mai=dval[i];
      if(mai > 0) {
	double f = 1.;
	auto it = fcalib.find(val[0]);
	if(it != fcalib.end()) {f = it->second;}
	double u0 = iTab.gTopt(mai,2); //U-Unmanaged
	double bm0 = iTab.gAvgBm(u0, mai);
	double u1 = iTab.gToptt(mai,0); //U-Managed
	double bm1 = 0.;
	double bm2 = iTab.gAvgBm(iTab.gTopt(mai,1), mai);
	bmMax += bm2 * shareIrri;
	for(int j=0; j<5; ++j) {
	  bm1 += iTab.gAvgBmt(u1*slpMult[j], mai)*val[4+j]/100.;
	}
	bmDef += bm0 * (100 - val[3])/100. * shareIrri;
	bmDef += bm1 * val[3] / 100. * shareIrri;
	bm0 *= f;
	bm1 *= f;
	if(bm0>bm2) {bm0=bm2;}
	if(bm1>bm2) {bm1=bm2;}
	bm += bm0 * (100 - val[3])/100. * shareIrri;
	bm += bm1 * val[3] / 100. * shareIrri;
	if(i==0) {bmUmanNoirri += bm0;
	  bmManNoirri += bm1;
	} else {bmUmanIrri += bm0;
	  bmManIrri += bm1;
	}
      }
      shareIrri = 1.-shareIrri;
    }
    total[val[0]].area += (val[1]+val[2])/100. * val[9]/10000.;
    total[val[0]].areaManNoirri += val[3]/100. * val[1]/100 * val[9]/10000.;
    total[val[0]].areaUmanNoirri += (100-val[3])/100. * val[1]/100 * val[9]/10000.;
    total[val[0]].areaManIrri += val[3]/100. * val[2]/100 * val[9]/10000.;
    total[val[0]].areaUmanIrri += (100-val[3])/100. * val[2]/100 * val[9]/10000.;
    total[val[0]].bm += bm * (val[1]+val[2])/100. * val[9]/10000.;
    total[val[0]].bmManNoirri += bmManNoirri * val[3]/100. * val[1]/100 * val[9]/10000.;
    total[val[0]].bmUmanNoirri += bmUmanNoirri * (100-val[3])/100. * val[1]/100 * val[9]/10000.;
    total[val[0]].bmManIrri += bmManIrri * val[3]/100. * val[2]/100 * val[9]/10000.;
    total[val[0]].bmUmanIrri += bmUmanIrri * (100-val[3])/100. * val[2]/100 * val[9]/10000.;
    total[val[0]].bmMax += bmMax * (val[1]+val[2])/100. * val[9]/10000.;

    if(fp[0].good()) {
      for(int j=0; j<6; ++j) {
	switch(j) {
	case 0: b2f.f = bm; break;
	case 1: b2f.f = bmManNoirri; break;
	case 2: b2f.f = bmUmanNoirri; break;
	case 3: b2f.f = bmManIrri; break;
	case 4: b2f.f = bmUmanIrri; break;
	case 5: b2f.f = bmMax; break;
	default: b2f.f = 0.;
	}
	for(int i=0; i<4; ++i) {fpo[j].put(b2f.c[i]);}
      }
      for(int j=0; j<9; ++j) {
	b2f.f = bmDef * fcalib2[val[0]][j];
	for(int i=0; i<4; ++i) {fpo[7+j*2].put(b2f.c[i]);}
	b2f.f *= (val[1]+val[2])/100. * val[9]/10000.;
	for(int i=0; i<4; ++i) {fpo[6+j*2].put(b2f.c[i]);}
      }
    }
  }
  for(int i=0; i<5; ++i) {fpo[i].close();}
  for(int i=0; i<12; ++i) {fp[i].close();}
  cout << "country\tarea\tareaManNoirri\tareaUmanNoirri\tareaManIrri\tareaUmanIrri\tbm\tbmManNoirri\tbmUmanNoirri\tbmManIrri\tbmUmanIrri\tbmMax" << endl;
  for(map<int, ab>::iterator it = total.begin(); it!=total.end(); ++it) {
    cout << it->first
	 << "\t" << it->second.area
	 << "\t" << it->second.areaManNoirri
	 << "\t" << it->second.areaUmanNoirri
	 << "\t" << it->second.areaManIrri
	 << "\t" << it->second.areaUmanIrri
	 << "\t" << it->second.bm
	 << "\t" << it->second.bmManNoirri
	 << "\t" << it->second.bmUmanNoirri
	 << "\t" << it->second.bmManIrri
	 << "\t" << it->second.bmUmanIrri
	 << "\t" << it->second.bmMax
	 << endl;
  }
  return(0);
}
