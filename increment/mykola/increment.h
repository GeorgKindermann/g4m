#ifndef G4M_INCREMENT_H
#define G4M_INCREMENT_H

#include <map>
#include <set>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <limits>

#include "misc.h"

namespace g4m {
  class incrementCurves {
  public:
    incrementCurves
      (const double &a0, const double &a1, const double &a2, const double &a3,
       const double &a4, const double &a5, const double &a6, const double &a7,
    const double &a8, const double &a9, const double &a10, const double &a11,
    const double &a12, const double &a13, const double &a14, const double &a15,
    const double &a16, const double &a17, const double &a18, const double &a19,
    const double &a20, const double &a21, const double &a22, const double &a23,
    const double &a24, const double &a25, const double &a26, const double &a27,
    const double &a28, const double &a29, const double &a30, const double &a31,
    const double &a32, const double &a33, const double &a34, const double &a35,
    const double &a36, const double &a37, const double &a38, const double &a39,
    const double &a40, const double &a41, const double &a42, const double &a43,
    const double &a44, const double &a45, const double &a46, const double &a47,
    const double &amai);
    int setCoef
      (const double &a0, const double &a1, const double &a2, const double &a3,
       const double &a4, const double &a5, const double &a6, const double &a7,
    const double &a8, const double &a9, const double &a10, const double &a11,
    const double &a12, const double &a13, const double &a14, const double &a15,
    const double &a16, const double &a17, const double &a18, const double &a19,
    const double &a20, const double &a21, const double &a22, const double &a23,
    const double &a24, const double &a25, const double &a26, const double &a27,
    const double &a28, const double &a29, const double &a30, const double &a31,
    const double &a32, const double &a33, const double &a34, const double &a35,
    const double &a36, const double &a37, const double &a38, const double &a39,
    const double &a40, const double &a41, const double &a42, const double &a43,
    const double &a44, const double &a45, const double &a46, const double &a47);
    int setMai(double amai);
    double gTOpt();   //increment optimal rotation time
    double gTcp(double t);//Total carbon production (at maximum stocking degree)
    //Reduction of total carbon production (TCP) to get maximum possible Volume
    double gMaxDens(double t);
    double gManDens(double t);  //Natural Stocking degree for managed forests
    double gH(double t);  //Tree height
    double gD(double t);  //Tree diameter at maximum stand density
    double gDmul(double sd);  //diameter multiplyer for stocking degree v/vMax
    double gImul(double sd, double t);  //increment multiplyer for stocking degree v/vMax
    double tcpMax;
  private:
    struct {
      double k[4];
      double tMax[4];
      double h[5];
      double d[8];
      double maxDens[15];
      double dmul[1];
      double imul[10];
      double iCrit; //Critical stocking density (increment = 95% of full stoked)
    } coef;
    double mai;
    double k;
    double tOpt;
    double tMax;
    double th13;
    double calcK();
    double calcTcpMax();
    double calcTMax();
    double calcTOpt();
    double calcTh13();
  };

  class incrementTab {
  public:
    ~incrementTab();
    incrementTab
      (const double &a0, const double &a1, const double &a2, const double &a3,
       const double &a4, const double &a5, const double &a6, const double &a7,
    const double &a8, const double &a9, const double &a10, const double &a11,
    const double &a12, const double &a13, const double &a14, const double &a15,
    const double &a16, const double &a17, const double &a18, const double &a19,
    const double &a20, const double &a21, const double &a22, const double &a23,
    const double &a24, const double &a25, const double &a26, const double &a27,
    const double &a28, const double &a29, const double &a30, const double &a31,
    const double &a32, const double &a33, const double &a34, const double &a35,
    const double &a36, const double &a37, const double &a38, const double &a39,
    const double &a40, const double &a41, const double &a42, const double &a43,
    const double &a44, const double &a45, const double &a46, const double &a47,
       const double amaiMax, const double amaiStep, const double atMax,
       const double atStep, const double asdNatStep, const double asdTabMax,
       const double asdTabStep, const double atimeframe=-1.);
    int setCoef
      (const double &a0, const double &a1, const double &a2, const double &a3,
       const double &a4, const double &a5, const double &a6, const double &a7,
    const double &a8, const double &a9, const double &a10, const double &a11,
    const double &a12, const double &a13, const double &a14, const double &a15,
    const double &a16, const double &a17, const double &a18, const double &a19,
    const double &a20, const double &a21, const double &a22, const double &a23,
    const double &a24, const double &a25, const double &a26, const double &a27,
    const double &a28, const double &a29, const double &a30, const double &a31,
    const double &a32, const double &a33, const double &a34, const double &a35,
    const double &a36, const double &a37, const double &a38, const double &a39,
    const double &a40, const double &a41, const double &a42, const double &a43,
    const double &a44, const double &a45, const double &a46, const double &a47);
    //get Average biomass, u .. rotation time, mai .. Site index
    double gAvgBm(double u, double mai);
    double gAvgBmt(double u, double mai); //With thinning like yield table
    double gAvgBmSdNat(double u, double mai, double sd);//natural stocking degre
    double gAvgBmSdTab(double u, double mai, double sd); //yield table SD
    //Get stocking Biomass
    double gBm(double age, double mai);
    double gBmt(double age, double mai);
    double gBmSdNat(double age, double mai, double sd);
    double gBmSdTab(double age, double mai, double sd);
    //Get Diameter
    double gDbh(double age, double mai);
    double gDbht(double age, double mai);
    double gDbhSdNat(double age, double mai, double sd);
    double gDbhSdTab(double age, double mai, double sd);
    //Get Total increment
    double gGwl(double age, double mai);
    double gGwlt(double age, double mai);
    double gGwlSdNat(double age, double mai, double sd);
    double gGwlSdTab(double age, double mai, double sd);
    //Get tree height
    double gHeight(double age, double mai);
    //Get natural stocking degree if yield table stocking degree is 1
    double gSdNat(double age, double mai);
    //Get Total increment increments
    double gIncGwl(double age, double mai);
    double gIncGwlt(double age, double mai);
    double gIncGwlSdNat(double age, double mai, double sd);
    double gIncGwlSdTab(double age, double mai, double sd);
    //Get Biomass change
    double gIncBm(double age, double mai);
    double gIncBmt(double age, double mai);
    double gIncBmSdNat(double age, double mai, double sd);
    double gIncBmSdTab(double age, double mai, double sd);
    //Get DBH-Increment without thinning
    double gIncDbh(double age, double mai);
    double gIncDbht(double age, double mai);
    double gIncDbhSdNat(double age, double mai, double sd);
    double gIncDbhSdTab(double age, double mai, double sd);
    //Get height increment
    double gIncHeight(double age, double mai);
      //Get Biomass which comes from mortality or thinning and can be used
    //If IncBm is also needed it will be faster to calculate by own
    double gRemBm(double age, double mai);
    double gRemBmt(double age, double mai);
    double gRemBmSdNat(double age, double mai, double sd);
    double gRemBmSdTab(double age, double mai, double sd);
    //Get Mean annual increment with specific rotation time
    double gMai(double u, double mai);
    double gMait(double u, double mai);
    double gMaiSdNat(double u, double mai, double sd);
    double gMaiSdTab(double u, double mai, double sd);
    //Get optimal rotation time
    //0 .. Highest average increment
    //1 . .Maximum average Biomass
    //2 .. Highest possible age
    //3 .. Maximum harvest at final cut
    //4 .. Average Maximum harvest at final cut
    double gTopt(double mai, int type);
    double gToptt(double mai, int type);
    double gToptSdNat(double mai, double sd, int type);
    double gToptSdTab(double mai, double sd, int type);
    //Get rotation time out of mean bm
    double gU(double bm, double mai);
    double gUt(double bm, double mai);
    double gUSdNat(double bm, double mai, double sd);
    double gUSdTab(double bm, double mai, double sd);
    //Get stocking degree with age, mai, biomass
    double gSdNat(double age, double mai, double bm);
    double gSdTab(double age, double mai, double bm);
    //Get average stocking degree with rotation time, mai, average biomass
    double gAvgSdNat(double u, double mai, double avgBm);
    double gAvgSdTab(double u, double mai, double avgBm);
    double gtimeframe();  //Get time step width
    double gTmax();  //Get the oldest age in the table
  private:
    //Optimal rotation time
    struct optRotTimes {
      int maxInc;       //Highest average increment
      int maxBm;        //Maximum avarage Biomass
      int maxHarv;      //Maximum harvest at final cut
      int maxAvgHarv;   //Average Maximum harvest at final cut
      int maxAge;       //Highest possible age
    };
    //Interpolate rotation time between mai
    double ip(double mai, optRotTimes const *tab, int const &type);
    //Interpolate rotation time between mai and stocking degree (sdNat: True..sdNat, false..sdTab)
    double ip(double mai, double sd, optRotTimes const *tab, int const &type, bool const sdNat);
    //Interpolate between age and mai
    double ip(double u, double mai, double const *tab);
//Interpolate between stocking degree, age and mai (sdNat: True..sdNat, false..sdTab)
    double ip(double u, double mai, double sd, double const *tab, bool const sdNat);
    int fillTables(); //Function to fill up the tables using incrementCurves ic
    incrementCurves ic;  //Class of the increment curves
    double maiHi;   //Highest MAI until table should be created
    double maiStep; //Step width in MAI
    double tHi;     //Highest age until table should be created
    double tStep;   //Step width in time
    double sdNatStep;  //Step width in Natural stocking degree
    double sdTabHi;  //Highest Yield Table stocking degree
    double sdTabStep;  //Step width in Yield Table stocking degree
 //Time steps for which the increment should be calculated (<=0 .. use tStep)
    double timeframe;
    int nt;        //Number of time steps
    int nmai;      //Number of mai steps
    int nsdNat;    //Number of natural stand density steps
    int nsdTab;    //Number of yield table stand density steps
    double *abm;   //average above ground biomass without thinning
    double *abmt;  //average above ground biomass with thinning
    double *abmSdTab; //average above ground biomass at specific stocking degree
    double *bm;    //above ground biomass without thinning
    double *bmt;   //above ground biomass with thinning
    double *dbh;   //Diameter at maximum stand density
    double *dbht;  //Diameter at managed stand density
    double *dbhSdTab; //BHD at different yield table stocking degrees
    double *gwl;   //Total increment without thinning
    double *gwlt;  //Total increment with thinning
    double *gwlSdNat; //Total increment at specific Natural stocking degree
    double *gwlSdTab; //Total increment at specific yield table stocking degree
    double *height;//Height
    double *sdNat; //Natural stocking degree at increment optimal stand density
    double *incGwl;//Total increment change without thinning
    double *incGwlt;//Total increment change with thinning
    double *incBm; //Biomass change without thinning
    double *incBmt;//Biomass change with thinning
    double *incDbh;//BHD-Increment without thinning
    double *incDbht;//BHD-Increment with thinning
    double *incHeight;//Height incremnet
    double *incGwlSdNat; //Total increment change at specific Natural stocking degree
    double *incDbhSdNat;//BHD-Increment at specific Natural stocking degree
    double *incGwlSdTab; //Total increment change at specific yield table stocking degree
    double *incDbhSdTab;//BHD-Increment at specific yield table stocking degree
    //Optimal rotation time array
    optRotTimes *optTime;
    optRotTimes *optTimet;
    optRotTimes *optTimeSdNat;
    optRotTimes *optTimeSdTab;
  };

}

#endif

