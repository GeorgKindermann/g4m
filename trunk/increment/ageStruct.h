#ifndef G4M_AGESTRUCT_H
#define G4M_AGESTRUCT_H

#include <cmath>
#include "increment.h"
#include "misc.h"

namespace g4m {

  class ageStruct {
  public:
    //stockingDegree < 0 .. Unmanaged Forest
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
	      , double minDbEn, double mai, int rotationPeriod
	      , double stockingDegree=1., double area=1., double minRot=0.75);
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
    double getArea(int age);
    double getArea();
    double getBm(int age);
    double getBm();
    double getD(int age);
    double createNormalForest(int rotationPeriod, double area, double sd=1.);
    v aging();
    double setMai(double mai);
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
  };

}
#endif
