#include "ageStruct.h"

namespace g4m {

  ageStruct::v::v() {
    area=0.; enSw=0.; enRw=0.; vnSw=0.; vnRw=0.; dbEn=0.; dbVn=0.;
  }

  ageStruct::v::~v() {}
  

  double ageStruct::updateHarvestArea() {
    if(targetRotationPeriod > 0.) {
      tragetHarvestArea = getArea()/targetRotationPeriod;
    } else {tragetHarvestArea = 0.;}
    return(tragetHarvestArea);
  }

  ageStruct::v ageStruct::deforest(double area, int type) {
    v ret;
    if(type == 0) { //Take from all age classes
      double totArea = getArea();
      if(totArea < area) {area = totArea;}
      if(totArea > 0.) {
	double mul = 1. - area/totArea;
	for(int i=0; i<activeAge; ++i) {
	  double sdEt = it->gSd(i, mai, dat[i].bm, 2);
	  double dbm= it->gLfz(i, mai, sdEt)/2.;
	  double id = it->gDi(i, mai, sdEt)/2.;
	  //double ih = it->gH(i, mai)/2.;
	  double totalWood = dat[i].area * (1.-mul) * (dat[i].bm+dbm);
	  ret.enSw += totalWood * sws->g(dat[i].d+id);
	  ret.enRw += totalWood * hle->g(dat[i].d+id) - totalWood * sws->g(dat[i].d+id);
	  ret.dbEn += totalWood * dbe->g(dat[i].d+id);
	  dat[i].area *= mul;
	}
      }
      ret.area=area;
      divArea(ret, area);
    } else { //Take from the old age classes
      ret =  finalCut(area, false);
    }
    updateHarvestArea();
    return(ret);
  }

  double ageStruct::afforest(double area) {
    dat[0].area += area;
    if(dat[0].area < 0.) {dat[0].area = 0.;}
    updateHarvestArea();
    return(dat[0].area);
  }

  double ageStruct::getBm() {
    double sum=0.;
    for(int i=0; i<activeAge; ++i) {
      sum += dat[i].bm * dat[i].area;
    }
    return(sum);
  }

  double ageStruct::getArea() {
    double sum=0.;
    for(int i=0; i<activeAge; ++i) {
      sum += dat[i].area;
    }
    return(sum);
  }

  double ageStruct::setStockingdegree(double sd) {
    targetSd = sd;
    return(targetSd);
  }

  double ageStruct::setRotPeriod(int rotPeriod) {
    if(rotPeriod > 0) {targetRotationPeriod = rotPeriod;}
    else {targetRotationPeriod = it->gTopt(mai, 0);} //Unmanaged Rotation time
    updateHarvestArea();
    return(targetRotationPeriod);
  }

  double ageStruct::setMai(double amai) {
    if(ageClasses < it->gTopt(amai, 0)) {setAgeClasses(it->gTopt(mai, 0));}
    mai = amai;
    return(mai);
  }

  ageStruct::v ageStruct::finalCut(double area, bool eco) {
    v ret;
    if(area > 0.) {
      int endYear = 0;
      if(eco) {endYear = minRot * targetRotationPeriod;}
      for(int i=activeAge; i>=endYear && ret.area<area; --i) {
	//Halben Zuwachs fuer die Endnutzungsbestaende
	double sdEt = it->gSd(i, mai, dat[i].bm, 2);
	double dbm= it->gLfz(i, mai, sdEt)/2.;
	double id = it->gDi(i, mai, sdEt)/2.;
	//double ih = it->gH(i, mai)/2.;
	if(dbe->g(dat[i].d+id) * (dat[i].bm+dbm) >= minDbEn || eco==false) {
	  double totalWood = 0.;
	  bool resetTreesize = false;
	  if(ret.area+dat[i].area < area) {
	    totalWood = dat[i].area * (dat[i].bm+dbm);
	    ret.area += dat[i].area;
	    dat[i].area = 0.; dat[i].bm = 0.;
	    resetTreesize = true;
	  } else {
	    totalWood = (area - ret.area) * (dat[i].bm+dbm);
	    dat[i].area -= area - ret.area;
	    ret.area = area;
	  }
	  ret.enSw += totalWood * sws->g(dat[i].d+id);
	  ret.enRw +=totalWood * (hle->g(dat[i].d+id) - sws->g(dat[i].d+id));
	  ret.dbEn += totalWood * dbe->g(dat[i].d+id);
	  if(resetTreesize) {dat[i].d = 0.; dat[i].h = 0.;}
	}
      }
      divArea(ret, getArea());
    }
    return(ret);
  }

  ageStruct::v ageStruct::aging() {
    v ret;
    //Endnutzung durchfuehren
    ret = finalCut(tragetHarvestArea, true);
    //Thinning
    ret.vnSw = 0.;
    ret.vnRw = 0.;
    ret.dbVn = 0.;
    for(int i=activeAge; i>=0; --i) {
      double dbm=0.;
      double thin=0.;
      if(targetSd > 0.) {  //With thinning
	std::pair<double,double> tmp = it->gDbmt(i, mai, dat[i].bm, targetSd);
	dbm = tmp.first; thin = tmp.second;
      } else {  //Without thinning
	std::pair<double,double> tmp = it->gDbm(i, mai, dat[i].bm, 1);
	dbm = tmp.first; thin = 0.;
      }
      double id = it->gDi(i, mai, it->gSd(i, mai, dat[i].bm, 2));
      //double ih = it->gHi(i, mai);
      dat[i].bm += dbm;
      dat[i].d += id;
      dat[i].h = it->gH(i+1, mai);
      if(thin > 0. && dbv->g(dat[i].d+id) * thin >= minDbVn) {
	double sawnWood = sws->g(dat[i].d-id/2.); //Possible Sawn wood
	double restWood = hlv->g(dat[i].d-id/2.) - sawnWood;
	double totalWood = dat[i].area * thin;
	ret.vnSw += totalWood * sawnWood;
	ret.vnRw += totalWood * restWood;
	ret.dbVn += totalWood * dbv->g(dat[i].d-id/2);
      }
      dat[i+1] = dat[i];
    }
    double area=getArea();
    if(area > 0.) {ret.vnSw /= area; ret.vnRw /= area; ret.dbVn /= area;}
    //Aelteste Altersklasse verjuengen (sollte keine Biomasse mehr haben)
    ret.area += dat[ageClasses].area;
    dat[ageClasses].area = 0.; dat[ageClasses].bm = 0.; dat[ageClasses].d = 0.;
    dat[ageClasses].h = 0.;
    dat[0].area = ret.area; dat[0].bm = 0.; dat[0].d = 0.; dat[0].h = 0.;
    fitActiveAge(0);
    return(ret);
  }

  int ageStruct::fitActiveAge(int type) { //Adjust active Age
    if(type==0) {
      while(dat[activeAge].area-1 == 0 && activeAge>0) {--activeAge;}
      while(dat[activeAge].area != 0 && activeAge<ageClasses) {++activeAge;}
    } else {
      for(activeAge=ageClasses; dat[activeAge].area == 0 && activeAge > 0; --activeAge) {;}
    }
    return(0);
  }

  double ageStruct::createNormalForest
  (int rotationPeriod, double area, double sd) {
    if(rotationPeriod < ageClasses) {
      area /= rotationPeriod;
      cohort tmp;
      tmp.area = area;
      tmp.bm = 0.; tmp.d = 0.; tmp.h = 0.;
      double dbm=0.; double thin=0.;
      for(int i=0; i<rotationPeriod; ++i) {
	if(targetSd > 0.) {
	  tmp.bm = it->gHbmt(i, mai) * targetSd;
	  std::pair<double,double> tt = it->gDbmt(i, mai, tmp.bm, targetSd);
	  dbm=tt.first; thin=tt.second;
	  tmp.d =  it->gD(i, mai, targetSd);
	  tmp.h =  it->gH(i, mai);
	}
	else {
	  tmp.bm = it->gHbm(i, mai);
	  std::pair<double,double> tt = it->gDbm(i, mai, tmp.bm, 1.);
	  dbm=tt.first; thin=tt.second;
	  tmp.d =  it->gD(i, mai, 999.);
	  tmp.h =  it->gH(i, mai);
	}
	dat[i] = tmp;
      }
      for(int i=rotationPeriod; i<ageClasses; ++i) {
	dat[i].area = 0.;
      }
      activeAge = rotationPeriod;
    } else {area = -1.;}
    updateHarvestArea();
    return(area);
  }

  int ageStruct::setAgeClasses(int aageClasses) {
    cohort *tmp = new cohort[aageClasses+1];
    for(int i = 0; i<aageClasses && i <ageClasses; ++i) {
      tmp[i] = dat[i];
    }
    delete[] dat;
    dat = tmp;
    ageClasses = aageClasses;    
    return(ageClasses);
  }

  double ageStruct::getD(int age) {
    double ret = -1.;
    if(age < ageClasses) {
      ret = dat[age].d;
    }
    return(ret);
  }

  double ageStruct::getBm(int age) {
    double ret = -1.;
    if(age < ageClasses) {
      ret = dat[age].bm;
    }
    return(ret);
  }

  double ageStruct::setBm(int age, double bm) {
    double ret = -1.;
    if(age < ageClasses) {
      dat[age].bm = bm;
      ret = dat[age].bm;
    }
    return(ret);
  }

  double ageStruct::getArea(int age) {
    double ret = -1.;
    if(age < ageClasses) {
      ret = dat[age].area;
    }
    return(ret);
  }

  double ageStruct::setArea(int age, double area) {
    double ret = -1.;
    if(age < ageClasses) {
      dat[age].area = area;
      ret = dat[age].area;
    }
    if(age > activeAge) {activeAge = age;}
    return(ret);
  }

  ageStruct::ageStruct
  (incrementTab *ait, ipol<double,double> *asws
   , ipol<double,double> *ahlv, ipol<double,double> *ahle
   , ipol<double,double> *adbv, ipol<double,double> *adbe
   , double aminDbVn
   , double aminDbEn, double amai
   , int arotationPeriod, double astockingDegree
   , double aarea, double aminRot)
  {
    it = ait;
    sws = asws;
    hlv = ahlv;
    hle = ahle;
    dbv = adbv;
    dbe = adbe;
    mai = amai;
    ageClasses = it->gTopt(mai, 0);
    if(ageClasses < 0) {ageClasses = 1;}
    dat = new cohort[ageClasses+1];
    setRotPeriod(arotationPeriod);
    targetSd = astockingDegree;
    updateHarvestArea();
    createNormalForest(targetRotationPeriod, aarea, targetSd);
    minDbVn = aminDbVn;
    minDbEn = aminDbEn;
    minRot = aminRot;
  }

  ageStruct::~ageStruct() {
    delete[] dat;
  }

  ageStruct::v ageStruct::divArea(v& x, double area) {
    if(area > 0.) {
      x.enSw /= area;
      x.enRw /= area;
      x.vnSw /= area;
      x.vnRw /= area;
      x.dbEn /= area;
      x.dbVn /= area;
    } else {
      x.enSw = 0.;
      x.enRw = 0.;
      x.vnSw = 0.;
      x.vnRw = 0.;
      x.dbEn = 0.;
      x.dbVn = 0.;
      x.area = 0.;
    }
    return(x);
  }

}

