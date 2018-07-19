#ifndef G4M_AGESTRUCT_H
#define G4M_AGESTRUCT_H

#include <vector>
#include <cmath>
#include "increment.h"
#include "misc.h"

namespace g4m {
  
  class ageStruct {
    friend class ageLUT;
  public:
    //stockingDegree Ertragstafel if < 0 .. no thinning -> max Stocking degree
    //sws ... sawnwood share (depending on d) bezogen auf Vorratsfestmeter
    //hlv ... 1 - harvesting losses Vornutzung (depending on d) von Vorratsfestmetern
    //hle ... 1 - harvesting losses Endnutzung (depending on d) von Vorratsfestmetern
    //dbv .. Deckungsbeitrag (depending on d) Vornutzung (auf VFM bezogen)
    //dbe .. Deckungsbeitrag (depending on d) Endnutzung (auf VFM bezogen)
 //minDbv .. Minimaler Deckungsbeitrag/Jahr/Ha damit Durchforstung gemacht wird
 //minDbe .. Minimaler Deckungsbeitrag/Jahr/Ha damit Endnutzung gemacht wird
    //Minimal rotation time share of the target rotation time
    ageStruct(incrementTab *it
	      , ipol<double,double> *sws
	      , ipol<double,double> *hlv, ipol<double,double> *hle
	      , ipol<double,double> *dbv, ipol<double,double> *dbe
	      , double minDbVn
	      , double minDbEn, double mai, double maiLongTime
	      , int rotationPeriod
	      , double stockingDegree=1., double area=1., double minRot=0.75
	      , bool changingNpp = false);
    class v {
    public:
      double area;
      double enSw;
      double enRw;
      double vnSw;
      double vnRw;
      double dbEn;
      double dbVn;
      v();
      ~v();
    };
    ~ageStruct();
    double setArea(int age, double area);
    double setBm(int age, double area);  //Biomass
    double getArea(int age);  //Area
    double getArea();
    double getBm(int age);    //Biomass
    double getBm();
    double getD(int age);     //Diameter
    double createNormalForest(int rotationPeriod, double area, double sd=1.);
    v aging();
    double setMai(double mai);
    double setMaiLongTime(double maiLongTime);
    double setRotPeriod(int rotPeriod);
    double setStockingdegree(double sd);
    double afforest(double area);
    //type: 0..Take from all age classes, 1..Take from the eldest age classes
    //area is positive number, return the deforested biomass
    v deforest(double area, int type=0);
  private:
    int setAgeClasses(int ageClasses);
    incrementTab *it;
    struct cohort {
      double area;
      double bm;  //Biomass
      double d;
      double h;
    };
    cohort *dat;
    int ageClasses;  //Rotation time Unmanaged forests (depend on NPP)
    int targetRotationPeriod;
    double tragetHarvestArea;
    double targetSd;  //if targetSd < 0. ... no thinning
    double mai;
    double maiLongTime;
    double updateHarvestArea();
    v finalCut(double area, bool eco=true);
    ipol<double,double> *sws;
    ipol<double,double> *hlv;
    ipol<double,double> *hle;
    ipol<double,double> *dbv;
    ipol<double,double> *dbe;
    double winVn;
    double winEn;
    v divArea(v& x, double area);
    double minDbVn;
    double minDbEn;
    double minRot;
    int activeAge; //Currently used AgeClsses
    int fitActiveAge(int type=0); //0..look around, 1..Full check
    bool changingNpp;
  };
  
  class ageLUT { //Lookup table for the age struct if we have 1 ha normal forest
  public:
    ageLUT(ageStruct *as, double acPriceIncentive = 0.);
    ~ageLUT();
    double getBm(double mai, double t);
    double getEnSw(double mai, double t);
    double getEnRw(double mai, double t);
    double getVnSw(double mai, double t);
    double getVnRw(double mai, double t);
    double getDbEn(double mai, double t);
    double getDbVn(double mai, double t);
    double getBmNT(double mai, double t); //Without thinning
    double getEnSwNT(double mai, double t);
    double getEnRwNT(double mai, double t);
    double getDbEnNT(double mai, double t);
    //Get optimal rotation time
    //1 .. Highest average harvest with thinning
    //2 .. Maximum avarage Biomass
    //3 .. Maximum average Biomass with thinning
    //4 .. Maximum harvest at final cut
    //5 .. Maximum average harvest with final cut
    //6 .. Maximize Deckungsbeitrag
    //7 .. Maximize Deckungsbeitrag with thinning
    double gTopt(double mai, int type);
  private:
    ageStruct *as;
    double cPriceIncentive;
    unsigned int maiHi;  //Highest Mai index 
    unsigned int tHi;    //Highest Age index
    double maiStep;      //Mai steps size
    double tStep;        //Age step size
    unsigned int fidx[2]; //fast interpol inde
    g4m::fipol<double>* tabOptRot;  //optimal rotation times
    g4m::fipol<double>* tabBm;  //Biomasstable  with thinning
    g4m::fipol<double>* tabEnSw; //Endnutzung sawnwood
    g4m::fipol<double>* tabEnRw; //Endnutzung restwood
    g4m::fipol<double>* tabVnSw; //Vornutzung sawnwood
    g4m::fipol<double>* tabVnRw; //Vornutzung restwood
    g4m::fipol<double>* tabDbEn; //Deckungsbeitrag Endnutzung ohne C-Incentive
    g4m::fipol<double>* tabDbVn; //Deckungsbeitrag Vornutzung ohne C-Incentive
    g4m::fipol<double>* tabBmNT;  //The same but now without thinning
    g4m::fipol<double>* tabEnSwNT;
    g4m::fipol<double>* tabEnRwNT;
    g4m::fipol<double>* tabDbEnNT;
  };

  //Something like Costs/Income if Rotation time is changed if there is a C-Tax
}
#endif
