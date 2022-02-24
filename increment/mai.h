#ifndef G4M_MAI_H
#define G4M_MAI_H

#include <vector>
#include <limits>
#include <cmath>
#include <valarray>

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

    mai(const std::valarray<double>& c //= {1.95866e-11,16.6774,5.30899,300,7.00127,0.0828,0.180158,50,0.002,0.85,20, 2, 0.5, 0.841, -22.131}  //Coefficients
	, const std::valarray<double>& c0 = {1.054684,1.099922,1.075196,0.980570,1.002155,1.044522,1.134524,1.073864,1.000548,1.070339,1.068615,1.086483,1.054495,1.036821,1.095323,1.008207,1.094867,1.031270,0.987843,1.035130,0.950606,1.074587,1.008381} //c0 Coefficients
	, const std::valarray<double>& t = std::valarray<double>(10., 24)  //Temperature of each month [deg C]
	, const std::valarray<double>& p = std::valarray<double>(70., 24)  //Precipitation [mm/30 Days]
	, const std::valarray<double>& r = std::valarray<double>(180., 24) //Radiation [Watt/m2]
	, double whc=100.    //Water holding capacity [mm]
	, double swr=0.  //Soil Water Regime, additional water from soil or irrigation [mm/month]
	, double co2=0.038   //CO2 concentration [volume %]
	, int soilType=0   //Soil type code
	, double altitude=0.    //Altitude [m]
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

    mai(const std::valarray<double>& c14t17 // = {2, 0.5, 0.841, -22.131}  //Coefficients c[14] to c[17]
	, const std::valarray<double>& c = {1.29460152,-0.09012495,0.17332495, 35, -1, 0.66170523, 2.8, 0.4138984, -54.4741443, -1.4, 1.155907e+00, 2.154028e-04, -3.733458e-01, 2.335792e-05}  //Coefficients
	, const std::valarray<double>& c0 = {0.06179233,0.06448844,0.07000044,0.07867775,0.06506758,0.08137664,0.06192571,0.07169721,0.07110523,0.06381677,0.05441309,0.06347873,0.07584091,0.07330926,0.05766713,0.07205265,0.05055277,0.06077571,0.07759581,0.05685617,0.06527024,0.05558023,0.06699292} //c0 Coefficients
	 , const std::valarray<double>& t = std::valarray<double>(10., 24)  //Temperature of each month [deg C]
	 , const std::valarray<double>& p = std::valarray<double>(70., 24)  //Precipitation [mm/month]
	, double whc=100.    //Water holding capacity [mm]
	, double swr=0.  //Soil Water Regime, additional water from soil or irrigation [mm/month]
	, double co2=0.038   //CO2 concentration [volume %]
	, int soilType=0   //Soil type code
	, double altitude=0.    //Altitude [m]
	, double latitude=0. //latitude of location [rad]
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
    void setN(const double& N);
    void setP(const double& P);
    void setS(const double& S);
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


}

#endif
