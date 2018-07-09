#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <iterator>

using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class mai {
public:
  mai(const std::vector<double>& c     //Coefficients
      , const std::vector<double>& c0  //c0 Coefficients
      , const double& whc         //Water holding capacity 
      , const double &swr               //Soil Water Regime
      , const double& co2         //Co2 concentration
      , const int& soilType       //Soil type
      , const std::vector<double>& t   //Temperature of the month
      , const std::vector<double>& p   //Precipitation
      , const double& altitude         //altitude
      , const double& tMinJ
      , const double& tMaxJ
      , const double& pMinJ
      , const double& pMaxJ
      , const double& tMinM
      , const double& tMaxM
      , const double& pMinM
      , const double& pMaxM
      , const double& minNpp
      , bool useBoundaries
      , bool weatherIsDynamic
      , double soilWaterDecayRate
      , double latitude
      );
  double getNpp();
  int setCoef(const std::vector<double>& c);
  int setCoefC0(const std::vector<double>& c);
  int setTemperature(const std::vector<double>& t);
  int setPrecipitation(const std::vector<double>& p);
  int setCo2(const double& co2);
  int setWhc(const double& whc);
  int setSwr(const double& swr);
  int setAltitude(const double& altitude);
  int setSoilType(const int& soilType);
  int setUseBoundaries(bool useBoundaries);
  int setBoundaries(const double& tMinJ, const double& tMaxJ, const double& pMinJ, const double& pMaxJ, const double& tMinM, const double& tMaxM, const double& pMinM, const double& pMaxM, const double& minNpp);
  int setWeatherAsDynamic(bool weatherIsDynamic);
  int setSoilWaterDecayRate(double soilWaterDecayRate);
  int setLatitude(double latitude);
private:
  bool inputWasChanged;
  int updateSoilWater();
  void testBoundaries();
  void calcWalterLieth();
  double c[18]; //Coefficients
  double c0[23]; //c0 Coefficients
  double whc;   //Water holding capacity 
  double swr;   //Soild water regime (Grundwassereinfluss)
  double co2;   //Co2 concentration
  double altitude;         //altitude
  int soilType; //Soil type
  double t[12]; //Temperature for each month of this year
  double p[12]; //Precipitation for each month of this year
  double sw[12]; //Soil water content
  double tp[12]; //Temperature for each month of previous year
  double pp[12]; //Precipitation for each month of previous year
  double walterLieth; //Walter lieth coefficient
  double tMinJ;
  double tMaxJ;
  double pMinJ;
  double pMaxJ;
  double tMinM;
  double tMaxM;
  double pMinM;
  double pMaxM;
  double minNpp;
  bool useBoundaries;
  bool outOfBoundaries;
  bool weatherIsDynamic;
  double soilWaterDecayRate;
  double latitude;
};

mai::mai(const std::vector<double>& ac
      , const std::vector<double>& ac0
      , const double& awhc
      , const double &aswr
      , const double& aco2
      , const int& asoilType
      , const std::vector<double>& at
      , const std::vector<double>& ap
      , const double& aaltitude
      , const double& atMinJ
      , const double& atMaxJ
      , const double& apMinJ
      , const double& apMaxJ
      , const double& atMinM
      , const double& atMaxM
      , const double& apMinM
      , const double& apMaxM
      , const double& aminNpp
      , bool auseBoundaries
      , bool aweatherIsDynamic
      , double asoilWaterDecayRate
      , double alatitude
	 ) {
  setCoef(ac);
  setCoefC0(ac0);
  whc = awhc;
  swr = aswr;
  co2 = aco2;
  soilType = asoilType;
  setTemperature(at);
  setPrecipitation(ap);
  altitude = aaltitude;
  setBoundaries(atMinJ, atMaxJ, apMinJ, apMaxJ, atMinM, atMaxM, apMinM, apMaxM, aminNpp);
  useBoundaries = auseBoundaries;
  weatherIsDynamic = aweatherIsDynamic;
  soilWaterDecayRate = asoilWaterDecayRate;
  inputWasChanged = true;
  latitude = alatitude;
}

void mai::calcWalterLieth() {
  //walterLieth = c[14] * (c[15] + 1./(1. + exp(c[16] + c[17]*co2)));
  walterLieth = c[14] / (c[15] + 1./(1. + exp(c[16] + c[17]*co2)));
}

void mai::testBoundaries() {
  outOfBoundaries = false;
  double avgT=0.;
  double avgTP=0.;
  double avgP=0.;
  double avgPP=0.;
  for(int i=0; i<12; ++i) {
    if(t[i] < tMinM || t[i] > tMaxM || p[i] < pMinM || p[i] > pMaxM) {outOfBoundaries = true; goto testBoundariesFinish;}
    if(weatherIsDynamic && (tp[i] < tMinM || tp[i] > tMaxM || pp[i] < pMinM || pp[i] > pMaxM)) {outOfBoundaries = true; goto testBoundariesFinish;}
    avgT += t[i]; avgTP += tp[i]; avgP += p[i]; avgPP += pp[i];
  }
  avgT /= 12.; avgTP /= 12.;
  if(avgT < tMinJ || avgT > tMaxJ || avgP < pMinJ || avgP > pMaxJ) {outOfBoundaries = true; goto testBoundariesFinish;}
  if(weatherIsDynamic && (avgT < tMinJ || avgT > tMaxJ || avgP < pMinJ || avgP > pMaxJ)) {outOfBoundaries = true; goto testBoundariesFinish;}
 testBoundariesFinish:
  ;
}

double mai::getNpp() {
  double ret=0.;
  if(inputWasChanged) {
    calcWalterLieth();
    updateSoilWater();
    if(useBoundaries) {testBoundaries();}
    inputWasChanged = false;
  }
  if(useBoundaries && outOfBoundaries) {
    ret = 0.;
  } else {
    double t3 = c[6] / (1. + exp(c[7] + c[8]*co2)) + c[9];
    double days[12] = {31.,28.25,31.,30.,31.,30.,31.,31.,30.,31.,30.,31.};
    for(int month=0; month<12; ++month) {
      double t1 = c0[soilType]/(1. + std::exp(c[0] + c[1]*t[month])) - c0[soilType]*c[2] - c0[soilType]/(1. + std::exp(c[3]+c[4]*t[month]));
      if(t1<0.) {t1=0.;}
      double t2 = 1. - 2./(1. + exp( (sw[month]+p[month]-std::max(0.,t[month])*walterLieth)/(std::max(1.,t[month]*c[5]))));
      if(t2<0.) {t2=0.;}
      ret += t1 * t2 * t3 * days[month];
    }
    ret *= c[10] + c[11]*altitude + c[12]*cos(latitude) + c[13]*altitude*cos(latitude);
    if(useBoundaries && ret < minNpp) {ret=0.;}
  }
  if(ret<0.) {ret = 0.;}
  return(ret);
}

int mai::setCoef(const std::vector<double>& ac) {
  for(int i=0; i<18; ++i) {c[i] = ac[i];}
  inputWasChanged = true;
  return(0);
}

int mai::setCoefC0(const std::vector<double>& ac) {
  for(int i=0; i<23; ++i) {c0[i] = ac[i];}
  inputWasChanged = true;
  return(0);
}

int mai::setTemperature(const std::vector<double>& at) {
  inputWasChanged = true;
  for(int i=0; i<12; ++i) {
    tp[i] = t[i];
    t[i] = at[i];
  }
  return(0);
}

int mai::setPrecipitation(const std::vector<double>& ap) {
  inputWasChanged = true;
  for(int i=0; i<12; ++i) {
    pp[i] = p[i];
    p[i] = ap[i];
  }
  return(0);
}

int mai::setCo2(const double& aco2) {
  co2 = aco2;
  inputWasChanged = true;
  return(0);
}

int mai::setSwr(const double& aswr) {
  swr = aswr;
  inputWasChanged = true;
  return(0);
}

int mai::setWhc(const double& awhc) {
  whc = awhc;
  inputWasChanged = true;
  return(0);
}

int mai::setAltitude(const double& aaltitude) {
  altitude = aaltitude;
  inputWasChanged = true;
  return(0);
}

int mai::setSoilType(const int& asoilType) {
  soilType = asoilType;
  return(0);
}

int mai::setUseBoundaries(bool auseBoundaries) {
  useBoundaries = auseBoundaries;
  return(0);
}

int mai::setWeatherAsDynamic(bool aweatherIsDynamic) {
  weatherIsDynamic = aweatherIsDynamic;
  return(0);
}

int mai::setSoilWaterDecayRate(double asoilWaterDecayRate) {
  soilWaterDecayRate = asoilWaterDecayRate;
  return(0);
}

int mai::setLatitude(double alatitude) {
  latitude = alatitude;
  inputWasChanged = true;
  return(0);
}

int mai::setBoundaries(const double& atMinJ, const double& atMaxJ, const double& apMinJ, const double& apMaxJ, const double& atMinM, const double& atMaxM, const double& apMinM, const double& apMaxM, const double& aminNpp) {
  tMinJ = atMinJ;
  tMaxJ = atMaxJ;
  pMinJ = apMinJ;
  pMaxJ = apMaxJ;
  tMinM = atMinM;
  tMaxM = atMaxM;
  pMinM = apMinM;
  pMaxM = apMaxM;
  minNpp = aminNpp;
  testBoundaries();
  return(0);
}

int mai::updateSoilWater() {
  double tsw[15];
  for(int i=0; i<4; ++i) { //The previous Years
    double tmp;
    if(weatherIsDynamic) {
      tmp = tp[i+8];
      if(tmp < 0) {tmp=0;}
      tmp *= walterLieth;
      tmp = pp[i+8] - tmp;
    } else {
      tmp = t[i+8];
      if(tmp < 0) {tmp=0;}
      tmp *= walterLieth;
      tmp = p[i+8] - tmp;
    }
    tsw[i] = tmp;
  }
  for(int i=0; i<11; ++i) {
    double tmp = t[i];
    if(tmp < 0) {tmp=0;}
    tmp *= walterLieth;
    tmp = p[i] - tmp;
    tsw[i+4] = tmp;
  }
  for(int i=0; i<12; ++i) {
    double tmp = 0.;
    for(int j=0; j<4; ++j) {
      tmp += tsw[i+j];
      if(tmp < 0) {tmp=0;}
      tmp *= soilWaterDecayRate;
      if(tmp > whc) {tmp = whc;}
    }
    sw[i] = tmp + swr;
  }
  return(0);
}


int main() {
  std::vector<double> c = {1.29460152,-0.09012495,0.17332495, 35, -1, 0.66170523, 2.8, 0.2366, -29.5767, -2.4, 1.155907e+00, 2.154028e-04, -3.733458e-01, 2.335792e-05, 2, 0.5, 0.841, -22.131};
  std::vector<double> c0 = {0.06179233,0.06448844,0.07000044,0.07867775,0.06506758,0.08137664,0.06192571,0.07169721,0.07110523,0.06381677,0.05441309,0.06347873,0.07584091,0.07330926,0.05766713,0.07205265,0.05055277,0.06077571,0.07759581,0.05685617,0.06527024,0.05558023,0.06699292};
  double tMinJ=-0.01;
  double tMaxJ=11.4;
  double pMinJ=518;
  double pMaxJ=1262;
  double tMinM=-13.2;
  double tMaxM=19.9;
  double pMinM=24;
  double pMaxM=151;
  double minNpp=3.2;
  double soilWaterDecayRate = 0.8;

  bool useBoundaries = true;
  bool weatherIsDynamic = false;
  double whc=150.;
  double swr=0.;
  double co2=0.038;
  int soilType=3;
  std::vector<double> t = {-5.5, -5.1, -1., 4, 9.4, 14.1, 16.1, 15.1, 11., 6., 5., -3.5};
  std::vector<double> p = {52,43,45,47,57,71,76,75,66,63,64,60};
  double altitude=400.;
  double latitude=45.*M_PI/90.;

  mai *npp[25];

  //Forest
  c0 = {0.06179233,0.06448844,0.07000044,0.07867775,0.06506758,0.08137664,0.06192571,0.07169721,0.07110523,0.06381677,0.05441309,0.06347873,0.07584091,0.07330926,0.05766713,0.07205265,0.05055277,0.06077571,0.07759581,0.05685617,0.06527024,0.05558023,0.06699292};
  c = {1.29460152,-0.09012495,0.17332495, 35, -1, 0.66170523, 2.8, 0.4138984, -54.4741443, -1.4, 1.155907e+00, 2.154028e-04, -3.733458e-01, 2.335792e-05, 2, 0.5, 0.841, -22.131};
  tMinJ=-12.0 - 2;
  tMaxJ=28.0 + 2.;
  pMinJ=203.;
  pMaxJ=5900. + 4000.;
  tMinM=-43.1 - 4;
  tMaxM=32.5 + 5.;
  pMinM=0;
  pMaxM=1045 + 1100;
  minNpp=1.86;
  npp[0] = new mai(c, c0, whc, swr, co2, soilType, t, p, altitude, tMinJ, tMaxJ, pMinJ, pMaxJ, tMinM, tMaxM, pMinM, pMaxM, minNpp, useBoundaries, weatherIsDynamic, soilWaterDecayRate, latitude);

  //evergreen needleleaf forest
  c0 = {0.07200521,0.11960568,0.09408846,0.10690904,0.08503689,0.09664446,0.08917135,0.12555602,0.10778141,0.08308198,0.11212895,0.11297067,0.11626721,0.10273018,0.12039956,0.11695393,0.06977949,0.10038106,0.14010804,0.08497696,0.12136300,0.10823336,0.10055720};
  c = {1.473547e+00,-9.469511e-02,1.855580e-01,3.500000e+01,-1.000000e+00,1.018910e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,9.349661e-01,1.698924e-04,-4.095052e-02,-1.261350e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01};
  tMinJ=-10.1 - 4;
  tMaxJ=28.0 + 2.;
  pMinJ=176.;
  pMaxJ=4100. + 4000.;
  tMinM=-39.1 - 7;
  tMaxM=29.2 + 5.;
  pMinM=1;
  pMaxM=562 + 2000;
  minNpp=0.79;
  npp[1] = new mai(c, c0, whc, swr, co2, soilType, t, p, altitude, tMinJ, tMaxJ, pMinJ, pMaxJ, tMinM, tMaxM, pMinM, pMaxM, minNpp, useBoundaries, weatherIsDynamic, soilWaterDecayRate, latitude);

  //evergreen broadleaf forest
  c0 = {0.06278559,0.05451733,0.07155252,0.07154880,0.06331560,0.07405309,0.05890702,0.06341198,0.06168992,0.06258061,0.04919143,0.05810440,0.08030827,0.07632525,0.05871569,0.06240980,0.05162692,0.05707812,0.07945510,0.05807941,0.06134032,0.05378656,0.06523997};
  c = {1.529964e+00,-9.626790e-02,1.892103e-01,3.500000e+01,-1.000000e+00,1.065803e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,2.006863e+00,-2.252521e-05,-1.190847e+00,3.509442e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01};
  tMinJ=4.1 + 7;
  tMaxJ=28.0 + 2.5;
  pMinJ=480. - 100;
  pMaxJ=6900. + 4000.;
  tMinM=-3.6 + 3.;
  tMaxM=33.5 + 2.5;
  pMinM=0;
  pMaxM=1300 + 2000;
  minNpp=2.98;
  npp[2] = new mai(c, c0, whc, swr, co2, soilType, t, p, altitude, tMinJ, tMaxJ, pMinJ, pMaxJ, tMinM, tMaxM, pMinM, pMaxM, minNpp, useBoundaries, weatherIsDynamic, soilWaterDecayRate, latitude);

  //deciduous needleleaf forest
  c0 = {0.1226366,0.1394199,0.1373671,0.1393298,0.1215172,0.1293482,0.1245032,0.1393599,0.1337010,0.1171449,0.1229379,0.1227410,0.1484251,0.1228124,0.1469167,0.1251016,0.1210288,0.1285934,0.1523116,0.1243452,0.1535025,0.1284694,0.1011582};
  c = {1.575326e+00,-9.890705e-02,1.966154e-01,3.500000e+01,-1.000000e+00,1.361797e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,8.640303e-01,2.381869e-04,-5.982080e-02,-5.153793e-05,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01};
  tMinJ=-14.1 - 2.3;
  tMaxJ=18.6 + 2.5;
  pMinJ=200. - 0;
  pMaxJ=6900. + 4000.;
  tMinM=-45. - 3.;
  tMaxM=28. + 5.5;
  pMinM=2;
  pMaxM=1300 + 2000;
  minNpp=1.19;
  npp[3] = new mai(c, c0, whc, swr, co2, soilType, t, p, altitude, tMinJ, tMaxJ, pMinJ, pMaxJ, tMinM, tMaxM, pMinM, pMaxM, minNpp, useBoundaries, weatherIsDynamic, soilWaterDecayRate, latitude);

  //deciduous needleleaf forest
  c0 = {0.08172423,0.12588981,0.07748284,0.11167975,0.07765499,0.10551991,0.08170487,0.06883372,0.09889102,0.09630474,0.08749524,0.07827634,0.15207574,0.08388147,0.12978135,0.09060370,0.08727573,0.10390721,0.14516723,0.09127422,0.07513953,0.10896841,0.11420354};
  c = {1.494767e+00,-9.632663e-02,1.886172e-01,3.500000e+01,-1.000000e+00,3.584477e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,1.107488e+00,1.957089e-04,-5.395287e-01,1.181403e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01};
  tMinJ=-8.2 - 2.3;
  tMaxJ=28.3 + 1.;
  pMinJ=235. - 0;
  pMaxJ=6900. + 4000.;
  tMinM=-32.9 - 9.;
  tMaxM=28. + 5.5;
  pMinM=0;
  pMaxM=1300 + 2000;
  minNpp=0.66;
  npp[4] = new mai(c, c0, whc, swr, co2, soilType, t, p, altitude, tMinJ, tMaxJ, pMinJ, pMaxJ, tMinM, tMaxM, pMinM, pMaxM, minNpp, useBoundaries, weatherIsDynamic, soilWaterDecayRate, latitude);

  //Tropical evergreen broadleaf
  c0 = {0.06136063,0.05394037,0.06884074,0.06795948,0.06069805,0.07048979,0.05777229,0.06212813,0.05874720,0.06193419,0.04811436,0.05803750,0.07435434,0.07529665,0.05663168,0.06045755,0.04999882,0.05534675,0.07479968,0.05449085,0.05932120,0.05244837,0.06286373};
  c = {1.619920e+00,-9.848997e-02,1.970912e-01,3.500000e+01,-1.000000e+00,5.963311e-01,2.800000e+00,0.4138984,-54.4741443,-1.4,1.941317e+00,-9.634048e-05,-1.100476e+00,4.401370e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01};
  tMinJ=10.1 + 8.;
  tMaxJ=28.3 + 3.;
  pMinJ=600. - 0;
  pMaxJ=6900. + 4000.;
  tMinM=7.2 + 4.1;
  tMaxM=33.6 + 2.2;
  pMinM=0;
  pMaxM=1300 + 2000;
  minNpp=4.6;
  npp[5] = new mai(c, c0, whc, swr, co2, soilType, t, p, altitude, tMinJ, tMaxJ, pMinJ, pMaxJ, tMinM, tMaxM, pMinM, pMaxM, minNpp, useBoundaries, weatherIsDynamic, soilWaterDecayRate, latitude);

  //Tropical deciduous broadleaf
  c0 = {0.06836858,0.06813469,0.05934084,0.07794042,0.05072694,0.06129334,0.06565496,0.04773324,0.07122714,0.08419983,0.05885219,0.06289404,0.09646647,0.06666715,0.09231248,0.03816949,0.04651217,0.07239042,0.10244473,0.07365863,0.05660622,0.08033533,0.07028939};
  c = {1.491055e+00,-9.734815e-02,1.951930e-01,3.500000e+01,-1.000000e+00,2.144010e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,1.018970e+00,5.690775e-04,-2.743858e-01,-3.154196e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01};
  tMinJ=8.3 + 9.8 - 3;
  tMaxJ=28.5 + 1.;
  pMinJ=225. - 0;
  pMaxJ=6900. + 4000.;
  tMinM=1.9 + 9.4 - 3;
  tMaxM=35.2 + 1.;
  pMinM=0;
  pMaxM=1300 + 2000;
  minNpp=1.13;
  npp[6] = new mai(c, c0, whc, swr, co2, soilType, t, p, altitude, tMinJ, tMaxJ, pMinJ, pMaxJ, tMinM, tMaxM, pMinM, pMaxM, minNpp, useBoundaries, weatherIsDynamic, soilWaterDecayRate, latitude);

   //Subtropical evergreen broadleaf
  c0 = {0.11606259,0.09230848,0.11152179,0.07736606,0.10174771,0.10232312,0.09956373,0.09869678,0.12651927,0.11027624,0.09064032,0.14575220,0.09281512,0.09487696,0.13310920,0.08328069,0.10088982,0.12739049,0.14853313,0.12082426,0.10963328,0.16620110,0.10230552};
  c = {1.538287e+00,-9.722637e-02,1.894521e-01,3.500000e+01,-1.000000e+00,1.249044e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,1.656060e+00,2.328051e-04,-1.522926e+00,1.101988e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01};
  tMinJ=3.4 + 9;
  tMaxJ=23.7 - 5. + 2.5;
  pMinJ=290. - 0;
  pMaxJ=6900. + 4000.;
  tMinM=-5.8 + 7.;
  tMaxM=29.9 + 2.;
  pMinM=0;
  pMaxM=1300 + 2000;
  minNpp=0.85;
  npp[7] = new mai(c, c0, whc, swr, co2, soilType, t, p, altitude, tMinJ, tMaxJ, pMinJ, pMaxJ, tMinM, tMaxM, pMinM, pMaxM, minNpp, useBoundaries, weatherIsDynamic, soilWaterDecayRate, latitude);

  //Temperate evergreen needleleaf
  c0 = {0.1177171,0.1308500,0.1070914,0.1288103,0.1519428,0.1280058,0.1521247,0.1562641,0.1294695,0.1486603,0.1318565,0.1337459,0.1723263,0.1428401,0.1618081,0.1389939,0.1357501,0.1216093,0.1503008,0.1142445,0.1521230,0.1520602,0.1590204};
  c = {1.570108e+00,-9.829125e-02,1.945205e-01,3.500000e+01,-1.000000e+00,1.416748e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,9.220346e-01,1.322653e-04,1.418027e-01,-1.572138e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01};
  tMinJ=-4.9 + 5;
  tMaxJ=14.6 - 2. + 3;
  pMinJ=220. - 0;
  pMaxJ=6900. + 4000.;
  tMinM=-27.2 + 0.;
  tMaxM=25.1 + 4.5;
  pMinM=0;
  pMaxM=1300 + 2000;
  minNpp=1.18;
  npp[8] = new mai(c, c0, whc, swr, co2, soilType, t, p, altitude, tMinJ, tMaxJ, pMinJ, pMaxJ, tMinM, tMaxM, pMinM, pMaxM, minNpp, useBoundaries, weatherIsDynamic, soilWaterDecayRate, latitude);

  //Temperate deciduous broadleaf
  c0 = {0.12156712,0.12492062,0.13087684,0.11666581,0.11019982,0.12730928
,0.12320618,0.07018051,0.11312888,0.09925496,0.10062111,0.12766330
,0.15216701,0.11568652,0.13923021,0.09930708,0.14071709,0.11279223
,0.11843090,0.12642849,0.08836875,0.12722117,0.10910369};
  c = {1.574017e+00,-9.893621e-02,1.965476e-01,3.500000e+01,-1.000000e+00,1.949731e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,9.597373e-01,3.182178e-04,-5.993809e-01,-2.135231e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01};
  tMinJ=-4.9 + 5;
  tMaxJ=14.6 - 2. + 3;
  pMinJ=305. - 0;
  pMaxJ=6900. + 4000.;
  tMinM=-29.7 + 0.;
  tMaxM=27.1 + 2.5;
  pMinM=2;
  pMaxM=1300 + 2000;
  minNpp=1.74;
  npp[9] = new mai(c, c0, whc, swr, co2, soilType, t, p, altitude, tMinJ, tMaxJ, pMinJ, pMaxJ, tMinM, tMaxM, pMinM, pMaxM, minNpp, useBoundaries, weatherIsDynamic, soilWaterDecayRate, latitude);

  //Boreal evergreen needleleaf
  c0 = {0.1218631,0.1295781,0.1280500,0.1356488,0.1362415,0.1322454,0.1237829,0.1503324,0.1172669,0.1551760,0.1293965,0.1360983,0.1388701,0.1233824,0.1500347,0.1331320,0.1210412,0.1245973,0.1598617,0.1205537,0.1492726,0.1203824,0.1453270};
  c = {1.591897e+00,-9.941671e-02,1.978756e-01,3.500000e+01,-1.000000e+00,1.454123e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,8.807388e-01,2.934434e-04,-9.318789e-02,1.280086e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01};
  tMinJ=-10.3 - 8;
  tMaxJ=6.6 - 6.;
  pMinJ=175. - 0;
  pMaxJ=6900. + 4000.;
  tMinM=-39.9 - 8.;
  tMaxM=19.0 + 2.;
  pMinM=2-2;
  pMaxM=1300 + 2000;
  minNpp=0.53;
  npp[10] = new mai(c, c0, whc, swr, co2, soilType, t, p, altitude, tMinJ, tMaxJ, pMinJ, pMaxJ, tMinM, tMaxM, pMinM, pMaxM, minNpp, useBoundaries, weatherIsDynamic, soilWaterDecayRate, latitude);

  //Boreal deciduous needleleaf
  c0 = {0.1346323,0.1450399,0.1434999,0.1450294,0.1312006,0.1314684,0.1292051,0.1472063,0.1373973,0.1556409,0.1264541,0.1303976,0.1576328,0.1361593,0.1508253,0.1275356,0.1355128,0.1356315,0.1637595,0.1318753,0.1610309,0.1345712,0.1568916};
  c = {1.588311e+00,-9.953446e-02,1.982941e-01,3.500000e+01,-1.000000e+00,1.535554e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,7.624086e-01,3.815986e-04,-2.984578e-01,3.871615e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01};
  tMinJ=-10.3 - 8;
  tMaxJ=6.6 - 6.;
  pMinJ=215. - 0;
  pMaxJ=6900. + 4000.;
  tMinM=-39.9 - 8.;
  tMaxM=19.6 + 2.;
  pMinM=2-2;
  pMaxM=1300 + 2000;
  minNpp=1.17;
  npp[11] = new mai(c, c0, whc, swr, co2, soilType, t, p, altitude, tMinJ, tMaxJ, pMinJ, pMaxJ, tMinM, tMaxM, pMinM, pMaxM, minNpp, useBoundaries, weatherIsDynamic, soilWaterDecayRate, latitude);

  //Tropical evergreen needleleaf
  c0 = {0.03992456,0.03344327,0.04377474,0.03088140,0.02940730,0.05420901,0.04138151,0.03681954,0.03061374,0.03588355,0.03004470,0.03692928,0.04303532,0.04515249,0.03687710,0.04236888,0.02213864,0.06108095,0.03835771,0.02997647,0.03281500,0.03696203,0.04425476};
  c = {1.197497e+00,-8.738827e-02,1.742595e-01,3.500000e+01,-1.000000e+00,6.502581e-01,2.800000e+00,0.4138984,-54.4741443,-1.4,1.223739e+00,-3.062217e-05,-3.844974e-01,2.294290e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01};
  tMinJ=4.3 + 13.8;
  tMaxJ=29.1 + 2.2;
  pMinJ=200. - 0;
  pMaxJ=6900. + 4000.;
  tMinM=-3.3 + 14.6;
  tMaxM=33.5 + 2.3;
  pMinM=0;
  pMaxM=1300 + 2000;
  minNpp=0.64;
  npp[12] = new mai(c, c0, whc, swr, co2, soilType, t, p, altitude, tMinJ, tMaxJ, pMinJ, pMaxJ, tMinM, tMaxM, pMinM, pMaxM, minNpp, useBoundaries, weatherIsDynamic, soilWaterDecayRate, latitude);

  //Subtropical evergreen needleleaf
  c0 = {0.08771526,0.06546871,0.14893432,0.08151210,0.05439819,0.08062922,0.06700593,0.06470260,0.09436382,0.06005037,0.08061899,0.10209852,0.06744991,0.11864577,0.10419921,0.07418190,0.05797453,0.10102862,0.21474297,0.05752559,0.08622142,0.10598824,0.05646660};
  c = {1.528454e+00,-9.328062e-02,1.778416e-01,3.500000e+01,-1.000000e+00,1.416668e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,1.225557e+00,1.253244e-04,-9.790134e-01,1.069189e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01};
  tMinJ=-3.1 + 15.5;
  tMaxJ=22.9 - 4.2;
  pMinJ=280. - 0;
  pMaxJ=6900. + 4000.;
  tMinM=-11. + 12.2;
  tMaxM=30.0 - 0.1;
  pMinM=0;
  pMaxM=1300 + 2000;
  minNpp=1.2;
  npp[13] = new mai(c, c0, whc, swr, co2, soilType, t, p, altitude, tMinJ, tMaxJ, pMinJ, pMaxJ, tMinM, tMaxM, pMinM, pMaxM, minNpp, useBoundaries, weatherIsDynamic, soilWaterDecayRate, latitude);

  //Subtropical deciduous broadleaf forest
  c0 = {0.07556472,0.05721778,0.08186703,0.06351753,0.07113803,0.08661813,0.06621690,0.06202732,0.09156333,0.05902771,0.07786552,0.10107472,0.09343677,0.06269478,0.08833595,0.06189770,0.08025677,0.09015629,0.12211850,0.06349837,0.07095126,0.08909001,0.06878965};
  c = {1.583149e+00,-9.801282e-02,1.927008e-01,3.500000e+01,-1.000000e+00,1.152546e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,1.414094e+00,1.465791e-05,-1.312110e+00,5.351233e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01};
  tMinJ=1.9 + 10.5;
  tMaxJ=23.8 - 5.1;
  pMinJ=190. - 0;
  pMaxJ=6900. + 4000.;
  tMinM=-10.5 + 11.7;
  tMaxM=29.9 - 0.;
  pMinM=1;
  pMaxM=1300 + 2000;
  minNpp=0.15;
  npp[14] = new mai(c, c0, whc, swr, co2, soilType, t, p, altitude, tMinJ, tMaxJ, pMinJ, pMaxJ, tMinM, tMaxM, pMinM, pMaxM, minNpp, useBoundaries, weatherIsDynamic, soilWaterDecayRate, latitude);

  //Temperate evergreen broadleaf
  c0 = {0.2168666,0.2138014,0.2167967,0.1928340,0.2017754,0.2355044,0.2524142,0.1972537,0.2380835,0.1964002,0.1976556,0.2424336,0.2547105,0.2266263,0.2276977,0.1861324,0.2121292,0.2138937,0.2618858,0.2109252,0.2477607,0.2324699,0.2197330};
  c = {1.580856e+00,-9.891061e-02,1.960383e-01,3.500000e+01,-1.000000e+00,2.521807e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,8.793388e-01,4.410353e-04,-6.640318e-02,-6.262073e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01};
  tMinJ=-5.8 + 5.9;
  tMaxJ=15.9 - 3.3 + 2.;
  pMinJ=330. - 0;
  pMaxJ=6900. + 4000.;
  tMinM=-25.7 + 0.;
  tMaxM=25.1 - 0.;
  pMinM=1;
  pMaxM=1300 + 2000;
  minNpp=3.18;
  for(int i=0; i<23; ++i) {c0[i] *= 0.5;}
  npp[15] = new mai(c, c0, whc, swr, co2, soilType, t, p, altitude, tMinJ, tMaxJ, pMinJ, pMaxJ, tMinM, tMaxM, pMinM, pMaxM, minNpp, useBoundaries, weatherIsDynamic, soilWaterDecayRate, latitude);

  //Temperate deciduous needleleaf
  c0 = {0.11462020,0.14112211,0.14168577,0.09944337,0.09829958,0.12095412,0.10770765,0.08238588,0.11642098,0.08694502,0.09464438,0.11746427,0.15485947,0.08529750,0.14770957,0.10658592,0.12643711,0.09626534,0.11686801,0.11183171,0.09877526,0.12544831,0.09643012};
  c = {1.542234e+00,-9.740093e-02,1.922796e-01,3.500000e+01,-1.000000e+00,1.721792e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,8.450042e-01,2.795868e-04,-2.644673e-01,-9.114047e-05,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01};
  tMinJ=-9.0 + 9.1;
  tMaxJ=15.4 - 2.8;
  pMinJ=100. + 150;
  pMaxJ=6900. + 4000.;
  tMinM=-32.6 + 0;
  tMaxM=27.9 - 0.;
  pMinM=1;
  pMaxM=1300 + 2000;
  minNpp=0.19;
  npp[16] = new mai(c, c0, whc, swr, co2, soilType, t, p, altitude, tMinJ, tMaxJ, pMinJ, pMaxJ, tMinM, tMaxM, pMinM, pMaxM, minNpp, useBoundaries, weatherIsDynamic, soilWaterDecayRate, latitude);

  //Boreal deciduous broadleaf
  c0 = {0.1790875,0.1904747,0.1910335,0.1684104,0.1584117,0.1621894,0.1534602,0.1590490,0.1779240,0.1891642,0.1545658,0.1446883,0.2448689,0.1942312,0.1896887,0.1457447,0.1693558,0.1594965,0.2401981,0.1745017,0.1885536,0.1538039,0.2030855};
  c = {1.580021e+00,-9.891136e-02,1.961251e-01,3.500000e+01,-1.000000e+00,3.816509e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,8.219060e-01,1.584236e-04,-3.908123e-01,7.472023e-05,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01};
  tMinJ=-11.4 - 2.9;
  tMaxJ=7.9 - 7.3;
  pMinJ=190. - 0;
  pMaxJ=6900. + 4000.;
  tMinM=-40.9 + 1.;
  tMaxM=21.4 + 1.;
  pMinM=2;
  pMaxM=1300 + 2000;
  minNpp=0.47;
  for(int i=0; i<23; ++i) {c0[i] *= 0.85;}
  npp[17] = new mai(c, c0, whc, swr, co2, soilType, t, p, altitude, tMinJ, tMaxJ, pMinJ, pMaxJ, tMinM, tMaxM, pMinM, pMaxM, minNpp, useBoundaries, weatherIsDynamic, soilWaterDecayRate, latitude);

  //euKiefer
  c0 = {0.1207741,0.1284495,0.1119152,0.1268292,0.1589158,0.1265268,0.1503300,0.1175510,0.1229715,0.1239201,0.1397111,0.1482649,0.1876775,0.1165897,0.1837231,0.1170574,0.1364137,0.1222335,0.1846275,0.1198684,0.1397678,0.1418467,0.1367707};
  c = {1.594495e+00,-9.970772e-02,1.989457e-01,3.500000e+01,-1.000000e+00,2.900761e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,8.494779e-01,3.309943e-04,-3.836232e-01,7.078381e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01};
  tMinJ=-2.1;
  tMaxJ=16.1;
  pMinJ=422;
  pMaxJ=3000.;
  tMinM=-15.8;
  tMaxM=25.3;
  pMinM=5;
  pMaxM=400;
  minNpp=2.36;
  npp[18] = new mai(c, c0, whc, swr, co2, soilType, t, p, altitude, tMinJ, tMaxJ, pMinJ, pMaxJ, tMinM, tMaxM, pMinM, pMaxM, minNpp, useBoundaries, weatherIsDynamic, soilWaterDecayRate, latitude);

  //euFichte
  c0 = {0.1794001,0.1891808,0.1533205,0.1762452,0.1999378,0.2355349,0.1821450,0.1958970,0.1793914,0.1797653,0.2065453,0.1827384,0.2898600,0.1948900,0.2261093,0.1593018,0.1913847,0.2079411,0.2694490,0.1856048,0.2042948,0.1803902,0.2540367};
  c = {1.592195e+00,-9.952615e-02,1.984377e-01,3.500000e+01,-1.000000e+00,5.058505e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,6.799446e-01,3.591072e-04,-6.739802e-01,8.385973e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01};
  tMinJ=-1.6;
  tMaxJ=16.8;
  pMinJ=438;
  pMaxJ=3000.;
  tMinM=-15.6;
  tMaxM=25.3;
  pMinM=25;
  pMaxM=400;
  minNpp=1.39;
  npp[19] = new mai(c, c0, whc, swr, co2, soilType, t, p, altitude, tMinJ, tMaxJ, pMinJ, pMaxJ, tMinM, tMaxM, pMinM, pMaxM, minNpp, useBoundaries, weatherIsDynamic, soilWaterDecayRate, latitude);

  //euBuche
  c0 = {0.1272625,0.1370621,0.1375981,0.1502073,0.1370428,0.1392780,0.1517152,0.1561900,0.1478397,0.1598463,0.1519745,0.1569613,0.1974999,0.1381542,0.1919486,0.1416385,0.1572946,0.1669360,0.1977402,0.1500649,0.1572065,0.1579615,0.1594094};
  c = {1.594232e+00,-9.968908e-02,1.990604e-01,3.500000e+01,-1.000000e+00,2.648344e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,5.331431e-01,4.369507e-04,-1.211302e+00,4.866968e-05,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01};
  tMinJ=1.6 + 4.;
  tMaxJ=16.7;
  pMinJ=389;
  pMaxJ=3000.;
  tMinM=-8.5 + 1;
  tMaxM=26.2;
  pMinM=20;
  pMaxM=400;
  minNpp=3.72;
  npp[20] = new mai(c, c0, whc, swr, co2, soilType, t, p, altitude, tMinJ, tMaxJ, pMinJ, pMaxJ, tMinM, tMaxM, pMinM, pMaxM, minNpp, useBoundaries, weatherIsDynamic, soilWaterDecayRate, latitude);

  //euPinaster
  c0 = {0.09728270,0.10831447,0.12124217,0.09071147,0.08161405,0.09897297,0.10858003,0.07446026,0.09442338,0.08711192,0.07474872,0.08281703,0.13107504,0.10532173,0.10599763,0.08877993,0.08999591,0.08280230,0.13986681,0.07536053,0.09880739,0.09069319,0.09606415};
  c = {1.594612e+00,-9.972969e-02,1.988237e-01,3.500000e+01,-1.000000e+00,1.469745e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,9.379329e-01,8.155537e-04,-1.486089e-01,-3.983407e-03,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01};
  tMinJ=5.5 + 5;
  tMaxJ=17.3;
  pMinJ=373;
  pMaxJ=3000.;
  tMinM=-1.3 + 3;
  tMaxM=26.2;
  pMinM=3;
  pMaxM=400;
  minNpp=2.79;
  npp[21] = new mai(c, c0, whc, swr, co2, soilType, t, p, altitude, tMinJ, tMaxJ, pMinJ, pMaxJ, tMinM, tMaxM, pMinM, pMaxM, minNpp, useBoundaries, weatherIsDynamic, soilWaterDecayRate, latitude);

  //euTanne
  c0 = {0.14777350,0.15048855,0.20410069,0.10166814,0.21977505,0.11141147
,0.08796520,0.08977260,0.12067930,0.14464023,0.14432563,0.21735400
,0.17989385,0.12323347,0.14592808,0.08893861,0.16532699,0.17645573
,0.17876554,0.18757352,0.15062610,0.14638657,0.14874606};
  c = {1.555171e+00,-9.932687e-02,1.979182e-01,3.500000e+01,-1.000000e+00,2.040056e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,8.108940e-01,2.491341e-04,-1.103119e-01,-4.295811e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01};
  tMinJ=1.6;
  tMaxJ=16.8;
  pMinJ=379;
  pMaxJ=3000.;
  tMinM=-7.5;
  tMaxM=25.8;
  pMinM=20;
  pMaxM=400;
  minNpp=2.51;
  npp[22] = new mai(c, c0, whc, swr, co2, soilType, t, p, altitude, tMinJ, tMaxJ, pMinJ, pMaxJ, tMinM, tMaxM, pMinM, pMaxM, minNpp, useBoundaries, weatherIsDynamic, soilWaterDecayRate, latitude);

  //euEiche
  c0 = {0.1165066,0.1204457,0.1220312,0.1196470,0.1390695,0.1480787,0.1184926,0.1046163,0.1438569,0.1256989,0.1210813,0.1365279,0.1677675,0.1241299,0.1747276,0.1287212,0.1163627,0.1416925,0.1892422,0.1099974,0.1368449,0.1479013,0.1333442};
  c = {1.597726e+00,-9.988829e-02,1.996781e-01,3.500000e+01,-1.000000e+00,2.830689e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,7.620456e-01,3.334448e-04,-1.357640e+00,1.191982e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01};
  tMinJ=4.2;
  tMaxJ=16.7;
  pMinJ=391-30;
  pMaxJ=3000.;
  tMinM=-8.4;
  tMaxM=25.7;
  pMinM=15;
  pMaxM=400;
  minNpp=3.77;
  npp[23] = new mai(c, c0, whc, swr, co2, soilType, t, p, altitude, tMinJ, tMaxJ, pMinJ, pMaxJ, tMinM, tMaxM, pMinM, pMaxM, minNpp, useBoundaries, weatherIsDynamic, soilWaterDecayRate, latitude);

  //euBirke //res$what[tc-1]
  c0 = {0.1585352,0.1791702,0.1456907,0.1627288,0.1767568,0.1793059,0.1678409,0.1450082,0.1665453,0.2132190,0.1415539,0.1917198,0.2829671,0.2034887,0.1929536,0.1500426,0.1630552,0.1636004,0.2562288,0.2461736,0.1877926,0.1706541,0.1949842};
  c = {1.595563e+00,-9.971459e-02,1.991162e-01,3.500000e+01,-1.000000e+00,4.408279e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,9.095339e-01,1.480798e-04,-4.059949e-02,-2.817819e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01};
  tMinJ=-2.9;
  tMaxJ=16.2;
  pMinJ=378;
  pMaxJ=3000.;
  tMinM=-16.6;
  tMaxM=25.9;
  pMinM=15;
  pMaxM=400;
  minNpp=1.67;
  npp[24] = new mai(c, c0, whc, swr, co2, soilType, t, p, altitude, tMinJ, tMaxJ, pMinJ, pMaxJ, tMinM, tMaxM, pMinM, pMaxM, minNpp, useBoundaries, weatherIsDynamic, soilWaterDecayRate, latitude);

  ifstream fpi;
  fpi.open("/tmp/modelData.txt", ios::in);

  ofstream fpo;
  fpo.open("/tmp/nppG4mTree.asc");

  //recode soil codes
  int soilTypeOrig=0;
  int soilRosetta[206];
  for(int i=0; i<206; ++i) {soilRosetta[i] = i;}
  int soilBreaks[] = {1,9,18,24,31,38,46,51,57,61,71,78,81,86,92,97,105,114,121,125,131,137,143,148,155,162,170,177,182,999};
  for(int i=0; i<206; ++i) {
    int tmp=0;
    if(i==63) {tmp = 30;}
    else if(i==65) {tmp = 31;}
    else if(i==12) {tmp = 32;}
    else if(i==196) {tmp = 33;}
    else if(i==103) {tmp = 34;}
    else if(i==153) {tmp = 35;}
    else if(i==106) {tmp = 36;}
    else if(i==161) {tmp = 37;}
    else if(i==17) {tmp = 38;}
    else if(i==62) {tmp = 39;}
    else if(i==151) {tmp = 40;}
    else {
      for(unsigned int j=1; j < sizeof(soilBreaks) / sizeof(soilBreaks[0]); ++j) {
	if(i >= soilBreaks[j-1] && i < soilBreaks[j]) {tmp = j; break;}
      }
    }
    switch (tmp) {
    case 24: tmp = 2; break;
    case 6: tmp = 3; break;
    case 30: tmp = 4; break;
    case 20: tmp = 5; break;
    case 31: tmp = 6; break;
    case 1: tmp = 7; break;
    case 32: tmp = 8; break;
    case 25: tmp = 9; break;
    case 33: tmp = 10; break;
    case 2: tmp = 11; break;
    case 21: tmp = 12; break;
    case 34: tmp = 13; break;
    case 35: tmp = 14; break;
    case 16: tmp = 15; break;
    case 14: tmp = 16; break;
    case 36: tmp = 17; break;
    case 17: tmp = 18; break;
    case 37: tmp = 19; break;
    case 10: tmp = 20; break;
    case 38: tmp = 21; break;
    case 39: tmp = 22; break;
    case 40: tmp = 23; break;
    default: tmp = 1;
    }
    soilRosetta[i] = tmp-1;
  }

  int whcFromSoiltype[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6,1,1,1,1,5,1,1,1,1,1,1,1,1,1,1,6,3,3,3,3,3,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,1,1,1,1,5,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,5,5,5,6,6,5,1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,1,1,1,1,1,1,1,1,1,5,1,6,6,3,1,1,3,1,1,3,3,6,1,1,1,1,3,1,1,1,1,1,3,3,1,1,1,1,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
  string oneChar;
  string line;
  do {
    //lon,lat,x,y,prec01,prec02,prec03,prec04,prec05,prec06,prec07,prec08,prec09,prec10,prec11,prec12,tmean01,tmean02,tmean03,tmean04,tmean05,tmean06,tmean07,tmean08,tmean09,tmean10,tmean11,tmean12,soilIiasaFao90,soilIiasaAwc,elevation,Landarea,irrigation
    getline(fpi, line);
    istringstream iss(line);
    vector<string> tokens;
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter<vector<string> >(tokens));
    for(int i=0; i<12; ++i) {p[i] = atof(tokens[i+4].c_str());}
    for(int i=0; i<12; ++i) {t[i] = atof(tokens[i+16].c_str())/10.;}
    soilTypeOrig = atoi(tokens[28].c_str());
    if(soilTypeOrig < 0) {soilTypeOrig = 0;}
    if(static_cast<unsigned int>(soilTypeOrig) >= sizeof(whcFromSoiltype) / sizeof(whcFromSoiltype[0])) {soilTypeOrig = sizeof(whcFromSoiltype) / sizeof(whcFromSoiltype[0]) - 1;}
    {
      int st = soilTypeOrig;
      if(st > 205) {st = 205;}
      if(st < 0) {st = 0;}
      soilType = soilRosetta[st];
    }
    whc = atoi(tokens[29].c_str());
    if(whc < 1) {whc = whcFromSoiltype[soilTypeOrig];}
    switch (static_cast<int>(whc)) {
    case 1: whc = 150.; break;
    case 2: whc = 125.; break;
    case 3: whc = 100.; break;
    case 4: whc = 75.; break;
    case 5: whc = 50.; break;
    case 6: whc = 15.; break;
    default: whc = 0.;
    }
    whc *= 1.5;
    swr = 0.; //SoilWater - irrigation
    altitude = atof(tokens[30].c_str());
    latitude= atof(tokens[1].c_str())/90.*M_PI;

    for(int i=0; i<25; ++i) {
      npp[i]->setTemperature(t);
      npp[i]->setPrecipitation(p);
      npp[i]->setWhc(whc);
      npp[i]->setSwr(swr);
      npp[i]->setSoilType(soilType);
      npp[i]->setAltitude(altitude);
      npp[i]->setLatitude(latitude);
    }

    fpo << tokens[0] << " " << tokens[1];

    double nppMax = 0.;
    int nppType = -1;
    for(int i=5; i<18; ++i) {
      double cnpp = npp[i]->getNpp();
      fpo << " " << cnpp;
      if(nppMax < cnpp) {
	nppMax = cnpp;
	nppType = i;
      }
    }
    fpo << " " << nppMax << " " << nppType;

    //Irrigate 80mm/year    
    {
      nppMax = 0.;
      nppType = -1;
      for(int i=5; i<18; ++i) {
	std::vector<double> pi(p);
	for(int i=0; i<12; ++i) {pi[i] += 80.;}
	npp[i]->setPrecipitation(pi);
	double cnpp = npp[i]->getNpp();
	fpo << " " << cnpp;
	if(nppMax < cnpp) {
	  nppMax = cnpp;
	  nppType = i;
	}
      }
    }
    fpo << " " << nppMax << " " << nppType << endl;

  } while(fpi.good());
  fpo.close();
  fpi.close();

  return(0);
}
