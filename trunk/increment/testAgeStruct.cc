#include <iostream>
#include "ageStruct.h"

using namespace std;

int main(int argc, char **argv) {
  g4m::incrementTab fi(-4.25, -1.39, 0.329, 169., 81., 4.48, -1.39, 0.780,
                  -0.176, 1./4.5, 0.5, -1.08, -7.7956, -0.2362, -0.6316,
                  24.62079, 0.46573, -0.02426, 2.58884, -0.30364, -0.5782,
		   0.8513, 0.0289, 1.4720, 0.5, 2., 1.6, 10., 0.25, 600., 1.);
  cout << fi.gTopt(1.5, 1) << endl;
  cout << fi.gMai(99, 1.5) << endl;
  g4m::ipol<double,double> sws;  //Schnittholzanteil an Vfm
  g4m::ipol<double,double> hlv;   //1-Ernteverluste Vornutzung
  g4m::ipol<double,double> hle;   //1-Ernteverluste Endnutzung
  g4m::ipol<double,double> dbv;  //Dekungsbeitrag vornutzung
  g4m::ipol<double,double> dbe;  //Dekungsbeitrag endnutzung

  sws.insert(10, .0);
  sws.insert(30, .6);

  hlv.insert(0, .0);
  hlv.insert(25, .7);
  hle.insert(0, .0);
  hle.insert(25, .7);

  dbv.insert(0, 2);
  dbe.insert(0, 3);

  //sws.insert(7, 0.);
  //sws.insert(50, .6);
  //hlv.insert(0, 0.);
  //hlv.insert(10, .5);
  //hlv.insert(50, .7);
  //hle.insert(0, 0.);
  //hle.insert(10, .5);
  //hle.insert(50, .7);
  //dbv.insert(0, -2.);
  //dbv.insert(15, 0.);
  //dbv.insert(70, 4.);
  //dbe.insert(0, -1.);
  //dbe.insert(10, 0.);
  //dbe.insert(70, 6.);

  //g4m::ageStruct cohort(&fi, &sws, &hlv, &hle, &dbv, &dbe, 0, 0, 1.5, 100, 1., 1., 0.75);
  double mai = 1.5;
  double optRot = fi.gTopt(mai, 5);
  cout << optRot << endl;
  g4m::ageStruct cohort(&fi, &sws, &hlv, &hle, &dbv, &dbe, 0, 0, mai, optRot, -1., 1., 0.75);
  //cohort.afforest(100.);
  //cohort.deforest(50., 1);
  g4m::ageStruct::v res;
  for(int i=0; i<120; ++i) {
    res = cohort.aging();
    cout << i << " " << res.area << " " << res.enSw << " " << res.enRw << " " << res.vnSw;
    cout << " " << res.vnRw << " " << res.dbEn << " " << res.dbVn << endl;
  }
  cout << cohort.getArea() << endl;

  cohort.setMai(3.);
  cohort.setRotPeriod(70);
  cohort.setStockingdegree(-1.);
  cohort.createNormalForest(70, 1, -1.);
  cout << cohort.getBm() << endl;
  cohort.aging();
  cout << cohort.getBm() << endl;

  cout << "fipol Begin" << endl;
  g4m::ageLUT lut(&cohort);
  cout << lut.getBm(3., 70) << endl;
  cout << lut.getBmNT(3., 70) << endl;
  cout << lut.getEnSw(3., 70) << endl;
  cout << lut.getEnRw(3., 70) << endl;
  cout << lut.getVnSw(3., 70) << endl;
  cout << lut.getVnRw(3., 70) << endl;
  cout << lut.getDbEn(3., 70) << endl;
  cout << lut.getDbVn(3., 70) << endl;
  cout << lut.getEnSwNT(3., 70) << endl;
  cout << lut.getEnRwNT(3., 70) << endl;
  cout << lut.getDbEnNT(3., 70) << endl;
  cout << "fipol End" << endl;
  cout << lut.gTopt(3., 1) << endl;
  cout << lut.gTopt(2., 1) << endl;
  cout << lut.gTopt(1., 1) << endl;
  cout << lut.gTopt(3., 2) << endl;
  cout << lut.gTopt(2., 2) << endl;
  cout << lut.gTopt(1., 2) << endl;

  return(0);
}
