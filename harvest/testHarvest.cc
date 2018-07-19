#include <iostream>
#include "harvest.h"


using namespace std;

int main() {
  g4m::harvest hc;

  double vs=0.7; //Stammvolumen
  double vl=15.; //Ladevolumen
  double slope=30.;  //Hangneigung in %
  double dist=300.; //Rueckedistanz
  double cr=0.6; //Kronenverhaeltniss

  cout << "vs\ttFPMmF\ttFPMmT\ttFMmF\ttFMmT\ttFPHaF\ttFPHaT\ttFFbF\ttFFbT\ttTAnF\ttTAnT\ttTFwF\ttTFwT\ttTSkLF\ttTSkLT\ttTSkTF\ttTSkTT\ttTCyLF\ttTCyLT\ttTCyTF\ttTCyTT\ttPPm" << endl;


  for(vs=0.01; vs<10.; vs+=0.01) {
    cout << vs;
    for(int i=0;i<8;++i) {cout << "\t" << hc.tfp(i,vs,slope,cr,dist);}
    for(int i=0;i<12;++i) {cout << "\t" << hc.tt(i,vs,vl,slope,dist);}
    for(int i=0;i<1;++i) {cout << "\t" << hc.tp(i,vs,cr);}
    cout << endl;
  }

  return(0);
}
