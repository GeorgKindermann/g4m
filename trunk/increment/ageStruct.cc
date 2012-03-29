#include "ageStruct.h"

#include <iostream>

namespace g4m {
  ageStruct::ageStruct 
  (incrementTab *ait
   , ffipol<double> *asws
   , ffipol<double> *ahlv, ffipol<double> *ahle
   , ffipolm<double> *acov, ffipolm<double> *acoe
   , ffipolm<bool> *adov, ffipolm<bool> *adoe
   , double amai
   , int aobjOfProd, double au
   , double aminSw, double aminRw, double aminHarv
   , int asdDef, double asdMax, double asdMin
   , unsigned int amaiYears
   , double aminRotVal, int aminRotRef
   , double aflexSd
   , ffipol<double> *sdMaxH, ffipol<double> *sdMinH)
  {
    it = ait;
    sws = asws;
    hlv = ahlv; hle = ahle;
    cov = acov; coe = acoe; dov = adov; doe = adoe;
    mai = amai;
    if(!qMai.empty()) {qMai.clear();}
    qMai.resize(amaiYears, mai);
    avgMai = mai;
    objOfProd = aobjOfProd;
    uRef = au;
    u = setRotationTime();
    minSw = aminSw; minRw = aminRw; minHarv = aminHarv;
    sdDef = asdDef;
    sdMin = asdMin; sdMax = asdMax;
    minRotVal = aminRotVal;
    minRotRef = aminRotRef;
    setMinRot();
    flexSd=aflexSd;
    timeStep = it->gtimeframe();
    area = 0.;
  }

  ageStruct::~ageStruct() {
    it = NULL;
    sws = NULL;
    hlv = NULL;
    hle = NULL;
    cov = NULL;
    coe = NULL;
    dov = NULL;
    doe = NULL;
  }

  double ageStruct::calcAvgMai() {
    if(qMai.size() > 0) {
      double product = 1.;
      double sWeight = 0.;
      double weight = 0.;
      double step = 2./qMai.size();
      std::deque<double>::iterator iter = qMai.begin();
      while(iter != qMai.end()) {
	weight += step;
	if(*iter <= 0.) {   //The years before 0 are unimportant
	  product = 1.;
	  sWeight = 0.;
	} else {
	  product *= std::pow(*iter, weight);
	  sWeight += weight;
	}
	++iter;
      }
      if(sWeight > 0.) {avgMai = std::pow(product, 1./sWeight);}
      else {avgMai = 0.;}
    } else {
      avgMai = mai;
    }
    return(avgMai);
  }
 
  double ageStruct::setRotationTime() {
    if(objOfProd == 0) {u=uRef;}
    else if(objOfProd == 1 || objOfProd == 2) {u=-1.;}
    else if(objOfProd > 2 && objOfProd < 8) {
      const int type = objOfProd - 3;
      double sd = (sdMin + sdMax)/2.;
      if(sd > 0) {
	if(sd == 1.) {u = it->gToptt(avgMai, type);}
	else {u = it->gToptSdTab(avgMai, sd, type);}
      } else {
	if(sd == -1.) {u = it->gTopt(avgMai, type);}
	else {u = it->gToptSdNat(avgMai, -1. * sd , type);}
      }
    }
    else {u=0.;}
    return(u);
  }

  double ageStruct::setMinRot() {
    if(minRotRef == 0) {minRot = minRotVal;}
    else if(minRotRef == 1 && u > 0.) {minRot = minRotVal * u;}
    else if(minRotRef < 7) {
      const int type = minRotRef - 2;
      double sd = (sdMin + sdMax)/2.;
      if(sd > 0) {
	if(sd == 1.) {minRot = minRotVal * it->gToptt(avgMai, type);}
	else {minRot = minRotVal * it->gToptSdTab(avgMai, sd, type);}
      } else {
	if(sd == -1.) {minRot = minRotVal * it->gTopt(avgMai, type);}
	else {minRot = minRotVal * it->gToptSdNat(avgMai, -sd , type);}
      }
    } else {minRot = -1.;}
    return(minRot);
  }

  double ageStruct::createNormalForest
  (double rotationPeriod, double aarea, double sd) {
    if(aarea < 0.) {aarea = 0.;}
    area = aarea;
    if(rotationPeriod < 1.) {rotationPeriod = 1;}
    if(rotationPeriod >= it->gTmax()) {rotationPeriod = it->gTmax()-1;}
    unsigned int ageClasses = std::ceil(0.5 + rotationPeriod/timeStep);
    if(dat.size() < ageClasses) {dat.resize(ageClasses);}
    aarea /= rotationPeriod;
    cohort tmp;
    tmp.area = aarea*timeStep;
    tmp.bm = 0.; tmp.d = 0.; tmp.h = 0.;
    for(unsigned int i=0; i<ageClasses; ++i) {
      double age = i*timeStep;
      if(sd > 0.) {
	if(sd == 1.) {
	  tmp.bm = it->gBmt(age, avgMai);
	  tmp.d = it->gDbht(age, avgMai);
	}
	else {
	  tmp.bm = it->gBmSdTab(age, avgMai, sd);
	  tmp.d = it->gDbhSdTab(age, avgMai, sd);
	}
      } else {
	if(sd == -1.) {
	  tmp.bm = it->gBm(age, avgMai);
	  tmp.d = it->gDbh(age, avgMai);
	}
	else {
	  tmp.bm = it->gBmSdNat(age, avgMai, -sd);
	  tmp.d = it->gDbhSdNat(age, avgMai, -sd);
	}
      }
      tmp.h =  it->gHeight(age, avgMai);
      dat[i] = tmp;
    }
    dat[0].area /= 2.;
    double share = 0.5 + rotationPeriod/timeStep - static_cast<int>(0.5 + rotationPeriod/timeStep);
    if(share > 0.) {dat[ageClasses-1].area = aarea * timeStep * share;}
    for(unsigned int i=ageClasses; i<dat.size(); ++i) {
      dat[i].area = 0.;
    }
    return(area);
  }

  double ageStruct::getD(double age) {
    age /= timeStep;
    unsigned int ageH = ceil(age);
    if(ageH >= dat.size()) {return(dat[dat.size()-1].d);}
    if(ageH < 1) {return(dat[0].d);}
    --ageH;
    return(dat[ageH].d + (dat[ageH+1].d - dat[ageH].d) * (age - ageH));
  }

  double ageStruct::getH(double age) {
    age /= timeStep;
    unsigned int ageH = ceil(age);
    if(ageH >= dat.size()) {return(dat[dat.size()-1].h);}
    if(ageH < 1) {return(dat[0].h);}
    --ageH;
    return(dat[ageH].h + (dat[ageH+1].h - dat[ageH].h) * (age - ageH));
  }

  double ageStruct::getBm(double age) {
    age /= timeStep;
    unsigned int ageH = ceil(age);
    if(ageH >= dat.size()) {return(dat[dat.size()-1].bm);}
    if(ageH < 1) {return(dat[0].bm);}
    --ageH;
    return(dat[ageH].bm + (dat[ageH+1].bm - dat[ageH].bm) * (age - ageH));
  }

  double ageStruct::getBm() {
    double area=0.;
    double bm=0.;
    for(unsigned int i=0; i<dat.size(); ++i) {
      area += dat[i].area;
      bm += dat[i].area * dat[i].bm;
    }
    if(area > 0.) {return(bm/area);}
    return(0.);
  }

  double ageStruct::getArea(int age) {
    if(age >= static_cast<int>(dat.size())) {return(0.);}
    return(dat[age].area);
  }

  double ageStruct::getArea(double age) {
    //return(dat[0].area);
    //Here seems to be an error - Above just to allow some colculations
    age /= timeStep;
    unsigned int ageH = static_cast<int>(age+0.5);
    if(ageH >= dat.size()) {return(0.);}
    if(ageH <= 0) {return(dat[0].area/timeStep);}
    return(dat[ageH].area/timeStep);
  }

  double ageStruct::getArea() {
    return(area);
  }

  double ageStruct::calcArea() { //Calculates the area
    area = 0.;
    for(unsigned int i=0; i<dat.size(); ++i) {
      area += dat[i].area;
    }
    return(area);
  }

  double ageStruct::setArea(unsigned int ageClass, double aarea) {
    //area += aarea - dat[0].area;
    //dat[0].area = aarea;
    //return(dat[0].area);
    //Here seems to be an error - Above just to allow some colculations
    if(aarea < 0.) {return(-1.);}
    if(ageClass >= static_cast<unsigned int>(it->gTmax()/timeStep)) {
      ageClass = static_cast<unsigned int>(it->gTmax()/timeStep)-1;
    }
    if(ageClass < dat.size()) {
      area += aarea - dat[ageClass].area;
      dat[ageClass].area = aarea;
      return(dat[ageClass].area);
    }
    area += aarea;
    unsigned int oldSize = dat.size();
    dat.resize(ageClass+1);
    initCohort(oldSize, ageClass);
    dat[ageClass].area = aarea;
    return(dat[ageClass].area);
  }

  double ageStruct::setBm(unsigned int ageClass, double biomass) {
    if(biomass < 0.) {return(0.);}
    if(ageClass >= static_cast<unsigned int>(it->gTmax()/timeStep)) {
      ageClass = static_cast<unsigned int>(it->gTmax()/timeStep)-1;
    }
    double maxBm = it->gBm(ageClass*timeStep, avgMai);
    if(biomass > maxBm) {biomass = maxBm;}
    if(ageClass < dat.size()) {
      dat[ageClass].bm = biomass;
      return(dat[ageClass].bm);
    }
    unsigned int oldSize = dat.size();
    dat.resize(ageClass+1);
    initCohort(oldSize, ageClass);
    dat[ageClass].bm = biomass;
    return(dat[ageClass].bm);
  }

  double ageStruct::setD(unsigned int ageClass, double dbh) {
    if(ageClass >= static_cast<unsigned int>(it->gTmax()/timeStep)) {
      ageClass = static_cast<unsigned int>(it->gTmax()/timeStep)-1;
    }
    double age = ageClass*timeStep;
    double minD = it->gDbh(age, avgMai);
    double maxD = it->gDbhSdNat(age, avgMai, 0.);
    if(dbh < minD) {dbh = minD;}
    if(dbh > maxD) {dbh = maxD;}
    if(ageClass < dat.size()) {
      dat[ageClass].d = dbh;
      return(dat[ageClass].d);
    }
    unsigned int oldSize = dat.size();
    dat.resize(ageClass+1);
    initCohort(oldSize, ageClass);
    dat[ageClass].d = dbh;
    return(dat[ageClass].d);
  }

  unsigned int ageStruct::initCohort(unsigned int ageClassL, unsigned int ageClassH) {
    cohort tmp;
    tmp.area = 0.;
    double sd = (sdMin + sdMax)/2.;
    for(unsigned int i = ageClassL; i <= ageClassH; ++i) {
      double age = i*timeStep;
      if(sd > 0.) {
	if(sd == 1.) {
	  tmp.bm = it->gBmt(age, avgMai);
	  tmp.d = it->gDbh(age, avgMai);
	}
	else {
	  tmp.bm = it->gBmSdTab(age, avgMai, sd);
	  tmp.d = it->gDbhSdTab(age, avgMai, sd);
	}
      } else {
	if(sd == -1.) {
	  tmp.bm = it->gBm(age, avgMai);
	  tmp.d = it->gDbh(age, avgMai);
	}
	else {
	  tmp.bm = it->gBmSdNat(age, avgMai, -sd);
	  tmp.d = it->gDbhSdNat(age, avgMai, -sd);
	}
      }
      tmp.h =  it->gHeight(age, avgMai);
      dat[i] = tmp;
    }
    return(ageClassH - ageClassL);
  }

  double ageStruct::setMai(double aMai) {
    mai = aMai;
    return(mai);
  }

  unsigned int ageStruct::setMaiYears(unsigned int maiYears) {
    while(maiYears > qMai.size()) {qMai.push_front(avgMai);}
    while(maiYears < qMai.size()) {qMai.pop_front();}
    calcAvgMai();
    setRotationTime();
    setMinRot();
    return(qMai.size());
  }

  double ageStruct::setAvgMai(double aavgMai) {
    avgMai = aavgMai;
    if(avgMai < 0.) {avgMai = 0.;}
    std::deque<double>::iterator iter = qMai.begin();
    while(iter != qMai.end()) {
      *iter = avgMai;
      ++iter;
    }
    setRotationTime();
    setMinRot();
    return(avgMai);
  }

  double ageStruct::getMai() {
    return(mai);
  }

  double ageStruct::getAvgMai() {
    return(avgMai);
  }

  int ageStruct::setObjOfProd(int aobjOfProd) {
    objOfProd = aobjOfProd;
    return(objOfProd);
  }

  double ageStruct::setU(double au) {
    uRef = au;
    setRotationTime();
    setMinRot();
    return(u);
  }

  double ageStruct::setStockingdegreeMin(double sd) {
    sdMin=sd;
    return(sdMin);
  }

  double ageStruct::setStockingdegreeMax(double sd) {
    sdMax=sd;
    return(sdMax);
  }

  int ageStruct::setMinRotRef(int aminRotRef) {
    minRotRef = aminRotRef;
    return(minRotRef);
  }

  double ageStruct::setMinRotVal(double aminRotVal) {
    minRotVal = aminRotVal;
    setMinRot();
    return(minRotVal);
  }

  double ageStruct::setFlexSd(double aflexSd) {
    flexSd=aflexSd;
    return(flexSd);
  }

  double ageStruct::afforest(double aarea) {
    if(area > 0) {
      dat[0].area += aarea;
      area += aarea;
    }
    return(dat[0].area);
  }

  double ageStruct::reforest(double aarea) {
    if(dat.size() < 2) {dat.resize(2);}
    if(area > 0) {
      dat[0].area += aarea/2.;
      dat[1].area += aarea/2.;
      area += aarea;
    }
    return(dat[0].area + dat[1].area);
  }

  ageStruct::v ageStruct::deforest(double aarea, int type) {
    v ret = {0., 0., 0., 0., 0.};
    if(type == 0) { //Take from all age classes
      if(area < aarea) {aarea = area;}
      if(area > 0.) {
	double mul = 1. - aarea/area;
	static std::vector<double> dbhBm(2,0); //Key of dbh and biomass
	for(unsigned int i=0; i<dat.size(); ++i) {
	  if(dat[i].area > 0.) {
	    double sdNat = it->gSdNat(i*timeStep, avgMai, dat[i].bm);
	    double dbm = it->gIncBmSdNat(i*timeStep, avgMai, sdNat)/2.;
	    double id = it->gIncDbhSdNat(i*timeStep, avgMai, sdNat)/2.;
	    dbhBm[0] = dat[i].d+id; dbhBm[1] = dat[i].bm+dbm;
	    double totalWood = dat[i].area * (1.-mul) * dbhBm[1];
	    ret.bm += totalWood;
	    double harvestedWood = totalWood * hle->g(dbhBm[0]);
	    double sawnWood = harvestedWood * sws->g(dbhBm[0]);
	    ret.sw += sawnWood;
	    ret.rw += harvestedWood - sawnWood;
	    ret.co += totalWood * coe->g(dbhBm);
	    area -= dat[i].area * (1. - mul);
	    dat[i].area *= mul;
	  }
	}
	ret.area=aarea;
      }
      if(ret.area > 0.) { //Values per hectare
	ret.sw /= area; ret.rw /= area; ret.co /= area; ret.bm /= area;}
    } else { //Take from the old age classes
      ret =  finalCut(aarea, false);
    }
    return(ret);
  }

  ageStruct::v ageStruct::finalCut(bool eco, double aarea, double minSw, double minRw, double minHarv) {
    v ret = {0., 0., 0., 0., 0.};
    if(aarea > 0.) {
      int endYear = 0;
      if(eco) {endYear = minRot/timeStep;}
      static std::vector<double> dbhBm(2,0); //Key to ask if harvest is economic
      for(int i=dat.size()-1; i>=endYear && (ret.area<aarea || (ret.sw<minSw || ret.rw<minRw || (ret.sw+ret.rw)<minHarv)); --i) {
	if(dat[i].area > 0.) {
	  //The Stands get half increment of the next growing period
	  double sdNat = it->gSdNat(i*timeStep, avgMai, dat[i].bm);
	  double dbm = it->gIncBmSdNat(i*timeStep, avgMai, sdNat)/2.;
	  double id = it->gIncDbhSdNat(i*timeStep, avgMai, sdNat)/2.;
	  dbhBm[0] = dat[i].d+id; dbhBm[1] = dat[i].bm+dbm;
	  if(doe->g(dbhBm) || eco==false) { //do harvest if it is economic
	    double totalWood = 0.;
	    if(ret.area+dat[i].area < aarea) { //Harvest all of this age class
	      totalWood = dat[i].area * dbhBm[1];
	      ret.area += dat[i].area;
	      area -= dat[i].area;
	      dat[i].area = 0.;
	    } else {
	      totalWood = (aarea - ret.area) * dbhBm[1];
	      area -= aarea - ret.area;
	      dat[i].area -= aarea - ret.area;
	      ret.area = aarea;
	    }
	    ret.bm += totalWood;
	    double harvestedWood = totalWood * hle->g(dbhBm[0]);
	    double sawnWood = harvestedWood * sws->g(dbhBm[0]);
	    ret.sw += sawnWood;
	    ret.rw += harvestedWood - sawnWood;
	    ret.co += totalWood * coe->g(dbhBm);
	  }
	}
      }
    }
    if(ret.area > 0.) { //Values per hectare
      ret.sw /= ret.area; ret.rw /= ret.area; ret.co /= ret.area; ret.bm /= ret.area;}
    return(ret);
  }

  ageStruct::v ageStruct::finalCut(double aarea, bool eco) {
    return(finalCut(eco, aarea, -1., -1., -1.));
  }

  ageStruct::v ageStruct::finalCut(double minSw, double minRw, double minHarv
				   , bool eco) {
    return(finalCut(eco, -1., minSw, minRw, minHarv));
  }

  std::pair<ageStruct::v, ageStruct::v> ageStruct::aging() {
    return(aging(mai));
  }

  std::pair<ageStruct::v, ageStruct::v> ageStruct::aging(double amai) {
    v retThin = {0., 0., 0., 0., 0.};
    v retHarvest = {0., 0., 0., 0., 0.};
    mai = amai;
    qMai.push_back(mai);
    qMai.pop_front();
    calcAvgMai();
    setRotationTime();
    setMinRot();
    if(objOfProd == 1 || objOfProd == 2) { //Fulfill an amount of harvest
      retThin = thinAndGrow();
      retHarvest = finalCut(minSw-retThin.sw, minRw-retThin.rw, minHarv-(retThin.sw+-retThin.rw), true);
    } else { //We have a rotation time to fulfill
      retHarvest = finalCut(area*timeStep/u, true); //do final cut
      retThin = thinAndGrow();
      //retThin = thinAndGrowOLD();
    }
    //Make reforestations on final harvested area
    reforest(retHarvest.area); //Maybe include also reforestation costs
    return(std::make_pair(retThin, retHarvest));
  }

  ageStruct::v ageStruct::thinAndGrow() {
    if(sdDef==0) { //Constant stocking degre
      return(thinAndGrowStatic());
    } else if(sdDef==1) { //Alternate between constant stocking degrees
      return(thinAndGrowOLD());
    }
    //Alternate between varying stocking degrees
    return(thinAndGrowOLD());
  }

  ageStruct::v ageStruct::thinAndGrowStatic() {
    v ret = {0., 0., 0., 0., 0.};
    for(int i = static_cast<int>(dat.size())-2; i>-1; --i) {
      if(dat[i].area > 0.) {
	double sd, iGwl, bmT, id;
	incStatic(i, sd, iGwl, bmT, id);
	if(flexSd > 0.) { //The typical amount of harvest
	  double bmTCom =  dat[i].bm + incCommon(i, sd, iGwl);
	  bmT = bmT * (1. - flexSd) + bmTCom * flexSd;
	}
	double totalWood = dat[i].area * (iGwl - (bmT-dat[i].bm));
	if(totalWood < 0.) {totalWood = 0.;}
	static std::vector<double> dbhBmSh(3,0);
	dbhBmSh[0] = dat[i].d+id/2.;
	dbhBmSh[1] = dat[i].bm+iGwl/2.;
	dbhBmSh[2] = totalWood/dbhBmSh[1];
	if(dov->g(dbhBmSh)) { //Do Thinning if it is economic
	  double harvestedWood = totalWood * hlv->g(dbhBmSh[0]);
	  double sawnWood = harvestedWood * sws->g(dbhBmSh[0]);
	  ret.area += dat[i].area;
	  ret.bm += totalWood;
	  ret.sw += sawnWood;
	  ret.rw += harvestedWood - sawnWood;
	  ret.co += totalWood * cov->g(dbhBmSh);
	  dat[i].bm += iGwl - totalWood/dat[i].area;
	} else {  //No thinning
	  dat[i].bm += iGwl;
	  double bmMax = it->gBm((i+1)*timeStep, avgMai);
	  if(dat[i].bm > bmMax) {dat[i].bm = bmMax;}
	}
	dat[i].d += id;
	dat[i].h += it->gIncHeight(i*timeStep, avgMai);
      }
    }
    cohortShift();
    return(ret);
  }

  int ageStruct::incStatic(int i, double &sd, double &iGwl, double &bm, double &id) {
    if(sdMax > 0) {  //Yield table stocking degree
      if(sdMax == 1.) {bm = it->gBmt((i+1)*timeStep, mai);}
      else {bm = it->gBmSdTab((i+1)*timeStep, mai, sdMax);}
      if(i) {sd = it->gSdTab(i*timeStep, avgMai, dat[i].bm);}
      else {sd = 1.;} //New plantations have a stocking degree of 1
      if(sd == 1.) {
	iGwl = it->gIncGwlt(i*timeStep, mai);
	id = it->gIncDbht(i*timeStep, mai);
      } else {
	iGwl = it->gIncGwlSdTab(i*timeStep, mai, sd);
	id = it->gIncDbhSdTab(i*timeStep, mai, sd);
      }
    } else {  //Natural stocking degree
      if(sdMax == -1.) {bm = it->gBm((i+1)*timeStep, mai);}
      else {bm = it->gBmSdNat((i+1)*timeStep, mai, -sdMax);}
      if(i) {sd = it->gSdNat(i*timeStep, avgMai, dat[i].bm);}
      else {sd = 1.;} //New plantations have a stocking degree of 1
      if(sd == 1.) {
	iGwl = it->gIncGwl(i*timeStep, mai);
	id = it->gIncDbh(i*timeStep, mai);
      } else {
	iGwl = it->gIncGwlSdNat(i*timeStep, mai, sd);
	id = it->gIncDbhSdNat(i*timeStep, mai, sd);
      }
    }
    return(0);
  }

  double ageStruct::incCommon(const int i, const double &sd, const double &iGwl) {
    double sdTarget = fabs(sdMax);
    double bmInc;
    if(sd > 0.) {
      if(sdMax > 0) {  //Yield table stocking degree
	if(sd == 1.) {bmInc = it->gIncBmt((i+1)*timeStep, mai);}
	else {bmInc = it->gIncBmSdTab((i+1)*timeStep, mai, sd);}
      } else {  //Natural stocking degree
	if(sd == 1.) {bmInc = it->gIncBm((i+1)*timeStep, mai);}
	else {bmInc = it->gIncBmSdNat((i+1)*timeStep, mai, sd);}
      }
      if(bmInc > iGwl) {bmInc = iGwl;}
      if(sd > sdTarget) {
	bmInc *= sdTarget/sd;
      } else {
	bmInc += (iGwl - bmInc) * (1. - 1./(1. + sdTarget-sd));
      }
    } else { //Current stocking degree is 0
      if(sdTarget > 0.) {
	bmInc = iGwl;
      } else {
	bmInc = 0.;
      }
    }
    return(bmInc);
  }

  int ageStruct::cohortShift() {
    int i = static_cast<int>(dat.size())-1;
    dat[i+1].d = (dat[i+1].d * dat[i+1].area + dat[i].d * dat[i].area) / (dat[i+1].area + dat[i].area);
    dat[i+1].h = (dat[i+1].h * dat[i+1].area + dat[i].h * dat[i].area) / (dat[i+1].area + dat[i].area);
    dat[i+1].bm = (dat[i+1].bm * dat[i+1].area + dat[i].bm * dat[i].area) / (dat[i+1].area + dat[i].area);
    dat[i+1].area += dat[i].area;
    for(i = static_cast<int>(dat.size())-2; i>-1; --i) {
      dat[i+1] = dat[i];
    }
    dat[0].area=0; dat[0].bm=0; dat[0].d=0; dat[0].h=0;
  return(0);
  }

  ageStruct::v ageStruct::thinAndGrowOLD() {
    v ret = {0., 0., 0., 0., 0.};
    bool constSd = (sdDef==0) ? true : false;
    for(int i = static_cast<int>(dat.size())-1; i>-1; --i) {
      if(dat[i].area > 0.) {
	double sdNat;
	if(i > 0) {sdNat = it->gSdNat(i*timeStep, avgMai, dat[i].bm);}
	else {  //Afforestations have a stocking degree of sdMin
	  if(sdMin > 0.) {sdNat = sdMin * it->gSdNat(0, avgMai);
	  } else {sdNat = -sdMin;}
	}
	double gwl = it->gIncGwlSdNat(i*timeStep, mai, sdNat);
	double sd=0.;  //Stocking degree after growing period
	if(sdMax > 0.) {sd = (gwl+dat[i].bm) / it->gBmt((i+1)*timeStep,avgMai);}
	else {sd = (gwl+dat[i].bm) / it->gBm((i+1)*timeStep, avgMai);}
	double id = it->gIncDbhSdNat(i*timeStep, avgMai, sdNat);
	//The oldest age class has no increment
	if(i == static_cast<int>(dat.size())-1) {
	  gwl = 0.;
	  id = 0.;
	} else {dat[i].h += it->gIncHeight(i*timeStep, avgMai);}
	bool thinningWasDone = false;
	//Key to ask if harvest is economic
	static std::vector<double> dbhBmSh(3,0);
 //Stocking degree to high or typical thinnigs are forced -> make thinning
	if(sd > fabs(sdMax) || flexSd > 0.) {
	  //Thinning caused by stand density
	  double reduce = 0.;
	  if(constSd) {
	    reduce = fabs(sdMax) / sd;
	  } else {
	    if(sd > fabs(sdMax)) {
	      if(sdMin > 0) {
		reduce = sdMin / ((gwl/2.+dat[i].bm) / it->gBmt((i+1)*timeStep,avgMai));
	      } else {
		reduce = -sdMin / ((gwl/2.+dat[i].bm) / it->gBm((i+1)*timeStep, avgMai));
	      }
	    }
	  }
	  if(reduce < 0.) {reduce = 0.;}
	  if(reduce > 1.) {reduce = 1.;}
	  //Thinning caused by typical thinnigs
	  double reduceB = 0.;
	  if(flexSd > 0.) {
	    double cutVol = gwl - it->gIncBmSdNat(i*timeStep, mai, sdNat);
	    if(cutVol > 0.) {
	      //A weighting by sd / sdMax is done to come sometime to sdMax
	      double weight;
	      if(sdMax > 0) {weight = it->gSdTab(i*timeStep, avgMai, dat[i].bm) / sdMax;}
	      else {weight = sdNat / (-sdMax);}
	      reduceB = 1. - (cutVol / (gwl/+dat[i].bm)) * weight;
	    }
	  }
	  if(reduceB < 0.) {reduceB = 0.;}
	  if(reduceB > 1.) {reduceB = 1.;}
	  //Bring in the thinning fluctuation softener
	  //flexSd > 0. && flexSd <= 1.
	  reduce = reduce * (1. - flexSd) + reduceB * flexSd;
	  dbhBmSh[0] = dat[i].d+id/2.;
	  dbhBmSh[1] = dat[i].bm+gwl/2.;
	  dbhBmSh[2] = (1. - reduce);
	  if(reduce > 0. && dov->g(dbhBmSh)) { //Do Thinning if it is economic
	    thinningWasDone = true;
	    if(constSd) {
	      dat[i].bm += gwl;
	      dat[i].d += id;
	    } else {
	      //The Stands get half increment at high stand density
	      dat[i].bm += gwl/2.;
	      dat[i].d += id/2.;
	    }
	    double totalWood = dat[i].area * dat[i].bm * (1. - reduce);
	    dat[i].bm *= reduce;
	    double harvestedWood = totalWood * hlv->g(dat[i].d);
	    double sawnWood = harvestedWood * sws->g(dat[i].d);
	    ret.area += dat[i].area;
	    ret.bm += totalWood;
	    ret.sw += sawnWood;
	    ret.rw += harvestedWood - sawnWood;
	    ret.co += totalWood * cov->g(dbhBmSh);
	    if(!constSd) {
	      //The Stands get half increment at low stand density
	      sdNat = it->gSdNat((i+.5)*timeStep, avgMai, dat[i].bm);
	      dat[i].bm += it->gIncBmSdNat((i+.5)*timeStep, avgMai, sdNat)/2.;
	      dat[i].d += it->gIncDbhSdNat((i+.5)*timeStep, avgMai, sdNat)/2.;
	    }
	  } else {  //No thinning
	    dat[i].bm += gwl;
	    dat[i].d += id;
	  }
	} else {  //No thinning
	  dat[i].bm += gwl;
	  dat[i].d += id;
	}
	//Check if Bm is not higher than maximum possible at end of period
	double bmMax = it->gBm((i+1)*timeStep, avgMai);
	if(dat[i].bm > bmMax) {
	  if(constSd) {
	    dat[i].bm = bmMax;
	  } else {
	    if(thinningWasDone) {
	      double totalWood = dat[i].area * (dat[i].bm - bmMax);
	      dat[i].bm = bmMax;
	      double harvestedWood = totalWood * hlv->g(dat[i].d);
	      double sawnWood = harvestedWood * sws->g(dat[i].d);
	      ret.bm += totalWood;
	      ret.sw += sawnWood;
	      ret.rw += harvestedWood - sawnWood;
	      ret.co += totalWood * cov->g(dbhBmSh);
	    } else {
	      dat[i].bm = bmMax;
	    }
	  }
	}
	//Bring the Data to the next age class
	if(i < static_cast<int>(dat.size())-1) {
	  dat[i+1] = dat[i];
	} else {
	  dat[i+1].d = (dat[i+1].d * dat[i+1].area + dat[i].d * dat[i].area) / (dat[i+1].area + dat[i].area);
	  dat[i+1].h = (dat[i+1].h * dat[i+1].area + dat[i].h * dat[i].area) / (dat[i+1].area + dat[i].area);
	  dat[i+1].bm = (dat[i+1].bm * dat[i+1].area + dat[i].bm * dat[i].area) / (dat[i+1].area + dat[i].area);
	  dat[i+1].area += dat[i].area;
	}
	dat[i].area = 0.; dat[i].bm = 0.; dat[i].d = 0.; dat[i].h = 0.;
      }
    }
    if(ret.area > 0.) { //Values per hectare
      ret.sw /= ret.area; ret.rw /= ret.area; ret.co /= ret.area; ret.bm /= ret.area;}
    return(ret);
  }


}
