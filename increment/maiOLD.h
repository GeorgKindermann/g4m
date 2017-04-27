#ifndef G4M_MAI_H
#define G4M_MAI_H

#include <vector>
#include <limits>
#include <cmath>
#include <valarray>

namespace g4m {

  class mai {
  public:
    mai(const std::valarray<double>& c14t17 = {2, 0.5, 0.841, -22.131}  //Coefficients c[14] to c[17]
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
    double getNpp(unsigned int type);
    std::valarray<double> getNpp();
    std::valarray<double> getNpp(std::valarray<bool> dontNeed);
    double getMai(unsigned int type);
    std::valarray<double> getMai();
    std::valarray<double> getMai(std::valarray<bool> dontNeed);
    double getNppB(unsigned int type); //The same like above but use also minNpp
    std::valarray<double> getNppB();
    std::valarray<double> getNppB(std::valarray<bool> dontNeed);
    double getMaiB(unsigned int type);
    std::valarray<double> getMaiB();
    std::valarray<double> getMaiB(std::valarray<bool> dontNeed);
    std::size_t setCoef(unsigned int type, const std::valarray<double>& c);
    std::size_t setCoefC0(unsigned int type, const std::valarray<double>& c);
    bool setBoundaries(unsigned int type, const double& tMinJ, const double& tMaxJ, const double& pMinJ, const double& pMaxJ, const double& tMinM, const double& tMaxM, const double& pMinM, const double& pMaxM, const double& minNpp); 
    void setTemperature(const std::valarray<double>& t);
    void setPrecipitation(const std::valarray<double>& p);
    void setCo2(const double& co2);
    void setSwr(const double& swr);
    void setWhc(const double& whc);
    void setAltitude(const double& altitude);
    void setSoilType(const int& soilType);
    void setLatitude(double latitude);
    double setSoilWaterDecayRate(double soilWaterDecayRate);
    unsigned int setcNpp2mai(const std::valarray<double>& acNpp2mai);
    bool setWeatherAsDynamic(bool weatherIsDynamic);
    bool testBoundaries(unsigned int type);
    std::valarray<bool> testBoundaries();
  private:
    bool inputWasChanged;
    int updateSoilWater();
    void calcWalterLieth();

    std::valarray<double> c14t17;
    unsigned int numberOfTypes;
    static const unsigned int nc=14;
    static const unsigned int nc0=23;
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
    double altitude;         //altitude
    int soilType; //Soil type
    std::valarray<double> t; //Temperature for each month of this year [12]
    std::valarray<double> p; //Precipitation for each month of this year [12]
    std::valarray<double> sw; //Soil water content [12]
    std::valarray<double> tp; //Temperature for each month of previous year [12]
    std::valarray<double> pp; //Precipitation for each month of previous year [12]
    bool weatherIsDynamic;
    double soilWaterDecayRate;
    double latitude;
    unsigned char fNpp2mai;
    std::valarray<double> cNpp2mai;
  };

}

#endif
