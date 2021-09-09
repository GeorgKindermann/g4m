#include "mai.h"

namespace g4m {

     mai::mai(const std::valarray<double>& ac //Coefficients: c0,cr0,cr1,ct0,ct1,co0,co1,cp0,cp1,cp2,cn0,cn1,cpo0,cpo1,cs0,cs1,cph0,cph1
	, const std::valarray<double>& at //Temperature of each month [deg C]
	, const std::valarray<double>& ap //Precipitation [mm/month]
	, const std::valarray<double>& ar //Short wave Radiation [W/m2]
	, double awhc    //Water holding capacity [mm]
	, double aswr  //Soil Water Regime, additional water from soil or irrigation [mm/month]
	, double aco2   //CO2 concentration [volume %]
	, double aaltitude    //Altitude [m]
	, double aN //Nitrogen [t/Ha]
	, double aP //Phosphorus Seconday Mineral [gP/m2]
	, double aS  //Salinity [dS/m]
	, double apH //pH-Value
	, double asoilWaterDecayRate  //lake of soil water
	, unsigned char afNpp2mai //Function type to convert npp to mai
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
	)  :
    version(3)
    , inputWasChanged(true)
    , nc(23)
    , nc0(0)
    , whc(awhc)
    , swr(aswr)
    , co2(aco2)
    , N(aN)
    , P(aP)
    , S(aS)
    , pH(apH)
    , altitude(aaltitude)
    , weatherIsDynamic(aweatherIsDynamic)
    , soilWaterDecayRate(asoilWaterDecayRate)
    , fNpp2mai(afNpp2mai)
  {
    numberOfTypes = ac.size() / nc;
    c = ac;
    t = at[std::slice(at.size()-12,12,1)];
    p = ap[std::slice(ap.size()-12,12,1)];
    r = ar[std::slice(ar.size()-12,12,1)];
    tp = at[std::slice(0,12,1)];
    pp = ap[std::slice(0,12,1)];
    rp = ar[std::slice(0,12,1)];
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
    version(2)
    , inputWasChanged(true)
    , nc(15)
    , nc0(23)
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
    rp = ar[std::slice(0,12,1)];
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

    mai::mai(const std::valarray<double>& ac14t17  //Coefficients c[14] to c[17]
	   , const std::valarray<double>& ac     //Coefficients
	   , const std::valarray<double>& ac0  //c0 Coefficients
	   , const std::valarray<double>& at   //Temperature of each month [deg C]
	   , const std::valarray<double>& ap   //Precipitation [mm/month]
	   , double awhc    //Water holding capacity [mm]
	   , double aswr    //Soil Water Regime, additional water from soil or irrigation [mm/month]
	   , double aco2     //CO2 concentration [volume %] (e.g. 0.038)
	   , int asoilType   //Soil type code
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
    version(1)
    , inputWasChanged(true)
    , nc(14)
    , nc0(23)
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

  std::size_t mai::setCoefC0(unsigned int type, std::valarray<double> ac, double lo, double hi) {
    for(unsigned int i=0; i<ac.size(); ++i) {
      if(ac[i] < lo) {ac[i] = lo;}
      if(ac[i] > hi) {ac[i] = hi;}
    }
    return(mai::setCoefC0(type, ac));
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

  double mai::getNpp1(unsigned int type, bool useMinNpp) {
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
    if(useMinNpp && ret < minNpp[type]) {ret = 0.;}
    return(ret);
   }
      
  double mai::getNpp2(unsigned int type, bool useMinNpp) {
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
	* c0[soilType] * c[0+type*nc]*exp(-altitude/7990.)
	* r[month]
	* pow(fmax(0., (c[1+type*nc]+temperatur)), c[2+type*nc])
	* pow(fmax(0, 1. - verdunstungPot/tanh(c[5+type*nc]*nutzbaresWasser)/c[3+type*nc]), c[4+type*nc]);
    }
    if(ret<0.) {ret = 0.;}
    if(useMinNpp && ret < minNpp[type]) {ret = 0.;}
    return(ret);
  }

  double mai::getNpp3(unsigned int type, bool useMinNpp) {
    double ret=0.;
    static const double days[12] = {31./30.,28.25/30.,31./30.,30./30.,31./30.,30./30.,31./30.,31./30.,30./30.,31./30.,30./30.,31./30.};
    double bodenwasser = whc / 2.;
    for(int month=0; month<12; ++month) {
      double niederschlag = p[month];
      double temperatur = t[month];
      double verdunstungPot = 30. * exp(17.62*temperatur/(243.12 + temperatur));
      double evapotrans = tanh(c[0+type*nc]) * verdunstungPot;
      double interceptionPot = abs(c[1+type*nc]) + fmin(abs(c[2+type*nc]) * tanh(abs(c[3+type*nc]) * verdunstungPot), evapotrans);
      double interception = fmin(niederschlag, interceptionPot);
      double niederschlagBoden = niederschlag - interception;
      double verfuegbaresBodenwasser = bodenwasser * pow((bodenwasser / whc), c[4+type*nc]);
      double nutzbaresWasser = niederschlagBoden + verfuegbaresBodenwasser;
      double genutztesWasser = fmin(nutzbaresWasser, evapotrans - interception);
      bodenwasser += (niederschlagBoden - genutztesWasser) * days[month];
      if(bodenwasser > whc) {bodenwasser = whc;}
      if(bodenwasser < 0.) {bodenwasser = 0.;}
    }
    for(int month=0; month<12; ++month) {
      double niederschlag = p[month];
      double temperatur = t[month];
      double radi = r[month];
      double verdunstungPot = 30. * exp(17.62*temperatur/(243.12 + temperatur));
      double evapotrans = tanh(c[0+type*nc]) * verdunstungPot;
      double interceptionPot = abs(c[1+type*nc]) + fmin(abs(c[2+type*nc]) * tanh(abs(c[3+type*nc]) * verdunstungPot), evapotrans);
      double interception = fmin(niederschlag, interceptionPot);
      double niederschlagBoden = niederschlag - interception;
      double verfuegbaresBodenwasser = bodenwasser * pow((bodenwasser / whc), c[4+type*nc]);
      double nutzbaresWasser = niederschlagBoden + verfuegbaresBodenwasser;
      double genutztesWasser = fmin(nutzbaresWasser, evapotrans - interception);
      bodenwasser += (niederschlagBoden - genutztesWasser) * days[month];
      if(bodenwasser > whc) {bodenwasser = whc;}
      if(bodenwasser < 0.) {bodenwasser = 0.;}
      ret += c[5+type*nc] * exp(-altitude/7990)    //Luftdruck
	* pow(fmax(0.,  c[6+type*nc] + radi), c[7+type*nc])   //Strahlung
	* pow(fmax(0., c[8+type*nc] + temperatur), c[9+type*nc])  //Temperatur
	* pow(fmax(0, tanh(c[10+type*nc] * (co2 - c[11+type*nc])) - c[12+type*nc]*exp(17.62*temperatur/(243.12 + temperatur)) / tanh(c[13+type*nc]*bodenwasser)), c[14+type*nc]) //Wasser (Niederschlag, Bodenwasser) + CO2
	* pow(1 - exp(c[15+type*nc] * N), c[16+type*nc])   //N - Mitscherlich
	* pow(1 - exp(c[17+type*nc] * P), c[18+type*nc]) //P
	* pow(exp(c[19+type*nc] * S), c[20+type*nc])          //Salz
	* exp(log(fmax(0., c[21+type*nc] + pH)) + pow(c[22+type*nc]*log(fmax(0., c[21+type*nc] + pH)),2));
    }
    if(ret<0.) {ret = 0.;}
    if(useMinNpp && ret < minNpp[type]) {ret = 0.;}
    return(ret);
  }
  
  double mai::getNpp(unsigned int type, bool useMinNpp) {
    double ret=0.;
    switch ( version ) {
    [[likely]] case 3:
      ret = getNpp3(type, useMinNpp);
      break;
    case 2:
      ret = getNpp2(type, useMinNpp);
      break;
    case 1:
      ret = getNpp1(type, useMinNpp);
      break;
    default:
      ret = getNpp3(type, useMinNpp);
      break;
    }
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

  void mai::setN(const double& aN) {
    N = aN;
    inputWasChanged = true;
  }

  void mai::setP(const double& aP) {
    P = aP;
    inputWasChanged = true;
  }

  void mai::setS(const double& aS) {
    S = aS;
    inputWasChanged = true;
  }

  void mai::setpH(const double& apH) {
    pH = apH;
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

  double mai::getMai(unsigned int type, bool minNpp) {
    double npp = 0.;
    if(!testBoundaries(type)) {npp = getNpp(type, minNpp);}
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
    for(unsigned int i=0; i<ret.size(); ++i) {ret[i] = getMai(i, minNpp);}
    return(ret);
  }

  std::valarray<double> mai::getMai(std::valarray<bool> dontNeed, bool minNpp) {
    std::valarray<double> ret(dontNeed.size());
    for(unsigned int i=0; i<ret.size(); ++i) {
      if(dontNeed[i]) {ret[i] = 0.;
      } else {ret[i] = getMai(i, minNpp);}
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
