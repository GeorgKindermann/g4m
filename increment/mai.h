#ifndef G4M_MAI_H
#define G4M_MAI_H

#include <vector>
#include <limits>
#include <cmath>
#include <valarray>

namespace g4m {

  class mai {
  public:
    mai(const std::valarray<double>& c = {1.95866e-11,16.6774,5.30899,300,7.00127,0.0828,0.180158,50,0.002,0.85,20, 2, 0.5, 0.841, -22.131}  //Coefficients
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
    double getNpp(unsigned int type=0, bool useMinNpp=false);
    std::valarray<double> getNpp(bool minNpp=false);
    std::valarray<double> getNpp(std::valarray<bool> dontNeed, bool useMinNpp=false);
    double getMai(unsigned int type=0, bool minNpp=false);
    std::valarray<double> getMai(bool minNpp=false);
    std::valarray<double> getMai(std::valarray<bool> dontNeed, bool useMinNpp=false);
    std::size_t setCoef(unsigned int type, const std::valarray<double>& c);
    std::size_t setCoefC0(unsigned int type, const std::valarray<double>& c);
    bool setBoundaries(unsigned int type, const double& tMinJ, const double& tMaxJ, const double& pMinJ, const double& pMaxJ, const double& tMinM, const double& tMaxM, const double& pMinM, const double& pMaxM, const double& minNpp); 
    void setTemperature(const std::valarray<double>& t);
    void setPrecipitation(const std::valarray<double>& p);
    void setRadiation(const std::valarray<double>& r);
    void setCo2(const double& co2);
    void setSwr(const double& swr);
    void setWhc(const double& whc);
    void setAltitude(const double& altitude);
    void setSoilType(const int& soilType);
    unsigned int setcNpp2mai(const std::valarray<double>& acNpp2mai);
    bool setWeatherAsDynamic(bool weatherIsDynamic);
    bool testBoundaries(unsigned int type);
    std::valarray<bool> testBoundaries();
  private:
    bool inputWasChanged;
    int updateSoilWater();

    unsigned int numberOfTypes;
    static const unsigned int nc=15; //###ACHTUNG 4 Koeffizienten fuer CO2 Einfluss sind hier reserviert!!!
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
  };

}

#endif
