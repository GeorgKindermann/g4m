#ifndef G4M_HARVEST_H
#define G4M_HARVEST_H

#include <cmath>
#include <vector>

namespace g4m {

  class harvest {
  public:
    harvest();
    //time felling (and processing)
    double tfp(int funcNr, double vs, double slope=0, double cr=0.5, double dist=300);
    //time transportation
    double tt(int funcNr, double vs, double vl, double slope=0, double dist=300);
    double tp(int funcNr, double vs, double cr=0.5); //time processing
  private:
    struct harvSystem {
      double (*timeFelling)(double vs, double slope, double cr, double dist);
      double (*timeTransport)(double vs, double vl, double slope, double dist);
      double (*timeProcessing)(double vs, double cr);
      double slpMin, slpMax; //Slope range
      double dbhMin, dbhMax; //Dbh range
      double fuelConsumption[3]; //[0]..felling, [1]..transport, [2]..processing
      double utilisationRate[3];
      double labourHours[3];
      double capitalCost[3];
      double operatingCost[3];
    };
    std::vector<double (*)(double,double,double,double)> ptfp;
    std::vector<double (*)(double,double,double,double)> ptt;
    std::vector<double (*)(double,double)> ptp;
  };

}

#endif
