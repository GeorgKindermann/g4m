//******************************************************************************
// DIMA model
//******************************************************************************
#ifndef dima_h_
#define dima_h_

double forVal(double [], double &);
double agrVal(double []);

// Value of Forestry during multiple rotation
double forVal(double compr[], double &rotInt)
 {
// decompressing data  
  double npp = compr[0];
  double sPopDens = compr[1];
  double priceIndex = compr[2];
  double priceIndex0 = compr[3];
  double r = compr[4];
  double priceC = compr[5];
  double plantingCosts0 = compr[6];
  double maxRotInter = compr[7];
  double minRotInter = compr[8];
  double ftimber = compr[9];
  double decLongProd = compr[10];
  double decShortProd = compr[11];
  double fracLongProd = compr[12];
  double baseline = compr[13];
  double priceTimberMax0 = compr[14];
  double priceTimberMin0 = compr[15];
  double fcuptake = compr[16];
  double harvLoos = compr[17];
  double forest = compr[18];  
// calculations
  if(forest < 0.) {forest = 0.;}
  if(forest > 1.) {forest = 1.;}
// Mean anual carbon uptake (t-C/ha/year)
  double cUptake = 10. * npp * fcuptake; //kg/m2 -> t/ha
// Harvestable wood-volume increment (m3/ha/year)
  double vIncr = cUptake * ftimber;
// Rotation interval of a Forest in Years
  double rotInter = 100;                                                        // vykorystovujetsya dali
  { double harvestVolume = 600. - abs(vIncr -6.) * 50.;
  if(cUptake > 0.) {rotInter = harvestVolume/vIncr;}
  if(rotInter < minRotInter) {rotInter = minRotInter;}
  if(rotInter > maxRotInter) {rotInter = maxRotInter;} }
//Costs to plant 1 ha of forest
  double plantingCosts;
  { double plantrate = (vIncr-3.)/6.;
  if(plantrate > 1.) {plantrate = 1.;}
  if(plantrate < 0.) {plantrate = 0.;}
  plantingCosts = plantrate * plantingCosts0 * priceIndex / priceIndex0; }
//Timber price
  double priceTimber;
  { double sfor = (1. - forest) * 9. + 1.;
  double c4 = (priceTimberMax0 - priceTimberMin0)/99.;
  double c3 = priceTimberMin0 - c4;
  priceTimber = (c3 + c4 * sPopDens * sfor) * priceIndex / priceIndex0; }
//Price to harvest the timber
  double priceHarvest = priceTimber * .0;
//Harvest volume of the timber
  double woodHarvestVol = vIncr * rotInter * (1. - harvLoos);
//Fraction of carbon costs during harvest
  double beta = 1. - decLongProd/(decLongProd+r)*fracLongProd
	     - decShortProd/(decShortProd+r) * (1.-fracLongProd);
//Carbon benefit
  double cBenefit = priceC * cUptake * (1. - baseline) * (((1. - pow(1.+r,-rotInter) ) /r) -
	     rotInter * (1.-beta) * pow(1.+r, -rotInter));
// Value of Forestry during one rotation
  double forestValueOne = (-plantingCosts + (priceTimber - priceHarvest)
         *woodHarvestVol) + cBenefit;
// Return values
  rotInt = rotInter;
  return(forestValueOne/(1.-pow(1.+r, -rotInter)));
 }

//Net present Value of Agriculture
double agrVal(double compr[])
{
// decompressing
  double sPopDens = compr[1];
  double priceIndex = compr[2];
  double priceIndex0 = compr[3];
  double sAgrSuit = compr[19];
  double priceLandMax0 = compr[20];
  double priceLandMin0 = compr[21];
// calculations
  double priceLevel = priceLandMin0 * priceIndex / priceIndex0;
//Importance of Population density
  double popImp = (log(priceLandMax0) - log(priceLandMin0))/(2. * log(10.));  
//Importance of the Suitable for Agriculture
  double agrImp = popImp;
  return(priceLevel * pow(sPopDens,popImp) * pow(sAgrSuit,agrImp));
}

#endif
