#ifndef G4M_AGESTRUCT_H
#define G4M_AGESTRUCT_H

#include <vector>
#include <cmath>
//#include "increment.h" //MG
#include "increment.cpp"
#include "misc.h"
#include <limits>
#include <deque>
#include <utility>

//objOfProd MISSING: Keep same biomass

namespace g4m {

  class ageStruct {
  public:
    ageStruct
      (incrementTab *it    //Increment table which will be used, the time step width (simulation period legth) of *it will also be used in ageStruct
       , ffipol<double> *sws //Sawnwood share of harvested wood depending on dbh
       , ffipol<double> *hlv //1-harvesting losses thinning (Vornutzung) (depending on d) in relation to standing timber (Vorratsfestmeter)
       , ffipol<double> *hle //1-harvesting losses final felling (Endnutzung) (depending on d) in relation to standing timber (Vorratsfestmeter)
       , ffipolm<double> *cov //Thinning costs depending on d and removed volume per hectare) in relation to standing timber (Vorratsfestmeter)
       , ffipolm<double> *coe //Harvesting costs depending on d and vol
       , ffipolm<bool> *dov //Do thinning (depending on d and removed volume per hectare) in relation to standing timber (Vorratsfestmeter)
       , ffipolm<bool> *doe //Do final felling (depending on d and stocking volume per hectare)
       , double mai  //mean annual increment in tC stemmwood per hectar and year at increment optimal rotation time
       , int objOfProd=3 //objective of production: 0..Rotation time in years, 1..ammount of wood which need to beharvested every year, 2..like 1 but the ammount will not be fulfilled if rotation time will be shorter than minRot; >2 ignore Value of u and claculate it instead  3 .. Highest average increment, 4 . .Maximum average Biomass, 5 .. Highest possible age, 6 .. Maximum harvest at final cut, 7 .. Average Maximum harvest at final cut
       , double u=0.  //Rotation time if objOfProd 0
       , double minSw=0. //if objOfProd 1,2 ammount of sawnwood to harvest
       , double minRw=0. //if objOfProd 1,2 ammount of restwood to harvest
       , double minHarv=0. //if objOfProd 1,2 ammount of total harvest
       //Usage of stocking degree: 0..Keep all the time sdMax, 1..alternate between sdMin and sdMax, 3..alternate between sdMinH and sdMaxH
       , int sdDef=0
       //Stocking degree: if sd exceeds sdMax do thinning until sdMin. sd > 0 stockingDegree yield table, sd -1 to 0 natural stocking degree
       //Maybe sdMin and sdMax can be made dependent on h/hmax and MAI
       , double sdMax=1.
       , double sdMin=1.
       , unsigned int maiYears=30  //Years to calculate average mai
       //Minimal rotation time in years or as share given in minRotRef which needs to be exceedes until final harvests are done
       , double minRotVal=0.75
       //meaning of minRotVal value
       //0..use it as years, 1..minRotVal*u (u>0), 2..*uMaxAvgIncrement, 3..*uMaxAvgBiomass, 4..*uMaxAge, 5..*uMaxHarvest, 6..*uAvgMaxHarvest
       , int minRotRef=2
       //how fast should the stoking degree target be reached
   //0..do only remove caused by stand density  to  1..do only typical removes
       , double flexSd=0.
       , ffipol<double> *sdMaxH=NULL//Stockindegree depending on max tree height
       , ffipol<double> *sdMinH=NULL//Stockindegree depending on max tree height
       , unsigned int maxAge=300
       );
    ~ageStruct();
    std::deque<double> qMai;  //Queue to store the mai's of previous years (youngest mai is at the end of que)
    double createNormalForest(double rotationPeriod, double area, double sd=1.);
    double getBm(double age);    //get biomass per ha by age
    double getBm();              //get average biomass per ha
    double getArea(int age);//get forest area by ageclass
    double getArea(double age);//get forest area by age (consider age class size
    double getArea();            //get forest area
    double getD(double age);     //get Diameter
    double getH(double age);     //get Height
    //Set area for a specific ageCLASS
    double setArea(unsigned int ageClass, double area);
    //Set biomass per hectare for a specific ageCLASS
    double setBm(unsigned int ageClass, double biomass);
    //Set bhd for a specific ageCLASS
    double setD(unsigned int ageClass, double dbh);

    double setMai(double mai); //Just set mai but don't influence avgMai
    unsigned int setMaiYears(unsigned int maiYears);
    double setAvgMai(double avgMai); //Set avgMai and all values in qMai
    double getMai();
    double getAvgMai();

    int setObjOfProd(int objOfProd);
    double setU(double u);

    double setStockingdegreeMin(double sd);
    double setStockingdegreeMax(double sd);

    int setMinRotRef(int minRotRef);
    double setMinRotVal(double minRotVal);

    double setFlexSd(double flexSd);

    double afforest(double area); //Insert in youngest age class
    double reforest(double area); //Make afforestations in age class 0 and 1

    struct v {
      double area;//Area where management was done
      double sw;  //Sawnwood [tC/Ha]
      double rw;  //Restwood [tC/Ha]
      double co;  //Costs [Costs/Ha]
      double bm;  //Total cut biomass including harvesting losses [tC/Ha]
      //For final harvest the values are per hectare for thinning they are total
    };

    //type: 0..Take from all age classes, 1..Take from the eldest age classes
    //area is positive number, return the deforested biomass
    v deforest(double area, int type=0);

    std::pair<v, v>  aging(); //pair.first = thinn, pair.second = harvest
    std::pair<v, v> aging(double mai);

    double u;       //Rotation time
    unsigned int getMaxAge();
    unsigned int setMaxAge(unsigned int maxAge);
    int getActiveAge(); //MG: added : Find "active Age" - the oldest cohort with area>0    
    private:
    incrementTab *it;
    ffipol<double> *sws;
    ffipol<double> *hlv;
    ffipol<double> *hle;
    ffipolm<double> *cov;
    ffipolm<double> *coe;
    ffipolm<bool> *dov;
    ffipolm<bool> *doe;
    double mai;
    double avgMai;
    int objOfProd;  //Objective of production
    double minSw; //Min sawnwood to harvest
    double minRw; //Min restwood to harvest
    double minHarv; //Minimum total harvest
    double setRotationTime();
    double uRef;
    int sdDef;
    double sdMin, sdMax;   //Target stocking degree (>0..Table, <0..natural)
    double area;  //Total forest area (sum of dat.area)
    double minRot;  //minimal age when final harvest will be done
    double minRotVal;
    int minRotRef;
    double calcAvgMai();  //Calculate average mai with vector qMai
    double setMinRot();   //Set the minimal rotation time "minRot"
    double timeStep; //How long is one time step
    double flexSd;
    struct cohort {
      double area; //Forest area
      double bm;   //Biomass
      double d;
      double h;
    };
    std::vector<cohort> dat;
    unsigned int initCohort(unsigned int ageClassL, unsigned int ageClassH);
    double calcArea();     //Calculates the forest area with dat[].area
    v finalCut(double area, bool eco=true); //Fulfill area
    //Fulfill ammount
    v finalCut(double minSw, double minRw, double minHarv, bool eco=true, bool sustainable=true);
    v finalCut(bool eco, double area, double minSw, double minRw, double minHarv, bool sustainable=true);
    v thinAndGrow();
    v thinAndGrowStatic();
    v thinAndGrowOLD(); //Needs to be removed after new restructuring
    int incStatic(int i, double &sd, double &iGwl, double &Bm, double &id);
    double incCommon(int i, const double &sd, const double &iGwl);
    int cohortShift();
    unsigned int maxNumberOfAgeClasses;
  };

}

#endif
