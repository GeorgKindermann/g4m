#include <iostream>
#include "increment.h"
#include "ageStruct.h"

using namespace std;

int main(int argc, char **argv) {
  g4m::incrementTab 
    fi(-4.250917, -1.386354, 0.328997, 168.8818, 80.5860, 4.4780, -1.3871
       , 0.779713, -0.175731, 1./4.5, 0.5, -1.08000, -7.79556, -0.23617
       , -0.63165, 24.6207944, 0.4657283, -0.0242588, 2.5888393, -0.3036447
       , -0.578238, 0.851274, 0.028899, 1.472047, 0.5, 2., 1.6, 10., 0.25, 600.
       , 1.);
  double u = 100.;
  double mai = 2.;
  double avgBm = 60.;
  avgBm = fi.gAvgBm(u, mai);
  cout << mai << "\t" << u << "\t" << avgBm << endl;
  u = fi.gU(avgBm, mai);
  cout << mai << "\t" << u << "\t" << avgBm << endl;
  

  g4m::ipol<double,double> sws;  //Schnittholzanteil an Vfm
  g4m::ipol<double,double> hlv;   //1-Ernteverluste Vornutzung
  g4m::ipol<double,double> hle;   //1-Ernteverluste Endnutzung
  g4m::ipol<double,double> dbv;  //Dekungsbeitrag vornutzung
  g4m::ipol<double,double> dbe;  //Dekungsbeitrag endnutzung
  sws.insert(0, .5);
  hlv.insert(0, .1);
  hle.insert(0, .1);
  dbv.insert(0, 1);

  g4m::ageStruct cohort(&fi, &sws, &hlv, &hle, &dbv, &dbe, 0, 0, mai, u+1, -1., 1.01, 0.75);
  g4m::ageStruct::v res;
  res = cohort.aging();
  avgBm = cohort.getBm();
  cout << mai << "\t" << u << "\t" << avgBm << endl;

  return(0);
}
