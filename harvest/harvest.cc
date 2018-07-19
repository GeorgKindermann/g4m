#include "harvest.h"

namespace {
  //vs..mean stem size with bark without top [m3]
  //vl..load size [m3]
  //slope..mean slope [%]
  //dist..mean distance for one-way trip [m]
  //cr..tree crown to height ratio
  
  //time felling and processing motor manual final harvest
  double tFPMmF(double vs, double slope=0, double cr=0.5, double dist=300) {
    double vs07=std::pow(vs,0.7);
    return((1.+slope/100.)*(1.35/vs+8.933*vs07+0.1087*vs07*cr));
  }

  //time felling and processing motor manual thinning
  double tFPMmT(double vs, double slope=0, double cr=0.5, double dist=300) {
    double vs07=std::pow(vs,0.7);
    return(1.5*(1.+slope/100.)*(1.35/vs+8.933*vs07+0.1087*vs07*cr));
  }

  //time felling motor manual final harvest
  double tFMmF(double vs, double slope=0, double cr=0.5, double dist=300) {
    return((1.+slope/120.)*(1.35/vs+8.933*std::pow(vs,0.7)));
  }

  //time felling motor manual thinning
  double tFMmT(double vs, double slope=0, double cr=0.5, double dist=300) {
    return(1.5*(1.+slope/120.)*(1.35/vs+8.933*std::pow(vs,0.7)));
  }

  //time felling and processing Harvester final harvest
  double tFPHaF(double vs, double slope=0, double cr=0.5, double dist=300) {
    return((1.+slope/85.)*(60./(4.067+78.623*vs-18.507*vs*vs)));
  }

  //time felling and processing Harvester thinning
  double tFPHaT(double vs, double slope=0, double cr=0.5, double dist=300) {
    return((1.+slope/85.)*(60./(0.383+135.896*vs-180.065*vs*vs)));
  }

  //time felling Feller-buncher final harvest
  double tFFbF(double vs, double slope=0, double cr=0.5, double dist=300) {
    return(0.6*(1.+slope/85.)*60./((262.+171.+35.315*dist+14.4*3.*35.315*dist)/35.315));
  }

  //time felling Feller-buncher thinning
  double tFFbT(double vs, double slope=0, double cr=0.5, double dist=300) {
    return((1.+slope/85.)*60./((262.+171.+35.315*dist+14.4*3.*35.315*dist)/35.315));
  }

  //time Transportation Animal final harvest
  double tTAnF(double vs, double vl, double slope=0, double dist=300) {
    return((1.+slope/90.)*60./((-0.878*std::log(dist)+7.1853)/1.3));
  }

  //time Transportation Animal thinning
  double tTAnT(double vs, double vl, double slope=0, double dist=300) {
    return(1.5*(1.+slope/90.)*60./((-0.878*std::log(dist)+7.1853)/1.3));
  }

  //time Transportation Forwarder final harvest
  double tTFwF(double vs, double vl=16., double slope=0, double dist=300) {
    return((1.+slope/50.)*1.3*(0.511*std::pow(vs,0.7)+(0.23*dist+0.24+vl)/vl+0.8145));
  }

  //time Transportation Forwarder thinning
  double tTFwT(double vs, double vl=12., double slope=0, double dist=300) {
    return(1.5*(1.+slope/50.)*1.3*(0.511*std::pow(vs,0.7)+(0.23*dist+0.24+vl)/vl+0.8145));
  }

  //time Transportation Skidder logs final harvest
  double tTSkLF(double vs, double vl, double slope=0, double dist=300) {
    return(1.3*(1.+slope/75.)*0.99*(19.04929-20.11795/(1.+std::pow(vs,0.85735)/0.704)));
  }

  //time Transportation Skidder logs thinning
  double tTSkLT(double vs, double vl, double slope=0, double dist=300) {
    return(1.5*1.3*(1.+slope/75.)*0.99*(19.04929-20.11795/(1.+std::pow(vs,0.85735)/0.704)));
  }

  //time Transportation Skidder whole tree final harvest
  double tTSkTF(double vs, double vl, double slope=0, double dist=300) {
    return((1.+slope/75.)*0.99*(19.04929-20.11795/(1.+std::pow(vs,0.85735)/0.704)));
  }

  //time Transportation Skidder whole tree thinning
  double tTSkTT(double vs, double vl, double slope=0, double dist=300) {
    return(1.5*(1.+slope/75.)*0.99*(19.04929-20.11795/(1.+std::pow(vs,0.85735)/0.704)));
  }

  //time Transportation Cable yarder logs final harvest
  double tTCyLF(double vs, double vl, double slope=0, double dist=300) {
    return(1.25*(-0.7548+0.0122*dist+0.7782*std::pow(vs,-1.05)+0.0377*slope));
  }

  //time Transportation Cable yarder logs thinning
  double tTCyLT(double vs, double vl, double slope=0, double dist=300) {
    return(1.5*1.25*(-0.7548+0.0122*dist+0.7782*std::pow(vs,-1.05)+0.0377*slope));
  }

  //time Transportation Cable yarder whole tree final harvest
  double tTCyTF(double vs, double vl, double slope=0, double dist=300) {
    return(0.75*1.25*(-0.7548+0.0122*dist+0.7782*std::pow(vs,-1.05)+0.0377*slope));
  }

  //time Transportation Cable yarder whole tree thinning
  double tTCyTT(double vs, double vl, double slope=0, double dist=300) {
    return(0.75*1.5*1.25*(-0.7548+0.0122*dist+0.7782*std::pow(vs,-1.05)+0.0377*slope));
  }

  //time processing road side processing machines
  double tPPm(double vs, double cr=0.5) {
    return(1.4*cr/std::pow(vs,0.4));
  }

}

namespace g4m {

  harvest::harvest() {
    ptfp.push_back(tFPMmF);
    ptfp.push_back(tFPMmT);
    ptfp.push_back(tFMmF);
    ptfp.push_back(tFMmT);
    ptfp.push_back(tFPHaF);
    ptfp.push_back(tFPHaT);
    ptfp.push_back(tFFbF);
    ptfp.push_back(tFFbT);

    ptt.push_back(tTAnF);
    ptt.push_back(tTAnT);
    ptt.push_back(tTFwF);
    ptt.push_back(tTFwT);
    ptt.push_back(tTSkLF);
    ptt.push_back(tTSkLT);
    ptt.push_back(tTSkTF);
    ptt.push_back(tTSkTT);
    ptt.push_back(tTCyLF);
    ptt.push_back(tTCyLT);
    ptt.push_back(tTCyTF);
    ptt.push_back(tTCyTT);

    ptp.push_back(tPPm);
  }

  double harvest::tfp(int funcNr, double vs, double slope, double cr, double dist) {
    return(ptfp[funcNr](vs,slope,cr,dist));
  }

  double harvest::tt(int funcNr, double vs, double vl, double slope, double dist) {
    return(ptt[funcNr](vs,vl,slope,dist));
  }

  double harvest::tp(int funcNr, double vs, double cr) {
    return(ptp[funcNr](vs,cr));
  }

}
