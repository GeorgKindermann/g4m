#include "increment.h"

namespace {
  double interpol(double const &x0, double const &x1, double const &y0, double const &y1, double const &y) {
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
  (const double &a0, const double &a1, const double &a2, const double &a3,
   const double &a4, const double &a5, const double &a6, const double &a7,
   const double &a8, const double &a9, const double &a10, const double &a11,
   const double &a12, const double &a13, const double &a14, const double &a15,
   const double &a16, const double &a17, const double &a18, const double &a19,
   const double &a20, const double &a21, const double &a22, const double &a23,
   const double &a24, const double &a25, const double &a26, const double &a27,
   const double &a28, const double &a29, const double &a30, const double &a31,
   const double &a32, const double &a33, const double &a34, const double &a35,
   const double &a36, const double &a37, const double &a38, const double &a39,
   const double &a40, const double &a41, const double &a42, const double &a43,
   const double &a44, const double &a45, const double &a46, const double &a47,
   const double &amai)
  {
    coef.k[0] = a0; coef.k[1] = a1; coef.k[2] = a2; coef.k[3] = a3;
    coef.tMax[0] = a4; coef.tMax[1] = a5; coef.tMax[2] = a6; coef.tMax[3] = a7;
    coef.maxDens[0] = a8; coef.maxDens[1] = a9; coef.maxDens[2] = a10;
    coef.maxDens[3] = a11; coef.maxDens[4] = a12; coef.maxDens[5] = a13;
    coef.maxDens[6] = a14; coef.maxDens[7] = a15; coef.maxDens[8] = a16;
    coef.maxDens[9] = a17; coef.maxDens[10] = a18; coef.maxDens[11] = a19;
    coef.maxDens[12] = a20; coef.maxDens[13] = a21; coef.maxDens[14] = a22;
    coef.imul[0] = a23; coef.imul[1] = a24; coef.imul[2] = a25;
    coef.imul[3] = a26; coef.imul[4] = a27; coef.imul[5] = a28;
    coef.imul[6] = a29; coef.imul[7] = a30; coef.imul[8] = a31;
    coef.imul[9] = a32;
    coef.h[0] = a33; coef.h[1] = a34; coef.h[2] = a35; coef.h[3] = a36;
    coef.h[4] = a37;
    coef.d[0] = a38; coef.d[1] = a39; coef.d[2] = a40; coef.d[3] = a41;
    coef.d[4] = a42; coef.d[5] = a43; coef.d[6] = a44; coef.d[7] = a45;
    coef.dmul[0] = a46;
    coef.iCrit = a47;
    mai = amai;
    calcK(); calcTMax(); calcTcpMax(); calcTOpt(); calcTh13();
  }

  int incrementCurves::setCoef
  (const double &a0, const double &a1, const double &a2, const double &a3,
   const double &a4, const double &a5, const double &a6, const double &a7,
   const double &a8, const double &a9, const double &a10, const double &a11,
   const double &a12, const double &a13, const double &a14, const double &a15,
   const double &a16, const double &a17, const double &a18, const double &a19,
   const double &a20, const double &a21, const double &a22, const double &a23,
   const double &a24, const double &a25, const double &a26, const double &a27,
   const double &a28, const double &a29, const double &a30, const double &a31,
   const double &a32, const double &a33, const double &a34, const double &a35,
   const double &a36, const double &a37, const double &a38, const double &a39,
   const double &a40, const double &a41, const double &a42, const double &a43,
   const double &a44, const double &a45, const double &a46, const double &a47)
  {
    coef.k[0] = a0; coef.k[1] = a1; coef.k[2] = a2; coef.k[3] = a3;
    coef.tMax[0] = a4; coef.tMax[1] = a5; coef.tMax[2] = a6; coef.tMax[3] = a7;
    coef.maxDens[0] = a8; coef.maxDens[1] = a9; coef.maxDens[2] = a10;
    coef.maxDens[3] = a11; coef.maxDens[4] = a12; coef.maxDens[5] = a13;
    coef.maxDens[6] = a14; coef.maxDens[7] = a15; coef.maxDens[8] = a16;
    coef.maxDens[9] = a17; coef.maxDens[10] = a18; coef.maxDens[11] = a19;
    coef.maxDens[12] = a20; coef.maxDens[13] = a21; coef.maxDens[14] = a22;
    coef.imul[0] = a23; coef.imul[1] = a24; coef.imul[2] = a25;
    coef.imul[3] = a26; coef.imul[4] = a27; coef.imul[5] = a28;
    coef.imul[6] = a29; coef.imul[7] = a30; coef.imul[8] = a31;
    coef.imul[9] = a32;
    coef.h[0] = a33; coef.h[1] = a34; coef.h[2] = a35; coef.h[3] = a36;
    coef.h[4] = a37;
    coef.d[0] = a38; coef.d[1] = a39; coef.d[2] = a40; coef.d[3] = a41;
    coef.d[4] = a42; coef.d[5] = a43; coef.d[6] = a44; coef.d[7] = a45;
    coef.dmul[0] = a46;
    coef.iCrit = a47;
    calcK(); calcTMax(); calcTcpMax(); calcTOpt(); calcTh13();
    return(0);
  }

  double incrementCurves::calcK() {
    double tmp = coef.k[0] + coef.k[1] * exp(coef.k[2]*pow(mai,coef.k[3]));
    if(tmp > 0.) {tmp = 0.;}
    k = tmp;
    return(tmp);
  }

  double incrementCurves::calcTMax() {
    double tmp = coef.tMax[0] + coef.tMax[1]/
      (1 + exp(coef.tMax[2] + coef.tMax[3]*mai));
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
    double t1 = coef.h[0] * pow(mai, coef.h[1]);
    double t2 = coef.h[3] * pow(mai, coef.h[4]);
    double res = log(1-pow(1.3/t1,1/t2))/coef.h[2];
    if(res < 0.) {res = 0.;}
    if(mai <= 0.) {res = INFINITY;}
    th13=res;
    return(res);
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
    double f[4];
    double *c = coef.maxDens;
    f[0] = c[0] + c[1]/(1 + exp(c[2]+c[3]*mai));
    f[1] = c[4]/(1+exp(c[5] + c[6]*mai)) + c[7]/(1+exp(c[8] + c[9]*mai));
    f[2] = c[10] + c[11]*exp(c[12]+c[13]*mai);
    f[3] = c[14];
    if(t > 0. && tOpt > 0.) {
      double tmp1 = f[2]*t/tOpt;
      if(tmp1 > 0) {
	if(tmp1 < 1) {
	  ret = (f[0] + f[1] * log(t/tOpt)) * pow(1. - tmp1, f[3]);
	} else {ret = 0.;}
      }
    }
    if(ret < 0.) {ret = 0.;}
    if(ret > 1.) {ret = 1.;}
    return(ret);
  }

  double incrementCurves::gManDens(double t) {
    //ipol<double, double> tmp;
    //tmp.insert(0.,0.); tmp.insert(1.,1.);
    //for(int i=1; i<10; ++i) {
    //  tmp.insert(gImul(double(i)/10., t), double(i)/10.);
    //}
    //return(tmp.g(coef.iCrit));
    int n=0;
    double up=1.;
    double lo=0.;
    double x=0.8;
    double vup=1.;
    double vlo=0.;
    double vx=gImul(x, t);
    while(std::abs(vx - coef.iCrit) > 0.01 && n<10) {
      if(vx > coef.iCrit) {up = x; vup = vx;}
      else {lo = x; vlo = vx;}
      x = lo + (coef.iCrit-vlo)*(up-lo)/(vup-vlo);
      vx=gImul(x, t);
      ++n;
    }
    return(x);
  }  

  double incrementCurves::gH(double t) {
    double tmp = 0.;
    if(t>0) {
      tmp = coef.h[0] * pow(mai, coef.h[1]) *
	pow(1. - exp(coef.h[2] * t), coef.h[3] * pow(mai,coef.h[4]));
    }     
    if(tmp < 0) {tmp = 0.;}
    return(tmp);
  }

  double incrementCurves::gD(double t) {
    double tmp = 0.;
    if(t>th13 && mai>0.) {
      double a = coef.d[0] + coef.d[1]*mai;
      double b = coef.d[2] / (1. + coef.d[3] * pow(mai, coef.d[4]));
      double c = coef.d[5] / (1. + coef.d[6] * pow(mai, coef.d[7]));
      tmp = a * pow(1. - exp(b * (t - th13)), c);
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
    tmp = 2 - pow(bg, coef.dmul[0]);
    return(tmp);
  }

  double incrementCurves::gImul(double bg, double t) {
    double res = 1.;
    double *c = coef.imul;
    if(bg <= 0.) {res = 0.;}
    else if(bg < 1. && t > 0.) {
      double o0 = 1. + pow(t,c[0])/c[1] * 1./(c[2] + c[3]*pow(mai,c[4]));
      double o1 = 1. + c[5] / (1. + c[6]*pow(t,c[7]) * 1./(c[8]+pow(mai,c[9])));
      double p = pow(1./o0, 1./(o0-1.));
      double p2 = p - pow(p, o0);
      double p4 = p * o1;
      double p3 = p;
      if(p3 > bg*p4) {p3 = bg*p4;}
      res = (p3 - pow(p3, o0)) / p2;
    }
    return(res);
  }


  incrementTab::incrementTab
      (const double &a0, const double &a1, const double &a2, const double &a3,
       const double &a4, const double &a5, const double &a6, const double &a7,
    const double &a8, const double &a9, const double &a10, const double &a11,
    const double &a12, const double &a13, const double &a14, const double &a15,
    const double &a16, const double &a17, const double &a18, const double &a19,
    const double &a20, const double &a21, const double &a22, const double &a23,
    const double &a24, const double &a25, const double &a26, const double &a27,
    const double &a28, const double &a29, const double &a30, const double &a31,
    const double &a32, const double &a33, const double &a34, const double &a35,
    const double &a36, const double &a37, const double &a38, const double &a39,
    const double &a40, const double &a41, const double &a42, const double &a43,
    const double &a44, const double &a45, const double &a46, const double &a47,
       const double amaiMax, const double amaiStep, const double atMax,
       const double atStep, const double asdNatStep, const double asdTabMax,
       const double asdTabStep, const double atimeframe) :
     ic(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15
	, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29
	, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43
	, a44, a45, a46, a47, 1.)
  {
    maiHi = amaiMax; maiStep = amaiStep;
    tHi = atMax; tStep = atStep;
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
    sdNatStep = asdNatStep;
    nsdNat = 0;
    if(sdNatStep > 0.) {
      nsdNat = 1+ceil(1./sdNatStep);
    }
    sdTabHi=asdTabMax; sdTabStep = asdTabStep;
    if(sdTabHi > 0. && sdTabStep > 0.) {
      nsdTab = 1+ceil(sdTabHi/sdTabStep);
    }
    timeframe = atimeframe;
    abm = new double[nt*nmai];
    abmt = new double[nt*nmai];
    abmSdTab = new double[nt*nmai*nsdTab];  //sd below 1 is not used
    bm = new double[nt*nmai];
    bmt = new double[nt*nmai];
    gwl = new double[nt*nmai];
    gwlt = new double[nt*nmai];
    gwlSdNat = new double[nt*nmai*nsdNat];
    gwlSdTab = new double[nt*nmai*nsdTab];
    dbh = new double[nt*nmai];
    dbht = new double[nt*nmai];
    height = new double[nt*nmai];
    sdNat = new double[nt*nmai];
    incGwl = new double[nt*nmai];  //The inc arrays could be in size (nt-1)
    incGwlt = new double[nt*nmai];
    incBm = new double[nt*nmai];
    incBmt = new double[nt*nmai];
    incDbh = new double[nt*nmai];
    incDbht = new double[nt*nmai];
    incHeight = new double[nt*nmai];
    incGwlSdNat = new double[nt*nmai*nsdNat];
    incDbhSdNat = new double[nt*nmai*nsdNat];
    incGwlSdTab = new double[nt*nmai*nsdTab];
    incDbhSdTab = new double[nt*nmai*nsdTab];
    dbhSdTab = new double[nt*nmai*nsdTab];
    optTime = new optRotTimes[nmai];
    optTimet = new optRotTimes[nmai];
    optTimeSdNat = new optRotTimes[nmai*nsdNat];
    optTimeSdTab = new optRotTimes[nmai*nsdTab];
    fillTables();
  }

  incrementTab::~incrementTab() {
    delete[] abm;
    delete[] abmt;
    delete[] abmSdTab;
    delete[] bm;
    delete[] bmt;
    delete[] gwl;
    delete[] gwlt;
    delete[] gwlSdNat;
    delete[] gwlSdTab;
    delete[] dbh;
    delete[] dbht;
    delete[] height;
    delete[] sdNat;
    delete[] incGwl;
    delete[] incGwlt;
    delete[] incBm;
    delete[] incBmt;
    delete[] incDbh;
    delete[] incDbht;
    delete[] incHeight;
    delete[] incGwlSdNat;
    delete[] incDbhSdNat;
    delete[] incGwlSdTab;
    delete[] incDbhSdTab;
    delete[] dbhSdTab;
    delete[] optTime;
    delete[] optTimet;
    delete[] optTimeSdNat;
    delete[] optTimeSdTab;
  }

  int incrementTab::fillTables() {
    double minMAI = 0.01;
    //Fill the arrays with gwl, biomass, ... for age/yield
    for(int cmai=0; cmai<nmai; ++cmai) {
      if(cmai * maiStep > minMAI) {ic.setMai(cmai * maiStep);}
      else {ic.setMai(minMAI);}
      double sbm=0.; double sbmt=0.; //Sum of Biomass (with treatment)
      std::vector<double> sbmSdTab;
      sbmSdTab.resize(nsdTab, 0.);
      for(int ct=0; ct<nt; ++ct) {
	double t=ct*tStep;
//Stokcing degree where 95% of the increment of a full stocked forest is reaced
	double sd95inc = ic.gManDens(t);
	sdNat[ct + cmai*nt] = sd95inc;
	dbh[ct + cmai*nt] = ic.gD(t);
	height[ct + cmai*nt] = ic.gH(t);
	double gwlMaxDens = ic.gTcp(t);
	gwl[ct + cmai*nt] = gwlMaxDens;
	double bmMaxDens = gwlMaxDens * ic.gMaxDens(t);
	bm[ct + cmai*nt] = bmMaxDens;
	double bmSd95inc = bmMaxDens * sd95inc;
	bmt[ct + cmai*nt] = bmSd95inc;
	sbm += bmMaxDens;
	sbmt += bmSd95inc;
	if(t > 0.) {
	  abm[ct + cmai*nt] = sbm*tStep/t;
	  abmt[ct + cmai*nt] = sbmt*tStep/t;
	} else {
	  abm[ct + cmai*nt] = 0.;
	  abmt[ct + cmai*nt] = 0.;
	}
	//Go to all natural stocking degree steps
	for(int csd=0; csd<nsdTab; ++csd) {
	  double tabSd = csd*sdTabStep;  //Yield table stocking degree
	  double bmSd = bmSd95inc * tabSd;
	  if(bmSd > bmMaxDens) {bmSd = bmMaxDens;}
	  sbmSdTab[csd] += bmSd;
	  if(t > 0.) {
	    abmSdTab[ct + cmai*nt + csd*nt*nmai] = sbmSdTab[csd]*tStep/t;
	  } else {
	    abmSdTab[ct + cmai*nt + csd*nt*nmai] = 0.;
	  }
	}
      }
      //Go to all time steps and get the increments
      dbht[0 + cmai*nt] = 0.;
      gwlt[0 + cmai*nt] = 0.;
      for(int csd=0; csd<nsdTab; ++csd) {
	dbhSdTab[0 + cmai*nt + csd*nt*nmai] = 0.;
	//gwlSdNat[0 + cmai*nt + csd*nt*nmai] = 0.;
	gwlSdTab[0 + cmai*nt + csd*nt*nmai] = 0.;
      }
      for(int csd=0; csd<nsdNat; ++csd) {
	gwlSdNat[0 + cmai*nt + csd*nt*nmai] = 0.;
      }
      for(int ct=1; ct<nt; ++ct) {
	//Natural stocking degree at beginn (0) and end (1) of period of
	//Yield table stocking degre = 1
	double sdNat0 = sdNat[ct - 1 + cmai*nt];
	double sdNat1 = sdNat[ct + cmai*nt];
	double t0=(ct-1)*tStep;
	double t1=ct*tStep;
	if(timeframe <= 0.) {incHeight[ct-1 + cmai*nt] = height[ct + cmai*nt] - height[ct-1 + cmai*nt];
	} else {
	  incHeight[ct-1 + cmai*nt] = ip(t0+timeframe, cmai*maiStep, height) - height[ct-1 + cmai*nt];
	}
	double incDbhMaxDens = dbh[ct + cmai*nt] - dbh[ct-1 + cmai*nt];
	double incDbhMaxDensY = incDbhMaxDens;
	if(timeframe <= 0.) {incDbh[ct-1 + cmai*nt] = incDbhMaxDens;
	} else {incDbhMaxDensY = ip(t0+timeframe, cmai*maiStep, dbh) - dbh[ct-1 + cmai*nt];
	  incDbh[ct-1 + cmai*nt] = incDbhMaxDensY;}
	double incDbht0 = incDbhMaxDens*(ic.gDmul(sdNat1)+ic.gDmul(sdNat0))/2.;
	if(timeframe <= 0.) {incDbht[ct-1 + cmai*nt] = incDbht0;
	} else {incDbht[ct-1 + cmai*nt] = incDbhMaxDensY*(ic.gDmul(ip(t0+timeframe,cmai*maiStep,sdNat))+ic.gDmul(sdNat0))/2.;}
	dbht[ct +cmai*nt] = dbht[ct-1 +cmai*nt] + incDbht0;
	double incGwlMaxDens = gwl[ct + cmai*nt] - gwl[ct-1 + cmai*nt];
	double incGwlMaxDensY = incGwlMaxDens;
	if(timeframe <= 0.) {incGwl[ct-1 + cmai*nt] = incGwlMaxDens;
	} else {incGwlMaxDensY = ip(t0+timeframe, cmai*maiStep, gwl) - gwl[ct-1 + cmai*nt];
	  incGwl[ct-1 + cmai*nt] = incGwlMaxDensY;}
	double incGwlt0 = incGwlMaxDens * (ic.gImul(sdNat1, t1) + ic.gImul(sdNat0, t0)) / 2.;
	if(timeframe <= 0.) {incGwlt[ct-1 + cmai*nt] = incGwlt0;
	} else {incGwlt[ct-1 + cmai*nt] = incGwlMaxDensY * (ic.gImul(ip(t0+timeframe,cmai*maiStep,sdNat), t0+timeframe) + ic.gImul(sdNat0, t0)) / 2.;}
	gwlt[ct + cmai*nt] = gwlt[ct-1 + cmai*nt] + incGwlt0;
	if(timeframe <= 0.) {
	  incBm[ct-1 + cmai*nt] = bm[ct + cmai*nt] - bm[ct-1 + cmai*nt];
	  incBmt[ct-1 + cmai*nt] = bmt[ct + cmai*nt] - bmt[ct-1 + cmai*nt];
	} else {
  incBm[ct-1 +cmai*nt] = ip(t0+timeframe,cmai*maiStep,bm)-bm[ct-1 + cmai*nt];
  incBmt[ct-1 +cmai*nt] =ip(t0+timeframe,cmai*maiStep,bmt)-bmt[ct-1 + cmai*nt];}
	//Go to all natural stocking degree steps
	for(int csd=0; csd<nsdNat; ++csd) {
	  double natSd = csd*sdNatStep;  //Natural stocking degree
	  incDbhSdNat[ct-1 + cmai*nt + csd*nt*nmai] = incDbhMaxDensY * ic.gDmul(natSd);
	  incGwlSdNat[ct-1 +cmai*nt +csd*nt*nmai] = incGwlMaxDensY * (ic.gImul(natSd,t0) + ic.gImul(natSd,t0+timeframe))/2.;
	  gwlSdNat[ct +cmai*nt +csd*nt*nmai] = gwlSdNat[ct-1 +cmai*nt +csd*nt*nmai] + incGwlMaxDens * (ic.gImul(natSd,t0) + ic.gImul(natSd,t1))/2.;
	}
	//Go to all yield table stocking degree steps
	for(int csd=0; csd<nsdTab; ++csd) {
	  double tabSd = csd*sdTabStep;  //Yield table stocking degree
	  double sdNat2 = ip(t0+timeframe,cmai*maiStep,sdNat);
	  incDbhSdTab[ct-1 + cmai*nt + csd*nt*nmai] = incDbhMaxDensY * (ic.gDmul(tabSd*sdNat2)+ic.gDmul(tabSd*sdNat0))/2.;
	  dbhSdTab[ct + cmai*nt + csd*nt*nmai] = dbhSdTab[ct-1 + cmai*nt + csd*nt*nmai] + incDbhMaxDens * (ic.gDmul(tabSd*sdNat1)+ic.gDmul(tabSd*sdNat0))/2.;
	  //in case "tabSd*sdNatX > 1" gImul returns 1 what is OK
	  incGwlSdTab[ct-1 + cmai*nt + csd*nt*nmai] = incGwlMaxDensY * (ic.gImul(tabSd*sdNat2, t0+timeframe) + ic.gImul(tabSd*sdNat0, t0)) / 2.;
	  gwlSdTab[ct + cmai*nt + csd*nt*nmai] = gwlSdTab[ct-1 + cmai*nt + csd*nt*nmai] + incGwlMaxDens * (ic.gImul(tabSd*sdNat1, t1) + ic.gImul(tabSd*sdNat0, t0)) / 2.;
	}
      }
    }
    //Search optimal rotation times
    for(int cmai=0; cmai<nmai; ++cmai) {
      //Highest Possible Age
      optTime[cmai].maxAge = (nt-1) * tStep;
      {
	int ct=nt-2;
	while(ct>=0 && bm[ct + cmai*nt] <= 0.) {
	  optTime[cmai].maxAge = ct * tStep;
	  --ct;
	}
      }
      optTimet[cmai].maxAge = optTime[cmai].maxAge;
      for(int csd=0; csd<nsdNat; ++csd) {
	optTimeSdNat[cmai + csd*nmai].maxAge = optTime[cmai].maxAge;
      }
      for(int csd=0; csd<nsdTab; ++csd) {
	optTimeSdTab[cmai + csd*nmai].maxAge = optTime[cmai].maxAge;
      }
      //Highest average increment
      {
	int ct=1;      //Current rotation time
	int maxInc;  //last rotation time
	double cBm = gwl[ct + cmai*nt]/ct;    //current biomass
	double maxBm;  //last biomass
	do {
	  maxInc = ct++;
	  maxBm = cBm;
	  cBm = gwl[ct + cmai*nt]/ct;
	} while(cBm > maxBm && ct<nt);
	maxInc *= tStep;
	optTime[cmai].maxInc = maxInc;
	ct = 1;
	cBm = gwlt[ct + cmai*nt]/ct;
	do {
	  maxInc = ct++;
	  maxBm = cBm;
	  cBm = gwlt[ct + cmai*nt]/ct;
	} while(cBm > maxBm && ct<nt);
	maxInc *= tStep;
	optTimet[cmai].maxInc = maxInc;
	for(int csd=0; csd<nsdNat; ++csd) {
	  ct = 1;
	  cBm = gwlSdNat[ct + cmai*nt + csd*nt*nmai]/ct;
	  do {
	    maxInc = ct++;
	    maxBm = cBm;
	    cBm = gwlSdNat[ct + cmai*nt + csd*nt*nmai]/ct;
	  } while(cBm > maxBm && ct<nt);
	  maxInc *= tStep;
	  optTimeSdNat[cmai + csd*nmai].maxInc = maxInc;
	}
	for(int csd=0; csd<nsdTab; ++csd) {
	  ct = 1;
	  cBm = gwlSdTab[ct + cmai*nt + csd*nt*nmai]/ct;
	  do {
	    maxInc = ct++;
	    maxBm = cBm;
	    cBm = gwlSdTab[ct + cmai*nt + csd*nt*nmai]/ct;
	  } while(cBm > maxBm && ct<nt);
	  maxInc *= tStep;
	  optTimeSdTab[cmai + csd*nmai].maxInc = maxInc;
	}
      }
      //Maximum avarage Biomass
      {
	int maxBm = 0;
	int ct=1;
	while(abm[ct + cmai*nt] > abm[maxBm + cmai*nt] && ct<nt) {maxBm = ct++;}
	maxBm *= tStep;
	optTime[cmai].maxBm = maxBm;
	for(int csd=0; csd<nsdNat; ++csd) {
	  optTimeSdNat[cmai + csd*nmai].maxBm = maxBm;}
	maxBm = 0;
	ct=1;
	while(abmt[ct + cmai*nt] > abmt[maxBm + cmai*nt] && ct<nt) {
	  maxBm = ct++;}
	maxBm *= tStep;
	optTimet[cmai].maxBm = maxBm;
	for(int csd=0; csd<nsdTab; ++csd) {
	  optTimeSdTab[cmai + csd*nmai].maxBm = maxBm;}
      }
      //Maximum harvest at final cut
      {
	int maxHarv = 0;
	int ct=1;
	while(bm[ct + cmai*nt] > bm[maxHarv + cmai*nt] && ct<nt) {
	  maxHarv = ct++;}
	maxHarv *= tStep;
	optTime[cmai].maxHarv = maxHarv;
	for(int csd=0; csd<nsdNat; ++csd) {
	  optTimeSdNat[cmai + csd*nmai].maxHarv = maxHarv;}
	maxHarv = 0;
	ct=1;
	while(bmt[ct + cmai*nt] > bmt[maxHarv + cmai*nt] && ct<nt) {
	  maxHarv = ct++;}
	maxHarv *= tStep;
	optTimet[cmai].maxHarv = maxHarv;
	for(int csd=0; csd<nsdTab; ++csd) {
	  optTimeSdTab[cmai + csd*nmai].maxHarv = maxHarv;}
      }
      //Average Maximum harvest at final cut
      {
	int maxAvgHarv = 1;
	int ct=2;
	while(bm[ct + cmai*nt]/ct > bm[maxAvgHarv + cmai*nt]/maxAvgHarv &&
	      ct<nt) {maxAvgHarv = ct++;}
	maxAvgHarv *= tStep;
	optTime[cmai].maxAvgHarv = maxAvgHarv;
	for(int csd=0; csd<nsdNat; ++csd) {
	  optTimeSdNat[cmai + csd*nmai].maxAvgHarv = maxAvgHarv;}
	maxAvgHarv = 1;
	ct=2;
	while(bmt[ct + cmai*nt]/ct > bmt[maxAvgHarv + cmai*nt]/maxAvgHarv &&
	      ct<nt) {maxAvgHarv = ct++;}
	maxAvgHarv *= tStep;
	optTimet[cmai].maxAvgHarv = maxAvgHarv;
	for(int csd=0; csd<nsdTab; ++csd) {
	  optTimeSdTab[cmai + csd*nmai].maxAvgHarv = maxAvgHarv;}
      }
    }
    return(0);
  }

  double incrementTab::ip(double mai, optRotTimes const *tab, int const &type) {
    mai /= maiStep;
    if(mai < 0.) {mai=0.;}
    int maih = ceil(mai);
    if(mai >= nmai) {mai=nmai-1; maih=mai;}
    else if(maih >= nmai) {maih=mai;}
    int mail = floor(mai);
    int uh;
    int ul;
    switch (type) {
    case 1: uh=tab[maih].maxBm; ul=tab[mail].maxBm; break;
    case 2: uh=tab[maih].maxAge; ul=tab[mail].maxAge; break;
    case 3: uh=tab[maih].maxHarv; ul=tab[mail].maxHarv; break;
    case 4: uh=tab[maih].maxAvgHarv; ul=tab[mail].maxAvgHarv; break;
    default: uh=tab[maih].maxInc; ul=tab[mail].maxInc; break;
    }
    return(interpol(ul, uh, mail, maih, mai));
  }

  double incrementTab::ip(double mai, double sd, optRotTimes const *tab, int const &type, bool const sdNat) {
    mai /= maiStep;
    if(mai < 0.) {mai=0.;}
    int maih = ceil(mai);
    if(mai >= nmai) {mai=nmai-1; maih=mai;}
    else if(maih >= nmai) {maih=mai;}
    int mail = floor(mai);
    if(sd < 0.) {sd = 0.;}
    int sdh = ceil(sd);
    if(sdNat == true) {
      sd /= sdNatStep;
      if(sd >= nsdNat) {sd=nsdNat-1.; sdh=sd;}
      else if(sdh >= nsdNat) {sdh=sd;}
    }
    else {
      sd /= sdTabStep;
      if(sd >= nsdTab) {sd=nsdTab-1.; sdh=sd;}
      else if(sdh >= nsdTab) {sdh=sd;}
   }
    int sdl = floor(sd);
    int mhsh, mhsl, mlsh, mlsl;
    switch (type) {
    case 1: mhsh=tab[maih + sdh*nmai].maxBm; mhsl=tab[maih + sdl*nmai].maxBm;
      mlsh=tab[mail + sdh*nmai].maxBm; mlsl=tab[mail + sdl*nmai].maxBm; break;
    case 2: mhsh=tab[maih + sdh*nmai].maxAge; mhsl=tab[maih + sdl*nmai].maxAge;
      mlsh=tab[mail + sdh*nmai].maxAge; mlsl=tab[mail + sdl*nmai].maxAge; break;
    case 3: mhsh=tab[maih +sdh*nmai].maxHarv; mhsl=tab[maih +sdl*nmai].maxHarv;
      mlsh=tab[mail +sdh*nmai].maxHarv; mlsl=tab[mail +sdl*nmai].maxHarv; break;
    case 4: mhsh=tab[maih + sdh*nmai].maxAvgHarv;
      mhsl=tab[maih +sdl*nmai].maxAvgHarv; mlsh=tab[mail +sdh*nmai].maxAvgHarv;
      mlsl=tab[mail + sdl*nmai].maxAvgHarv; break;
    default: mhsh=tab[maih +sdh*nmai].maxInc; mhsl=tab[maih +sdl*nmai].maxInc;
      mlsh=tab[mail +sdh*nmai].maxInc; mlsl=tab[mail +sdl*nmai].maxInc; break;
    }
    double t0 = interpol(mlsl, mhsl, mail, maih, mai);
    double t1 = interpol(mlsh, mhsh, mail, maih, mai);
    return(interpol(t0, t1, sdl, sdh, sd));
  }

  double incrementTab::ip(double u, double mai, double const *tab) {
    mai /= maiStep;
    if(mai < 0.) {mai=0.;}
    int maih = ceil(mai);
    if(mai >= nmai) {mai=nmai-1; maih=mai;}
    else if(maih >= nmai) {maih=mai;}
    int mail = floor(mai);
    u /= tStep;
    if(u < 0) {u=0.;}
    int uh = ceil(u);
    if(u >= nt) {u=nt-1.; uh=u;}
    else if(uh >= nt) {uh=u;}
    int ul = floor(u);
    double t1 = interpol(tab[ul + mail*nt], tab[uh + mail*nt], ul, uh, u);
    double t2 = interpol(tab[ul + maih*nt], tab[uh + maih*nt], ul, uh, u);
    return(interpol(t1, t2, mail, maih, mai));
  }

  double incrementTab::ip(double u, double mai, double sd, double const *tab, bool const sdNat) {
    mai /= maiStep;

   if(mai < 0.) {mai=0.;}
    int maih = ceil(mai);
    if(mai >= nmai) {mai=nmai-1; maih=mai;}
    else if(maih >= nmai) {maih=mai;}
    int mail = floor(mai);
    u /= tStep;
    if(u < 0) {u=0.;}
    int uh = ceil(u);
    if(u >= nt) {u=nt-1.; uh=u;}
    else if(uh >= nt) {uh=u;}
    int ul = floor(u);

    if(sd < 0.) {sd = 0.;}
    int sdh = ceil(sd);
    if(sdNat == true) {
      sd /= sdNatStep;
      if(sd >= nsdNat) {sd=nsdNat-1.; sdh=sd;}
      else if(sdh >= nsdNat) {sdh=sd;}
    }
    else {
      sd /= sdTabStep;
      if(sd >= nsdTab) {sd=nsdTab-1.; sdh=sd;}
      else if(sdh >= nsdTab) {sdh=sd;}
    }
    int sdl = floor(sd);
    double t1 = interpol(tab[ul + mail*nt + sdl*nt*nmai], tab[uh + mail*nt + sdl*nt*nmai], ul, uh, u);
    double t2 = interpol(tab[ul + maih*nt + sdl*nt*nmai], tab[uh + maih*nt + sdl*nt*nmai], ul, uh, u);
    double t0 = interpol(t1, t2, mail, maih, mai);
    t1 = interpol(tab[ul + mail*nt + sdh*nt*nmai], tab[uh + mail*nt + sdh*nt*nmai], ul, uh, u);
    t2 = interpol(tab[ul + maih*nt + sdh*nt*nmai], tab[uh + maih*nt + sdh*nt*nmai], ul, uh, u);
    t1 = interpol(t1, t2, mail, maih, mai);
    return(interpol(t0, t1, sdl, sdh, sd));
  }

  double incrementTab::gAvgBm(double u, double mai) {return(ip(u, mai, abm));}
  double incrementTab::gAvgBmt(double u, double mai) {return(ip(u, mai, abmt));}
  double incrementTab::gAvgBmSdNat(double u, double mai, double sd) {
    if(sd > 1.) {sd = 1.;}
    if(sd < 0.) {sd = 0.;}
    return(ip(u, mai, abm) * sd);}
  double incrementTab::gAvgBmSdTab(double u, double mai, double sd) {
    if(sd < 0.) {sd = 0.;}
    if(sd <= 1.) {return(ip(u, mai, abmt) * sd);}
    return(ip(u, mai, sd, abmSdTab, false));}

  double incrementTab::gBm(double age, double mai) {return(ip(age, mai, bm));}
  double incrementTab::gBmt(double age, double mai) {return(ip(age, mai, bmt));}
  double incrementTab::gBmSdNat(double age, double mai, double sd) {
    if(sd > 1.) {sd = 1.;}
    if(sd < 0.) {sd = 0.;}
    return(ip(age, mai, bm) * sd);}
  double incrementTab::gBmSdTab(double age, double mai, double sd) {
    if(sd < 0.) {sd = 0.;}
    if(sd <= 1.) {return(ip(age, mai, bmt) * sd);}
    double t1 = ip(age, mai, bmt) * sd;
    double t2 = ip(age, mai, bm);
    if(t1 > t2) {t1 = t2;}
    return(t1);
  }

  double incrementTab::gDbh(double age, double mai) {return(ip(age, mai, dbh));}
  double incrementTab::gDbht(double age, double mai) {return(ip(age,mai,dbht));}
  double incrementTab::gDbhSdNat(double age, double mai, double sd) {
    if(sd > 1.) {sd = 1.;}
    if(sd < 0.) {sd = 0.;}
    return(ip(age, mai, dbh) * ic.gDmul(sd));}
  double incrementTab::gDbhSdTab(double age, double mai, double sd) {
    return(ip(age, mai, sd, dbhSdTab, false));}

  double incrementTab::gGwl(double age, double mai) {return(ip(age, mai, gwl));}
  double incrementTab::gGwlt(double age, double mai) {return(ip(age,mai,gwlt));}
  double incrementTab::gGwlSdNat(double age, double mai, double sd) {
    return(ip(age, mai, sd, gwlSdNat, true));}
  double incrementTab::gGwlSdTab(double age, double mai, double sd) {
    return(ip(age, mai, sd, gwlSdTab, false));}

  double incrementTab::gHeight(double age, double mai) {
    return(ip(age, mai, height));}

  double incrementTab::gSdNat(double age, double mai) {
    return(ip(age, mai, sdNat));}

  double incrementTab::gIncGwl(double age, double mai) {return(ip(age, mai, incGwl));}
  double incrementTab::gIncGwlt(double age, double mai) {return(ip(age,mai,incGwlt));}
  double incrementTab::gIncGwlSdNat(double age, double mai, double sd) {
    return(ip(age, mai, sd, incGwlSdNat, true));}
  double incrementTab::gIncGwlSdTab(double age, double mai, double sd) {
    return(ip(age, mai, sd, incGwlSdTab, false));}

  double incrementTab::gIncBm(double age, double mai) {
    return(ip(age, mai, incBm));}
  double incrementTab::gIncBmt(double age, double mai) {
    return(ip(age,mai,incBmt));}
  double incrementTab::gIncBmSdNat(double age, double mai, double sd) {
    if(sd > 1.) {sd = 1.;}
    if(sd < 0.) {sd = 0.;}
    return(ip(age, mai, incBm) * sd);}
  double incrementTab::gIncBmSdTab(double age, double mai, double sd) {
    if(sd < 0.) {sd = 0.;}
    if(sd <= 1.) {return(ip(age, mai, incBmt) * sd);}
    double t1 = ip(age, mai, incBmt) * sd;
    double t2 = ip(age, mai, incBm);
    if(t1 > t2) {t1 = t2;}
    return(t1);
  }

  double incrementTab::gIncDbh(double age, double mai) {
    return(ip(age, mai, incDbh));}
  double incrementTab::gIncDbht(double age, double mai) {
    return(ip(age, mai, incDbht));}
  double incrementTab::gIncDbhSdNat(double age, double mai, double sd) {
    return(ip(age, mai, sd, incDbhSdNat, true));}
  double incrementTab::gIncDbhSdTab(double age, double mai, double sd) {
    return(ip(age, mai, sd, incDbhSdTab, false));}

  double incrementTab::gIncHeight(double age, double mai) {
    return(ip(age, mai, incHeight));}

  double incrementTab::gRemBm(double age, double mai) {
    return(gIncGwl(age, mai) - gIncBm(age, mai));}
  double incrementTab::gRemBmt(double age, double mai) {
    return(gIncGwlt(age, mai) - gIncBmt(age, mai));}
  double incrementTab::gRemBmSdNat(double age, double mai, double sd) {
    return(gIncGwlSdNat(age, mai, sd) -gIncBmSdNat(age, mai, sd));}
  double incrementTab::gRemBmSdTab(double age, double mai, double sd) {
    return(gIncGwlSdTab(age, mai, sd) -gIncBmSdTab(age, mai, sd));}

  double incrementTab::gMai(double u, double mai) {
    if(u > 0.) {return(gGwl(u, mai)/u);}
    return(0.);}
  double incrementTab::gMait(double u, double mai) {
    if(u > 0.) {return(gGwlt(u, mai)/u);}
    return(0.);}
  double incrementTab::gMaiSdNat(double u, double mai, double sd) {
    if(u > 0.) {return(gGwlSdNat(u, mai, sd)/u);}
    return(0.);}
  double incrementTab::gMaiSdTab(double u, double mai, double sd) {
    if(u > 0.) {return(gGwlSdTab(u, mai, sd)/u);}
    return(0.);}

  double incrementTab::gTopt(double mai, int type) {
    return(ip(mai, optTime, type));}
  double incrementTab::gToptt(double mai, int type) {
    return(ip(mai, optTimet, type));}
  double incrementTab::gToptSdNat(double mai, double sd, int type) {
    return(ip(mai, sd, optTimeSdNat, type, true));}
  double incrementTab::gToptSdTab(double mai, double sd, int type) {
    return(ip(mai, sd, optTimeSdTab, type, false));}

  //This can also be stored in a table for faster access
  double incrementTab::gU(double bm, double mai) {
    int tL = 0;
    int tH = ceil(gTopt(mai, 1));
    double bmL = 0;
    double bmH = gAvgBm(tH, mai);
    double t;
    if(bm < bmH) {
      int tX = floor(0.5+tH/2.);
      double bmX = gAvgBm(tX, mai);
      do {
	if(bmX > bm) {tH = tX; bmH = bmX;}
	else {tL = tX; bmL = bmX;}
	tX = floor(0.5+interpol(tL, tH, bmL, bmH, bm));
	bmX = gAvgBm(tX, mai);
      } while(tX != tL && tX != tH);
      t = interpol(tL, tH, bmL, bmH, bm);
    } else {t = tH;}
    return(t);
  }
  double incrementTab::gUt(double bm, double mai) {
    int tL = 0;
    int tH = ceil(gToptt(mai, 1));
    double bmL = 0;
    double bmH = gAvgBmt(tH, mai);
    double t;
    if(bm < bmH) {
      int tX = floor(0.5+tH/2.);
      double bmX = gAvgBmt(tX, mai);
      do {
	if(bmX > bm) {tH = tX; bmH = bmX;}
	else {tL = tX; bmL = bmX;}
	tX = floor(0.5+interpol(tL, tH, bmL, bmH, bm));
	bmX = gAvgBmt(tX, mai);
      } while(tX != tL && tX != tH);
      t = interpol(tL, tH, bmL, bmH, bm);
    } else {t = tH;}
    return(t);
  }
  double incrementTab::gUSdNat(double bm, double mai, double sd) {
    if(sd > 1.) {sd = 1.;}
    if(sd > 0.) {return(gU(bm / sd, mai));}
    if(bm <= 0.) {return(0.);}
    return(gTopt(mai, 1));
  }
  double incrementTab::gUSdTab(double bm, double mai, double sd) {
    if(sd <= 1.) {
      if(sd > 0.) {return(gUt(bm / sd, mai));}
      if(bm <= 0.) {return(0.);}
      return(gToptt(mai, 1));
   }
    int tL = 0;
    int tH = ceil(gToptSdTab(mai, sd, 1));
    double bmL = 0;
    double bmH = gAvgBmSdTab(tH, mai, sd);
    double t;
    if(bm < bmH) {
      int tX = floor(0.5+tH/2.);
      double bmX = gAvgBmSdTab(tX, mai, sd);
      do {
	if(bmX > bm) {tH = tX; bmH = bmX;}
	else {tL = tX; bmL = bmX;}
	tX = floor(0.5+interpol(tL, tH, bmL, bmH, bm));
	bmX = gAvgBmSdTab(tX, mai, sd);
      } while(tX != tL && tX != tH);
      t = interpol(tL, tH, bmL, bmH, bm);
    } else {t = tH;}
    return(t);
  }

  double incrementTab::gSdNat(double age, double mai, double bm) {
    double sd = bm / gBm(age, mai);
    if(sd < 0.) {sd = 0.;}
    if(sd > 1.) {sd = 1.;}
    return(sd);
  }
  double incrementTab::gSdTab(double age, double mai, double bm) {
    double sd = bm / gBmt(age, mai);
    if(sd < 0.) {sd = 0.;}
    if(sd > 1.) {
      double sdnat = gSdNat(age, mai);
      if(sd * sdnat > 1.) {sd = 1. / sdnat;}
    }
    return(sd);
  }

  double incrementTab::gAvgSdNat(double u, double mai, double bm) {
    double sd = bm / gAvgBm(u, mai);
    if(sd < 0.) {sd = 0.;}
    if(sd > 1.) {sd = 1.;}
    return(sd);
  }
  double incrementTab::gAvgSdTab(double u, double mai, double bm) {
    double sd = bm / gAvgBmt(u, mai);
    if(sd < 0.) {sd = 0.;}
    if(sd > 1.) {
      double sdnat = gAvgSdNat(u, mai, bm);
      if(sdnat < 1. && bm > 0.) {
	int count = 0;
	double bmEst;
	double dif = 1.;
	do {
	  sd /= dif;
	  bmEst = gAvgBmSdTab(u, mai, sd);
	  dif = bmEst / bm;
	  ++count;
	} while(count < 20 && fabs(dif-1.) > 1.e-32);
      } else {sd = std::numeric_limits<double>::infinity();}
    }
    return(sd);
  }

  double incrementTab::gtimeframe() {
    if(timeframe <= 0.) {return(tStep);}
    return(timeframe);
  }

  double incrementTab::gTmax() {
    return(tHi);
  }

}
