#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <iterator>

#include "..//mai.h"

using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int main() {
  g4m::mai mai;
  mai.setSwr(0.8);
  mai.setcNpp2mai({1./3.});

  //lc1er1 Nadel-Evergreen-Tropical
  mai.setCoef(0, {6.94357e-06,8,1.15427,300,2.38,0.025,0.8,50,0.002,0.85,20,0,0,0,0});
  mai.setCoefC0(0, {1.167273,1.181957,1.148051,1.144072,1.057347,1.183415,0.839456,0.998758,1.066749,0.998820,0.888920,1.132417,1.070195,1.146386,1.444292,1.338450,0.926202,1.164307,1.482320,1.161192,1.119969,1.010927,1.126816}, 0.7, 1.3);
  mai.setBoundaries(0,4.3 + 13.8,29.1 + 2.2,200. - 0,6900. + 4000.,-3.3 + 14.6,33.5 + 2.3,0,1300 + 2000,0.64);

  //lc2er1 Laub-Evergreen-Tropical
mai.setCoef(1, {6.97374e-07,8,2.08263,300,2.36,0.025,0.6,50,0.002,0.85,20,0,0,0,0});
mai.setCoefC0(1, {1.027236,0.930810,1.026614,1.058107,1.044301,1.088828,1.284784,0.997171,1.030403,0.930521,0.917611,1.071603,1.167654,1.063758,1.260789,1.139207,0.999577,0.966556,1.161625,0.990053,1.105658,1.056113,1.107417}, 0.7, 1.3);
mai.setBoundaries(1,10.1 + 8.,28.3 + 3.,600. - 0,6900. + 4000.,7.2 + 4.1,33.6 + 2.2,0,1300 + 2000,4.6);

  //lc3er1 Nadel-Deciduous-Tropical
mai.setCoef(2, {4.98896e-09,4,3.3326,300,3,0.04,0.8,50,0.002,0.85,20,0,0,0,0});
mai.setCoefC0(2, {1.25067,1.30666,1.42140,1.37749,1.19130,1.15079,0.42130,1.41935,1.35999,1.15866,1.22622,1.17439,1.46707,1.43959,1.89361,1.66058,1.01811,1.55586,1.65774,1.17218,1.36042,1.39463,1.66056}, 0.7, 1.3);
 mai.setBoundaries(2,8.3 + 9.8 - 3,28.5 + 1.,225. - 0,6900. + 4000.,1.9 + 9.4 - 3,35.2 + 1.,0,1300 + 2000,1.13);
 
  //lc4er1 Laub-Deciduous-Tropical
mai.setCoef(3, {3.17454e-07,8,1.98803,300,1.82,0.025,0.42,50,0.002,0.85,20,0,0,0,0});
mai.setCoefC0(3, {1.014203,0.655291,0.932896,1.681138,0.909431,0.724058,1.170861,0.730898,0.584047,0.946718,0.868518,0.866705,0.973624,1.075178,2.262815,0.867673,0.700617,0.794007,1.981404,1.001099,1.352024,1.284358,1.225052}, 0.7, 1.3);
 mai.setBoundaries(3,8.3 + 9.8 - 3,28.5 + 1.,225. - 0,6900. + 4000.,1.9 + 9.4 - 3,35.2 + 1.,0,1300 + 2000,1.13);
  
  //lc1er2 Nadel-Evergreen-Subtropical
mai.setCoef(4, {2.98529e-07,8,2.52942,300,5,0.025,0.8,50,0.002,0.85,20,0,0,0,0});
mai.setCoefC0(4, {1.084565,1.258374,1.544269,1.208804,1.048047,1.152335,0.798060,1.058165,1.172564,1.078057,1.483471,1.160240,1.351295,1.353240,1.784506,1.224317,1.034672,1.308062,1.714973,1.010345,1.210056,1.215814,0.994749}, 0.7, 1.3);
mai.setBoundaries(4,-3.1 + 15.5,22.9 - 4.2,280. - 0,6900. + 4000.,-11. + 12.2,30.0 - 0.1,0,1300 + 2000,1.2);

  //lc2er2 Laub-Evergreen-Subtropical
mai.setCoef(5, {3.68661e-07*.5,8,2.4796,300,3.4,0.025,0.6,50,0.002,0.85,20,0,0,0,0});
//mai.setCoef(5, {1.85e-07,8,2.4796,300,3.4,0.025,0.6,50,0.002,0.85,20,0,0,0,0});
mai.setCoefC0(5, {1.0392105,0.9697758,1.1182245,1.0074064,0.9719184,0.9856556,0.8268068,0.9808688,1.0001164,0.9635926,1.0681479,1.0059678,1.2294432,1.0322276,1.0641367,0.9296392,1.0646439,1.1399712,1.0061634,1.1424225,0.9144326,1.1622985,1.0073817}, 0.7, 1.3);
mai.setBoundaries(5,3.4 + 9,23.7 - 5. + 2.5,290. - 0,6900. + 4000.,-5.8 + 7.,29.9 + 2.,0,1300 + 2000,0.85);

  //lc3er2 Nadel-Deciduous-Subtropical
mai.setCoef(6, {2.78094e-06,4,1.46148,300,3,0.04,0.8,50,0.002,0.85,20,0,0,0,0});
mai.setCoefC0(6, {1.33731,1.21174,2.09552,1.45462,1.11848,1.78982,0.44737,1.14455,1.33598,1.16703,1.59218,1.89613,1.54779,1.33450,1.61385,1.23386,1.15095,1.48598,1.50806,1.13044,1.04776,1.27060,1.15392}, 0.7, 1.3);
mai.setBoundaries(6,-3.1 + 15.5,22.9 - 4.2,280. - 0,6900. + 4000.,-11. + 12.2,30.0 - 0.1,0,1300 + 2000,1.2);
 
  //lc4er2 Laub-Deciduous-Subtropical
mai.setCoef(7, {5.22354e-08,3.81171,2.94545,300,3.18175,0.0232828,0.809172,50,0.002,0.851749,20,0,0,0,0});
mai.setCoefC0(7, {0.983435,1.216336,1.208377,1.164035,1.127213,1.427934,0.427887,1.041255,0.899436,0.951187,1.505357,1.606139,1.512237,1.196615,1.284658,1.063652,1.121445,1.179302,1.375756,0.836196,1.106857,1.427041,1.055856}, 0.7, 1.3);
mai.setBoundaries(7,1.9 + 10.5,23.8 - 5.1,190. - 0,6900. + 4000.,-10.5 + 11.7,29.9 - 0.,1,1300 + 2000,0.15);

  //lc1er3 Nadel-Evergreen-Temperate
mai.setCoef(8, {1.95866e-11,16.6774,5.30899,300,7.00127,0.0828,0.180158,50,0.002,0.85,20,0,0,0,0});
mai.setCoefC0(8, {1.054684,1.099922,1.075196,0.980570,1.002155,1.044522,1.134524,1.073864,1.000548,1.070339,1.068615,1.086483,1.054495,1.036821,1.095323,1.008207,1.094867,1.031270,0.987843,1.035130,0.950606,1.074587,1.008381}, 0.7, 1.3);
 mai.setBoundaries(8,-4.9 + 5,14.6 - 2. + 3,220. - 0,6900. + 4000.,-27.2 + 0.,25.1 + 4.5,0,1300 + 2000,1.18);

  //lc2er3 Laub-Evergreen-Temperate
mai.setCoef(9, {3.15e-09,9.35918,3.66485,300,4.64592,0.06,0.26,50,0.002,0.85,20,0,0,0,0});
mai.setCoefC0(9, {1.0118956,1.0615462,1.0779899,1.0120382,0.9627487,0.9898199,1.0791497,1.0310722,0.9754276,0.8438813,1.1233655,1.1068356,1.0901310,1.0101304,1.0565290,1.0779955,1.0621784,1.0007144,0.9295641,1.0472222,1.0161400,1.0173837,1.0171552}, 0.7, 1.3);
mai.setBoundaries(9,-5.8 + 5.9,15.9 - 3.3 + 2.,330. - 0,6900. + 4000.,-25.7 + 0.,25.1 - 0.,1,1300 + 2000,3.18);

  //lc3er3 Nadel-Deciduous-Temperate
mai.setCoef(10, {1.50349e-07,7.07274,2.54163,300,4.00133,0.06,0.25,50,0.002,0.85,20,0,0,0,0});
mai.setCoefC0(10, {0.862190,1.145883,1.135764,0.950486,1.102457,1.030473,0.956814,0.975896,1.170444,1.003162,1.169161,1.053181,1.126635,1.232963,0.864202,1.179097,0.774995,0.995233,1.321486,1.032954,1.041349,1.029158,1.056834}, 0.7, 1.3);
mai.setBoundaries(10,-9.0 + 9.1,15.4 - 2.8,100. + 150,6900. + 4000.,-32.6 + 0,27.9 - 0.,1,1300 + 2000,0.19);

  //lc4er3 Laub-Deciduous-Temperate
mai.setCoef(11, {6.30082e-09,9.35918,3.66485,300,4.64592,0.06,0.26,50,0.002,0.85,20,0,0,0,0});
mai.setCoefC0(11, {0.8426849,1.1336508,0.9219486,1.0428545,1.1223109,0.8339236,1.0641686,0.5452793,1.0681361,1.0280533,1.0013517,0.6224940,1.2335694,1.1491972,1.0465842,0.9879109,1.1978397,0.9430111,1.0909037,1.1027924,1.0124663,1.2082930,1.0756028}, 0.7, 1.3);
mai.setBoundaries(11,-4.9 + 5,14.6 - 2. + 3,305. - 0,6900. + 4000.,-29.7 + 0.,27.1 + 2.5,2,1300 + 2000,1.74);

  //lc1er4 Nadel-Evergreen-Boreal
mai.setCoef(12, {1.36116e-07,6.7432,2.82955,300,4.57307,0.06,0.50,50,0.002,0.85,20,0,0,0,0});
mai.setCoefC0(12, {1.035657,1.036384,1.032846,1.030142,1.055238,1.052846,1.045243,1.024852,1.002326,0.998242,1.081536,1.108310,1.013428,1.036258,1.054235,1.041855,1.031564,1.027304,1.115472,1.011239,1.016438,1.006963,1.013799}, 0.7, 1.3);
mai.setBoundaries(12,-10.3 - 8,6.6 - 6.,175. - 0,6900. + 4000.,-39.9 - 8.,19.0 + 2.,2-2,1300 + 2000,0.53);
 
  //lc2er4 Laub-Evergreen-Boreal
mai.setCoef(13, {6.94485e-11,10.3926,5.18179,300,8.21249,0.06,0.2,50,0.002,0.85,20,0,0,0,0});
mai.setCoefC0(13, {0.93566,1.04938,0.99955,1.17027,0.92923,0.90336,1.00938,1.02759,0.95376,0.90328,1.00736,0.99617,1.14578,1.09325,1.25368,1.11511,0.92586,0.99376,1.10367,0.90914,0.96348,0.94270,1.41604}, 0.7, 1.3);
mai.setBoundaries(13,-11.4 - 2.9,7.9 - 7.3,190. - 0,6900. + 4000.,-40.9 + 1.,21.4 + 1.,2,1300 + 2000,0.47);
 
  //lc3er4 Nadel-Deciduous-Boreal
mai.setCoef(14, {7.54653e-07,3.39758,2.39893,300,5.10519,0.06,0.59,50,0.002,0.85,20,0,0,0,0});
mai.setCoefC0(14, {0.997382,1.164807,1.137406,1.157535,1.025785,1.016309,0.922417,1.078434,1.033414,0.960146,1.018151,1.064526,1.083873,1.061930,1.108677,1.053499,1.025711,0.942647,1.277620,1.014716,1.114794,1.009307,1.115872}, 0.7, 1.3);
mai.setBoundaries(14,-10.3 - 8,6.6 - 6.,215. - 0,6900. + 4000.,-39.9 - 8.,19.6 + 2.,2-2,1300 + 2000,1.17);

  //lc3er4 Laub-Deciduous-Boreal
mai.setCoef(15, {1.38897e-10,10.3926,5.18179,300,8.21249,0.06,0.2,50,0.002,0.85,20,0,0,0,0});
mai.setCoefC0(15, {0.816807,1.171890,1.106970,1.007987,0.960848,0.923770,1.029693,1.114420,1.059554,1.006887,1.168652,0.913977,1.082629,1.532929,1.053006,1.072970,0.989127,0.994642,1.297751,0.959316,1.233419,0.953433,1.041302}, 0.7, 1.3);
mai.setBoundaries(15,-11.4 - 2.9,7.9 - 7.3,190. - 0,6900. + 4000.,-40.9 + 1.,21.4 + 1.,2,1300 + 2000,0.47);

  ifstream fpi;
  fpi.open("./result/baseData30.txt", ios::in);  //Low res

  ofstream fpo;
  fpo.open("./result/nppG4mTree30.asc");

  fpo << "#x y"; 
  for(int i=0; i<16; ++i) {fpo << " mai" << i;}
  fpo << " maiMax maiMaxWhichOne";
  for(int i=0; i<16; ++i) {fpo << " maiIri" << i;}
  fpo << " maiIriMax maiIriMaxWhichOne" << endl;
   
  //recode soil codes
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
  while(getline(fpi, line)) {
    //lon,lat,x,y,prec01,prec02,prec03,prec04,prec05,prec06,prec07,prec08,prec09,prec10,prec11,prec12,tmean01,tmean02,tmean03,tmean04,tmean05,tmean06,tmean07,tmean08,tmean09,tmean10,tmean11,tmean12,soilIiasaFao90,soilIiasaAwc,elevation,Landarea,irrigation
    //lon,lat,x,y,watershare16,DeciduousBroadleaf100,DeciduousNeedleleaf,EvergreenBroadleaf,EvergreenNeedleleaf,soilIiasaFao90,soilIiasaSwr,soilIiasaAwc,elevation,prec01,prec02,prec03,prec04,prec05,prec06,prec07,prec08,prec09,prec10,prec11,prec12,tmean01,tmean02,tmean03,tmean04,tmean05,tmean06,tmean07,tmean08,tmean09,tmean10,tmean11,tmean12,radi01,radi02,radi03,radi04,radi05,radi06,radi07,radi08,radi09,radi10,radi11,radi12
    
    istringstream iss(line);
    vector<string> tokens{istream_iterator<string>{iss}, istream_iterator<string>{}};

    std::valarray<double> p(12);
    for(int i=0; i<12; ++i) {p[i] = atof(tokens[i+13].c_str());}
    std::valarray<double> t(12);
    for(int i=0; i<12; ++i) {t[i] = atof(tokens[i+25].c_str());}
    std::valarray<double> r(12);
    for(int i=0; i<12; ++i) {r[i] = atof(tokens[i+37].c_str());}
    r *= 1000./(24.*60.*60.);
    
    int soilType;
    int soilTypeOrig = atoi(tokens[9].c_str());
    if(soilTypeOrig < 0) {soilTypeOrig = 0;}
    if(static_cast<unsigned int>(soilTypeOrig) >= sizeof(whcFromSoiltype) / sizeof(whcFromSoiltype[0])) {soilTypeOrig = sizeof(whcFromSoiltype) / sizeof(whcFromSoiltype[0]) - 1;}
    {
      int st = soilTypeOrig;
      if(st > 205) {st = 205;}
      if(st < 0) {st = 0;}
      soilType = soilRosetta[st];
    }

    int whc = atoi(tokens[11].c_str());
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
    int swr = 0.; //SoilWater - irrigation
    int altitude = atof(tokens[12].c_str());

    mai.setTemperature(t);
    mai.setPrecipitation(p);
    mai.setRadiation(r);
    mai.setCo2(0.038);
    mai.setSwr(swr);
    mai.setWhc(whc);
    mai.setAltitude(altitude);
    mai.setSoilType(soilType);

    fpo << tokens[0] << " " << tokens[1];

    std::valarray<double> rmai = mai.getMai(mai.testBoundaries(), true);
    for(double m : rmai) {fpo << " " << m;}
    fpo << " " << rmai.max() << " " << distance(begin(rmai), max_element(begin(rmai), end(rmai)));
    
    //Irrigate 80mm/year
    p += 80.;
    mai.setPrecipitation(p);
    rmai = mai.getMai(mai.testBoundaries(), true);
    for(double m : rmai) {fpo << " " << m;}
    fpo << " " << rmai.max() << " " << distance(begin(rmai), max_element(begin(rmai), end(rmai))) << endl;

  }
  fpo.close();
  fpi.close();

  return(0);
}
