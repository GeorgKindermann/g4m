#include <iostream>
#include <fstream>
#include <map>

extern"C" {
  void __yasso_MOD_mod5c(float *a, float *t, float *cl, float *init, float *inf, float *s, float *z);
}

using namespace std;

struct ab {
  ab() : area(0.), areaManNoirri(0.), areaUmanNoirri(0.), areaManIrri(0.), areaUmanIrri(0.), bm(0.), bmManNoirri(0.), bmUmanNoirri(0.), bmManIrri(0.), bmUmanIrri(0.) {}
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
};

int main(int argc, char **argv) {
  float a[45] = {-0.462,-4.802,-0.307,-0.205,0.000227,0.014,0.985,0.844,0.229,0.003,0.021,0.000207,0.009,0.132,0.198,0.266,0.06,-0.000279,0,0,0,0,0,0,0,-1.166,0,0,0,0,0,0,0,0,-0.001,0.002,0,0,-0.964,0.915,-0.155,0,0,0}; //Parameters from Paper for Global
  float t = 10000; //Time to run
  float cl[3] = {10., 600., 12.}; //Temp, precip, amplit
  float init[5] = {0,0,0,0,0}; //Startpoint[acid,water,ethanol,nonSoluble,humus]
  float inf[5] = {0.6,0.06,0.04,0.3,0.}; //C input
  float s = 1; //Diameter

  ifstream fp[10];
  double val[10];
  fp[0].open("/tmp/country.bin", ios::in | ios::binary);
  fp[1].open("/tmp/forest.bin", ios::in | ios::binary);
  fp[2].open("/tmp/forestIrri.bin", ios::in | ios::binary);
  fp[3].open("/tmp/managedForest.bin", ios::in | ios::binary);
  fp[4].open("/tmp/precip.bin", ios::in | ios::binary);  //2
  fp[5].open("/tmp/landarea.bin", ios::in | ios::binary);  //4
  fp[6].open("/tmp/npp.bin", ios::in | ios::binary);  //4
  fp[7].open("/tmp/nppIrri.bin", ios::in | ios::binary);  //4
  fp[8].open("/tmp/tempMean.bin", ios::in | ios::binary);  //4
  fp[9].open("/tmp/tempAmpl.bin", ios::in | ios::binary);  //4
  ofstream fpo[5];
  fpo[0].open("/tmp/soilcMan.bin", ios::out | ios::binary);
  fpo[1].open("/tmp/soilcUman.bin", ios::out | ios::binary);
  fpo[2].open("/tmp/soilcManIrri.bin", ios::out | ios::binary);
  fpo[3].open("/tmp/soilcUmanIrri.bin", ios::out | ios::binary);
  fpo[4].open("/tmp/soilc.bin", ios::out | ios::binary);
  map<int, ab> total;
  int lines=933120000;
  while (fp[0].good() && lines > 0) {
    --lines;
    float z[5] = {0,0,0,0,0}; //Result [acid,water,ethanol,nonSoluble,humus]
    union {
      float f;
      int i;
      char c[4];
    } b2f = {.i=0};
    for(int i=0; i<4; ++i) {b2f.c[0] = fp[i].get(); val[i] = b2f.i;}
    b2f.c[0] = fp[4].get(); b2f.c[1] = fp[4].get(); val[4] = b2f.i;
    for(int j=0; j<4; ++j) {b2f.c[j] = fp[5].get();} val[5] = b2f.i;
    for(int i=6; i<10; ++i) {
      for(int j=0; j<4; ++j) {b2f.c[j] = fp[i].get();}
      val[i] = b2f.f;
    }
    total[val[0]].area += (val[1]+val[2])/100. * val[5]/10000.;
    total[val[0]].areaManNoirri += val[3]/100. * val[1]/100 * val[5]/10000.;
    total[val[0]].areaUmanNoirri += (100-val[3])/100. * val[1]/100 * val[5]/10000.;
    total[val[0]].areaManIrri += val[3]/100. * val[2]/100 * val[5]/10000.;
    total[val[0]].areaUmanIrri += (100-val[3])/100. * val[2]/100 * val[5]/10000.;
    double totalC = 0.;
    double forest = val[1] + val[2];
    cl[0] = val[8]; //tmean
    cl[1] = val[4]; //Precip
    cl[2] = val[9]; //tAmpl
    //no irrigation, Managed
    if(forest>0.) {
      float inf2[5];
      double cinfolw = val[6] * 0.7;
      for(int i=0; i<5; ++i) {inf2[i] = inf[i]*cinfolw;}
      __yasso_MOD_mod5c(a,&t,cl,init,inf2,&s,z);
    }
    double cSoil = z[0];
    for(int i=1; i<5; ++i) {cSoil += z[i];}
    b2f.f = cSoil;
    if(fp[0].good()) {for(int i=0; i<4; ++i) {fpo[0].put(b2f.c[i]);}}
    total[val[0]].bmManNoirri += cSoil * val[3]/100. * val[1]/100 * val[5]/10000.;
    totalC += cSoil * val[3]/100. * val[1]/100 * val[5]/10000.;
    for(int i=0; i<5; ++i) {z[i]=0.;}
    //no irrigation, Unmanaged
    if(forest>0.) {
      float inf2[5];
      double cinfolw = val[6];
      for(int i=0; i<5; ++i) {inf2[i] = inf[i]*cinfolw;}
      __yasso_MOD_mod5c(a,&t,cl,init,inf2,&s,z);
    }
    cSoil = z[0];
    for(int i=1; i<5; ++i) {cSoil += z[i];}
    b2f.f = cSoil;
    if(fp[0].good()) {for(int i=0; i<4; ++i) {fpo[1].put(b2f.c[i]);}}
    total[val[0]].bmUmanNoirri += cSoil * (100-val[3])/100. * val[1]/100 * val[5]/10000.;
    totalC += cSoil * (100-val[3])/100. * val[1]/100 * val[5]/10000.;
    for(int i=0; i<5; ++i) {z[i]=0.;}
    cl[1] = val[4] + 80.; //Precip
    //irrigation, Managed
    if(forest>0.) {
      float inf2[5];
      double cinfolw = val[7] * 0.7;
      for(int i=0; i<5; ++i) {inf2[i] = inf[i]*cinfolw;}
      __yasso_MOD_mod5c(a,&t,cl,init,inf2,&s,z);
    }
    cSoil = z[0];
    for(int i=1; i<5; ++i) {cSoil += z[i];}
    b2f.f = cSoil;
    if(fp[0].good()) {for(int i=0; i<4; ++i) {fpo[2].put(b2f.c[i]);}}
    total[val[0]].bmManIrri += cSoil * val[3]/100. * val[2]/100 * val[5]/10000.;
    totalC += cSoil * val[3]/100. * val[2]/100 * val[5]/10000.;
    for(int i=0; i<5; ++i) {z[i]=0.;}
    //irrigation, Unmanaged
    if(forest>0.) {
      float inf2[5];
      double cinfolw = val[7];
      for(int i=0; i<5; ++i) {inf2[i] = inf[i]*cinfolw;}
      __yasso_MOD_mod5c(a,&t,cl,init,inf2,&s,z);
    }
    cSoil = z[0];
    for(int i=1; i<5; ++i) {cSoil += z[i];}
    b2f.f = cSoil;
    if(fp[0].good()) {for(int i=0; i<4; ++i) {fpo[3].put(b2f.c[i]);}}
    total[val[0]].bmUmanIrri += cSoil * (100-val[3])/100. * val[2]/100 * val[5]/10000.;
    totalC += cSoil * (100-val[3])/100. * val[2]/100 * val[5]/10000.;
    b2f.f = totalC;
    if(fp[0].good()) {for(int i=0; i<4; ++i) {fpo[4].put(b2f.c[i]);}}

    /*
      double npp= (val[1]*val[6] + val[2]*val[7]) / forest;
      double cinfolw = (npp * val[3] * 0.7 + npp * (100. - val[3])) / 100.;
      float inf2[5];
      for(int i=0; i<5; ++i) {inf2[i] = inf[i]*cinfolw;}
      cl[0] = val[8]; //tmean
      cl[1] = val[4]; //Precip
      cl[2] = val[9]; //tAmpl
      __yasso_MOD_mod5c(a,&t,cl,init,inf2,&s,z);
    }
    double cSoil = z[0];
    for(int i=1; i<5; ++i) {cSoil += z[i];}
    if(fp[0].good()) {
      for(int j=0; j<6; ++j) {
	if(j>0) {b2f.f = z[j-1];}
	else {b2f.f = cSoil;}
	for(int i=0; i<4; ++i) {fpo[j].put(b2f.c[i]);}
      }
    }
    */

  }
  cout << "country\tarea\tareaManNoirri\tareaUmanNoirri\tareaManIrri\tareaUmanIrri\tbm\tbmManNoirri\tbmUmanNoirri\tbmManIrri\tbmUmanIrri" << endl;
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
	 << endl;
  }
  return(0);
}
