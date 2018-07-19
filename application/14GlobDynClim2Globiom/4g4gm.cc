//time ./4g4gm /data/tmp/w_hadgem2.bin >/data/tmp/g4m_w_hadgem2.csv

const int firstYear = 2010;
const int decades = 10;

#include <iostream>
#include <fstream>
#include <sstream>
#include <valarray>
#include <vector>
#include <cmath>
#include <map>
#include <algorithm>

#include "/home/georg/prg/g4m/increment/ageStruct.h"
#include "/home/georg/prg/g4m/increment/mai.h"
#include "/home/georg/prg/g4m/increment/misc.h"

using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct siteData {
  unsigned char landshare;
  unsigned char forestShare;
  unsigned char forestShareIrri;
  unsigned char primaryForest;
  unsigned char shareDecidousBroadleaved;
  unsigned char shareDecidousNeadleleaved;
  unsigned char shareEvergreenBroadleaved;
  unsigned char shareEvergreenNeadleleaved;
  unsigned char slope3;
  unsigned char slope6;
  unsigned char slope10;
  unsigned char slope15;
  unsigned char slope30;
  unsigned char slope50;
  unsigned char slope50p;
  unsigned char soiltype;
  //signed char latitude;
  short unsigned int row;
  short unsigned int col;
  short unsigned int whc;
  short int country;
  short int altitude;
  short int cstemManIrri;
  short int cstemManNoirri;
  short int cstemUmanIrri;
  short int cstemUmanNoirri;
  unsigned int gridsize;
  float latitude;
};


const int numberOfMaiTypes = 13;
struct globiomOutput {
  map<short int, double> countryLandarea;
  double currentForestArea[decades];
  double currentForestAreaIrri[decades];
  double potentialForestArea[decades];
  double primaryForest;
  unsigned char shareDecidousBroadleaved;
  unsigned char shareDecidousNeadleleaved;
  unsigned char shareEvergreenBroadleaved;
  unsigned char shareEvergreenNeadleleaved;
  double maiSum[numberOfMaiTypes][decades]; //[maiType][decade]
  double maiArea[numberOfMaiTypes][decades]; //[maiType][decade]
  double maiSumBest[decades];
  double maiAreaBest[decades];
  double cstemMan;
  double cstemUman;
  double slopet10;  //Slope where forest is located
  double slope15;
  double slope30;
  double slope50;
  double slope50p;
  double lslopet10;  //Slope of the landarea
  double lslope15;
  double lslope30;
  double lslope50;
  double lslope50p;
};

int main(int argc, char **argv) {

  //Country number to ISO code
  map<int, string> countryCode;
  {
    ifstream fi;
    fi.open("/home/georg/13/g4mData/country/countrynames.csv");
    std::string line;
    getline(fi,line);
    while(getline(fi,line)) {
      stringstream ss(line);
      string item;
      std::vector<string> elem;
      while (getline(ss, item, ',')) {
	elem.push_back(item);
      }
      countryCode[atoi(elem[0].c_str())] = elem[1];
    }
    fi.close();
  }

  const int altistep = 300; //all 300m a seperate altitude class
  //globiomOutput globiom[720][360][alti];
  vector<vector<map<int,globiomOutput> > > grid(720,vector<map<int,globiomOutput> >(360)); //[col][row][alti]

  //MAI-NPP
  g4m::mai mai;
  mai.setSwr(0.8);
  mai.setcNpp2mai({1./3.});

  const double pMaxAdd = 10000.;
  const double tMaxAdd = 20.;
  //Tropical evergreen broadleaf
  mai.setCoef(0, {1.619920e+00,-9.848997e-02,1.970912e-01,3.500000e+01,-1.000000e+00,5.963311e-01,2.800000e+00,0.4138984,-54.4741443,-1.4,1.941317e+00,-9.634048e-05,-1.100476e+00,4.401370e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  mai.setCoefC0(0, {0.06136063,0.05394037,0.06884074,0.06795948,0.06069805,0.07048979,0.05777229,0.06212813,0.05874720,0.06193419,0.04811436,0.05803750,0.07435434,0.07529665,0.05663168,0.06045755,0.04999882,0.05534675,0.07479968,0.05449085,0.05932120,0.05244837,0.06286373});
  mai.setBoundaries(0,10.1 + 8.,28.3 + 3.+tMaxAdd,600. - 0,6900. + 4000.+pMaxAdd,7.2 + 4.1,33.6 + 2.2+tMaxAdd,0,1300 + 2000+pMaxAdd,4.6);
  //Tropical evergreen needleleaf
  mai.setCoef(1, {1.197497e+00,-8.738827e-02,1.742595e-01,3.500000e+01,-1.000000e+00,6.502581e-01,2.800000e+00,0.4138984,-54.4741443,-1.4,1.223739e+00,-3.062217e-05,-3.844974e-01,2.294290e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  mai.setCoefC0(1, {0.03992456,0.03344327,0.04377474,0.03088140,0.02940730,0.05420901,0.04138151,0.03681954,0.03061374,0.03588355,0.03004470,0.03692928,0.04303532,0.04515249,0.03687710,0.04236888,0.02213864,0.06108095,0.03835771,0.02997647,0.03281500,0.03696203,0.04425476});
  mai.setBoundaries(1,4.3 + 13.8,29.1 + 2.2+tMaxAdd,200. - 0,6900. + 4000.+pMaxAdd,-3.3 + 14.6,33.5 + 2.3+tMaxAdd,0,1300 + 2000+pMaxAdd,0.64);
  //Tropical deciduous broadleaf
  mai.setCoef(2, {1.491055e+00,-9.734815e-02,1.951930e-01,3.500000e+01,-1.000000e+00,2.144010e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,1.018970e+00,5.690775e-04,-2.743858e-01,-3.154196e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  mai.setCoefC0(2, {0.06836858,0.06813469,0.05934084,0.07794042,0.05072694,0.06129334,0.06565496,0.04773324,0.07122714,0.08419983,0.05885219,0.06289404,0.09646647,0.06666715,0.09231248,0.03816949,0.04651217,0.07239042,0.10244473,0.07365863,0.05660622,0.08033533,0.07028939});
  mai.setBoundaries(2,8.3 + 9.8 - 3,28.5 + 1.+tMaxAdd,225. - 0,6900. + 4000.+pMaxAdd,1.9 + 9.4 - 3,35.2 + 1.+tMaxAdd,0,1300 + 2000+pMaxAdd,1.13);
   //Subtropical evergreen broadleaf
    mai.setCoef(3, {1.538287e+00,-9.722637e-02,1.894521e-01,3.500000e+01,-1.000000e+00,1.249044e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,1.656060e+00,2.328051e-04,-1.522926e+00,1.101988e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  mai.setCoefC0(3, {0.11606259,0.09230848,0.11152179,0.07736606,0.10174771,0.10232312,0.09956373,0.09869678,0.12651927,0.11027624,0.09064032,0.14575220,0.09281512,0.09487696,0.13310920,0.08328069,0.10088982,0.12739049,0.14853313,0.12082426,0.10963328,0.16620110,0.10230552});
  mai.setBoundaries(3,3.4 + 9,23.7 - 5. + 2.5,290. - 0,6900. + 4000.+pMaxAdd,-5.8 + 7.,29.9 + 2.,0,1300 + 2000+pMaxAdd,0.85);
  //Subtropical evergreen needleleaf
  mai.setCoef(4, {1.528454e+00,-9.328062e-02,1.778416e-01,3.500000e+01,-1.000000e+00,1.416668e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,1.225557e+00,1.253244e-04,-9.790134e-01,1.069189e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  mai.setCoefC0(4, {0.08771526,0.06546871,0.14893432,0.08151210,0.05439819,0.08062922,0.06700593,0.06470260,0.09436382,0.06005037,0.08061899,0.10209852,0.06744991,0.11864577,0.10419921,0.07418190,0.05797453,0.10102862,0.21474297,0.05752559,0.08622142,0.10598824,0.05646660});
  mai.setBoundaries(4,-3.1 + 15.5,22.9 - 4.2,280. - 0,6900. + 4000.+pMaxAdd,-11. + 12.2,30.0 - 0.1,0,1300 + 2000+pMaxAdd,1.2);
  //Subtropical deciduous broadleaf forest
  mai.setCoef(5, {1.583149e+00,-9.801282e-02,1.927008e-01,3.500000e+01,-1.000000e+00,1.152546e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,1.414094e+00,1.465791e-05,-1.312110e+00,5.351233e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  mai.setCoefC0(5, {0.07556472,0.05721778,0.08186703,0.06351753,0.07113803,0.08661813,0.06621690,0.06202732,0.09156333,0.05902771,0.07786552,0.10107472,0.09343677,0.06269478,0.08833595,0.06189770,0.08025677,0.09015629,0.12211850,0.06349837,0.07095126,0.08909001,0.06878965});
  mai.setBoundaries(5,1.9 + 10.5,23.8 - 5.1,190. - 0,6900. + 4000.+pMaxAdd,-10.5 + 11.7,29.9 - 0.,1,1300 + 2000+pMaxAdd,0.15);
  //Temperate evergreen broadleaf
  mai.setCoef(6, {1.580856e+00,-9.891061e-02,1.960383e-01,3.500000e+01,-1.000000e+00,2.521807e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,8.793388e-01,4.410353e-04,-6.640318e-02,-6.262073e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  mai.setCoefC0(6, {0.1084333,0.1069007,0.10839835,0.096417,0.1008877,0.1177522,0.1262071,0.09862685,0.11904175,0.0982001,0.0988278,0.1212168,0.12735525,0.11331315,0.11384885,0.0930662,0.1060646,0.10694685,0.1309429,0.1054626,0.12388035,0.11623495,0.1098665});
  mai.setBoundaries(6,-5.8 + 5.9,15.9 - 3.3 + 2.,330. - 0,6900. + 4000.+pMaxAdd,-25.7 + 0.,25.1 - 0.,1,1300 + 2000+pMaxAdd,3.18);
  //Temperate evergreen needleleaf
  mai.setCoef(7, {1.570108e+00,-9.829125e-02,1.945205e-01,3.500000e+01,-1.000000e+00,1.416748e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,9.220346e-01,1.322653e-04,1.418027e-01,-1.572138e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  mai.setCoefC0(7, {0.1177171,0.1308500,0.1070914,0.1288103,0.1519428,0.1280058,0.1521247,0.1562641,0.1294695,0.1486603,0.1318565,0.1337459,0.1723263,0.1428401,0.1618081,0.1389939,0.1357501,0.1216093,0.1503008,0.1142445,0.1521230,0.1520602,0.1590204});
  mai.setBoundaries(7,-4.9 + 5,14.6 - 2. + 3,220. - 0,6900. + 4000.+pMaxAdd,-27.2 + 0.,25.1 + 4.5,0,1300 + 2000+pMaxAdd,1.18);
  //Temperate deciduous broadleaf
  mai.setCoef(8, {1.574017e+00,-9.893621e-02,1.965476e-01,3.500000e+01,-1.000000e+00,1.949731e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,9.597373e-01,3.182178e-04,-5.993809e-01,-2.135231e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  mai.setCoefC0(8, {0.12156712,0.12492062,0.13087684,0.11666581,0.11019982,0.12730928,0.12320618,0.07018051,0.11312888,0.09925496,0.10062111,0.12766330,0.15216701,0.11568652,0.13923021,0.09930708,0.14071709,0.11279223,0.11843090,0.12642849,0.08836875,0.12722117,0.10910369});
  mai.setBoundaries(8,-4.9 + 5,14.6 - 2. + 3,305. - 0,6900. + 4000.+pMaxAdd,-29.7 + 0.,27.1 + 2.5,2,1300 + 2000+pMaxAdd,1.74);
  //Temperate deciduous needleleaf
  mai.setCoef(9, {1.542234e+00,-9.740093e-02,1.922796e-01,3.500000e+01,-1.000000e+00,1.721792e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,8.450042e-01,2.795868e-04,-2.644673e-01,-9.114047e-05,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  mai.setCoefC0(9, {0.11462020,0.14112211,0.14168577,0.09944337,0.09829958,0.12095412,0.10770765,0.08238588,0.11642098,0.08694502,0.09464438,0.11746427,0.15485947,0.08529750,0.14770957,0.10658592,0.12643711,0.09626534,0.11686801,0.11183171,0.09877526,0.12544831,0.09643012});
  mai.setBoundaries(9,-9.0 + 9.1,15.4 - 2.8,100. + 150,6900. + 4000.+pMaxAdd,-32.6 + 0,27.9 - 0.,1,1300 + 2000+pMaxAdd,0.19);
  //Boreal evergreen needleleaf
  mai.setCoef(10, {1.591897e+00,-9.941671e-02,1.978756e-01,3.500000e+01,-1.000000e+00,1.454123e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,8.807388e-01,2.934434e-04,-9.318789e-02,1.280086e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  mai.setCoefC0(10, {0.1218631,0.1295781,0.1280500,0.1356488,0.1362415,0.1322454,0.1237829,0.1503324,0.1172669,0.1551760,0.1293965,0.1360983,0.1388701,0.1233824,0.1500347,0.1331320,0.1210412,0.1245973,0.1598617,0.1205537,0.1492726,0.1203824,0.1453270});
  mai.setBoundaries(10,-10.3 - 8,6.6 - 6.,175. - 0,6900. + 4000.+pMaxAdd,-39.9 - 8.,19.0 + 2.,2-2,1300 + 2000+pMaxAdd,0.53);
  //Boreal deciduous broadleaf
  mai.setCoef(11, {1.580021e+00,-9.891136e-02,1.961251e-01,3.500000e+01,-1.000000e+00,3.816509e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,8.219060e-01,1.584236e-04,-3.908123e-01,7.472023e-05,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  mai.setCoefC0(11, {0.152224375,0.161903495,0.162378475,0.14314884,0.134649945,0.13786099,0.13044117,0.13519165,0.1512354,0.16078957,0.13138093,0.122985055,0.208138565,0.16509652,0.161235395,0.123882995,0.14395243,0.135572025,0.204168385,0.148326445,0.16027056,0.130733315,0.172622675});
  mai.setBoundaries(11,-11.4 - 2.9,7.9 - 7.3,190. - 0,6900. + 4000.+pMaxAdd,-40.9 + 1.,21.4 + 1.,2,1300 + 2000+pMaxAdd,0.47);
  //Boreal deciduous needleleaf
  mai.setCoef(12, {1.588311e+00,-9.953446e-02,1.982941e-01,3.500000e+01,-1.000000e+00,1.535554e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,7.624086e-01,3.815986e-04,-2.984578e-01,3.871615e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  mai.setCoefC0(12, {0.1346323,0.1450399,0.1434999,0.1450294,0.1312006,0.1314684,0.1292051,0.1472063,0.1373973,0.1556409,0.1264541,0.1303976,0.1576328,0.1361593,0.1508253,0.1275356,0.1355128,0.1356315,0.1637595,0.1318753,0.1610309,0.1345712,0.1568916});
  mai.setBoundaries(12,-10.3 - 8,6.6 - 6.,215. - 0,6900. + 4000.+pMaxAdd,-39.9 - 8.,19.6 + 2.,2-2,1300 + 2000+pMaxAdd,1.17);

  g4m::incrementCurvesHf rtShort  //Short rotation time
    (150, 6.55, 0., -1.2, -0.35,
     0., 0.9, -0.8242, -0.4273, -0.4, -1.476, 4.283, -0.3, 3.610, -1.071,
     0.1, 1., -2.0670, -0.3028, 0.5, 1.5, 150, 0.01, 0.5, 0.5,
     0.8, 1./500., 2., 0.01, 0.5, 22.09082, 0.62065,-0.025, 1.50614, -0.25346,
     22.70, 16.56, -0.012, 0.24754, -1.81373, 1.0945, 0.0999, -1.6033,
     1.6, 0.95, 1.5);
  g4m::incrementCurvesHf rtNormal  //Average rotation times
    (150, 8.0, 0., -1.4, -0.3,
     0., 0.9, -0.8242, -0.4273, -0.4, -1.476, 4.283, -0.3, 3.610, -1.071,
     0.1, 1., -2.0670, -0.3028, 0.5, 1.5, 150, 0.01, 0.5, 0.5,
     0.8, 1./500., 2., 0.01, 0.5, 22.09082, 0.62065,-0.02, 1.50614, -0.25346,
     22.70, 16.56, -0.01, 0.24754, -1.81373, 1.0945, 0.0999, -1.6033,
     1.6, 0.95, 1.5);
  g4m::incrementCurvesHf rtLong  //Long rotation times
    (150, 9.0, 0., -1.5, -0.2,
     0., 0.9, -0.8242, -0.4273, -0.4, -1.476, 4.283, -0.3, 3.610, -1.071,
     0.1, 1., -2.0670, -0.3028, 0.5, 1.5, 150, 0.01, 0.5, 0.5,
     0.8, 1./500., 2., 0.01, 0.5, 22.09082, 0.62065,-0.015, 1.50614, -0.25346,
     22.70, 16.56, -0.008, 0.24754, -1.81373, 1.0945, 0.0999, -1.6033,
     1.6, 0.95, 1.5);

  g4m::incrementTab iTabShort(rtShort,15.,0.25,600,1,0.25,1.5,0.25,10.);
  g4m::incrementTab iTabNormal(rtNormal,15.,0.25,600,1,0.25,1.5,0.25,10.);
  g4m::incrementTab iTabLong(rtLong,15.,0.25,600,1,0.25,1.5,0.25,10.);
  g4m::incrementTab* iTab[3];
  iTab[0] = &iTabShort;
  iTab[1] = &iTabNormal;
  iTab[2] = &iTabLong;

  //Harvesting cost definition
  g4m::ipol<double,double> sws;  //Schnittholzanteil an Vfm
  g4m::ipol<double,double> hlv;   //1-Ernteverluste Vornutzung
  g4m::ipol<double,double> hle;   //1-Ernteverluste Endnutzung
  g4m::ipol<vector<double>,double> cov;  //costs vornutzung
  g4m::ipol<vector<double>,double> coe;  //Costs endnutzung
  g4m::ipol<vector<double>,bool> dov;  //Do vornutzung
  g4m::ipol<vector<double>,bool> doe;  //Do endnutzung
  g4m::ipol<double,double> sdMaxH;  //sdMaxH
  g4m::ipol<double,double> sdMinH;  //sdMinH

  sws.insert(10, .0);
  sws.insert(30, .6);

  hlv.insert(0, .0);
  hlv.insert(25, .7);
  hle.insert(0, .0);
  hle.insert(25, .75);

  sdMaxH.insert(0.,1.);
  sdMinH.insert(0.,1.);

  {
    vector<double> idx;
    idx.push_back(0); //diameter
    idx.push_back(0); //stocking volume [tC stemwood/ha]
    idx.push_back(0); //Share of harvest (0 - 1)
    idx[0]=0; idx[1]=2; idx[2]=.3;
    cov.insert(idx, 4);
    idx[0]=40; idx[1]=30; idx[2]=.2;
    cov.insert(idx, 2);
    
    idx[0]=4; idx[1]=2; idx[2]=.1;
    dov.insert(idx,false);
    idx[0]=5; idx[1]=3; idx[2]=.2;
    dov.insert(idx,true);

    idx.pop_back();
    idx[0]=0; idx[1]=2;
    coe.insert(idx, 3);
    idx[0]=40; idx[1]=30;
    coe.insert(idx, 1);

    idx[0]=15; idx[1]=10;
    doe.insert(idx,false);
    idx[0]=16; idx[1]=11;
    doe.insert(idx,true);
  }

  g4m::ffipol<double> ffsws(sws);
  g4m::ffipol<double> ffhlv(hlv);
  g4m::ffipol<double> ffhle(hle);
  g4m::ffipolm<double> ffcov(cov);
  g4m::ffipolm<double> ffcoe(coe);
  g4m::ffipolm<bool> ffdov(dov);
  g4m::ffipolm<bool> ffdoe(doe);
  g4m::ffipol<double> ffsdMaxH(sdMaxH);
  g4m::ffipol<double> ffsdMinH(sdMinH);

  double sdMin=1.;
  double sdMax=1.;
  unsigned int maiYears = 0;
  double minSw, minRw, minHarv;
  minSw = minRw = minHarv = 0.;
  int maxAge=600;
  g4m::ageStruct forestShort
    (&iTabShort, &ffsws, &ffhlv, &ffhle, &ffcov, &ffcoe, &ffdov, &ffdoe
     , 1. //mai
     , 0  //What stands the value of u for
     , 100  //Rotation time
     , minSw, minRw, minHarv
     ,0 , sdMax, sdMin, maiYears
     , .75 //minRotVal
     , 1  //reference of minrot
     , 0. //Flexibility of stocking degree
     , &ffsdMaxH, &ffsdMinH
     , maxAge
     );
  g4m::ageStruct forestNormal
    (&iTabNormal, &ffsws, &ffhlv, &ffhle, &ffcov, &ffcoe, &ffdov, &ffdoe
     , 1. //mai
     , 0  //What stands the value of u for
     , 100  //Rotation time
     , minSw, minRw, minHarv
     ,0 , sdMax, sdMin, maiYears
     , .75 //minRotVal
     , 1  //reference of minrot
     , 0. //Flexibility of stocking degree
     , &ffsdMaxH, &ffsdMinH
     , maxAge
     );
  g4m::ageStruct forestLong
    (&iTabLong, &ffsws, &ffhlv, &ffhle, &ffcov, &ffcoe, &ffdov, &ffdoe
     , 1. //mai
     , 0  //What stands the value of u for
     , 100  //Rotation time
     , minSw, minRw, minHarv
     ,0 , sdMax, sdMin, maiYears
     , .75 //minRotVal
     , 1  //reference of minrot
     , 0. //Flexibility of stocking degree
     , &ffsdMaxH, &ffsdMinH
     , maxAge
     );

  //Rot times with maximum average harvest
  g4m::ipol<double, double> optUShort;
  g4m::ipol<double, double> optUNormal;
  g4m::ipol<double, double> optULong;
  g4m::ipol<double, double>* optU[3];
  optU[0] = &optUShort;
  optU[1] = &optUNormal;
  optU[2] = &optULong;
  {
    g4m::ageStruct* ageStruct[3];
    ageStruct[0] = &forestShort;
    ageStruct[1] = &forestNormal;
    ageStruct[2] = &forestLong;
    for(int rot=0; rot<3; ++rot) {
      for(double mai = 0.1; mai < 15; mai+=0.1) {
	ageStruct[rot]->setMai(mai);
	ageStruct[rot]->setAvgMai(mai);
	double uOptMM = iTab[rot]->gToptt(mai,0);
	double uOpt = uOptMM;
	double lHarv = 0.;
	for(;uOpt<uOptMM*2; ++uOpt) {
	  ageStruct[rot]->setU(uOpt);
	  ageStruct[rot]->createNormalForest(uOpt, 1., 1.);
	  ageStruct[rot]->aging(mai);
	  pair<g4m::ageStruct::v, g4m::ageStruct::v> ret = ageStruct[rot]->aging();
	  double harv = ret.first.sw + ret.second.sw*ret.second.area + ret.first.rw + ret.second.rw*ret.second.area;
	  if(lHarv > harv) {
	    break;}
	  lHarv = harv;
	}
	--uOpt;
	optU[rot]->insert(mai, uOpt);
      }
    }
  }

  siteData site;
  ifstream IN(argv[1], ios::binary);
  short int nyears;
  IN.read(reinterpret_cast<char*>(&nyears), sizeof(nyears));
  valarray<short int> years(nyears);
  IN.read(reinterpret_cast<char*>(&years[0]), years.size()*sizeof(years[0]));
  valarray<short int> tempPrec(24*nyears);
  int clat=0;
  unsigned int globiomRow;
  unsigned int globiomCol;
  do {
    IN.read(reinterpret_cast<char*>(&site), sizeof(site));
    IN.read(reinterpret_cast<char*>(&tempPrec[0]), tempPrec.size()*sizeof(tempPrec[0]));
    globiomCol = (site.col-1) / 60;
    globiomRow = (site.row-1) / 60;
    int altiClass = site.altitude/altistep;
    grid[globiomCol][globiomRow][altiClass].countryLandarea[site.country] += site.landshare*site.gridsize;
    int forestShare = static_cast<int>(site.forestShare);
    int forestShareIrri = static_cast<int>(site.forestShareIrri);
    for(int decade=0; decade<decades; ++decade) {
      valarray<double> p(12);
      valarray<double> t(12);
      int currentYear=firstYear+10*decade;
      g4m::ipol<double, double> yearIpl;
      for(int i=0; i<nyears; ++i) {yearIpl.insert(years[i],i);}
      double yearIdx = yearIpl[currentYear];
      int yearIdxFloor = floor(yearIdx);
      int yearIdxCeil = ceil(yearIdx);
      if(yearIdxFloor<0) {yearIdxFloor=0;}
      if(yearIdxCeil>(nyears-1)) {yearIdxCeil=(nyears-1);}
      for(int i=0; i<12; ++i) {
	t[i]=(tempPrec[yearIdxFloor*24 + i] + tempPrec[yearIdxCeil*24 + i])/20.;
	p[i] = (tempPrec[yearIdxFloor*24 + 12 + i] +
		tempPrec[yearIdxCeil*24 + 12 + i])/2.;
      }
      mai.setTemperature(t);
      mai.setPrecipitation(p);
      mai.setWhc(site.whc);
      mai.setSoilType(site.soiltype);
      mai.setAltitude(site.altitude);
      mai.setLatitude(static_cast<double>(site.latitude)*M_PI/180.);
      if(clat != static_cast<int>(site.latitude)) {
	cerr << static_cast<int>(site.latitude) << endl;
	clat = static_cast<int>(site.latitude);
      }
      for(int irri=0; irri<2; ++irri) {
	if(irri) {
	  if(forestShareIrri <=0) {continue;}
	  mai.setPrecipitation(p+80.); //Irrigation
	}
	valarray<double> rmai = mai.getMaiB(mai.testBoundaries());
	double mmai = rmai.max();
	if(forestShare <= 0. && mmai < 0.7) { //Threshold for mai on areas with no forest
	  mmai=0.; rmai=0.;}
	if(mmai > 0.) {
	  grid[globiomCol][globiomRow][altiClass].maiSumBest[decade] += mmai*site.landshare*site.gridsize;
	  grid[globiomCol][globiomRow][altiClass].maiAreaBest[decade] += site.landshare*site.gridsize;
	}
	for(int i=0; i<numberOfMaiTypes; ++i) {
	  if(rmai[i] > 0.) {
	    grid[globiomCol][globiomRow][altiClass].maiSum[i][decade] += rmai[i]*site.landshare*site.gridsize;
	    grid[globiomCol][globiomRow][altiClass].maiArea[i][decade] += site.landshare*site.gridsize;
	  }
	}
	
	if(mmai > 0.) {
	  if(irri) {
	    grid[globiomCol][globiomRow][altiClass].currentForestAreaIrri[decade] += forestShareIrri*site.gridsize;
	  } else {
	    grid[globiomCol][globiomRow][altiClass].currentForestArea[decade] += forestShare*site.gridsize;
	    grid[globiomCol][globiomRow][altiClass].potentialForestArea[decade] += site.landshare*site.gridsize;
	  }
	  if(decade == 0 && !irri) {
	    grid[globiomCol][globiomRow][altiClass].primaryForest += forestShare*site.primaryForest/100.*site.gridsize;
	    grid[globiomCol][globiomRow][altiClass].cstemMan += site.cstemManNoirri*(100-site.primaryForest)/100.*forestShare*site.gridsize;
	    grid[globiomCol][globiomRow][altiClass].cstemUman += site.cstemUmanNoirri*site.primaryForest/100.*forestShare*site.gridsize;
	    grid[globiomCol][globiomRow][altiClass].slopet10 += (site.slope3+site.slope6+site.slope10)/100.*forestShare*site.gridsize;
	    grid[globiomCol][globiomRow][altiClass].slope15 += site.slope15/100.*forestShare*site.gridsize;
	    grid[globiomCol][globiomRow][altiClass].slope30 += site.slope30/100.*forestShare*site.gridsize;
	    grid[globiomCol][globiomRow][altiClass].slope50 += site.slope50/100.*forestShare*site.gridsize;
	    grid[globiomCol][globiomRow][altiClass].slope50p += site.slope50p/100.*forestShare*site.gridsize;
	    grid[globiomCol][globiomRow][altiClass].lslopet10 += (site.slope3+site.slope6+site.slope10)/100.*site.landshare*site.gridsize;
	    grid[globiomCol][globiomRow][altiClass].lslope15 += site.slope15/100.*site.landshare*site.gridsize;
	    grid[globiomCol][globiomRow][altiClass].lslope30 += site.slope30/100.*site.landshare*site.gridsize;
	    grid[globiomCol][globiomRow][altiClass].lslope50 += site.slope50/100.*site.landshare*site.gridsize;
	    grid[globiomCol][globiomRow][altiClass].lslope50p += site.slope50p/100.*site.landshare*site.gridsize;
	  }
	}
      }
    }
  } while(IN.good());
  //} while(IN.good() && clat>30);
  //} while(IN.good() && clat>70);
  //[Rotation time][maiType][current/affornow/afor+30/afor+60]

  //Write out the grid data //Which is read in by 5g4gm
  // for(int row=0; row<360; ++row) {
  //   for(int col=0; col<720; ++col) {
  //    for(const auto& alti : grid[col][row]) {
  //      double area = 0.;
  //      for(const auto& pcountry : alti.second.countryLandarea) {
  // 	 area += pcountry.second;
  //      }
  //      if(area > 0) {
  //      cout << col << "," << row << "," << alti.first;
  //      for(int i=0; i<decades; ++i) {cout << ',' << alti.second.currentForestArea[i];}
  //      for(int i=0; i<decades; ++i) {cout << ',' << alti.second.currentForestAreaIrri[i];}
  //      for(int i=0; i<decades; ++i) {cout << ',' << alti.second.potentialForestArea[i];}
  //      cout << ',' << alti.second.primaryForest;
  //      cout << ',' << static_cast<int>(alti.second.shareDecidousBroadleaved);
  //      cout << ',' << static_cast<int>(alti.second.shareDecidousNeadleleaved);
  //      cout << ',' << static_cast<int>(alti.second.shareEvergreenBroadleaved);
  //      cout << ',' << static_cast<int>(alti.second.shareEvergreenNeadleleaved);
  //      for(int decade=0; decade<decades; ++decade) {
  // 	 for(int i=0; i<numberOfMaiTypes; ++i) {
  // 	   cout << ',' << alti.second.maiSum[i][decade];}}
  //      for(int decade=0; decade<decades; ++decade) {
  // 	 for(int i=0; i<numberOfMaiTypes; ++i) {
  // 	   cout << ',' << alti.second.maiArea[i][decade];}}
  //      for(int i=0; i<decades; ++i) {cout << ',' << alti.second.maiSumBest[i];}
  //      for(int i=0; i<decades; ++i) {cout << ',' << alti.second.maiAreaBest[i];}
  //      cout << ',' << alti.second.cstemMan;
  //      cout << ',' << alti.second.cstemUman;
  //      cout << ',' << alti.second.slopet10;
  //      cout << ',' << alti.second.slope15;
  //      cout << ',' << alti.second.slope30;
  //      cout << ',' << alti.second.slope50;
  //      cout << ',' << alti.second.slope50p;
  //      cout << ',' << alti.second.lslopet10;
  //      cout << ',' << alti.second.lslope15;
  //      cout << ',' << alti.second.lslope30;
  //      cout << ',' << alti.second.lslope50;
  //      cout << ',' << alti.second.lslope50p;
  //      for(const auto& pcountry : alti.second.countryLandarea) {
  // 	 cout << "," << pcountry.first << '=' << pcountry.second;}
  //      cout << endl;
  //      }
  //    }
  //   }
  // }
  // cout << endl;

  g4m::ageStruct* ageStruct[3][numberOfMaiTypes][4];
  for(int i=0; i<numberOfMaiTypes; ++i) {
    for(int j=0; j<4; ++j) {
      ageStruct[0][i][j] = new() g4m::ageStruct(&iTabShort, &ffsws, &ffhlv, &ffhle, &ffcov, &ffcoe, &ffdov, &ffdoe, 1., 0, 100, minSw, minRw, minHarv,0 , sdMax, sdMin, maiYears, .75, 1, 0., &ffsdMaxH, &ffsdMinH, maxAge);
      ageStruct[1][i][j] = new() g4m::ageStruct(&iTabNormal, &ffsws, &ffhlv, &ffhle, &ffcov, &ffcoe, &ffdov, &ffdoe, 1., 0, 100, minSw, minRw, minHarv,0 , sdMax, sdMin, maiYears, .75, 1, 0., &ffsdMaxH, &ffsdMinH, maxAge);
      ageStruct[2][i][j] = new() g4m::ageStruct(&iTabLong, &ffsws, &ffhlv, &ffhle, &ffcov, &ffcoe, &ffdov, &ffdoe, 1., 0, 100, minSw, minRw, minHarv,0 , sdMax, sdMin, maiYears, .75, 1, 0., &ffsdMaxH, &ffsdMinH, maxAge);

    }
  }
  g4m::ageStruct* ageStrUmanfor = new() g4m::ageStruct(&iTabNormal, &ffsws, &ffhlv, &ffhle, &ffcov, &ffcoe, &ffdov, &ffdoe, 1., 0, 100, minSw, minRw, minHarv,0 , sdMax, sdMin, maiYears, .75, 1, 0., &ffsdMaxH, &ffsdMinH, maxAge);
  map<int,int> afforestation {{0,1},{3,2},{6,3}}; //decade,ref

  //header
  cout << "col,row,altitudeclass[m],country[iso=share],landarea[m2],forestshare[1],primaryForestShare[1],slope0-10deg[1],slope10-15deg[1],slope15-30deg[1],slope30-50deg[1],slope>50deg[1]";
  for(int decade=0; decade<decades; ++decade) {
    cout << "," << decade << "potForestArea[ha]";
  }
  for(int decade=0; decade<decades; ++decade) {
    cout << "," << decade << "bmUnmanaged[tc/ha]";
  }
  for(int decade=0; decade<decades; ++decade) {
    vector<string> rotName = {"short","typical","long"};
    vector<string> refName = {"Cur","Afor0j","Afor30j","Afor60j"};
    for(int rot=0; rot<3; ++rot) {
      for(int ref=0; ref<4; ++ref) {
     	cout << ","<< decade << rotName[rot] << refName[ref] << "HarvestSw[tC]";
     	cout << ","<< decade << rotName[rot] << refName[ref] << "HarvestRw[tC]";
     	cout << "," << decade << rotName[rot] << refName[ref] << "TotalCut[tC]";
     	cout << "," << decade << rotName[rot] << refName[ref] << "Cost[1]";
     	cout << "," << decade << rotName[rot] << refName[ref] << "Stock[tC]";
     	cout << "," << decade << rotName[rot] << refName[ref] << "Area[ha]";
      }
    }
  }
  cout << endl;

  pair<g4m::ageStruct::v, g4m::ageStruct::v> ret;
  for(int row=0; row<360; ++row) {
    for(int col=0; col<720; ++col) {
      for(const auto& alti : grid[col][row]) {
	double area = 0.;
	for(const auto& pcountry : alti.second.countryLandarea) {
	  area += pcountry.second;
	}
	if(area > 0) {
	  cout << col+1 << "," << row+1 << "," << alti.first*altistep << ",";
	  {
	    bool first=true;
	    for(const auto& pcountry : alti.second.countryLandarea) {
	      if(first) {first=false;}
	      else {cout << " ";}
	      cout << countryCode[pcountry.first] << "=" << static_cast<int>(0.5 + 1000*pcountry.second/area)/10.;
	    }
	  }
	  cout << "," << area/16.; //Land area
	  cout << "," << alti.second.currentForestArea[0]*16./area/100.; //Forest share
	  double primaryForestShare = 0.;
	  if(alti.second.currentForestArea[0] > 0.) {
	    primaryForestShare = alti.second.primaryForest / alti.second.currentForestArea[0];
	  }
	  cout << "," << primaryForestShare;

	  double slopeSum = alti.second.slopet10 + alti.second.slope15 + alti.second.slope30 + alti.second.slope50 + alti.second.slope50p;
	  if(slopeSum > 0.) {
	    cout << "," << alti.second.slopet10/slopeSum;
	    cout << "," << alti.second.slope15/slopeSum;
	    cout << "," << alti.second.slope30/slopeSum;
	    cout << "," << alti.second.slope50/slopeSum;
	    cout << "," << alti.second.slope50p/slopeSum;
	  } else {
	    slopeSum = alti.second.lslopet10 + alti.second.lslope15 + alti.second.lslope30 + alti.second.lslope50 + alti.second.lslope50p;
	    if(slopeSum > 0.) {
	      cout << "," << alti.second.lslopet10/slopeSum;
	      cout << "," << alti.second.lslope15/slopeSum;
	      cout << "," << alti.second.lslope30/slopeSum;
	      cout << "," << alti.second.lslope50/slopeSum;
	      cout << "," << alti.second.lslope50p/slopeSum;
	    } else {
	      cout << ",1,0,0,0,0";
	    }
	  }
	  
	  for(int i=0; i<decades; ++i) {
	    cout << "," << alti.second.potentialForestArea[i]/160000.;
	  }

	  double maiMaxT0 = 0;
	  if(alti.second.maiAreaBest[0] > 0.) {
	    maiMaxT0 = alti.second.maiSumBest[0]/alti.second.maiAreaBest[0];
	  }
	  if(maiMaxT0 > 14.9) {maiMaxT0 = 14.9;}
	  double relRotUman = 1.;
	  double bmUman = 0.;
	  if(alti.second.primaryForest > 0.) {
	    bmUman = alti.second.cstemUman/alti.second.primaryForest;}
	  if(bmUman > 0) {
	    double uCur = iTab[1]->gUt(bmUman, maiMaxT0);
	    double uOpt = iTab[1]->gToptt(maiMaxT0,0);
	    if(uOpt > 0.) {
	      relRotUman = uCur/uOpt;
	      if(relRotUman > 10.) {relRotUman = 10.;}
	      if(relRotUman < .1) {relRotUman = .1;}
	      if(relRotUman*uOpt > 599.) {uOpt = 599./relRotUman;}
	      ageStrUmanfor->setMai(maiMaxT0);
	      ageStrUmanfor->setAvgMai(maiMaxT0);
	      ageStrUmanfor->setU(relRotUman*uOpt);
	      ageStrUmanfor->createNormalForest(relRotUman*uOpt, 1., 1.);
	    }
	  }
	  for(int decade=0; decade<decades; ++decade) {
	    double maiMax = 0;
	    if(alti.second.maiAreaBest[decade] > 0.) {
	      maiMax = alti.second.maiSumBest[decade]/alti.second.maiAreaBest[decade];
	    }
	    double uOpt = iTab[1]->gToptt(maiMax,0); //eventuell gTopt()
	    if(relRotUman*uOpt > 599.) {uOpt = 599./relRotUman;}
	    if(ageStrUmanfor->getArea() < 1.) {
	      ageStrUmanfor->afforest(1. - ageStrUmanfor->getArea());
	    }
	    ageStrUmanfor->setU(relRotUman*uOpt);
	    ageStrUmanfor->aging(maiMax);
	    if(maiMax > 0.) {
	      cout << "," << ageStrUmanfor->getBm();
	    } else {
	      ageStrUmanfor->deforest(ageStrUmanfor->getArea(),0);
	      cout << ",0";
	    }
	  }

	  vector<int> typZuordnung = {2,3,0,2,3,0,2,3,0,1,3,0,1};
	  vector< vector<double> > refind = {{2,5,8,11}, {9,12}, {0,3,6}, {1,4,7,10}};
	  vector< vector<double> > maiPerType(4);
	  vector<double> maiSingle(numberOfMaiTypes);
	  for(int i=0; i<numberOfMaiTypes; ++i) {
	    double mai=0;
	    if(alti.second.maiArea[i][0] > 0.) {
	      mai = alti.second.maiSum[i][0] / alti.second.maiArea[i][0];
	    }
	    maiPerType[typZuordnung[i]].push_back(mai);
	    maiSingle[i] = mai;
	  }
	  int maiMaxIndex = distance(maiSingle.begin(), max_element(maiSingle.begin(), maiSingle.end()));
	  //Distribute Forest types
	  vector<int> share = {static_cast<int>(alti.second.shareDecidousBroadleaved),static_cast<int>(alti.second.shareDecidousNeadleleaved),static_cast<int>(alti.second.shareEvergreenBroadleaved),static_cast<int>(alti.second.shareEvergreenNeadleleaved)};
	  valarray<double> shareOk(4);
	  vector<double> maiMaxPerType(4);
	  vector<double> maiMaxPerTypeIndex(4);
	  for(int i=0; i<4; ++i) {
	    maiMaxPerType[i] = *max_element(maiPerType[i].begin(), maiPerType[i].end());
	    maiMaxPerTypeIndex[i] = distance(maiPerType[i].begin(), max_element(maiPerType[i].begin(), maiPerType[i].end()));
	    if(maiMaxPerType[i] > 0.) {
	      shareOk[i] = share[i];
	    }
	  }
	  if(shareOk.sum() > 0.) {
	    shareOk /= shareOk.sum();
	  } else {
	    shareOk = 0.;
	    shareOk[distance(maiMaxPerType.begin(), max_element(maiMaxPerType.begin(), maiMaxPerType.end()))] = 1.;
	  }
	  vector<double> shareSingle(numberOfMaiTypes);
	  for(int i=0; i<4; ++i) {
	    if(shareOk[i] > 0.) {
	      shareSingle[refind[i][maiMaxPerTypeIndex[i]]] = shareOk[i];
	    }
	  }
	  //Initialize the forests
	  double bmMan = alti.second.cstemMan/(alti.second.currentForestArea[0] - alti.second.primaryForest);
	  for(int maiType=0; maiType<numberOfMaiTypes; ++maiType) {
	    for(int rotTime=0; rotTime<3; ++rotTime) {
	      double u=0;
	      if(maiSingle[maiType] > 0. && shareSingle[maiType] > 0.) {
		u=iTab[rotTime]->gUt(bmMan, maiSingle[maiType]);
	      }
	      for(int j=0; j<4; ++j) {
		ageStruct[rotTime][maiType][j]->setMai(maiSingle[maiType]);
		ageStruct[rotTime][maiType][j]->setAvgMai(maiSingle[maiType]);
		ageStruct[rotTime][maiType][j]->setU(u);
		//ageStruct[rotTime][maiType][j]->deforest(ageStruct[rotTime][maiType][j]->getArea(),0);
		unsigned int maxageClasses = ageStruct[rotTime][maiType][j]->getMaxAge() / 10;
		for(unsigned int i=0; i<maxageClasses; ++i) {
		  ageStruct[rotTime][maiType][j]->setArea(i,0.);
		  ageStruct[rotTime][maiType][j]->setBm(i,0.);
		}
		if(u > 0. && j == 0) {
		  double forestArea = alti.second.currentForestArea[0] * shareSingle[maiType];
		  if(forestArea < 0.) {forestArea = 0.;}
		  ageStruct[rotTime][maiType][j]->createNormalForest(u, forestArea, 1.);
		  //ageStruct[rotTime][maiType][j]->createNormalForest(u, alti.second.currentForestArea[0]*shareSingle[maiType], 1.);
		}
		if(u > 0. && j==1 && maiType==maiMaxIndex) {
		  ageStruct[rotTime][maiType][j]->afforest(1);
		}
	      }
	    }
	  }

	  for(int decade=0; decade<decades; ++decade) {
	    for(int i=0; i<numberOfMaiTypes; ++i) {
	      double mai=0;
	      if(alti.second.maiArea[i][decade] > 0.) {
		mai = alti.second.maiSum[i][decade] / alti.second.maiArea[i][decade];
	      }
	      maiSingle[i] = mai;
	    }
	    maiMaxIndex = distance(maiSingle.begin(), max_element(maiSingle.begin(), maiSingle.end()));
	    for(int rotTime=0; rotTime<3; ++rotTime) {
	      double uOpt=0;
	      if(maiSingle[maiMaxIndex] > 0.) {
		uOpt=iTab[rotTime]->gToptt(maiSingle[maiMaxIndex], 0);
	      }
	      if(decade == 3) {//make new afforestations
		ageStruct[rotTime][maiMaxIndex][2]->afforest(1);
	      }
	      if(decade == 6) {//make new afforestations
		ageStruct[rotTime][maiMaxIndex][3]->afforest(1);
	      }
	      for(int j=0; j<4; ++j) {
		double harvestSw=0.;
		double harvestRw=0.;
		double totalCut=0.;
		double cost=0.;
		double stock=0.;
		double areaTotal=0.;
		for(int maiType=0; maiType<numberOfMaiTypes; ++maiType) {
		  //Set uOpt from the best mai to all other so that they don't
		  //get very long rotation times if their mai is low
		  ageStruct[rotTime][maiType][j]->setU(uOpt);
		  double area = ageStruct[rotTime][maiType][j]->getArea();
		  if(area > 0.) {
		    ret = ageStruct[rotTime][maiType][j]->aging(maiSingle[maiType]);
		    harvestSw += ret.first.sw + ret.second.sw*ret.second.area;
		    harvestRw += ret.first.rw + ret.second.rw*ret.second.area;
		    totalCut += ret.first.bm + ret.second.bm*ret.second.area;
		    cost += ret.first.co + ret.second.co*ret.second.area;
		    stock += area * (ageStruct[rotTime][maiType][j]->getBm());
		    areaTotal += area;
		  }
		}
		if(areaTotal > 0.) {
		  cout << "," << harvestSw/areaTotal/10.; //m3/ha/year
		  cout << "," << harvestRw/areaTotal/10.; //m3/ha/year
		  cout << "," << totalCut/areaTotal/10.; //m3/ha/year
		  cout << "," << cost/areaTotal/10.; //x/ha/year
		  cout << "," << stock/areaTotal; //m3/ha
		  //cout << "," << areaTotal;
		} else {cout << ",0,0,0,0,0";}
		//Flaechen der Juengesten Altersklasse wegnehmen
		//Und bei der Besten dazugeben
		double areaSum=0.;
		for(int maiType=0; maiType<numberOfMaiTypes; ++maiType) {
		  areaSum += ageStruct[rotTime][maiType][j]->getArea(0);
		  ageStruct[rotTime][maiType][j]->setArea(0,0.);
		}
		ageStruct[rotTime][maiMaxIndex][j]->setArea(0,areaSum);
	      }
	    }
	  }
	  cout << endl;
	}
      }
    }
  }
  return(0);
}
