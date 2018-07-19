#ifndef G4M_DATASTRUCT_H
#define G4M_DATASTRUCT_H

#include <string>
#include "misc.h"

namespace g4m{

struct dataStruct
 {
  int x,y;
//  std::string classes;
  ipol<double,double> NPP;
  ipol<double,double> AGRSUIT;
  ipol<double,double> BIOMASS;
  ipol<double,double> FOREST;
  ipol<double,double> PROTECT;

// Constructor: initializing default values
  dataStruct()
   {
    x=0;
    y=0;
//    classes = "AAA";
   }
 };

struct coeffStruct
 {
// Starting Year of simmulation
  int bYear;
// Ending Year of simmulation
  int eYear;
// Interaction between cells
  int cellsInteract;
// Consider afforestation
  int inclAffor;
// No pay
  int noPay;
// Belowground biomass
  int uBiomass;
// Litter
  int litter;
// Soil organic carbon
  int SOC;
//******************************************************************************
//** parameters
//******************************************************************************
// Priceindex of reference country
  ipol<double,double> PriceIndexE;
// Minimum Landprice [cash/ha]
  ipol<double,double> PriceLandMinR;
// Maximum Landprice [cash/ha]
  ipol<double,double> PriceLandMaxR;
// Factor Carbon uptake (DIMA-Model)
  ipol<double,double> FCuptake;
// Comercial timbervolume per ton of carbon [m3/tC]
  ipol<double,double> FTimber;
// HarvestingLosses (Share of losses during harvest)
  ipol<double,double> HarvLoos;
// Carbon price [Cash/tC] (9/25)
  ipol<double,double> PriceC;
// Share of Long-living products [0-1]
  ipol<double,double> FracLongProd;
// Decrease rate of long living products
  ipol<double,double> decRateL;
// Decrease rate of short living products
  ipol<double,double> decRateS;
// Share of SlashBurn deforestation [0-1]
  ipol<double,double> SlashBurn;
// Frequency of aid payments (PRICECAID) [Years]
  ipol<double,double> FreqAid;
// Aid Carbon Price [Cash/tC/FREQAID] (6)
  ipol<double,double> PriceCAid;
// Maximum rotation time im Years
  ipol<double,double> MaxRotInter;
// Minimum rotation time im Years
  ipol<double,double> MinRotInter;
// Baseline
  ipol<double,double> baseline;
// Maximum Timberprice [cash/m3]
  ipol<double,double> PriceTimberMaxR;
// Minimum Timberprice [cash/m3]
  ipol<double,double> PriceTimberMinR;
// Planting costs in reference country [Cash/ha]
  ipol<double,double> PlantingCostsR;
// Standardised Populationdensity [1-10]
  ipol<double,double> sPopDens;
 
  // Constructor: initializing default values
  coeffStruct()
   {
    bYear = 2008;
    eYear = 2050;
   }
 };

}

#endif
