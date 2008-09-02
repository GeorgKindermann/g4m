#ifndef G4M_INCREMENT_H
#define G4M_INCREMENT_H

#include <map>
#include <set>
#include <vector>
#include <cmath>

namespace g4m {
  class incrementCurves {
  public:
    incrementCurves(double a0, double a1, double a2, double a3, double a4
		    , double a5, double a6, double a7, double a8, double a9
		    , double a10, double a11, double a12, double a13, double a14
		    , double a15, double a16, double a17, double a18, double a19
		    , double a20, double a21, double a22, double a23, double a24
		    , double a25, double a26, double amai);
    int setCoef(double a0, double a1, double a2, double a3, double a4
		, double a5, double a6, double a7, double a8, double a9
		, double a10, double a11, double a12, double a13, double a14
		, double a15, double a16, double a17, double a18, double a19
		, double a20, double a21, double a22, double a23, double a24
		, double a25, double a26);
    int setMai(double amai);
    double gTOpt();   //increment optimal rotation time
    double gTcp(double t);  //Total carbon production
    double gMaxDens(double t);  //Maximum density
    double gManDens(double t);  //Managed density
    double gH(double t);  //Tree height
    double gD(double t);  //Tree diameter at increment optimal stand density
    double gDmul(double sd);  //diameter multiplyer for stocking degree v/vMax
    double gImul(double sd);  //increment multiplyer for stocking degree v/vOpt
    double tcpMax;
  private:
    double c[27]; //coefficients
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
    incrementTab(double a0, double a1, double a2, double a3, double a4,
		 double a5, double a6, double a7, double a8, double a9,
		 double a10, double a11, double a12, double a13, double a14,
		 double a15, double a16, double a17, double a18, double a19,
		 double a20, double a21, double a22, double a23, double a24,
		 double a25, double a26, double amaiMax, double amaiStep,
		 double atMax, double atStep, double atimeframe=1.);
    int setCoef(double a0, double a1, double a2, double a3, double a4,
		double a5, double a6, double a7, double a8, double a9,
		double a10, double a11, double a12, double a13, double a14,
		double a15, double a16, double a17, double a18, double a19,
		double a20, double a21, double a22, double a23, double a24,
		double a25, double a26);
    //Average biomass, u .. rotation time, mai .. Site index
    double gAvgBm(double u, double mai);
    double gAvgBmt(double u, double mai); //with thining
    double gHbm(double t, double mai); //Harvestable Biomass at t
    double gHbmt(double t, double mai); //Harvestable Biomass at t with thinning
    double gGwl(double t, double mai);  //Gesamtwuchsleistung
    double gMai(double t, double mai); //Mean annual increment
    //BHD bei Ertragstafelbestockung
    double gD(double t, double mai, double sd=1);
    //BHD increment mit Ertragstafelbestockungsgrad
    double gDi(double t, double mai, double sd=1);
    double gH(double t, double mai); //Height
    double gHi(double t, double mai); //Height increment
    //Get optimal rotation time
    //0 .. Rotation time Unmanaged forests
    //1 .. Highest average increment
    //2 .. Maximum avarage Biomass
    //3 .. Maximum average Biomass with thinning
    //4 .. Maximum harvest at final cut
    //5 .. Maximum average harvest with final cut
    double gTopt(double mai, int type);
    double gU(double bm, double mai, double sd=1.);  //Get u out of mean bm
    double gUt(double bm, double mai, double sd=1.);
    //Stocking degree: typ1..Max, 2..with Thinning
    double gSd(double age, double mai, double bm, int type=1);
    double gLfz(double age, double mai, double sd=1.); //etragstafelbestockung
    //pair:(dbm, mort),  sd .. Natuerlicher Bestockungsgrad der angestrebt wird
    std::pair<double, double> gDbm(double age, double mai, double bm, double sd=1.);
    //pair:(dbm, thin),  sd .. Ertragstafel Bestockungsgrad der angestrebt wird
    std::pair<double, double> gDbmt(double age, double mai, double bm, double sd=1.);
    double gSdopt2Sdnat(double u, double mai, double sdOpt); //returns sdNat
    double gMaiHi() {return(maiHi);}
    double gMaiStep() {return(maiStep);}
    double gTHi() {return(tHi);}
    double gTStep() {return(tStep);}
  private:
    double ip(double u, double mai, double *tab);
    incrementCurves ic;
    double maiHi;
    double maiStep;
    double tHi;
    double tStep;
    int fillTables();
    double *abm;      //average above ground biomass without thinning
    double *abmt;     //average above ground biomass with thinning
    double *bm;       //above ground biomass without thinning
    double *bmt;      //above ground biomass with thinning
    double *gwl;      //Gesamtwuchsleistung
    double *dbh;      //Diameter
    double *height;   //Height
    double *sdNat; //Natuerlicher Bestockungsgrad wenn optimale Bestandesdichte
    int *optTime;  //Optimal rotation time array
    int nt;    //Number of time steps
    int nmai;  //Number of mai steps
    double timeframe; //Time steps for which the increment should be calculated
  };
  
}

#endif

