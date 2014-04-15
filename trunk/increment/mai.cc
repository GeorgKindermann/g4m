#include "mai.h"

namespace g4m {

  mai::mai(const std::valarray<double>& ac14t17  //Coefficients c[14] to c[17]
	   , const std::valarray<double>& ac     //Coefficients
	   , const std::valarray<double>& ac0  //c0 Coefficients
	   , const std::valarray<double>& at   //Temperature of each month [deg C]
	   , const std::valarray<double>& ap   //Precipitation [mm/month]
	   , double awhc    //Water holding capacity [mm]
	   , double aswr    //Soil Water Regime, additional water from soil or irrigation [mm/month]
	   , double aco2     //CO2 concentration [volume %] (e.g. 0.038)
	   , unsigned char asoilType   //Soil type code
	   , double aaltitude    //Altitude [m]
	   , double alatitude //latitude of location
	   , double asoilWaterDecayRate  //lake of soil water
	   , unsigned char afNpp2mai  //Function type to convert npp to mai
	   , const std::valarray<double>& acNpp2mai //Coefficients to convert npp to mai
	   //the followign temperatures set boundaries where this species can exist
	   , const std::valarray<double>& atMinJ //Average annual temperature - minimum
	   , const std::valarray<double>& atMaxJ //Average annual temperature - maximum
	   , const std::valarray<double>& apMinJ //Annual precipitation - minimum
	   , const std::valarray<double>& apMaxJ //Annual precipitation - maximum
	   , const std::valarray<double>& atMinM //Month temperature - minimum
	   , const std::valarray<double>& atMaxM //Month temperature - maximum
	   , const std::valarray<double>& apMinM //Month precipitation - minimum
	   , const std::valarray<double>& apMaxM //Month precipitation - maximum
	   , const std::valarray<double>& aminNpp //Minimum NPP
	   , bool aweatherIsDynamic
	   ) : 
    inputWasChanged(true)
    , whc(awhc)
    , swr(aswr)
    , co2(aco2)
    , altitude(aaltitude)
    , soilType(asoilType)
    , weatherIsDynamic(aweatherIsDynamic)
    , soilWaterDecayRate(asoilWaterDecayRate)
    , latitude(alatitude*2.)
    , fNpp2mai(afNpp2mai)

  {
    c14t17 = ac14t17;
    numberOfTypes =ac.size() / nc;
    c = ac;
    c0 = ac0;
    t = at[std::slice(at.size()-12,12,1)];
    p = ap[std::slice(ap.size()-12,12,1)];
    tp = at[std::slice(0,12,1)];
    pp = ap[std::slice(0,12,1)];
    cNpp2mai = acNpp2mai;
    tMinJ = atMinJ;
    tMaxJ = atMaxJ;
    pMinJ = apMinJ;
    pMaxJ = apMaxJ;
    tMinM = atMinM;
    tMaxM = atMaxM;
    pMinM = apMinM;
    pMaxM = apMaxM;
    minNpp = aminNpp;
    sw.resize(12);
    outOfBoundaries.resize(numberOfTypes);
  }

  void mai::resizeAll(unsigned int types) {
    if(numberOfTypes < types) {
      numberOfTypes = types;
      std::valarray<double> tmp = c;
      c.resize((numberOfTypes)*nc);
      c[std::slice(0,tmp.size(),1)] = tmp;
      tmp = c0;
      c0.resize((numberOfTypes)*nc0);
      c0[std::slice(0,tmp.size(),1)] = tmp;
  tmp=tMinJ; tMinJ.resize(numberOfTypes); tMinJ[std::slice(0,tmp.size(),1)]=tmp;
  tmp=tMaxJ; tMaxJ.resize(numberOfTypes); tMaxJ[std::slice(0,tmp.size(),1)]=tmp;
  tmp=pMinJ; pMinJ.resize(numberOfTypes); pMinJ[std::slice(0,tmp.size(),1)]=tmp;
  tmp=pMaxJ; pMaxJ.resize(numberOfTypes); pMaxJ[std::slice(0,tmp.size(),1)]=tmp;
  tmp=tMinM; tMinM.resize(numberOfTypes); tMinM[std::slice(0,tmp.size(),1)]=tmp;
  tmp=tMaxM; tMaxM.resize(numberOfTypes); tMaxM[std::slice(0,tmp.size(),1)]=tmp;
  tmp=pMinM; pMinM.resize(numberOfTypes); pMinM[std::slice(0,tmp.size(),1)]=tmp;
  tmp=pMaxM; pMaxM.resize(numberOfTypes); pMaxM[std::slice(0,tmp.size(),1)]=tmp;
      tmp=minNpp; minNpp.resize(numberOfTypes);
      minNpp[std::slice(0,tmp.size(),1)]=tmp;
      std::valarray<bool> tmp2=outOfBoundaries;
      outOfBoundaries.resize(numberOfTypes);
      outOfBoundaries[std::slice(0,tmp2.size(),1)]=tmp2;
    }
  }

  std::size_t mai::setCoef(unsigned int type, const std::valarray<double>& ac) {
    resizeAll(type+1);
    c[std::slice(type*nc,nc,1)] = ac;
    return(c.size());
  }

  std::size_t mai::setCoefC0(unsigned int type, const std::valarray<double>& ac) {
    resizeAll(type+1);
    c0[std::slice(type*nc0,nc0,1)] = ac;
    return(c0.size());
  }

  bool mai::setBoundaries(unsigned int type, const double& atMinJ, const double& atMaxJ, const double& apMinJ, const double& apMaxJ, const double& atMinM, const double& atMaxM, const double& apMinM, const double& apMaxM, const double& aminNpp) {
    resizeAll(type+1);
    tMinJ[type] = atMinJ;
    tMaxJ[type] = atMaxJ;
    pMinJ[type] = apMinJ;
    pMaxJ[type] = apMaxJ;
    tMinM[type] = atMinM;
    tMaxM[type] = atMaxM;
    pMinM[type] = apMinM;
    pMaxM[type] = apMaxM;
    minNpp[type] = aminNpp;
    return(testBoundaries(type));
  }

  void mai::calcWalterLieth() {
    walterLieth = c14t17[0] / (c14t17[1] + 1./(1. + exp(c14t17[2] + c14t17[3]*co2)));
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

  double mai::getNpp(unsigned int type) {
    double ret=0.;
    if(inputWasChanged) {
      calcWalterLieth();
      updateSoilWater();
      inputWasChanged = false;
    }
    double t3 = c[6+type*nc] / (1. + exp(c[7+type*nc] + c[8+type*nc]*co2)) + c[9+type*nc];
    double days[12] = {31.,28.25,31.,30.,31.,30.,31.,31.,30.,31.,30.,31.};
    for(int month=0; month<12; ++month) {
      double t1 = c0[soilType+type*nc0]/(1. + std::exp(c[0+type*nc] + c[1+type*nc]*t[month])) - c0[soilType+type*nc0]*c[2+type*nc] - c0[soilType+type*nc0]/(1. + std::exp(c[3+type*nc]+c[4+type*nc]*t[month]));
      if(t1<0.) {t1=0.;}
      double t2 = 1. - 2./(1. + exp( (sw[month]+p[month]-std::max(0.,t[month])*walterLieth)/(std::max(1.,t[month]*c[5+type*nc]))));
      if(t2<0.) {t2=0.;}
      ret += t1 * t2 * t3 * days[month];
    }
    ret *= c[10+type*nc] + c[11+type*nc]*altitude + c[12+type*nc]*cos(latitude) + c[13+type*nc]*altitude*cos(latitude);
    if(ret<0.) {ret = 0.;}
    return(ret);
  }

  std::valarray<double> mai::getNpp() {
    std::valarray<double> ret(outOfBoundaries.size());
    for(unsigned int i=0; i<ret.size(); ++i) {ret[i] = getNpp(i);}
    return(ret);
  }

  std::valarray<double> mai::getNpp(std::valarray<bool> dontNeed) {
    std::valarray<double> ret(dontNeed.size());
    for(unsigned int i=0; i<ret.size(); ++i) {
      if(dontNeed[i]) {ret[i] = 0.;
      } else {ret[i] = getNpp(i);}
    }
    return(ret);
  }

  double mai::getNppB(unsigned int type) {
    double ret = getNpp(type);
    if(ret < minNpp[type]) {ret = 0.;}
    return(ret);
  }

  std::valarray<double> mai::getNppB() {
    std::valarray<double> ret(outOfBoundaries.size());
    for(unsigned int i=0; i<ret.size(); ++i) {ret[i] = getNppB(i);}
    return(ret);
  }

  std::valarray<double> mai::getNppB(std::valarray<bool> dontNeed) {
    std::valarray<double> ret(dontNeed.size());
    for(unsigned int i=0; i<ret.size(); ++i) {
      if(dontNeed[i]) {ret[i] = 0.;
      } else {ret[i] = getNppB(i);}
    }
    return(ret);
  }

  bool mai::testBoundaries(unsigned int type) {
    outOfBoundaries[type] = false;
    if(t.min() < tMinM[type]) {outOfBoundaries[type] = true; return(outOfBoundaries[type]);}
    if(t.max() > tMaxM[type]) {outOfBoundaries[type] = true; return(outOfBoundaries[type]);}
    if(p.min() < pMinM[type]) {outOfBoundaries[type] = true; return(outOfBoundaries[type]);}
    if(p.max() > pMaxM[type]) {outOfBoundaries[type] = true; return(outOfBoundaries[type]);}
    double avg=t.sum()/12.;
    if(avg < tMinJ[type]) {outOfBoundaries[type] = true; return(outOfBoundaries[type]);}
    if(avg > tMaxJ[type]) {outOfBoundaries[type] = true; return(outOfBoundaries[type]);}
    avg=p.sum();
    if(avg < pMinJ[type]) {outOfBoundaries[type] = true; return(outOfBoundaries[type]);}
    if(avg > pMaxJ[type]) {outOfBoundaries[type] = true; return(outOfBoundaries[type]);}
    if(weatherIsDynamic) {
      if(tp.min() < tMinM[type]) {outOfBoundaries[type] = true; return(outOfBoundaries[type]);}
      if(tp.max() > tMaxM[type]) {outOfBoundaries[type] = true; return(outOfBoundaries[type]);}
      if(pp.min() < pMinM[type]) {outOfBoundaries[type] = true; return(outOfBoundaries[type]);}
      if(pp.max() > pMaxM[type]) {outOfBoundaries[type] = true; return(outOfBoundaries[type]);}
      double avg=tp.sum()/12.;
      if(avg < tMinJ[type]) {outOfBoundaries[type] = true; return(outOfBoundaries[type]);}
      if(avg > tMaxJ[type]) {outOfBoundaries[type] = true; return(outOfBoundaries[type]);}
      avg=pp.sum();
      if(avg < pMinJ[type]) {outOfBoundaries[type] = true; return(outOfBoundaries[type]);}
      if(avg > pMaxJ[type]) {outOfBoundaries[type] = true; return(outOfBoundaries[type]);}
    }
    return(outOfBoundaries[type]);
  }
  
  std::valarray<bool> mai::testBoundaries() {
    for(unsigned int i=0; i<outOfBoundaries.size(); ++i) {testBoundaries(i);}
    return(outOfBoundaries);
  }

  void mai::setTemperature(const std::valarray<double>& at) {
    inputWasChanged = true;
    if(weatherIsDynamic) {tp = t;}
    t = at;
  }

  void mai::setPrecipitation(const std::valarray<double>& ap) {
    inputWasChanged = true;
    if(weatherIsDynamic) {pp = p;}
    p = ap;
  }

  void mai::setCo2(const double& aco2) {
    co2 = aco2;
    inputWasChanged = true;
  }

  void mai::setSwr(const double& aswr) {
    swr = aswr;
    inputWasChanged = true;
  }

  void mai::setWhc(const double& awhc) {
    whc = awhc;
    inputWasChanged = true;
  }

  void mai::setAltitude(const double& aaltitude) {
    altitude = aaltitude;
    inputWasChanged = true;
  }

  void mai::setSoilType(const int& asoilType) {
    soilType = asoilType;
  }

  void mai::setLatitude(double alatitude) {
    latitude = alatitude*2.;
    inputWasChanged = true;
  }

  double mai::getMai(unsigned int type) {
    double npp = getNpp(type);
    double mai = npp;
    if(fNpp2mai == 0) {
      mai *= cNpp2mai[0];
    } else {
      mai *= 0.35;
    }
    return(mai);
  }

  std::valarray<double> mai::getMai() {
    std::valarray<double> ret(outOfBoundaries.size());
    for(unsigned int i=0; i<ret.size(); ++i) {ret[i] = getMai(i);}
    return(ret);
  }

  std::valarray<double> mai::getMai(std::valarray<bool> dontNeed) {
    std::valarray<double> ret(dontNeed.size());
    for(unsigned int i=0; i<ret.size(); ++i) {
      if(dontNeed[i]) {ret[i] = 0.;
      } else {ret[i] = getMai(i);}
    }
    return(ret);
  }

  double mai::getMaiB(unsigned int type) {
    double npp = getNppB(type);
    double mai = npp;
    if(fNpp2mai == 0) {
      mai *= cNpp2mai[0];
    } else {
      mai *= 0.35;
    }
    return(mai);
  }

  std::valarray<double> mai::getMaiB() {
    std::valarray<double> ret(outOfBoundaries.size());
    for(unsigned int i=0; i<ret.size(); ++i) {ret[i] = getMaiB(i);}
    return(ret);
  }

  std::valarray<double> mai::getMaiB(std::valarray<bool> dontNeed) {
    std::valarray<double> ret(dontNeed.size());
    for(unsigned int i=0; i<ret.size(); ++i) {
      if(dontNeed[i]) {ret[i] = 0.;
      } else {ret[i] = getMaiB(i);}
    }
    return(ret);
  }

  double mai::setSoilWaterDecayRate(double asoilWaterDecayRate) {
    soilWaterDecayRate = asoilWaterDecayRate;
    return(soilWaterDecayRate);
  }

  unsigned int mai::setcNpp2mai(const std::valarray<double>& acNpp2mai) {
    cNpp2mai = acNpp2mai;
    return(cNpp2mai.size());
  }

  bool mai::setWeatherAsDynamic(bool aweatherIsDynamic) {
    weatherIsDynamic = aweatherIsDynamic;
    return(weatherIsDynamic);
  }

}
