#include <vector>
#include <limits>
#include <cmath>
#include <valarray>

#include <Rcpp.h>

using namespace Rcpp;

namespace g4m {
  class mai {
  public:
    mai(const std::valarray<double>& c = {.8, 0., 0., 0., .75, 0.004609729, -32.90496, 0.6081886, 1.719932, 1.323597, 0.003, 50., 0.1291932, 0.1711536, 1.1960215, -0.2604522, 0.7, -0.2446678, 0.7, -0.1, 1., -3.370857, -0.8971804}  //Coefficients: 0boden0, 1boden1, 2boden2, 3boden3, 4boden4, 5c0,6cr0,7cr1,8ct0,9ct1,10co0,11co1,12cp0,13cp1,14cp2,15cn0,16cn1,17cpo0,18cpo1,19cs0,20cs1,21cph0,22cph1
	, const std::valarray<double>& t = std::valarray<double>(10., 12)  //Temperature of each month [deg C]
	, const std::valarray<double>& p = std::valarray<double>(70., 12)  //Precipitation [mm/month]
	, const std::valarray<double>& r = std::valarray<double>(180., 12) //Short wave Radiation [W/m2]
	, double whc=100.    //Water holding capacity [mm]
	, double swr=0.  //Soil Water Regime, additional water from soil or irrigation [mm/month]
	, double co2=0.038   //CO2 concentration [volume %]
	, double altitude=0.    //Altitude [m]
	, double N=50. //Nitrogen [t/Ha]
	, double P=30. //Phosphorus Seconday Mineral [gP/m2]
	, double S=0.  //Salinity [dS/m]
	, double pH=7. //pH-Value
	, double soilWaterDecayRate=0.8  //lake of soil water
	, unsigned char fNpp2mai=0 //Function type to convert npp to mai
	, const std::valarray<double>& cNpp2mai = {0.35} //Coefficients to convert npp to mai
	//the followign temperatures set boundaries where this species can exist
	, const std::valarray<double>& tMinJ = {0.} //Average annual temperature - minimum
	, const std::valarray<double>& tMaxJ = {std::numeric_limits<double>::infinity()} //Average annual temperature - maximum
	, const std::valarray<double>& pMinJ = {0.} //Annual precipitation - minimum
	, const std::valarray<double>& pMaxJ = {std::numeric_limits<double>::infinity()} //Annual precipitation - maximum
	, const std::valarray<double>& tMinM = {0.} //Month temperature - minimum
	, const std::valarray<double>& tMaxM = {std::numeric_limits<double>::infinity()} //Month temperature - maximum
	, const std::valarray<double>& pMinM = {0.} //Month precipitation - minimum
	, const std::valarray<double>& pMaxM = {std::numeric_limits<double>::infinity()} //Month precipitation - maximum
	, const std::valarray<double>& minNpp = {0.} //Minimum NPP
	, bool weatherIsDynamic = false
	);

    double getNpp(unsigned int type=0, bool useMinNpp=false);
    std::valarray<double> getNpp(bool minNpp=false);
    std::valarray<double> getNpp(std::valarray<bool> dontNeed, bool useMinNpp=false);
    double getMai(unsigned int type=0, bool minNpp=false);
    std::valarray<double> getMai(bool minNpp=false);
    std::valarray<double> getMai(std::valarray<bool> dontNeed, bool useMinNpp=false);
    std::size_t setCoef(unsigned int type, const std::valarray<double>& c);
    std::size_t setCoefC0(unsigned int type, const std::valarray<double>& c);
    std::size_t setCoefC0(unsigned int type, std::valarray<double> c, double lo, double hi);
    bool setBoundaries(unsigned int type, const double& tMinJ, const double& tMaxJ, const double& pMinJ, const double& pMaxJ, const double& tMinM, const double& tMaxM, const double& pMinM, const double& pMaxM, const double& minNpp); 
    void setTemperature(const std::valarray<double>& t);
    void setPrecipitation(const std::valarray<double>& p);
    void setRadiation(const std::valarray<double>& r);
    void setCo2(const double& co2);
    void setSwr(const double& swr);
    void setWhc(const double& whc);
    void setN(const double& n);
    void setP(const double& p);
    void setS(const double& s);
    void setpH(const double& pH);
    void setAltitude(const double& altitude);
    void setSoilType(const int& soilType);
    void setLatitude(double latitude);
    double setSoilWaterDecayRate(double soilWaterDecayRate);
    unsigned int setcNpp2mai(const std::valarray<double>& acNpp2mai);
    bool setWeatherAsDynamic(bool weatherIsDynamic);
    bool testBoundaries(unsigned int type);
    std::valarray<bool> testBoundaries();
  private:
    int version;
    bool inputWasChanged;
    int updateSoilWater();
    void calcWalterLieth();
 
    std::valarray<double> c14t17;
    unsigned int numberOfTypes;
    const unsigned int nc; //Number of coefficients (v3:24, v2:15, v1:14)
    const unsigned int nc0; //Number of c0 coefficients (23)
    std::valarray<double> c;  //18 Coefficients
    std::valarray<double> c0;  //23 c0 Coefficients
    std::valarray<double> tMinJ;
    std::valarray<double> tMaxJ;
    std::valarray<double> pMinJ;
    std::valarray<double> pMaxJ;
    std::valarray<double> tMinM;
    std::valarray<double> tMaxM;
    std::valarray<double> pMinM;
    std::valarray<double> pMaxM;
    std::valarray<double> minNpp;
    std::valarray<bool> outOfBoundaries;
    void resizeAll(unsigned int types);

    double walterLieth; //Walter lieth coefficient
    double whc;   //Water holding capacity 
    double swr;   //Soild water regime (Grundwassereinfluss)
    double co2;   //Co2 concentration
    double N;     //Nitrogen
    double P;     //Phosphorus
    double S;     //Salinity
    double pH;    //pH-Value
     double altitude;         //altitude
    int soilType; //Soil type
    std::valarray<double> t; //Temperature for each month of this year [12]
    std::valarray<double> p; //Precipitation for each month of this year [12]
    std::valarray<double> r; //Radiation for each month of this year [12]
    std::valarray<double> sw; //Soil water content [12]
    std::valarray<double> tp; //Temperature for each month of previous year [12]
    std::valarray<double> pp; //Precipitation for each month of previous year [12]
    std::valarray<double> rp; //Radiation for each month of previous year [12]
    bool weatherIsDynamic;
    double soilWaterDecayRate;
    double latitude;
    unsigned char fNpp2mai;
    std::valarray<double> cNpp2mai;
    
    double getNpp1(unsigned int type=0, bool useMinNpp=false);
    double getNpp2(unsigned int type=0, bool useMinNpp=false);
    double getNpp3(unsigned int type=0, bool useMinNpp=false);
  };

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
    , nc(24)
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
      double uw = genutztesWasser / evapotrans;
      ret += c[5+type*nc] * exp(-altitude/7990)    //Luftdruck
  * pow(fmax(0.,  c[6+type*nc] + radi), c[7+type*nc])   //Strahlung
//* pow(fmax(0., c[8+type*nc] + temperatur), c[9+type*nc])  //Temperatur
   * fmax(0., tanh(c[8+type*nc] * fmax(0., c[9+type*nc] + temperatur))
          - pow(fmax(0., c[10+type*nc] * temperatur), c[11+type*nc])) //Temperatur
//* pow(fmax(0, tanh(c[10+type*nc] * (co2 - c[11+type*nc])) - c[12+type*nc]*exp(17.62*temperatur/(243.12 + temperatur)) / tanh(c[13+type*nc]*bodenwasser)), c[14+type*nc]) //Wasser (Niederschlag, Bodenwasser) + CO2
  * pow(fmax(0, (tanh(c[12+type*nc] * (co2 - c[13+type*nc]))  * uw - c[14+type*nc]) / (1 - c[14+type*nc])), c[15+type*nc]) //Trockenheit
	* pow(1 - exp(c[16+type*nc] * N), c[17+type*nc])   //N - Mitscherlich
	* pow(1 - exp(c[18+type*nc] * P), c[19+type*nc]) //P
	* pow(exp(c[20+type*nc] * S), c[21+type*nc])          //Salz
  * exp(log(fmax(0., c[22+type*nc] + pH)) + c[23+type*nc]*pow(log(fmax(0., c[22+type*nc] + pH)),2))
	;
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
      ret = getNpp2(type, useMinNpp);
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

g4m::mai mai;

// [[Rcpp::export]]
void maiInit() {
  mai.setSwr(0.8);
  mai.setcNpp2mai({1./3.});
  //mai.setCo2(0.038);
  mai.setCo2(380);

  double BITT = 99.; //Boundary Increase Tropical Temperature

  //lc1er1 Nadel-Evergreen-Tropical
  mai.setCoef(0, {0.8, 0., 0., 0., 0.75, 0.996, -20., 0.648, 0.00544, 5., 0.0178, 2.57, 0.003, 50., 0.1, 0.25, -0.1, 0.702, -0.1, 0.705, 0.03, 2., -3.71, -1.74});
  mai.setBoundaries(0,4.3 + 13.8,29.1 + 2.2 + BITT,200. - 0,6900. + 4000.,-3.3 + 14.6,33.5 + 2.3,0,1300 + 2000,0.64);
  //lc1er2 Nadel-Evergreen-Subtropical
  mai.setCoef(1, {0.8, 0., 0., 0., 0.75, 1.63, -51.1, 0.614, 0.00543, 1.9, 0.0129, 1.83, 0.003, 50., 0.1, 0.25, -0.1, 0.514, -0.4, 10., 0.02, 4., -3.22, -0.923});
  mai.setBoundaries(1,-3.1 + 15.5,22.9 - 4.2,280. - 0,6900. + 4000.,-11. + 12.2,30.0 - 0.1,0,1300 + 2000,1.2);
//lc1er3 Nadel-Evergreen-Temperate
  mai.setCoef(2, {0.8, 0., 0., 0., 0.75, 0.734, -26., 0.743, 0.00363, 0.335, 0.022, 3.86, 0.003, 50., 0.1, 0.25, -0.179, 0.504, -0.1, 0.5, 0.02, 2.59, -3.36, -0.883});
  mai.setBoundaries(2,-4.9 + 5,14.6 - 2. + 3,220. - 0,6900. + 4000.,-27.2 + 0.,25.1 + 4.5,0,1300 + 2000,1.18);
//lc1er4 Nadel-Evergreen-Boreal
  mai.setCoef(3, {0.8, 0., 0., 0., 0.75, 1.29, -21.1, 0.588, 0.00308, -0.839, 0.0264, 5.91, 0.003, 50., 0.1, 0.25, -0.4, 0.584, -0.1, 0.5, 0.02, 2., -2.01, -0.518});
  mai.setBoundaries(3,-10.3 - 8,6.6 - 6.,175. - 0,6900. + 4000.,-39.9 - 8.,19.0 + 2.,2-2,1300 + 2000,0.53);
//lc2er1 Laub-Evergreen-Tropical
  mai.setCoef(4, {0.8, 0., 0., 0., 0.75, 0.881, -20., 0.615, 0.00476, 2.99, 0.0194, 3.67, 0.003, 50., 0.1, 0.25, -0.244, 0.5, -0.297, 1.24, 0.0243, 2.01, -2.08, -0.692});
  mai.setBoundaries(4,10.1 + 8.,28.3 + 3. + BITT,600. - 0,6900. + 4000.,7.2 + 4.1,33.6 + 2.2,0,1300 + 2000,4.6);
//lc2er2 Laub-Evergreen-Subtropical
  mai.setCoef(5, {0.8, 0., 0., 0., 0.75, .9 * 1.09, -42.8, 0.67, 0.00517, 2., 0.0179, 2.9, 0.003, 50., 0.1, 0.25, -0.1, 0.5, -0.4, 4.65, 0.03, 2., -3.95, -1.39});
  mai.setBoundaries(5,3.4 + 9,23.7 - 5. + 2.5,290. - 0,6900. + 4000.,-5.8 + 7.,29.9 + 2.,0,1300 + 2000,0.85);
//lc2er3 Laub-Evergreen-Temperate
  mai.setCoef(6, {0.8, 0., 0., 0., 0.75, .3 * 0.975, -37.8, 0.724, 0.00439, -0.0408, 0.0207, 3.55, 0.003, 50., 0.1, 0.25, -0.4, 0.5, -0.327, 0.5, 0.02, 2., -3.33, -1.12});
  mai.setBoundaries(6,-5.8 + 5.9,15.9 - 3.3 + 2.,330. - 0,6900. + 4000.,-25.7 + 0.,25.1 - 0.,1,1300 + 2000,3.18);
//lc2er4 Laub-Evergreen-Boreal
  mai.setCoef(7, {0.8, 0., 0., 0., 0.75, .3 * 1.72, -41.5, 0.482, 0.00607, -1.47, 0.0322, 6.74, 0.003, 50., 0.1, 0.402, -0.4, 0.5, -0.1, 0.723, 0.02, 4., -2.89, -0.793});
  mai.setBoundaries(7,-11.4 - 2.9,7.9 - 7.3,190. - 0,6900. + 4000.,-40.9 + 1.,21.4 + 1.,2,1300 + 2000,0.47);
//lc3er1 Nadel-Deciduous-Tropical
  mai.setCoef(8, {0.8, 0., 0., 0., 0.75, 2.75, -20., 0.595, 0.01, 5., 0.0137, 1.23, 0.003, 50., 0.1, 0.25, -0.1, 0.5, -0.1, 2.26, 0.03, 4., -1.5, -1.08});
  mai.setBoundaries(8,8.3 + 9.8 - 3,28.5 + 1. + BITT,225. - 0,6900. + 4000.,1.9 + 9.4 - 3,35.2 + 1.,0,1300 + 2000,1.13);
//lc3er2 Nadel-Deciduous-Subtropical
  mai.setCoef(9, {0.8, 0., 0., 0., 0.75, 0.43, -63.7, 0.722, 0.00454, 4.78, 0.0199, 3.36, 0.003, 50., 0.1, 0.25, -0.1, 0.581, -0.1, 0.5, 0.02, 2., -3.91, -1.03});
  mai.setBoundaries(9,-3.1 + 15.5,22.9 - 4.2,280. - 0,6900. + 4000.,-11. + 12.2,30.0 - 0.1,0,1300 + 2000,1.2);
//lc3er3 Nadel-Deciduous-Temperate
  mai.setCoef(10, {0.8, 0., 0., 0., 0.75, 0.811, -25.3, 0.729, 0.00343, -0.728, 0.0206, 4.01, 0.003, 50., 0.1, 0.25, -0.1, 0.5, -0.1, 0.648, 0.03, 2., -2.98, -0.778});
  mai.setBoundaries(10,-9.0 + 9.1,15.4 - 2.8,100. + 150,6900. + 4000.,-32.6 + 0,27.9 - 0.,1,1300 + 2000,0.19);
//lc3er4 Nadel-Deciduous-Boreal
  mai.setCoef(11, {0.8, 0., 0., 0., 0.75, 0.876, -22.6, 0.663, 0.00383, -0.947, 0.0279, 5.29, 0.003, 50., 0.1, 0.25, -0.4, 0.5, -0.4, 0.5, 0.02, 2., -3.81, -0.939});
  mai.setBoundaries(11,-10.3 - 8,6.6 - 6.,215. - 0,6900. + 4000.,-39.9 - 8.,19.6 + 2.,2-2,1300 + 2000,1.17);
//lc4er1 Laub-Deciduous-Tropical
  mai.setCoef(12, {0.8, 0., 0., 0., 0.75, 1.13, -20., 0.618, 0.00461, 2.56, 0.0199, 3.54, 0.003, 50., 0.1, 0.25, -0.1, 0.5, -0.4, 1.57, 0.02, 2., -3.54, -1.16});
  mai.setBoundaries(12,8.3 + 9.8 - 3,28.5 + 1. + BITT,225. - 0,6900. + 4000.,1.9 + 9.4 - 3,35.2 + 1.,0,1300 + 2000,1.13);
//lc4er2 Laub-Deciduous-Subtropical
  mai.setCoef(13, {0.8, 0., 0., 0., 0.75, 1.22, -44.7, 0.659, 0.0027, 0.631, 0.0204, 4.32, 0.003, 50., 0.1, 0.276, -0.1, 0.5, -0.263, 2.81, 0.02, 2.93, -3.75, -0.723});
  mai.setBoundaries(13,1.9 + 10.5,23.8 - 5.1,190. - 0,6900. + 4000.,-10.5 + 11.7,29.9 - 0.,1,1300 + 2000,0.15);
//lc4er3 Laub-Deciduosu-Temperate
  mai.setCoef(14, {0.8, 0., 0., 0., 0.75, 0.876, -23.4, 0.753, 0.00307, -2.24, 0.0215, 4.46, 0.003, 50., 0.1, 0.48, -0.195, 0.817, -0.4, 3.62, 0.02, 2., -1.5, -0.546});
  mai.setBoundaries(14,-4.9 + 5,14.6 - 2. + 3,305. - 0,6900. + 4000.,-29.7 + 0.,27.1 + 2.5,2,1300 + 2000,1.74);
  //lc4er4 Laub-Deciduous-Boreal
  mai.setCoef(15, {0.8, 0., 0., 0., 0.75, 1.11, -21.4, 0.574, 0.00433, -1.71, 0.0292, 6.16, 0.003, 50., 0.1, 0.276, -0.315, 0.612, -0.1, 0.5, 0.03, 2., -1.5, -0.5});
  mai.setBoundaries(15,-11.4 - 2.9,7.9 - 7.3,190. - 0,6900. + 4000.,-40.9 + 1.,21.4 + 1.,2,1300 + 2000,0.47);
//lc8er1 woody savannas-Tropical
  mai.setCoef(16, {0.8, 0., 0., 0., 0.75, 1.78, -20., 0.439, 0.00517, 5., 0.0195, 3.21, 0.003, 50., 0.1, 0.25, -0.1, 0.5, -0.1, 0.5, 0.02, 2., -2.63, -0.647});
  mai.setBoundaries(16, 18., 33. + BITT, 170., 11000., 9., 37., 0., 4000., .2);
//lc8er2 woody savannas-Subtropical
  mai.setCoef(17, {0.8, 0., 0., 0., 0.75, 2.05, -53.4, 0.476, 0.0053, -0.578, 0.0177, 3.01, 0.003, 50., 0.1, 0.25, -0.16, 0.767, -0.1, 0.5, 0.03, 2., -3.74, -0.943});
  mai.setBoundaries(17, 12., 19., 170., 11000., 1., 32., 0., 4000., .2);
//lc8er3 woody savannas-Temperate
  mai.setCoef(18, {0.8, 0., 0., 0., 0.75, 1.05, -26.5, 0.698, 0.00311, -1.12, 0.0183, 3.63, 0.003, 50., 0.1, 0.303, -0.146, 0.5, -0.1, 0.5, 0.02, 2.34, -3.53, -0.868});
  mai.setBoundaries(18, 0., 13., 170., 11000., -33., 30., 0., 4000., .2);
//lc8er4 woody savannas-Boreal
  mai.setCoef(19, {0.8, 0., 0., 0., 0.75, 1.02, -20., 0.597, 0.00427, -1.52, 0.0276, 5.46, 0.003, 50., 0.1, 0.25, -0.383, 0.5, -0.1, 0.615, 0.03, 2., -3.52, -0.719});
  mai.setBoundaries(19, -19., 1., 170., 11000., -50., 23., 0., 4000, .2);
}

// [[Rcpp::export]]
void maiSetSwr(double x) {mai.setSwr(x);}

// [[Rcpp::export]]
void maiSetCo2(double x) {mai.setCo2(x);}

// [[Rcpp::export]]
void maiSetTemperature(std::vector<double> x) {
  mai.setTemperature(std::valarray<double> (x.data(), x.size()));
}

// [[Rcpp::export]]
void maiSetPrecipitation(std::vector<double> x) {
  mai.setPrecipitation(std::valarray<double> (x.data(), x.size()));
}

// [[Rcpp::export]]
void maiSetRadiation(std::vector<double> x) {
  mai.setRadiation(std::valarray<double> (x.data(), x.size()));
}

// [[Rcpp::export]]
void maiSetWhc(double x) {mai.setWhc(x);}

// [[Rcpp::export]]
void maiSetAltitude(double x) {mai.setAltitude(x);}

// [[Rcpp::export]]
void maiSetN(double x) {mai.setN(x);}

// [[Rcpp::export]]
void maiSetP(double x) {mai.setP(x);}

// [[Rcpp::export]]
void maiSetS(double x) {mai.setS(x);}

// [[Rcpp::export]]
void maiSetpH(double x) {mai.setpH(x);}

// [[Rcpp::export]]
std::vector<double> maiGet() {
  std::valarray<double> rmai = mai.getMai(mai.testBoundaries(), true);
  return(std::vector<double> (std::begin(rmai), std::end(rmai)));
}
