#include <iostream>
#include "ageStruct.h"

using namespace std;

int main(int argc, char **argv) {
  g4m::incrementTab tabPine (-0.3835, -0.2416, -1.7576, 1.1638, 170, 114.343, -2.804, 1.044, 0.9, -0.8242, -0.4273, -0.4, -1.476, 4.283, -0.3, 3.610, -1.071, 0.1, -2.0670, -0.3028, 0.5, 1.5, 150, 0.01, 0.5, 0.5, 0.8, 1./500., 2., 0.01, 0.5, 22.09082, 0.62065, -0.01965, 1.50614, -0.25346, 22.70, 16.56, -0.01068, 0.24754, -1.81373, 1.0945, 0.0999, -1.6033, 1.6, 0.95, 5., 0.25, 600, 1, 0.25, 1.5, 0.25, 10.);
  g4m::ipol<double,double> sws;  //Schnittholzanteil an Vfm
  g4m::ipol<double,double> hlv;   //1-Ernteverluste Vornutzung
  g4m::ipol<double,double> hle;   //1-Ernteverluste Endnutzung
  g4m::ipol<vector<double>,double> cov;  //costs vornutzung
  g4m::ipol<vector<double>,double> coe;  //Costs endnutzung
  g4m::ipol<vector<double>,bool> dov;  //Do vornutzung
  g4m::ipol<vector<double>,bool> doe;  //Do endnutzung

  sws.insert(10, .0);
  sws.insert(30, .6);

  hlv.insert(0, .0);
  hlv.insert(25, .7);
  hle.insert(0, .0);
  hle.insert(25, .75);

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

  double mai=1.5;
  double u=100.;
  double sdMin=1.;
  double sdMax=1.;
  unsigned int maiYears = 30;
  double minRot = 60.;
  double minSw, minRw, minHarv;
  minSw = minRw = minHarv = 0.;

  g4m::ageStruct forest
    (&tabPine, &ffsws, &ffhlv, &ffhle, &ffcov, &ffcoe, &ffdov, &ffdoe, mai
     , 0  //What stands the value of u for
     , u  //Rotation time
     , minSw, minRw, minHarv
     , sdMin, sdMax, maiYears, minRot
     , 0  //reference of minrot
     , 0. //Flexibility of stocking degree
     );

  g4m::ageStruct forest2
    (&tabPine, &ffsws, &ffhlv, &ffhle, &ffcov, &ffcoe, &ffdov, &ffdoe, mai
     , 0  //What stands the value of u for
     , u  //Rotation time
     , minSw, minRw, minHarv
     , sdMin, sdMax, maiYears, minRot
     , 0  //reference of minrot
     , 0. //Flexibility of stocking degree
     );

  forest.createNormalForest(124.3, 1., 1.);
  forest.setBm(4, 38.);
  forest.setArea(4, .1);
  forest.setD(15, 100.);
  for(int i=0; i<165; ++i) {
    cout << i
	 << "\t" << forest.getArea(i)
	 << "\t" << forest.getBm(i)
	 << "\t" << tabPine.gBmt(i, mai)
	 << "\t" << forest.getD(i)
	 << "\t" << tabPine.gDbht(i, mai)
	 << "\t" << forest.getH(i)
	 << "\t" << tabPine.gHeight(i, mai)
	 << endl;
  }
  cout << forest.getArea() << "\t" << forest.getBm() << endl;

  forest.createNormalForest(100, 1., 1.);
  {
    pair<g4m::ageStruct::v, g4m::ageStruct::v> ret;
    for(int i=0; i<20; ++i) {
      ret = forest.aging();
      cout << i
	   << "\t" << ret.first.area
	   << "\t" << ret.first.sw
	   << "\t" << ret.first.rw
	   << "\t" << ret.first.co
	   << "\t" << ret.first.bm
	   << "\t" << ret.second.area
	   << "\t" << ret.second.sw
	   << "\t" << ret.second.rw
	   << "\t" << ret.second.co
	   << "\t" << ret.second.bm
	   << endl;;
    }
  }

  forest.createNormalForest(100, 1., 1.);
  forest2.createNormalForest(100, 1., 1.);
  forest.aging(); forest.aging(); forest.aging();
  for(int i=0; i<120; i+=10) {
    cout << i
	 << "\t" << forest.getArea(i)
	 << "\t" << forest2.getArea(i)
	 << "\t" << forest.getBm(i)
	 << "\t" << forest2.getBm(i)
	 << "\t" << forest.getD(i)
	 << "\t" << forest2.getD(i)
	 << "\t" << forest.getH(i)
	 << "\t" << forest2.getH(i)
	 << endl;
  }


  return(0);
}

/*
  g4m::incrementTab fi(-4.25, -1.39, 0.329, 169., 81., 4.48, -1.39, 0.780,
                  -0.176, 1./4.5, 0.5, -1.08, -7.7956, -0.2362, -0.6316,
                  24.62079, 0.46573, -0.02426, 2.58884, -0.30364, -0.5782,
		   0.8513, 0.0289, 1.4720, 0.5, 2., 1.6, 10., 0.25, 600., 1.);
  //cout << fi.gTopt(1.5, 1) << endl;
  //cout << fi.gMai(99, 1.5) << endl;
  g4m::ipol<double,double> sws;  //Schnittholzanteil an Vfm
  g4m::ipol<double,double> hlv;   //1-Ernteverluste Vornutzung
  g4m::ipol<double,double> hle;   //1-Ernteverluste Endnutzung
  g4m::ipol<double,double> dbv;  //Dekungsbeitrag vornutzung
  g4m::ipol<double,double> dbe;  //Dekungsbeitrag endnutzung

  sws.insert(10, .0);
  sws.insert(30, .6);

  hlv.insert(0, .0);
  hlv.insert(25, .7);
  hle.insert(0, .0);
  hle.insert(25, .7);

  dbv.insert(0, 2);
  dbe.insert(0, 3);

  //sws.insert(7, 0.);
  //sws.insert(50, .6);
  //hlv.insert(0, 0.);
  //hlv.insert(10, .5);
  //hlv.insert(50, .7);
  //hle.insert(0, 0.);
  //hle.insert(10, .5);
  //hle.insert(50, .7);
  //dbv.insert(0, -2.);
  //dbv.insert(15, 0.);
  //dbv.insert(70, 4.);
  //dbe.insert(0, -1.);
  //dbe.insert(10, 0.);
  //dbe.insert(70, 6.);

  //Test long time
  {
    cout << "time\trot\tharvest" << endl;
    double mai = 3.128;
    double biomass = 79.1728;
    int biomasRot = fi.gUt(biomass,mai,1);
    cout << biomasRot << endl;
    double curRot = 90.;
    g4m::ageStruct cohort(&fi, &sws, &hlv, &hle, &dbv, &dbe, 0, 0, mai, curRot, 1., 1., 0.75);
    g4m::ageStruct::v res;
    int age=0;
    for(;age<20;++age) {
      res = cohort.aging();
      cout << age << "\t" << curRot << "\t" << res.enSw+res.enRw+res.vnSw+res.vnRw << endl;
    }
    curRot = fi.gTopt(mai, 3);
    cohort.setRotPeriod(curRot);
    for(;age<400;++age) {
      res = cohort.aging();
      cout << age << "\t" << curRot << "\t" << res.enSw+res.enRw+res.vnSw+res.vnRw << endl;
    }
    curRot = fi.gTopt(mai, 1);
    cohort.setRotPeriod(curRot);
    for(;age<=600;++age) {
      res = cohort.aging();
      cout << age << "\t" << curRot << "\t" << res.enSw+res.enRw+res.vnSw+res.vnRw << endl;
    }
  }
  return(0);

//g4m::ageStruct cohort(&fi, &sws, &hlv, &hle, &dbv, &dbe, 0, 0, 1.5, 100, 1., 1., 0.75);
  double mai = 1.5;
  double optRot = fi.gTopt(mai, 5);
  cout << optRot << endl;
  g4m::ageStruct cohort(&fi, &sws, &hlv, &hle, &dbv, &dbe, 0, 0, mai, optRot, -1., 1., 0.75);
  //cohort.afforest(100.);
  //cohort.deforest(50., 1);
  g4m::ageStruct::v res;
  for(int i=0; i<120; ++i) {
    res = cohort.aging();
    cout << i << " " << res.area << " " << res.enSw << " " << res.enRw << " " << res.vnSw;
    cout << " " << res.vnRw << " " << res.dbEn << " " << res.dbVn << endl;
  }
  cout << cohort.getArea() << endl;

  cohort.setMai(3.);
  cohort.setRotPeriod(70);
  cohort.setStockingdegree(-1.);
  cohort.createNormalForest(70, 1, -1.);
  cout << cohort.getBm() << endl;
  cohort.aging();
  cout << cohort.getBm() << endl;

  cout << "fipol Begin" << endl;
  g4m::ageLUT lut(&cohort);
  cout << lut.getBm(3., 70) << endl;
  cout << lut.getBmNT(3., 70) << endl;
  cout << lut.getEnSw(3., 70) << endl;
  cout << lut.getEnRw(3., 70) << endl;
  cout << lut.getVnSw(3., 70) << endl;
  cout << lut.getVnRw(3., 70) << endl;
  cout << lut.getDbEn(3., 70) << endl;
  cout << lut.getDbVn(3., 70) << endl;
  cout << lut.getEnSwNT(3., 70) << endl;
  cout << lut.getEnRwNT(3., 70) << endl;
  cout << lut.getDbEnNT(3., 70) << endl;
  cout << "fipol End" << endl;
  cout << lut.gTopt(3., 1) << endl;
  cout << lut.gTopt(2., 1) << endl;
  cout << lut.gTopt(1., 1) << endl;
  cout << lut.gTopt(3., 2) << endl;
  cout << lut.gTopt(2., 2) << endl;
  cout << lut.gTopt(1., 2) << endl;

  //Test more cohorts at the same time
  {
    cout << "Test more cohorts at the same time" << endl;
    g4m::ageStruct cohort1(&fi, &sws, &hlv, &hle, &dbv, &dbe, 0, 0, mai, optRot, -1., 1., 0.75);
    g4m::ageStruct cohort2(&fi, &sws, &hlv, &hle, &dbv, &dbe, 0, 0, mai, optRot, -1., 1., 0.75);
    g4m::ageStruct cohort3(&fi, &sws, &hlv, &hle, &dbv, &dbe, 0, 0, mai, optRot, -1., 1., 0.75);
    cohort2.setRotPeriod(200);
    cohort3.setRotPeriod(20);
    g4m::ageStruct::v res;
    for(int i=0; i<120; ++i) {
      res = cohort1.aging();
      double charv1 = res.enSw + res.enRw + res.vnSw + res.vnRw;
      res = cohort2.aging();
      double charv2 = res.enSw + res.enRw + res.vnSw + res.vnRw;
      res = cohort3.aging();
      double charv3 = res.enSw + res.enRw + res.vnSw + res.vnRw;
      cout << charv1 << " " << charv2 << " " << charv3 << " " << cohort1.getBm() << " " << cohort2.getBm() << " " << cohort3.getBm() << endl;
    }
  }

  return(0);
}
*/
