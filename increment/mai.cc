#include "mai.h"

namespace g4m {

  mai::mai(const std::valarray<double>& ac     //Coefficients
	   , const std::valarray<double>& ac0  //c0 Coefficients (soilType)
	   , const std::valarray<double>& at   //Temperature of each month [deg C]
	   , const std::valarray<double>& ap   //Precipitation [mm/30 Day]
	   , const std::valarray<double>& ar   //Radiation [Watt/m2]
	   , double awhc    //Water holding capacity [mm]
	   , double aswr    //Soil Water Regime, additional water from soil or irrigation [mm/month]
	   , double aco2     //CO2 concentration [volume %] (e.g. 0.038)
	   , int asoilType   //Soil type code
	   , double aaltitude    //Altitude [m]
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
    , fNpp2mai(afNpp2mai)
  {
    numberOfTypes = ac.size() / nc;
    c = ac;
    c0 = ac0;
    t = at[std::slice(at.size()-12,12,1)];
    p = ap[std::slice(ap.size()-12,12,1)];
    r = ar[std::slice(ar.size()-12,12,1)];
    tp = at[std::slice(0,12,1)];
    pp = ap[std::slice(0,12,1)];
    rp = rp[std::slice(0,12,1)];
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

  double mai::getNpp(unsigned int type, bool useMinNpp) {
    double ret=0.;
    static const double days[12] = {31.,28.25,31.,30.,31.,30.,31.,31.,30.,31.,30.,31.};
    double bodenwasser = 0.;
    for(int month=0; month<12; ++month) {
      double niederschlag = p[month];
      double temperatur = t[month];
      double verdunstungPot = 30. * exp(17.62*temperatur/(243.12 + temperatur));
      double evapotrans = tanh(c[6+type*nc]) * verdunstungPot;
      double interceptionPot = abs(c[10+type*nc]) + fmin(abs(c[7+type*nc]) * tanh(abs(c[8+type*nc]) * verdunstungPot), evapotrans);
      double interception = fmin(niederschlag, interceptionPot);
      double niederschlagBoden = niederschlag - interception;
      double verfuegbaresBodenwasser = bodenwasser * pow((bodenwasser / whc), c[9+type*nc]);
      double nutzbaresWasser = niederschlagBoden + verfuegbaresBodenwasser;
      double genutztesWasser = fmin(nutzbaresWasser, evapotrans - interception);
      bodenwasser += (niederschlagBoden - genutztesWasser) * days[month]/30.;
      if(bodenwasser > whc) {bodenwasser = whc;}
      if(bodenwasser < 0.) {bodenwasser = 0.;}
    }
    for(int month=0; month<12; ++month) {
      double niederschlag = p[month];
      double temperatur = t[month];
      double verdunstungPot = 30. * exp(17.62*temperatur/(243.12 + temperatur));
      double evapotrans = tanh(c[6+type*nc]) * verdunstungPot;
      double interceptionPot = abs(c[10+type*nc]) + fmin(abs(c[7+type*nc]) * tanh(abs(c[8+type*nc]) * verdunstungPot), evapotrans);
      double interception = fmin(niederschlag, interceptionPot);
      double niederschlagBoden = niederschlag - interception;
      double verfuegbaresBodenwasser = bodenwasser * pow((bodenwasser / whc), c[9+type*nc]);
      double nutzbaresWasser = niederschlagBoden + verfuegbaresBodenwasser;
      double genutztesWasser = fmin(nutzbaresWasser, evapotrans - interception);
      bodenwasser += (niederschlagBoden - genutztesWasser) * days[month]/30.;
      if(bodenwasser > whc) {bodenwasser = whc;}
      if(bodenwasser < 0.) {bodenwasser = 0.;}
      ret += days[month]
	* c[0+type*nc]*exp(-altitude/7990.)
	* r[month]
	* pow(fmax(0., (c[1+type*nc]+temperatur)), c[2+type*nc])
	* pow(fmax(0, 1. - verdunstungPot/tanh(c[5+type*nc]*nutzbaresWasser)/c[3+type*nc]), c[4+type*nc]);
    }
    if(ret<0.) {ret = 0.;}
    if(useMinNpp && ret < minNpp[type]) {ret = 0.;}
    return(ret);
  }

  std::valarray<double> mai::getNpp(bool useMinNpp) {
    std::valarray<double> ret(outOfBoundaries.size());
    for(unsigned int i=0; i<ret.size(); ++i) {ret[i] = getNpp(i, useMinNpp);}
    return(ret);
  }

  std::valarray<double> mai::getNpp(std::valarray<bool> dontNeed, bool useMinNpp) {
    std::valarray<double> ret(dontNeed.size());
    for(unsigned int i=0; i<ret.size(); ++i) {
      if(dontNeed[i]) {ret[i] = 0.;
      } else {ret[i] = getNpp(i, useMinNpp);}
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

  void mai::setRadiation(const std::valarray<double>& ar) {
    inputWasChanged = true;
    if(weatherIsDynamic) {rp = r;}
    r = ar;
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

  double mai::getMai(unsigned int type, bool minNpp) {
    double npp = 0.;
    if(!testBoundaries(type)) {npp = getNpp(type);}
    double mai = npp;
    if(fNpp2mai == 0) {
      mai *= cNpp2mai[0];
    } else {
      mai *= 0.35;
    }
    return(mai);
  }

  std::valarray<double> mai::getMai(bool minNpp) {
    std::valarray<double> ret(outOfBoundaries.size());
    for(unsigned int i=0; i<ret.size(); ++i) {ret[i] = getMai(i);}
    return(ret);
  }

  std::valarray<double> mai::getMai(std::valarray<bool> dontNeed, bool minNpp) {
    std::valarray<double> ret(dontNeed.size());
    for(unsigned int i=0; i<ret.size(); ++i) {
      if(dontNeed[i]) {ret[i] = 0.;
      } else {ret[i] = getMai(i);}
    }
    return(ret);
  }

  /*
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
  */

  unsigned int mai::setcNpp2mai(const std::valarray<double>& acNpp2mai) {
    cNpp2mai = acNpp2mai;
    return(cNpp2mai.size());
  }

  bool mai::setWeatherAsDynamic(bool aweatherIsDynamic) {
    weatherIsDynamic = aweatherIsDynamic;
    return(weatherIsDynamic);
  }

}
