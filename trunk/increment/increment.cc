#include "increment.h"

namespace {
  double interpol(double x0, double x1, double y0, double y1, double y) {
    double dx = x1 - x0;
    double dy = y1 - y0;
    double d = 0.;
    if(dy > 0. || dy < 0.) {
      d = dx * (y-y0) / dy;}
    return(x0 + d);
  }
}

namespace g4m {
  incrementCurves::incrementCurves
  (double a0, double a1, double a2, double a3, double a4,
   double a5, double a6, double a7, double a8, double a9,
   double a10, double a11, double a12, double a13,
   double a14, double a15, double a16, double a17,
   double a18, double a19, double a20, double a21,
   double a22, double a23, double a24, double a25,
   double a26, double amai) {
    c[0] = a0; c[1] = a1; c[2] = a2; c[3] = a3; c[4] = a4; c[5] = a5; c[6] = a6; 
    c[7] = a7; c[8] = a8; c[9] = a9; c[10] = a10; c[11] = a11; c[12] = a12;
    c[13] = a13; c[14] = a14; c[15] = a15; c[16] = a16; c[17] = a17; c[18] = a18;
    c[19] = a19; c[20] = a20; c[21] = a21; c[22] = a22; c[23] = a23; c[24] = a24;
    c[25] = a25; c[26] = a26;
    mai = amai;
    calcK(); calcTMax(); calcTcpMax(); calcTOpt(); calcTh13();
  }
  
  int incrementCurves::setCoef
  (double a0, double a1, double a2, double a3, double a4,
   double a5, double a6, double a7, double a8, double a9,
   double a10, double a11, double a12, double a13,
   double a14, double a15, double a16, double a17,
   double a18, double a19, double a20, double a21,
   double a22, double a23, double a24, double a25,
   double a26) {
    c[0] = a0; c[1] = a1; c[2] = a2; c[3] = a3; c[4] = a4; c[5] = a5; c[6] = a6; 
    c[7] = a7; c[8] = a8; c[9] = a9; c[10] = a10; c[11] = a11; c[12] = a12;
    c[13] = a13; c[14] = a14; c[15] = a15; c[16] = a16; c[17] = a17; c[18] = a18;
    c[19] = a19; c[20] = a20; c[21] = a21; c[22] = a22; c[23] = a23; c[24] = a24;
    c[25] = a25;  c[26] = a26;
    calcK(); calcTMax(); calcTcpMax(); calcTOpt(); calcTh13();
    return(0);
  }
  
  double incrementCurves::calcK() {
    double tmp = c[0] * exp(c[1]*pow(mai,c[2]));
    if(tmp > 0.) {tmp = 0.;}
    k = tmp;
    return(tmp);
  }
  
  double incrementCurves::calcTMax() {
    double tmp = c[3] + c[4]/(1 + exp(c[5] + c[6]*mai));
    if(tmp < 0.) {tmp = 0.;}
    tMax = tmp;
    return(tmp);
  }
  
  double incrementCurves::calcTcpMax() {
    double tmp = mai * tMax;
    if(k<0.) {tmp *= exp(0.25/k);}
    else {tmp = 0.;}
    if(tmp < 0.) {tmp = 0.;}
    tcpMax = tmp;
    return(tmp);
  }
  
  double incrementCurves::calcTOpt() {
    double tmp = tMax;
    if(k<0.) {tmp *= exp(0.5/k);}
    else {tmp = 0.;}
    if(tmp < 0.) {tmp = 0.;}
    tOpt = tmp;
    return(tmp);
  }
  
  double incrementCurves::calcTh13() {
    double tmp=0;
    if((c[15] > 0. || c[15] < 0.) && (c[17] > 0. || c[17] < 0.) &&
       (c[18] > 0. || c[18] < 0.)) {
      double t = pow(mai, -c[19])/c[18];
      t = pow(mai,(-c[16])*t) * (pow(mai,c[16]*t) - pow(1.3/c[15], t));
      if(t>0.) {tmp = log(t)/c[17];}
    }
    if(tmp < 0.) {tmp = 0.;}
    th13=tmp;
    return(tmp);
  }
  
  double incrementCurves::gTOpt() {
    return(tOpt);
  }
  
  double incrementCurves::gTcp(double t) {
    double tmp2 = 0.;
    if(t > 0. && tMax > 0.) {
      if(t < tMax) {
	double tmp1 = log(t/tMax);
	tmp2 = tcpMax * exp(k * tmp1 * tmp1);
      } else {tmp2 = tcpMax;}
    }
    if(tmp2 < 0.) {tmp2 = 0.;}
    return(tmp2);
  }
  
  double incrementCurves::gMaxDens(double t) {
    double ret=1.;
    if(t > 0. && tOpt > 0.) {
      double tmp1 = c[9]*t/tOpt;
      if(tmp1 > 0) {
	if(tmp1 < 1) {
	  ret = (c[7] + c[8] * log(t/tOpt)) * pow(1. - tmp1, c[10]);
	} else {ret = 0.;}
      }
    }
    if(ret < 0.) {ret = 0.;}
    if(ret > 1.) {ret = 1.;}
    return(ret);
  }
  
  double incrementCurves::gManDens(double t) {
    double ret=1.;
    if(t >= 0. && tOpt > 0.) {
      double tmp=t/tOpt;
      ret = 1. - exp(c[11] + c[12]/exp(mai) + c[13]*tmp + c[14]*tmp*tmp);
    }
    if(ret < 0.) {ret = 0.;}
    if(ret > 1.) {ret = 1.;}
    return(ret);
  }
  
  double incrementCurves::gH(double t) {
    double tmp = 0.;
    if(t>0) {
      tmp = c[15] * pow(mai, c[16]) *
	pow(1. - exp(c[17] * t), c[18] * pow(mai,c[19]));
    }     
    if(tmp < 0) {tmp = 0.;}
    return(tmp);
  }
  
  double incrementCurves::gD(double t) {
    double tmp = 0.;
    if(t>th13 && mai>0.) {
      double a = 1-exp(c[20]*mai);
      double b = c[21]+c[22]/pow(mai,c[23]);
      if(b>1.) {b=1.;}
      tmp = a * pow(t - th13, b);
    }     
    if(tmp < 0) {tmp = 0.;}
    return(tmp);
  }
  
  int incrementCurves::setMai(double amai) {
    mai = amai;
    calcK(); calcTMax(); calcTcpMax(); calcTOpt(); calcTh13();
    return(0);
  }
  
  double incrementCurves::gDmul(double bg) {
    double tmp = 1.;
    if(bg > 1.) {bg = 1.;}
    if(bg < 0.) {bg = 0.;}
    tmp = 1 - c[24]*pow(bg, c[25]);
    return(tmp);
  }
  
  double incrementCurves::gImul(double bg) {
    double tmp = 1.;
    if(bg > 1.) {bg = 1.;}
    if(bg < 0.) {bg = 0.;}
    tmp = 1. - pow(1. - bg, c[26]);
    return(tmp);
  }


  std::pair<double, double> incrementTab::gDbmt(double age, double mai, double bm, double sd) {
    double bm1 = gHbmt(age+timeframe, mai) * sd;
    double sd0 = gSd(age, mai, bm, 2);
    double lfz = gLfz(age, mai, sd0);
    double thin = bm - bm1 + lfz;
    if(thin < 0.) {thin=0.;}
    if(bm + lfz > bm1) {lfz = bm1-bm;}
    return(std::pair<double, double> (lfz, thin));
  }

  std::pair<double, double> incrementTab::gDbm(double age, double mai, double bm, double sd) {
    if(sd > 1.) {sd = 1.;}
    double sd0 = gSd(age, mai, bm, 2);
    double bm1 = gHbm(age+timeframe, mai) * sd;
    double lfz = gLfz(age, mai, sd0);
    double thin = bm - bm1 + lfz;
    if(thin < 0.) {thin=0.;}
    if(bm + lfz > bm1) {lfz = bm1-bm;}
    return(std::pair<double, double> (lfz, thin));
  }
  
  double incrementTab::gLfz(double age, double mai, double sd) {
    return((gGwl(age+timeframe, mai) - gGwl(age, mai)) * ic.gImul(sd));
  }

  double incrementTab::gSd(double age, double mai, double bm, int type) {
    //typ 1..Natuerlicher Bestockungsgrad, 2..Ertragstafel bestockungsgrad
    double ret=1.;
    if(type == 1) {
      double hbm = gHbm(age, mai);
      if(hbm > 0.) {ret = bm/hbm;}
      //if(ret > 1.) {ret = 1.;}
    } else {
      double hbm = gHbmt(age, mai);
      if(hbm > 0.) {ret = bm/hbm;}
    }
    if(ret < 0.) {ret = 0.;}
    return(ret);
  }

  double incrementTab::gU(double bm, double mai, double sd) {
    //bm .. Biomass, mai .. ean annual incremetn, sd .. stocking degree
    int tL = 0;
    int tH = ceil(gTopt(mai, 2));
    double bmL = 0;
    double bmH = gAvgBm(tH, mai) * sd;
    double t = 0.;
    if(bm < bmH) {
      int tX = 0;
      double bmX = 0.;
      do {
	if(bmX > bm) {tH = tX; bmH = bmX;}
	else {tL = tX; bmL = bmX;}
	tX = floor(0.5+interpol(tL, tH, bmL, bmH, bm));
	bmX = gAvgBm(tX, mai) * sd;
      } while(tX != tL && tX != tH);
      t = interpol(tL, tH, bmL, bmH, bm);
    } else {t = tH;}
    return(t);
  }
  
  double incrementTab::gUt(double bm, double mai, double sd) {
    //bm .. Biomass, mai .. ean annual incremetn, sd .. stocking degree
    int tL = 0;
    int tH = ceil(gTopt(mai, 3));
    double bmL = 0;
    double bmH = gAvgBmt(tH, mai) * sd;
    double t = 0.;
    if(bm < bmH) {
      int tX = 0;
      double bmX = 0.;
      do {
	if(bmX > bm) {tH = tX; bmH = bmX;}
	else {tL = tX; bmL = bmX;}
	tX = floor(0.5+interpol(tL, tH, bmL, bmH, bm));
	bmX = gAvgBmt(tX, mai) * sd;
      } while(tX != tL && tX != tH);
      t = interpol(tL, tH, bmL, bmH, bm);
    } else {t = tH;}
    return(t);
  }
  
  double incrementTab::gTopt(double mai, int type) {
    if(type < 0) {type = 0;}
    if(type > 5) {type = 5;}
    int cmai = mai/maiStep;
    if(cmai >= nmai-1) {cmai = nmai-2;}
    if(cmai <= 0) {cmai = 1;}
    double t = interpol(optTime[cmai + type*nmai], optTime[cmai+1 + type*nmai], cmai, cmai+1, cmai);
    if(t < 0.) {t = 0.;}
    return(t);
  }
  
  double incrementTab::ip(double u, double mai, double *tab) {
    mai /= maiStep;
    if(mai >= nmai) {mai=nmai-1;}
    if(mai < 0.) {mai=0.;}
    int maih = ceil(mai);
    int mail = floor(mai);
    u /= tStep;
    if(u >= nt) {u=nt-1.;}
    if(u < 0) {u=0.;}
    int uh = ceil(u/tStep);
    int ul = floor(u/tStep);
    double t1 = interpol(tab[ul + mail*nt], tab[uh + mail*nt], ul, uh, u);
    double t2 = interpol(tab[ul + maih*nt], tab[uh + maih*nt], ul, uh, u);
    return(interpol(t1, t2, mail, maih, mai));
  }
  
  double incrementTab::gHbm(double u, double mai) {
    return(ip(u, mai, bm));
  }
  
  double incrementTab::gHbmt(double u, double mai) {
    return(ip(u, mai, bmt));
  }

  double incrementTab::gH(double u, double mai) {
    return(ip(u, mai, height));
  }

  double incrementTab::gHi(double u, double mai) {
    return(gH(u+timeframe, mai) - gH(u, mai));
  }
  
  double incrementTab::gD(double u, double mai, double sd) {
    double d = ip(u, mai, dbh);
    if(sd < 1. || sd > 1.) {
      double dmulOpt = ic.gDmul(gSdopt2Sdnat(u, mai, 1.));
      double dmulAkt = ic.gDmul(gSdopt2Sdnat(u, mai, sd));
      d *= dmulAkt/dmulOpt;
    }
    return(d);
  }

  double incrementTab::gDi(double u, double mai, double sd) {
    double di = gD(u+timeframe, mai, sd) - gD(u, mai, sd);
    return(di);
  }

  double incrementTab::gSdopt2Sdnat(double u, double mai, double sd) {
    return(ip(u, mai, sdNat)*sd);
  }

  double incrementTab::gGwl(double u, double mai) {
    return(ip(u, mai, gwl));
  }
  
  double incrementTab::gMai(double u, double mai) {
    double ret=0.;
    if(u > 0.) {ret = ip(u, mai, gwl)/u;}
    return(ret);
  }
  
  double incrementTab::gAvgBm(double u, double mai) {
    return(ip(u, mai, abm));
  }
  
  double incrementTab::gAvgBmt(double u, double mai) {
    return(ip(u, mai, abmt));
  }
  
  incrementTab::~incrementTab() {
    delete[] abm;
    delete[] abmt;
    delete[] bm;
    delete[] bmt;
    delete[] gwl;
    delete[] dbh;
    delete[] height;
    delete[] sdNat;
    delete[] optTime;
  }
  
  incrementTab::incrementTab(double a0, double a1, double a2, double a3, double a4,
			     double a5, double a6, double a7, double a8, double a9,
			     double a10, double a11, double a12, double a13,
			     double a14, double a15, double a16, double a17,
			     double a18, double a19, double a20, double a21,
			     double a22, double a23, double a24, double a25,
			     double a26,
			     double amaiHi, double amaiStep,
			     double atHi, double atStep,
			     double atimeframe) :
    ic(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15
       , a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, 1.)
  {
    maiHi = amaiHi; maiStep = amaiStep;
    tHi = atHi; tStep = atStep;
    if(tHi < 0.) {tHi = 0.;}
    if(tStep <= 0. && tHi > 0.) {tStep = tHi/100.;}
    if(maiHi < 0.) {maiHi = 0.;}
    if(maiStep <= 0. && maiHi > 0.) {maiStep = maiHi/20.;}
    nt = 0;
    if(tHi > 0. && tStep > 0.) {
      nt = 1+ceil(tHi/tStep);
    }
    nmai = 0;
    if(maiHi > 0. && maiStep > 0.) {
      nmai = 1+ceil(maiHi/maiStep);
    }
    
    abm = new double[nt*nmai];
    abmt = new double[nt*nmai];
    bm = new double[nt*nmai];
    bmt = new double[nt*nmai];
    gwl = new double[nt*nmai];
    dbh = new double[nt*nmai];
    height = new double[nt*nmai];
    sdNat = new double[nt*nmai];
    optTime = new int[6*nmai];
    timeframe = atimeframe;
    fillTables();
  }
  
  int incrementTab::fillTables() {
    //Fill the arrays with gwl, biomass, ... for age/yield
    for(int cmai=0; cmai<nmai; ++cmai) {
      ic.setMai(cmai * maiStep);
      double sbm=0.; double sbmt=0.;
      for(int ct=0; ct<nt; ++ct) {
	double t=ct*tStep;
	sdNat[ct + cmai*nt] = ic.gManDens(t);
	dbh[ct + cmai*nt] = ic.gD(t);
	height[ct + cmai*nt] = ic.gH(t);
	gwl[ct + cmai*nt] = ic.gTcp(t);
	bm[ct + cmai*nt] = gwl[ct + cmai*nt] * ic.gMaxDens(t);
	bmt[ct + cmai*nt] = bm[ct + cmai*nt] * ic.gManDens(t);
	sbm += bm[ct + cmai*nt];
	sbmt += bmt[ct + cmai*nt];
	if(t > 0.) {
	  abm[ct + cmai*nt] = sbm/t;
	  abmt[ct + cmai*nt] = sbmt/t;
	} else {
	  abm[ct + cmai*nt] = 0.;
	  abmt[ct + cmai*nt] = 0.;
	}
      }
    }
    //Search optimal rotation times
    for(int cmai=0; cmai<nmai; ++cmai) {
      optTime[cmai + 0*nmai] = nt-1;  //0 .. Rotation time Unmanaged forests
      for(int ct=nt-2; ct>=0; --ct) {
	if(bm[ct + cmai*nt] <= 0.) {optTime[cmai + 0*nmai] = ct;}
	else {break;}
      }
      optTime[cmai + 1*nmai] = 1;  //1 .. Highest average increment
      optTime[cmai + 2*nmai] = 0;  //2 .. Maximum avarage Biomass
      optTime[cmai + 3*nmai] = 0;  //3 .. Maximum average Biomass with thinning
      optTime[cmai + 4*nmai] = 0;  //4 .. Maximum harvest at final cut
      optTime[cmai + 5*nmai] = 1;  //5 .. Maximum average harvest with final cut
      for(int ct=1; ct<nt; ++ct) {
	if(gwl[ct + cmai*nt]/ct > gwl[optTime[cmai + 1*nmai] + cmai*nt]/optTime[cmai + 1*nmai]) {optTime[cmai + 1*nmai] = ct;}
	if(abm[ct + cmai*nt] > abm[optTime[cmai + 2*nmai] + cmai*nt]) {optTime[cmai + 2*nmai] = ct;}
	if(abmt[ct + cmai*nt] > abmt[optTime[cmai + 3*nmai] + cmai*nt]) {optTime[cmai + 3*nmai] = ct;}
	if(bm[ct + cmai*nt] > bm[optTime[cmai + 4*nmai] + cmai*nt]) {optTime[cmai + 4*nmai] = ct;}
	if(bm[ct + cmai*nt]/ct > bm[optTime[cmai + 5*nmai] + cmai*nt]/optTime[cmai + 5*nmai]) {optTime[cmai + 5*nmai] = ct;}
      }
    }
    return(0);
  }
  
  int incrementTab::setCoef
  (double a0, double a1, double a2, double a3, double a4, double a5, double a6
   , double a7, double a8, double a9, double a10, double a11, double a12
   , double a13, double a14, double a15, double a16, double a17, double a18
   , double a19, double a20, double a21, double a22, double a23, double a24
   , double a25, double a26) {
    ic.setCoef(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14
	       , a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26);
    fillTables();
    return(0);
  }
  
}
