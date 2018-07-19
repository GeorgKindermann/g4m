#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <iterator>
#include "/home/georg/prg/g4m/increment/mai.h"
#include "/home/georg/prg/g4m/increment/ageStruct.h"

using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int main(int argc, char **argv) {
  cout << "x y area";
  for(int i=2006; i<2051; ++i) {cout << " h" << i;}
  for(int i=2006; i<2051; ++i) {cout << " c" << i;}
  for(int i=2006; i<2051; ++i) {cout << " s" << i;}
  cout << endl;
  
    g4m::incrementCurvesHf rtNormal  //Average rotation times
    (150, 8.0, 0., -1.4, -0.3,
     0., 0.9, -0.8242, -0.4273, -0.4, -1.476, 4.283, -0.3, 3.610, -1.071,
     0.1, 1., -2.0670, -0.3028, 0.5, 1.5, 150, 0.01, 0.5, 0.5,
     0.8, 1./500., 2., 0.01, 0.5, 22.09082, 0.62065,-0.02, 1.50614, -0.25346,
     22.70, 16.56, -0.01, 0.24754, -1.81373, 1.0945, 0.0999, -1.6033,
     1.6, 0.95, 1.5);
    g4m::incrementTab iTab(rtNormal,
			   5,      //maiMax
			   0.25,   //maiStep
			   600,    //tMax
			   1,      //tStep
			   0.25,   //sdNatStep
			   1.5,    //sdTabMax
			   0.25,   //sdTabStep
			   1.);    //timeframe
 
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

    g4m::ffipol<double> ffsws(0);
    sws.clear();
    sws.insert(10, .0);
    sws.insert(30, .6);
    ffsws.overwrite(sws);
    
    g4m::ffipol<double> ffhlv(hlv);
    g4m::ffipol<double> ffhle(hle);
    g4m::ffipolm<double> ffcov(cov);
    g4m::ffipolm<double> ffcoe(coe);
    g4m::ffipolm<bool> ffdov(dov);
    g4m::ffipolm<bool> ffdoe(doe);
    g4m::ffipol<double> ffsdMaxH(sdMaxH);
    g4m::ffipol<double> ffsdMinH(sdMinH);
  
   g4m::mai maiOld({2, 0.5, 0.841, -22.131},{1.29460152,-0.09012495,0.17332495, 35, -1, 0.66170523, 2.8, 0.4138984, -54.4741443, -1.4, 1.155907e+00, 2.154028e-04, -3.733458e-01, 2.335792e-05},{0.06179233,0.06448844,0.07000044,0.07867775,0.06506758,0.08137664,0.06192571,0.07169721,0.07110523,0.06381677,0.05441309,0.06347873,0.07584091,0.07330926,0.05766713,0.07205265,0.05055277,0.06077571,0.07759581,0.05685617,0.06527024,0.05558023,0.06699292},std::valarray<double>(10., 24),std::valarray<double>(70., 24),100,0,0.038,0,0.,0.,0.8,0);
  maiOld.setSwr(0.8);
  maiOld.setcNpp2mai({1./3.});
  maiOld.setWeatherAsDynamic(true);

  //Forest
  maiOld.setCoef(0, {1.29460152,-0.09012495,0.17332495, 35, -1, 0.66170523, 2.8, 0.4138984, -54.4741443, -1.4, 1.155907e+00, 2.154028e-04, -3.733458e-01, 2.335792e-05});
  maiOld.setCoefC0(0, {0.06179233,0.06448844,0.07000044,0.07867775,0.06506758,0.08137664,0.06192571,0.07169721,0.07110523,0.06381677,0.05441309,0.06347873,0.07584091,0.07330926,0.05766713,0.07205265,0.05055277,0.06077571,0.07759581,0.05685617,0.06527024,0.05558023,0.06699292});
  maiOld.setBoundaries(0, -12.0 - 2, 28.0 + 2., 203., 5900. + 4000., -43.1 - 4, 32.5 + 5., 0, 1045 + 1100, 1.86);
  //evergreen needleleaf forest
  maiOld.setCoef(1, {1.473547e+00,-9.469511e-02,1.855580e-01,3.500000e+01,-1.000000e+00,1.018910e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,9.349661e-01,1.698924e-04,-4.095052e-02,-1.261350e-04});
  maiOld.setCoefC0(1, {0.07200521,0.11960568,0.09408846,0.10690904,0.08503689,0.09664446,0.08917135,0.12555602,0.10778141,0.08308198,0.11212895,0.11297067,0.11626721,0.10273018,0.12039956,0.11695393,0.06977949,0.10038106,0.14010804,0.08497696,0.12136300,0.10823336,0.10055720});
  maiOld.setBoundaries(1, -10.1 - 4, 28.0 + 2., 176., 4100. + 4000., -39.1 - 7, 29.2 + 5., 1, 562 + 2000, 0.79);
  //evergreen broadleaf forest
  maiOld.setCoef(2, {1.529964e+00,-9.626790e-02,1.892103e-01,3.500000e+01,-1.000000e+00,1.065803e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,2.006863e+00,-2.252521e-05,-1.190847e+00,3.509442e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiOld.setCoefC0(2, {0.06278559,0.05451733,0.07155252,0.07154880,0.06331560,0.07405309,0.05890702,0.06341198,0.06168992,0.06258061,0.04919143,0.05810440,0.08030827,0.07632525,0.05871569,0.06240980,0.05162692,0.05707812,0.07945510,0.05807941,0.06134032,0.05378656,0.06523997});
  maiOld.setBoundaries(2, 4.1 + 7,28.0 + 2.5,480. - 100,6900. + 4000.,-3.6 + 3.,33.5 + 2.5,0,1300 + 2000,2.98);
  //deciduous needleleaf forest
  maiOld.setCoef(3, {1.575326e+00,-9.890705e-02,1.966154e-01,3.500000e+01,-1.000000e+00,1.361797e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,8.640303e-01,2.381869e-04,-5.982080e-02,-5.153793e-05,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiOld.setCoefC0(3, {0.1226366,0.1394199,0.1373671,0.1393298,0.1215172,0.1293482,0.1245032,0.1393599,0.1337010,0.1171449,0.1229379,0.1227410,0.1484251,0.1228124,0.1469167,0.1251016,0.1210288,0.1285934,0.1523116,0.1243452,0.1535025,0.1284694,0.1011582});
  maiOld.setBoundaries(3,-14.1 - 2.3,18.6 + 2.5,200. - 0,6900. + 4000.,-45. - 3.,28. + 5.5,2,1300 + 2000,1.19);
  //deciduous broadleaved forest
  maiOld.setCoef(4, {1.494767e+00,-9.632663e-02,1.886172e-01,3.500000e+01,-1.000000e+00,3.584477e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,1.107488e+00,1.957089e-04,-5.395287e-01,1.181403e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiOld.setCoefC0(4, {0.08172423,0.12588981,0.07748284,0.11167975,0.07765499,0.10551991,0.08170487,0.06883372,0.09889102,0.09630474,0.08749524,0.07827634,0.15207574,0.08388147,0.12978135,0.09060370,0.08727573,0.10390721,0.14516723,0.09127422,0.07513953,0.10896841,0.11420354});
  maiOld.setBoundaries(4, -8.2 - 2.3,28.3 + 1.,235. - 0,6900. + 4000.,-32.9 - 9.,28. + 5.5,0,1300 + 2000,0.66);
  //Tropical evergreen broadleaf
  maiOld.setCoef(5, {1.619920e+00,-9.848997e-02,1.970912e-01,3.500000e+01,-1.000000e+00,5.963311e-01,2.800000e+00,0.4138984,-54.4741443,-1.4,1.941317e+00,-9.634048e-05,-1.100476e+00,4.401370e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiOld.setCoefC0(5, {0.06136063,0.05394037,0.06884074,0.06795948,0.06069805,0.07048979,0.05777229,0.06212813,0.05874720,0.06193419,0.04811436,0.05803750,0.07435434,0.07529665,0.05663168,0.06045755,0.04999882,0.05534675,0.07479968,0.05449085,0.05932120,0.05244837,0.06286373});
  maiOld.setBoundaries(5,10.1 + 8.,28.3 + 3.,600. - 0,6900. + 4000.,7.2 + 4.1,33.6 + 2.2,0,1300 + 2000,4.6);
  //Tropical deciduous broadleaf
  maiOld.setCoef(6, {1.491055e+00,-9.734815e-02,1.951930e-01,3.500000e+01,-1.000000e+00,2.144010e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,1.018970e+00,5.690775e-04,-2.743858e-01,-3.154196e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiOld.setCoefC0(6, {0.06836858,0.06813469,0.05934084,0.07794042,0.05072694,0.06129334,0.06565496,0.04773324,0.07122714,0.08419983,0.05885219,0.06289404,0.09646647,0.06666715,0.09231248,0.03816949,0.04651217,0.07239042,0.10244473,0.07365863,0.05660622,0.08033533,0.07028939});
  maiOld.setBoundaries(6,8.3 + 9.8 - 3,28.5 + 1.,225. - 0,6900. + 4000.,1.9 + 9.4 - 3,35.2 + 1.,0,1300 + 2000,1.13);
   //Subtropical evergreen broadleaf
    maiOld.setCoef(7, {1.538287e+00,-9.722637e-02,1.894521e-01,3.500000e+01,-1.000000e+00,1.249044e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,1.656060e+00,2.328051e-04,-1.522926e+00,1.101988e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiOld.setCoefC0(7, {0.11606259,0.09230848,0.11152179,0.07736606,0.10174771,0.10232312,0.09956373,0.09869678,0.12651927,0.11027624,0.09064032,0.14575220,0.09281512,0.09487696,0.13310920,0.08328069,0.10088982,0.12739049,0.14853313,0.12082426,0.10963328,0.16620110,0.10230552});
  maiOld.setBoundaries(7,3.4 + 9,23.7 - 5. + 2.5,290. - 0,6900. + 4000.,-5.8 + 7.,29.9 + 2.,0,1300 + 2000,0.85);
  //Temperate evergreen needleleaf
  maiOld.setCoef(8, {1.570108e+00,-9.829125e-02,1.945205e-01,3.500000e+01,-1.000000e+00,1.416748e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,9.220346e-01,1.322653e-04,1.418027e-01,-1.572138e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiOld.setCoefC0(8, {0.1177171,0.1308500,0.1070914,0.1288103,0.1519428,0.1280058,0.1521247,0.1562641,0.1294695,0.1486603,0.1318565,0.1337459,0.1723263,0.1428401,0.1618081,0.1389939,0.1357501,0.1216093,0.1503008,0.1142445,0.1521230,0.1520602,0.1590204});
  maiOld.setBoundaries(8,-4.9 + 5,14.6 - 2. + 3,220. - 0,6900. + 4000.,-27.2 + 0.,25.1 + 4.5,0,1300 + 2000,1.18);
  //Temperate deciduous broadleaf
  maiOld.setCoef(9, {1.574017e+00,-9.893621e-02,1.965476e-01,3.500000e+01,-1.000000e+00,1.949731e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,9.597373e-01,3.182178e-04,-5.993809e-01,-2.135231e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiOld.setCoefC0(9, {0.12156712,0.12492062,0.13087684,0.11666581,0.11019982,0.12730928,0.12320618,0.07018051,0.11312888,0.09925496,0.10062111,0.12766330,0.15216701,0.11568652,0.13923021,0.09930708,0.14071709,0.11279223
	,0.11843090,0.12642849,0.08836875,0.12722117,0.10910369});
  maiOld.setBoundaries(9,-4.9 + 5,14.6 - 2. + 3,305. - 0,6900. + 4000.,-29.7 + 0.,27.1 + 2.5,2,1300 + 2000,1.74);
  //Boreal evergreen needleleaf
  maiOld.setCoef(10, {1.591897e+00,-9.941671e-02,1.978756e-01,3.500000e+01,-1.000000e+00,1.454123e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,8.807388e-01,2.934434e-04,-9.318789e-02,1.280086e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiOld.setCoefC0(10, {0.1218631,0.1295781,0.1280500,0.1356488,0.1362415,0.1322454,0.1237829,0.1503324,0.1172669,0.1551760,0.1293965,0.1360983,0.1388701,0.1233824,0.1500347,0.1331320,0.1210412,0.1245973,0.1598617,0.1205537,0.1492726,0.1203824,0.1453270});
  maiOld.setBoundaries(10,-10.3 - 8,6.6 - 6.,175. - 0,6900. + 4000.,-39.9 - 8.,19.0 + 2.,2-2,1300 + 2000,0.53);
  //Boreal deciduous needleleaf
  maiOld.setCoef(11, {1.588311e+00,-9.953446e-02,1.982941e-01,3.500000e+01,-1.000000e+00,1.535554e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,7.624086e-01,3.815986e-04,-2.984578e-01,3.871615e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiOld.setCoefC0(11, {0.1346323,0.1450399,0.1434999,0.1450294,0.1312006,0.1314684,0.1292051,0.1472063,0.1373973,0.1556409,0.1264541,0.1303976,0.1576328,0.1361593,0.1508253,0.1275356,0.1355128,0.1356315,0.1637595,0.1318753,0.1610309,0.1345712,0.1568916});
  maiOld.setBoundaries(11,-10.3 - 8,6.6 - 6.,215. - 0,6900. + 4000.,-39.9 - 8.,19.6 + 2.,2-2,1300 + 2000,1.17);
  //Tropical evergreen needleleaf
  maiOld.setCoef(12, {1.197497e+00,-8.738827e-02,1.742595e-01,3.500000e+01,-1.000000e+00,6.502581e-01,2.800000e+00,0.4138984,-54.4741443,-1.4,1.223739e+00,-3.062217e-05,-3.844974e-01,2.294290e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiOld.setCoefC0(12, {0.03992456,0.03344327,0.04377474,0.03088140,0.02940730,0.05420901,0.04138151,0.03681954,0.03061374,0.03588355,0.03004470,0.03692928,0.04303532,0.04515249,0.03687710,0.04236888,0.02213864,0.06108095,0.03835771,0.02997647,0.03281500,0.03696203,0.04425476});
  maiOld.setBoundaries(12,4.3 + 13.8,29.1 + 2.2,200. - 0,6900. + 4000.,-3.3 + 14.6,33.5 + 2.3,0,1300 + 2000,0.64);
  //Subtropical evergreen needleleaf
  maiOld.setCoef(13, {1.528454e+00,-9.328062e-02,1.778416e-01,3.500000e+01,-1.000000e+00,1.416668e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,1.225557e+00,1.253244e-04,-9.790134e-01,1.069189e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiOld.setCoefC0(13, {0.08771526,0.06546871,0.14893432,0.08151210,0.05439819,0.08062922,0.06700593,0.06470260,0.09436382,0.06005037,0.08061899,0.10209852,0.06744991,0.11864577,0.10419921,0.07418190,0.05797453,0.10102862,0.21474297,0.05752559,0.08622142,0.10598824,0.05646660});
  maiOld.setBoundaries(13,-3.1 + 15.5,22.9 - 4.2,280. - 0,6900. + 4000.,-11. + 12.2,30.0 - 0.1,0,1300 + 2000,1.2);
  //Subtropical deciduous broadleaf forest
  maiOld.setCoef(14, {1.583149e+00,-9.801282e-02,1.927008e-01,3.500000e+01,-1.000000e+00,1.152546e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,1.414094e+00,1.465791e-05,-1.312110e+00,5.351233e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiOld.setCoefC0(14, {0.07556472,0.05721778,0.08186703,0.06351753,0.07113803,0.08661813,0.06621690,0.06202732,0.09156333,0.05902771,0.07786552,0.10107472,0.09343677,0.06269478,0.08833595,0.06189770,0.08025677,0.09015629,0.12211850,0.06349837,0.07095126,0.08909001,0.06878965});
  maiOld.setBoundaries(14,1.9 + 10.5,23.8 - 5.1,190. - 0,6900. + 4000.,-10.5 + 11.7,29.9 - 0.,1,1300 + 2000,0.15);
  //Temperate evergreen broadleaf
  maiOld.setCoef(15, {1.580856e+00,-9.891061e-02,1.960383e-01,3.500000e+01,-1.000000e+00,2.521807e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,8.793388e-01,4.410353e-04,-6.640318e-02,-6.262073e-04,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiOld.setCoefC0(15, {0.1084333,0.1069007,0.10839835,0.096417,0.1008877,0.1177522,0.1262071,0.09862685,0.11904175,0.0982001,0.0988278,0.1212168,0.12735525,0.11331315,0.11384885,0.0930662,0.1060646,0.10694685,0.1309429,0.1054626,0.12388035,0.11623495,0.1098665});
  maiOld.setBoundaries(15,-5.8 + 5.9,15.9 - 3.3 + 2.,330. - 0,6900. + 4000.,-25.7 + 0.,25.1 - 0.,1,1300 + 2000,3.18);
  //Temperate deciduous needleleaf
  maiOld.setCoef(16, {1.542234e+00,-9.740093e-02,1.922796e-01,3.500000e+01,-1.000000e+00,1.721792e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,8.450042e-01,2.795868e-04,-2.644673e-01,-9.114047e-05,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiOld.setCoefC0(16, {0.11462020,0.14112211,0.14168577,0.09944337,0.09829958,0.12095412,0.10770765,0.08238588,0.11642098,0.08694502,0.09464438,0.11746427,0.15485947,0.08529750,0.14770957,0.10658592,0.12643711,0.09626534,0.11686801,0.11183171,0.09877526,0.12544831,0.09643012});
  maiOld.setBoundaries(16,-9.0 + 9.1,15.4 - 2.8,100. + 150,6900. + 4000.,-32.6 + 0,27.9 - 0.,1,1300 + 2000,0.19);
  //Boreal deciduous broadleaf
  maiOld.setCoef(17, {1.580021e+00,-9.891136e-02,1.961251e-01,3.500000e+01,-1.000000e+00,3.816509e+00,2.800000e+00,0.4138984,-54.4741443,-1.4,8.219060e-01,1.584236e-04,-3.908123e-01,7.472023e-05,2.000000e+00,5.000000e-01,8.410000e-01,-2.213100e+01});
  maiOld.setCoefC0(17, {0.152224375,0.161903495,0.162378475,0.14314884,0.134649945,0.13786099,0.13044117,0.13519165,0.1512354,0.16078957,0.13138093,0.122985055,0.208138565,0.16509652,0.161235395,0.123882995,0.14395243,0.135572025,0.204168385,0.148326445,0.16027056,0.130733315,0.172622675});
  maiOld.setBoundaries(17,-11.4 - 2.9,7.9 - 7.3,190. - 0,6900. + 4000.,-40.9 + 1.,21.4 + 1.,2,1300 + 2000,0.47);


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

  ifstream fpi;
  //fpi.open("/data/tmp/baseData.txt", ios::in);
  fpi.open(argv[1], ios::in);
  //0lon,1lat,2x,3y,4watershare16,5DeciduousBroadleaf100,6DeciduousNeedleleaf,7EvergreenBroadleaf,8EvergreenNeedleleaf,9soilIiasaFao90,10soilIiasaSwr,11soilIiasaAwc,12elevation,13area,14Slope3,15Slope6,16Slope10,17Slope15,18Slope30,19Slope50,20SlopeL50
  ifstream fpiw;
  //fpiw.open("/data/tmp/baseDataClima.txt", ios::in);
  fpiw.open(argv[2], ios::in);
  //x,y,jahr,monat,t,p
  //ofstream fpo;
  //fpo.open("/data/tmp/nppG4mTree.asc");
  
  string oneChar;
  string line, linew;
  string px,py = "";
  vector<double> tmpt;
  vector<double> tmpp;
  getline(fpiw, linew);
  istringstream issw(linew);
  vector<string> tokensw{istream_iterator<string>{issw}, istream_iterator<string>{}};
  px = tokensw[0]; py = tokensw[1];
  getline(fpi, line);
  while(fpiw.good()) {
    istringstream issw(linew);
    vector<string> tokensw{istream_iterator<string>{issw}, istream_iterator<string>{}};
    map<unsigned int, double> cutSum;
    map<unsigned int, double> costSum;
    map<unsigned int, double> stockSum;
    double forestArea=0;
    if(px != tokensw[0] || py != tokensw[1]) {
      cout << px << " " << py;
      std::valarray<double> t(tmpt.data(), tmpt.size()); tmpt.clear();
      std::valarray<double> p(tmpp.data(), tmpp.size()); tmpp.clear();
      while(fpi.good()) {
	istringstream iss(line);
	vector<string> tokens{istream_iterator<string>{iss}, istream_iterator<string>{}};
	if(px != tokens[2] || py != tokens[3]) {break;}
	getline(fpi, line);
	
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
	
	maiOld.setTemperature(t[slice(0, 12, 1)]);
	maiOld.setPrecipitation(p[slice(0, 12, 1)]);
	maiOld.setCo2(0.038);
	maiOld.setSwr(swr);
	maiOld.setWhc(whc);
	maiOld.setAltitude(altitude);
	maiOld.setSoilType(soilType);
	maiOld.setLatitude(atof(tokens[1].c_str())*M_PI/180.);

	double mai=4.;
	double u=349.;
	double sdMin=0.96;
	double sdMax=0.96;
	unsigned int maiYears = 30;
	double minRot = 58.;
	double minSw, minRw, minHarv;
	minSw = minRw = minHarv = 0.;
	int maxAge=300;
	g4m::ageStruct forest0
	  (&iTab, &ffsws, &ffhlv, &ffhle, &ffcov, &ffcoe, &ffdov, &ffdoe, mai
	   , 0  //What stands the value of u for
	   , u  //Rotation time
	   , minSw, minRw, minHarv
	   ,0 , sdMax, sdMin, maiYears, minRot
	   , 0  //reference of minrot
	   , 0. //Flexibility of stocking degree
	   , &ffsdMaxH, &ffsdMinH
	   , maxAge
	   );
	g4m::ageStruct forest1
	  (&iTab, &ffsws, &ffhlv, &ffhle, &ffcov, &ffcoe, &ffdov, &ffdoe, mai
	   , 0, u, minSw, minRw, minHarv,0 , sdMax, sdMin, maiYears, minRot
	   , 0, 0., &ffsdMaxH, &ffsdMinH, maxAge);
	g4m::ageStruct forest2
	  (&iTab, &ffsws, &ffhlv, &ffhle, &ffcov, &ffcoe, &ffdov, &ffdoe, mai
	   , 0, u, minSw, minRw, minHarv,0 , sdMax, sdMin, maiYears, minRot
	   , 0, 0., &ffsdMaxH, &ffsdMinH, maxAge);
	g4m::ageStruct forest3
	  (&iTab, &ffsws, &ffhlv, &ffhle, &ffcov, &ffcoe, &ffdov, &ffdoe, mai
	   , 0, u, minSw, minRw, minHarv,0 , sdMax, sdMin, maiYears, minRot
	   , 0, 0., &ffsdMaxH, &ffsdMinH, maxAge);

	std::valarray<g4m::ageStruct*> aStr(4);
	aStr[0] = &forest0;
	aStr[1] = &forest1;
	aStr[2] = &forest2;
	aStr[3] = &forest3;
	
	//Hier je Gruppe den groesten MAI raussuchen und dann bei diesen Spalte bleiben (Kein Wechsel der Oekoregion)
	int nppCol[4] = {0,0,0,0};
	valarray<bool> nppNotUse(true, 17);

	forestArea += atof(tokens[13].c_str());
	for(unsigned int jahr=0; jahr<t.size()/12; ++jahr) {
	  maiOld.setTemperature(t[slice(jahr*12, 12, 1)]);
	  maiOld.setPrecipitation(p[slice(jahr*12, 12, 1)]);
	  if(jahr == 0) {
	    maiOld.setTemperature(t[slice(jahr*12, 12, 1)]);
	    maiOld.setPrecipitation(p[slice(jahr*12, 12, 1)]);
	    std::valarray<double> rmai = maiOld.getMai(maiOld.testBoundaries(), true);
	    //Alle: 0
	    //Nadel immergruen: 1  8,10,12,13
	    {
	      int idx[] = {8,10,12,13};
	      vector<double> tmp = {rmai[idx[0]], rmai[idx[1]], rmai[idx[2]], rmai[idx[3]]};
	      if(*max_element(tmp.begin(), tmp.end()) > 0.) {
		nppCol[0] = idx[distance(tmp.begin(), max_element(tmp.begin(), tmp.end()))];
	      } else {nppCol[0] = 1;}
	    }
	    //Laub immergruen: 2  5,7,15
	    {
	      int idx[] = {5,7,15};
	      vector<double> tmp = {rmai[idx[0]], rmai[idx[1]], rmai[idx[2]]};
	      if(*max_element(tmp.begin(), tmp.end()) > 0.) {
		nppCol[1] = idx[distance(tmp.begin(), max_element(tmp.begin(), tmp.end()))];
	      } else {nppCol[1] = 2;}
	    }
	    //Nadel abwerfend: 3  11,14,16
	    {
	      int idx[] = {11,14,16};
	      vector<double> tmp = {rmai[idx[0]], rmai[idx[1]], rmai[idx[2]]};
	      if(*max_element(tmp.begin(), tmp.end()) > 0.) {
		nppCol[2] = idx[distance(tmp.begin(), max_element(tmp.begin(), tmp.end()))];
	      } else {nppCol[2] = 3;}
	    }
	    //Laub abwerfend: 4  6,9,17
	    {
	      int idx[] = {6,9,17};
	      vector<double> tmp = {rmai[idx[0]], rmai[idx[1]], rmai[idx[2]]};
	      if(*max_element(tmp.begin(), tmp.end()) > 0.) {
		nppCol[3] = idx[distance(tmp.begin(), max_element(tmp.begin(), tmp.end()))];
	      } else {nppCol[3] = 4;}
	    }
	    for(int i=0; i<4; ++i) {
	      nppNotUse[nppCol[i]] = false;
	      double mai = rmai[nppCol[i]];
	      double biomass = atof(tokens[21].c_str());
	      int biomasRot = iTab.gUt(biomass,mai);
	      aStr[i]->setMai(min(3.5,mai));  //Achtund hier begrenzen
	      aStr[i]->setAvgMai(mai);
	      aStr[i]->createNormalForest(biomasRot + 2, 1., 1.);
	      aStr[i]->setObjOfProd(3); //maximize increment
	      aStr[i]->aging();
	      aStr[i]->aging();
	    }
	  }
	  std::valarray<double> rmai = maiOld.getMai(nppNotUse, false);
	  
	  for(int i=0; i<4; ++i) {
	    aStr[i]->setMai(min(3.5,rmai[nppCol[i]])); //Achtund hier begrenzen
	    pair<g4m::ageStruct::v, g4m::ageStruct::v> ret = aStr[i]->aging();
	    //double harvestSw = ret.first.sw + ret.second.sw*ret.second.area;
	    //double harvestRw = ret.first.rw + ret.second.rw*ret.second.area;
	    double totalCut = ret.first.bm + ret.second.bm*ret.second.area;
	    double cost = ret.first.co + ret.second.co*ret.second.area;
	    double stock = aStr[i]->getBm();
    totalCut *= atof(tokens[8-i].c_str()) * atof(tokens[13].c_str()) / 100.;
    cost *= atof(tokens[8-i].c_str()) * atof(tokens[13].c_str()) / 100.;
    stock *= atof(tokens[8-i].c_str()) * atof(tokens[13].c_str()) / 100.;
            cutSum[jahr] += totalCut;
	    costSum[jahr] += cost;
	    stockSum[jahr] += stock;
	  }

	  //
	  //Und dann diesen mit dem Anteil (laub,nadel,immergruen,blattabwerfend) und Waldflaeche multiplizieren und je Klimagrid je Jahr und Baumartengruppe aufsummieren oder gleich in agestruct verwenden
	  //Anschliesend je Klimagrid mit diesem mittleeren MAI je Baumartengruppe Altersklassen bilden und Stock und Harvest ausgeben.
	  //Eventuell Datensatz aufteilen und paralel abarbeiten-Dateiname

	}

      }
      cout << " " << forestArea;
      for(const auto i : cutSum) {cout << " " << i.second;}
      for(const auto i : costSum) {cout << " " << i.second;}
      for(const auto i : stockSum) {cout << " " << i.second;}
      cout << endl;
    }
    
    cutSum.clear();
    costSum.clear();
    stockSum.clear();
    forestArea = 0;
    
    px = tokensw[0];
    py = tokensw[1];
    tmpt.push_back(atof(tokensw[4].c_str()));
    tmpp.push_back(atof(tokensw[5].c_str()));
    getline(fpiw, linew);
  }
  return(0);
}
