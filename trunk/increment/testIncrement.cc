#include <iostream>
#include "increment.h"

#include <cstdlib>

using namespace std;

int main(int argc, char **argv) {
  g4m::incrementCurves pine
    (-0.3835, -0.2416, -1.7576, 1.1638,         //k
     170, 114.343, -2.804, 1.044,               //tMax
     0.9, -0.8242, -0.4273,                     //maxDens0
     -0.4, -1.476, 4.283, -0.3, 3.610, -1.071,  //maxDens1
     0.1, -2.0670, -0.3028,                     //maxDens2
     0.5,                                       //maxDens3
     1.5, 150, 0.01, 0.5, 0.5,                  //imul0
     0.8, 1./500., 2., 0.01, 0.5,               //imul0
     22.09082, 0.62065, -0.01965, 1.50614, -0.25346, //h
     22.70, 16.56,                              //d0
     -0.01068, 0.24754, -1.81373,               //d1
     1.0945, 0.0999, -1.6033,                   //d2
     1.6,                                       //dmul
     0.95,                                      //iCrit
     1.5);                                      //MAI

 
  // for(int i=0; i<10; ++i) {
  //   for(int mai=0; mai<41; ++mai) {
  //     pine.setMai(mai/4.);
  //     for(int t=0; t<250; ++t) {
  // 	double tmp = pine.gManDens(t);
  // 	cout << mai;
  // 	cout << "\t" << t;
  // 	cout << "\t" << pine.gTcp(t);
  // 	cout << "\t" << pine.gMaxDens(t);
  // 	cout << "\t" << tmp;
  // 	cout << "\t" << pine.gH(t);
  // 	cout << "\t" << pine.gD(t);
  // 	cout << "\t" << pine.gImul(tmp, t);
  // 	cout << "\t" << pine.gDmul(tmp);
  // 	cout << endl;
  //     }
  //   }
  // }

  g4m::incrementTab tabPine
    (-0.3835, -0.2416, -1.7576, 1.1638,         //k
     170, 114.343, -2.804, 1.044,               //tMax
     0.9, -0.8242, -0.4273,                     //maxDens0
     -0.4, -1.476, 4.283, -0.3, 3.610, -1.071,  //maxDens1
     0.1, -2.0670, -0.3028,                     //maxDens2
     0.5,                                       //maxDens3
     1.5, 150, 0.01, 0.5, 0.5,                  //imul0
     0.8, 1./500., 2., 0.01, 0.5,               //imul0
     22.09082, 0.62065, -0.01965, 1.50614, -0.25346, //h
     22.70, 16.56,                              //d0
     -0.01068, 0.24754, -1.81373,               //d1
     1.0945, 0.0999, -1.6033,                   //d2
     1.6,                                       //dmul
     0.95,                                      //iCrit
     5.,      //maiMax
     0.25,   //maiStep
     600,    //tMax
     1,      //tStep
     0.25,   //sdNatStep
     1.5,    //sdTabMax
     0.25,   //sdTabStep
     10.);    //timeframe

  // {
  //   cout << "Test Average Biomass" << endl;
  //   double mai = 1.5;
  //   for(int i=0; i<20; ++i) {
  //     cout << i*10
  // 	   << "\t" << tabPine.gAvgBm(i*10, mai)
  // 	   << "\t" << tabPine.gAvgBmt(i*10, mai)
  // 	   << "\t" << tabPine.gAvgBmSdNat(i*10, mai, 1.5)
  // 	   << "\t" << tabPine.gAvgBmSdNat(i*10, mai, 1.)
  // 	   << "\t" << tabPine.gAvgBmSdNat(i*10, mai, .5)
  // 	   << "\t" << tabPine.gAvgBmSdNat(i*10, mai, 0.)
  // 	   << "\t" << tabPine.gAvgBmSdTab(i*10, mai, 1.5)
  // 	   << "\t" << tabPine.gAvgBmSdTab(i*10, mai, 1.)
  // 	   << "\t" << tabPine.gAvgBmSdTab(i*10, mai, .5)
  // 	   << "\t" << tabPine.gAvgBmSdTab(i*10, mai, 0.)
  // 	   << endl;
  //   }
  // }

  // {
  //   cout << "Test stocking Biomass" << endl;
  //   double mai = 1.5;
  //   for(int i=0; i<20; ++i) {
  //     cout << i*10
  // 	   << "\t" << tabPine.gBm(i*10, mai)
  // 	   << "\t" << tabPine.gBmt(i*10, mai)
  // 	   << "\t" << tabPine.gBmSdNat(i*10, mai, 1.5)
  // 	   << "\t" << tabPine.gBmSdNat(i*10, mai, 1.)
  // 	   << "\t" << tabPine.gBmSdNat(i*10, mai, .5)
  // 	   << "\t" << tabPine.gBmSdNat(i*10, mai, 0.)
  // 	   << "\t" << tabPine.gBmSdTab(i*10, mai, 1.5)
  // 	   << "\t" << tabPine.gBmSdTab(i*10, mai, 1.)
  // 	   << "\t" << tabPine.gBmSdTab(i*10, mai, .5)
  // 	   << "\t" << tabPine.gBmSdTab(i*10, mai, 0.)
  // 	   << endl;
  //   }
  // }


  // {
  //   cout << "Test DBH" << endl;
  //   double mai = 1.5;
  //   for(int i=0; i<20; ++i) {
  //     cout << i*10
  // 	   << "\t" << tabPine.gDbh(i*10, mai)
  // 	   << "\t" << tabPine.gDbht(i*10, mai)
  // 	   << "\t" << tabPine.gDbhSdNat(i*10, mai, 1.5)
  // 	   << "\t" << tabPine.gDbhSdNat(i*10, mai, 1.)
  // 	   << "\t" << tabPine.gDbhSdNat(i*10, mai, .5)
  // 	   << "\t" << tabPine.gDbhSdNat(i*10, mai, 0.)
  // 	   << "\t" << tabPine.gDbhSdTab(i*10, mai, 1.5)
  // 	   << "\t" << tabPine.gDbhSdTab(i*10, mai, 1.)
  // 	   << "\t" << tabPine.gDbhSdTab(i*10, mai, .5)
  // 	   << "\t" << tabPine.gDbhSdTab(i*10, mai, 0.)
  // 	   << endl;
  //   }
  // }

  // {
  //   cout << "Test GWL" << endl;
  //   double mai = 1.5;
  //   for(int i=0; i<20; ++i) {
  //     cout << i*10
  // 	   << "\t" << tabPine.gGwl(i*10, mai)
  // 	   << "\t" << tabPine.gGwlt(i*10, mai)
  // 	   << "\t" << tabPine.gGwlSdNat(i*10, mai, 1.5)
  // 	   << "\t" << tabPine.gGwlSdNat(i*10, mai, 1.)
  // 	   << "\t" << tabPine.gGwlSdNat(i*10, mai, .5)
  // 	   << "\t" << tabPine.gGwlSdNat(i*10, mai, 0.)
  // 	   << "\t" << tabPine.gGwlSdTab(i*10, mai, 1.5)
  // 	   << "\t" << tabPine.gGwlSdTab(i*10, mai, 1.)
  // 	   << "\t" << tabPine.gGwlSdTab(i*10, mai, .5)
  // 	   << "\t" << tabPine.gGwlSdTab(i*10, mai, 0.)
  // 	   << endl;
  //   }
  // }

  // {
  //   cout << "Test Tree height" << endl;
  //   for(int i=0; i<20; ++i) {
  //     cout << i*10;
  //     for(int mai=0; mai<5; ++mai) {
  // 	cout << "\t" << tabPine.gHeight(i*10, mai*0.5);
  //     }
  //     cout << endl;
  //   }
  // }

  // {
  //   cout << "Test natural stocking degree where yield table stocking degree = 1" << endl;
  //   for(int i=0; i<20; ++i) {
  //     cout << i*10;
  //     for(int mai=0; mai<5; ++mai) {
  // 	cout << "\t" << tabPine.gSdNat(i*10, mai*0.5);
  //     }
  //     cout << endl;
  //   }
  // }

  // {
  //   cout << "Test GWL" << endl;
  //   double mai = 1.5;
  //   for(int i=0; i<20; ++i) {
  //     cout << i*10
  // 	   << "\t" << tabPine.gIncGwl(i*10, mai)
  // 	   << "\t" << tabPine.gIncGwlt(i*10, mai)
  // 	   << "\t" << tabPine.gIncGwlSdNat(i*10, mai, 1.5)
  // 	   << "\t" << tabPine.gIncGwlSdNat(i*10, mai, 1.)
  // 	   << "\t" << tabPine.gIncGwlSdNat(i*10, mai, .5)
  // 	   << "\t" << tabPine.gIncGwlSdNat(i*10, mai, 0.)
  // 	   << "\t" << tabPine.gIncGwlSdTab(i*10, mai, 1.5)
  // 	   << "\t" << tabPine.gIncGwlSdTab(i*10, mai, 1.)
  // 	   << "\t" << tabPine.gIncGwlSdTab(i*10, mai, .5)
  // 	   << "\t" << tabPine.gIncGwlSdTab(i*10, mai, 0.)
  // 	   << endl;
  //   }
  // }

  // {
  //   cout << "Test Bimass change" << endl;
  //   double mai = 1.5;
  //   for(int i=0; i<20; ++i) {
  //     cout << i*10
  // 	   << "\t" << tabPine.gIncBm(i*10, mai)
  // 	   << "\t" << tabPine.gIncBmt(i*10, mai)
  // 	   << "\t" << tabPine.gIncBmSdNat(i*10, mai, 1.5)
  // 	   << "\t" << tabPine.gIncBmSdNat(i*10, mai, 1.)
  // 	   << "\t" << tabPine.gIncBmSdNat(i*10, mai, .5)
  // 	   << "\t" << tabPine.gIncBmSdNat(i*10, mai, 0.)
  // 	   << "\t" << tabPine.gIncBmSdTab(i*10, mai, 1.5)
  // 	   << "\t" << tabPine.gIncBmSdTab(i*10, mai, 1.)
  // 	   << "\t" << tabPine.gIncBmSdTab(i*10, mai, .5)
  // 	   << "\t" << tabPine.gIncBmSdTab(i*10, mai, 0.)
  // 	   << endl;
  //   }
  // }

  // {
  //   cout << "Test Diameter change" << endl;
  //   double mai = 1.5;
  //   for(int i=0; i<20; ++i) {
  //     cout << i*10
  // 	   << "\t" << tabPine.gIncDbh(i*10, mai)
  // 	   << "\t" << tabPine.gIncDbht(i*10, mai)
  // 	   << "\t" << tabPine.gIncDbhSdNat(i*10, mai, 1.5)
  // 	   << "\t" << tabPine.gIncDbhSdNat(i*10, mai, 1.)
  // 	   << "\t" << tabPine.gIncDbhSdNat(i*10, mai, .5)
  // 	   << "\t" << tabPine.gIncDbhSdNat(i*10, mai, 0.)
  // 	   << "\t" << tabPine.gIncDbhSdTab(i*10, mai, 1.5)
  // 	   << "\t" << tabPine.gIncDbhSdTab(i*10, mai, 1.)
  // 	   << "\t" << tabPine.gIncDbhSdTab(i*10, mai, .5)
  // 	   << "\t" << tabPine.gIncDbhSdTab(i*10, mai, 0.)
  // 	   << endl;
  //   }
  // }

  // {
  //   cout << "Test Thinning/Mortality" << endl;
  //   double mai = 1.5;
  //   for(int i=0; i<20; ++i) {
  //     cout << i*10
  // 	   << "\t" << tabPine.gRemBm(i*10, mai)
  // 	   << "\t" << tabPine.gRemBmt(i*10, mai)
  // 	   << "\t" << tabPine.gRemBmSdNat(i*10, mai, 1.5)
  // 	   << "\t" << tabPine.gRemBmSdNat(i*10, mai, 1.)
  // 	   << "\t" << tabPine.gRemBmSdNat(i*10, mai, .5)
  // 	   << "\t" << tabPine.gRemBmSdNat(i*10, mai, 0.)
  // 	   << "\t" << tabPine.gRemBmSdTab(i*10, mai, 1.5)
  // 	   << "\t" << tabPine.gRemBmSdTab(i*10, mai, 1.)
  // 	   << "\t" << tabPine.gRemBmSdTab(i*10, mai, .5)
  // 	   << "\t" << tabPine.gRemBmSdTab(i*10, mai, 0.)
  // 	   << endl;
  //   }
  // }

  // {
  //   cout << "Test get MAI for different rotation times" << endl;
  //   double mai = 1.5;
  //   for(int i=0; i<20; ++i) {
  //     cout << i*10
  // 	   << "\t" << tabPine.gMai(i*10, mai)
  // 	   << "\t" << tabPine.gMait(i*10, mai)
  // 	   << "\t" << tabPine.gMaiSdNat(i*10, mai, 1.5)
  // 	   << "\t" << tabPine.gMaiSdNat(i*10, mai, 1.)
  // 	   << "\t" << tabPine.gMaiSdNat(i*10, mai, .5)
  // 	   << "\t" << tabPine.gMaiSdNat(i*10, mai, 0.)
  // 	   << "\t" << tabPine.gMaiSdTab(i*10, mai, 1.5)
  // 	   << "\t" << tabPine.gMaiSdTab(i*10, mai, 1.)
  // 	   << "\t" << tabPine.gMaiSdTab(i*10, mai, .5)
  // 	   << "\t" << tabPine.gMaiSdTab(i*10, mai, 0.)
  // 	   << endl;
  //   }
  // }


  // {
  //   cout << "Test get optimal rotation times" << endl;
  //   for(int i=0; i<20; ++i) {
  //     cout << i/5.
  // 	   << "\t" << tabPine.gTopt(i/5., 0)
  // 	   << "\t" << tabPine.gTopt(i/5., 1)
  // 	   << "\t" << tabPine.gTopt(i/5., 2)
  // 	   << "\t" << tabPine.gTopt(i/5., 3)
  // 	   << "\t" << tabPine.gTopt(i/5., 4)
  // 	   << "\t" << tabPine.gToptt(i/5., 0)
  // 	   << "\t" << tabPine.gToptSdNat(i/5., 1.5, 0)
  // 	   << "\t" << tabPine.gToptSdNat(i/5., 1., 0)
  // 	   << "\t" << tabPine.gToptSdNat(i/5., .5, 0)
  // 	   << "\t" << tabPine.gToptSdNat(i/5., 0., 0)
  // 	   << "\t" << tabPine.gToptSdTab(i/5., 1.5, 0)
  // 	   << "\t" << tabPine.gToptSdTab(i/5., 1., 0)
  // 	   << "\t" << tabPine.gToptSdTab(i/5., .5, 0)
  // 	   << "\t" << tabPine.gToptSdTab(i/5., 0., 0)
  // 	   << endl;
  //   }
  // }

  // {
  //   cout << "Test get rotation time from biomass" << endl;
  //   double mai = 1.5;
  //   for(int i=0; i<20; ++i) {
  //     cout << i*5.
  // 	   << "\t" << tabPine.gU(i*5., mai)
  // 	   << "\t" << tabPine.gAvgBm(tabPine.gU(i*5., mai), mai)
  // 	   << "\t" << tabPine.gUt(i*5., mai)
  // 	   << "\t" << tabPine.gAvgBmt(tabPine.gUt(i*5., mai), mai)
  // 	   << "\t" << tabPine.gUSdNat(i*5., mai, 1.5)
  //  << "\t" << tabPine.gAvgBmSdNat(tabPine.gUSdNat(i*5., mai, 1.5), mai, 1.5)
  // 	   << "\t" << tabPine.gUSdNat(i*5., mai, 1.)
  //  << "\t" << tabPine.gAvgBmSdNat(tabPine.gUSdNat(i*5., mai, 1.), mai, 1.)
  // 	   << "\t" << tabPine.gUSdNat(i*5., mai, .5)
  //  << "\t" << tabPine.gAvgBmSdNat(tabPine.gUSdNat(i*5., mai, .5), mai, .5)
  // 	   << "\t" << tabPine.gUSdNat(i*5., mai, 0.)
  //  << "\t" << tabPine.gAvgBmSdNat(tabPine.gUSdNat(i*5., mai, 0.), mai, 0.)
  // 	   << "\t" << tabPine.gUSdTab(i*5., mai, 1.5)
  //  << "\t" << tabPine.gAvgBmSdTab(tabPine.gUSdTab(i*5., mai, 1.5), mai, 1.5)
  // 	   << "\t" << tabPine.gUSdTab(i*5., mai, 1.)
  //  << "\t" << tabPine.gAvgBmSdTab(tabPine.gUSdTab(i*5., mai, 1.), mai, 1.)
  // 	   << "\t" << tabPine.gUSdTab(i*5., mai, .5)
  //  << "\t" << tabPine.gAvgBmSdTab(tabPine.gUSdTab(i*5., mai, .5), mai, .5)
  // 	   << "\t" << tabPine.gUSdTab(i*5., mai, 0.)
  //  << "\t" << tabPine.gAvgBmSdTab(tabPine.gUSdTab(i*5., mai, 0.), mai, 0.)
  // 	   << endl;
  //   }
  // }

  // {
  //   cout << "Test get Stockingdegree" << endl;
  //   double mai = 1.5;
  //   double bm = 35;
  //   for(int i=0; i<20; ++i) {
  //     cout << i*10.
  // 	   << "\t" << tabPine.gSdNat(i*10., mai, bm)
  // 	   << "\t" << tabPine.gBmSdNat(i*10., mai, tabPine.gSdNat(i*10., mai, bm))
  // 	   << "\t" << tabPine.gSdTab(i*10., mai, bm)
  // 	   << "\t" << tabPine.gBmSdTab(i*10., mai, tabPine.gSdTab(i*10., mai, bm))
  // 	   << endl;
  //    }
  // }

  {
    cout << "Test get Average Stockingdegree" << endl;
    double mai = 1.5;
    double bm = 35;
    for(int i=0; i<20; ++i) {
      cout << i*10.
	   << "\t" << tabPine.gAvgSdNat(i*10., mai, bm)
	   << "\t" << tabPine.gAvgBmSdNat(i*10., mai, tabPine.gAvgSdNat(i*10., mai, bm))
	   << "\t" << tabPine.gAvgSdTab(i*10., mai, bm)
	   << "\t" << tabPine.gAvgBmSdTab(i*10., mai, tabPine.gAvgSdTab(i*10., mai, bm))
	   << "\t" << tabPine.gAvgBmSdTab(i*10., mai, 9999.)
	   << endl;
     }
  }

  /*
  cout << "t\tTCP\tMaxDens\tManDens\th\tDBH" << endl;
  for(int t=0; t<250; t+=10) {
    cout << t;
    cout << "\t" << pine.gTcp(t);
    cout << "\t" << pine.gMaxDens(t);
    cout << "\t" << pine.gManDens(t);
    cout << "\t" << pine.gH(t);
    cout << "\t" << pine.gD(t);
    cout << "\t" << pine.gTcp(t) * pine.gMaxDens(t);
    cout << "\t" << pine.gImul(pine.gManDens(t), t);
    cout << "\t" << pine.gDmul(pine.gManDens(t));
    cout << endl;
  }
  */

  return(0);
}
/*
  //g4m::incrementCurves assmann
  //(-4.250917, -1.386354, 0.328997, 168.8818, 80.5860, 4.4780, -1.3871
  //, 0.779713, -0.175731, 1./4.5, 0.5, -1.08000, -7.79556, -0.23617, -0.63165
  //, 24.6207944, 0.4657283, -0.0242588, 2.5888393, -0.3036447, -0.578238
  //, 0.851274, 0.028899, 1.472047, 0.5, 2., 1.6, 1.5);
  g4m::incrementCurves assmann
  (-0.7403, -1.0772, 1.4803, 0.6713, 300, -0.2151, -0.9929, 0.5, 0.2, -0.8786, 0.4243, -1.08000, -7.79556, -0.23617, -0.63165, 24.6207944, 0.4657283, -0.0242588, 2.5888393, -0.3036447, -0.578238, 0.851274, 0.028899, 1.472047, 0.5, 2., 1.6, 0., -0.4, 0.4868, 0.1036, 0., 0., 0., 0.25, -1., -2., -0.5, 0.5, -0.4562
     , 1.5);
  assmann.setMai(2.5);
  cout << assmann.gTOpt() << endl;
  cout << assmann.tcpMax << endl;
  cout << assmann.gTcp(88.2754)/88.2754 << endl;
  cout << assmann.gTcp(85.)/85. << endl;
  cout << assmann.gTcp(90.)/90. << endl;
  //g4m::incrementTab fi(-4.250917, -1.386354, 0.328997, 168.8818, 80.5860, 4.4780, -1.3871
  //, 0.779713, -0.175731, 1./4.5, 0.5, -1.08000, -7.79556, -0.23617, -0.63165
  //, 24.6207944, 0.4657283, -0.0242588, 2.5888393, -0.3036447, -0.578238
  //, 0.851274, 0.028899, 1.472047, 0.5, 2., 1.6, 10., 0.25, 600., 1.);
  g4m::incrementTab fi
  (-0.7403, -1.0772, 1.4803, 0.6713, 300, -0.2151, -0.9929, 0.5, 0.2, -0.8786, 0.4243, -1.08000, -7.79556, -0.23617, -0.63165, 24.6207944, 0.4657283, -0.0242588, 2.5888393, -0.3036447, -0.578238, 0.851274, 0.028899, 1.472047, 0.5, 2., 1.6
  , 0., -0.4, 0.4868, 0.1036, 0., 0., 0., 0.25, -1., -2., -0.5, 0.5, -0.4562
  , 10., 0.25, 600., 1.);
  cout << fi.gTopt(2.5, 1) << endl;
  cout << fi.gMai(90, 2.5) << endl;
  cout << fi.gH(40, 5) << " " << fi.gH(41, 5) << endl;
  cout << fi.gHi(40, 5.) << endl;
  cout << fi.gD(40, 5) << " " << fi.gD(41, 5) << endl;
  cout << fi.gDi(40, 5., 1.) << endl;
  cout << fi.gDi(40, 5., 0.9) << endl;
  cout << fi.gDi(40, 5., 1.1) << endl;
  cout << fi.gDbm(40, 5, 143.4, 0.9).first << " " << fi.gDbm(40, 5, 143.4, 0.9).second << endl;
  cout << fi.gDbmt(40, 5, 110.4, 1).first << " " << fi.gDbmt(40, 5, 110.4, 1).second << endl;
  cout << fi.gLfz(40, 5, 0.5) << endl;
  cout << fi.gSd(70, 5, 30, 2) << endl;
  cout << fi.gHbm(20,5) << endl;
  cout << fi.gHbmt(20,5) << endl;
  cout << "Umtriebszeit:" << endl;
  cout << fi.gU(70, 6., 1.) << endl;
  cout << fi.gUt(75, 3.2, 1.) << endl;
  cout << fi.gAvgBmt(85., 3.2) << endl;
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
    cout << "\t" << assmann.gD(t);
    cout << "\t" << assmann.gTcp(t) * assmann.gMaxDens(t);
    cout << endl;
  }

  {
    double u = 100.;
    double mai = 2.;
    double avgBm = 60.;
    avgBm = fi.gAvgBm(u, mai);
    cout << mai << "\t" << u << "\t" << avgBm << endl;
    u = fi.gU(avgBm, mai);
    cout << mai << "\t" << u << "\t" << avgBm << endl;
  }

  return(0);
}

*/
