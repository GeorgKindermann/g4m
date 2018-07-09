#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

#include "../ageStruct.h"

using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int main() {
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

  double mai=4.;
  double u=349.;
  double sdMin=0.96;
  double sdMax=0.96;
  unsigned int maiYears = 30;
  double minRot = 58.;
  double minSw, minRw, minHarv;
  minSw = minRw = minHarv = 0.;
  int maxAge=600;
  g4m::ageStruct forest
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
  forest.setObjOfProd(3); //maximize increment
  pair<g4m::ageStruct::v, g4m::ageStruct::v> ret;
  
  ifstream fpi;
  fpi.open("./result/nppG4mTree30.asc", ios::in);
  ofstream fpo;
  fpo.open("./result/ccs30.asc");
  fpo << "#x y mai cut0 cost0 stock0 cut1 cost1 stock1" << endl;
  string line;
  getline(fpi, line);
  while(getline(fpi, line)) {
    istringstream iss(line);
    vector<string> tokens{istream_iterator<string>{iss}, istream_iterator<string>{}};
    mai = atof(tokens[18].c_str());
    if(mai > 0.) {
      fpo << tokens[0] << " " << tokens[1] << " " << mai;
      forest.setMai(mai);
      forest.createNormalForest(100, 1., 1.);
      forest.aging();
      ret = forest.aging();
      double totalCut = ret.first.bm + ret.second.bm*ret.second.area;
      double cost = ret.first.co + ret.second.co*ret.second.area;
      double stock = forest.getBm();
      fpo << " " << totalCut << " " << cost << " " << stock;
      ret = forest.aging();
      totalCut = ret.first.bm + ret.second.bm*ret.second.area;
      cost = ret.first.co + ret.second.co*ret.second.area;
      stock = forest.getBm();
      fpo << " " << totalCut << " " << cost << " " << stock << endl;
    }
  }
}
