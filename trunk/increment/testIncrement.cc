#include <iostream>
#include "increment.h"

#include <cstdlib>

using namespace std;

int main(int argc, char **argv) {
  g4m::incrementCurves assmann
    (-4.250917, -1.386354, 0.328997, 168.8818, 80.5860, 4.4780, -1.3871
     , 0.779713, -0.175731, 1./4.5, 0.5, -1.08000, -7.79556, -0.23617, -0.63165
     , 24.6207944, 0.4657283, -0.0242588, 2.5888393, -0.3036447, -0.578238
     , 0.851274, 0.028899, 1.472047, 0.5, 2., 1.6, 1.5);
  assmann.setMai(2.5);
  cout << assmann.gTOpt() << endl;
  cout << assmann.tcpMax << endl;
  cout << assmann.gTcp(88.2754)/88.2754 << endl;
  cout << assmann.gTcp(85.)/85. << endl;
  cout << assmann.gTcp(90.)/90. << endl;
  g4m::incrementTab fi(-4.250917, -1.386354, 0.328997, 168.8818, 80.5860, 4.4780, -1.3871
     , 0.779713, -0.175731, 1./4.5, 0.5, -1.08000, -7.79556, -0.23617, -0.63165
     , 24.6207944, 0.4657283, -0.0242588, 2.5888393, -0.3036447, -0.578238
     , 0.851274, 0.028899, 1.472047, 0.5, 2., 1.6, 10., 0.25, 600., 1.);
  cout << fi.gTopt(2.5, 1) << endl;
  cout << fi.gMai(90, 2.5) << endl;
  exit(0);
  cout << fi.gH(40, 5) << " " << fi.gH(41, 5) << endl;
  cout << fi.gHi(40, 5.) << endl;
  cout << fi.gD(40, 5) << " " << fi.gD(41, 5) << endl;
  cout << fi.gDi(40, 5., 1.) << endl;
  cout << fi.gDi(40, 5., 1.1) << endl;
  cout << fi.gDbm(40, 5, 143.4, 0.9).first << " " << fi.gDbm(40, 5, 143.4, 0.9).second << endl;
  cout << fi.gDbmt(40, 5, 110.4, 1).first << " " << fi.gDbmt(40, 5, 110.4, 1).second << endl;
  cout << fi.gLfz(40, 5, 0.5) << endl;
  cout << fi.gSd(70, 5, 30, 2) << endl;
  cout << fi.gU(70, 6., 1.) << endl;
  cout << fi.gUt(70, 6., 1.) << endl;
  cout << fi.gTopt(0.3, 2) << endl;
  cout << fi.gAvgBm(180, 6) << endl;
  cout << fi.gAvgBmt(180, 6) << endl;
  cout << fi.gMai(180.3, 6.1) << endl;
  cout << fi.gGwl(180.3, 6.1) << endl;
  cout << fi.gHbm(50.3, 6.1) << endl;
  cout << fi.gHbmt(50.3, 6.1) << endl;
  assmann.setMai(2.25);
  cout << assmann.gTOpt() << endl;
  cout << "t\tTCP\tMaxDens\tManDens\th\tDBH" << endl;
  for(int t=0; t<250; t+=10) {
    cout << t;
    cout << "\t" << assmann.gTcp(t);
    cout << "\t" << assmann.gMaxDens(t);
    cout << "\t" << assmann.gManDens(t);
    cout << "\t" << assmann.gH(t);
    cout << "\t" << assmann.gD(t) << endl;
  }
  return(0);
}
