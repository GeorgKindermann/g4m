//******************************************************************************
// DIMA model
//******************************************************************************
#ifndef dima_h_
#define dima_h_

double forVal(double);

// Value of Forestry during multiple rotation
double forVal(double x)
 {

  double forest = x;
  if(forest < 0.) {forest = 0.;}
  if(forest > 1.) {forest = 1.;}  
// Mean anual carbon uptake (t-C/ha/year)
  double cUptake = 10. * npp.v(year) * fcuptake.v(year); //kg/m2 -> t/ha
// Harvestable wood-volume increment (m3/ha/year)
  double vIncr = cUptake * ftimber.v(year);
// Rotation interval of a Forest in Years
  double rotInter = 100;
  { double harvestVolume = 600. - abs(vIncr -6.) * 50.;
  if(cUptake > 0.) {rotInter = harvestVolume/vIncr;}
  if(rotInter < minRotInter.v(year)) {rotInter = minRotInter.v(year);}
  if(rotInter > maxRotInter.v(year)) {rotInter = maxRotInter.v(year);} }
//Costs to plant 1 ha of forest
  double plantingCosts;
  { double plantrate = (vIncr-3.)/6.;
  if(plantrate > 1.) {plantrate = 1.;}
  if(plantrate < 0.) {plantrate = 0.;}
  plantingCosts = (plantrate*plantingCosts0.v(year)*priceIndex.v(year)/ priceIndex0.v(year); }
//Timber price
  double priceTimber;
  { double sfor = (1. - forest) * 9. + 1.;
  double c4 = (priceTimberMax0.v(year) - priceTimberMin0.v(year))/99.;
  double c3 = priceTimberMin0.v(year) - c4;
  priceTimber = (c3 + c4 * sPopDens.v(year) * sfor) * priceIndex.v(year)/priceIndex0.v(year); }
//Price to harvest the timber
  double priceHarvest = priceTimber() * .0;
//Harvest volume of the timber
  double woodHarvestVol = vIncr * rotInter * (1. - harvLoos.v(year)));
//Fraction of carbon costs during harvest
  double beta = 1. - decLongProd.v(year)/(decLongProd.v(year)+r.v(year))*fracLongProd.v(year)
	     - decShortProd.v(year)/(decShortProd.v(year)+r.v(year)) * (1.-fracLongProd.v(year));
//Carbon benefit
  double cBenefit = priceC.v(year) * cUptake * (1. - baseline.v(year)) * 
         (((1. - pow(1.+r.v(year),-rotInter) ) /r.v(year)) -
	     rotInter * (1.-beta) * pow(1.+r.v(year), -rotInter));
// Value of Forestry during one rotation
  double forestValueOne = (-plantingCosts + (priceTimber - priceHarvest)
         *woodHarvestVol + cBenefit);
// Return values
  return(forestValueOne/(1.-pow(1.+r.v(year), -rotInter)));
 }

/*
double dima::forestValueOneNC()
{
  //Value of Forestry one rotation NoCarbonPrice
  return(-plantingCosts() + (priceTimber()
    -priceHarvest())*woodHarvestVol()*pow(1+r.v(year), -rotInter()));
}

double dima::forValNC()
{
  //Value of Forestry multiple rotation No Carbon Price
  return(forestValueOneNC()/(1.-pow(1.+r.v(year), -rotInter())));
}

double dima::agrVal()
{
  //Net present Value of Agriculture (Eq.5)
  double priceLevel = priceLandMin0.v(year)
                    * priceIndex.v(year)/priceIndex0.v(year);
  //Importance of Population density
  double popImp = (log(priceLandMax0.v(year))
		        - log(priceLandMin0.v(year)))/(2. * log(10.));  
  //Importance of the Suitable for Agriculture
  double agrImp = popImp;
  return(priceLevel * pow(sPopDens.v(year),popImp)
	                * pow(sAgrSuit.v(year),agrImp));
}

double dima::amenVal()
{
  //Value of amenity
  double priceLevel = priceLandMin0.v(year)
                    * priceIndex.v(year)/priceIndex0.v(year);
  //Importance of Population density
  double popImp = (log(priceLandMax0.v(year))
		        - log(priceLandMin0.v(year)))/(2. * log(10.));  
  //Importance of GDP
  double gdpImp = popImp;
  return(priceLevel * pow(sPopDens.v(year),popImp)
	                * pow(gdp.v(year),gdpImp));
}

double dima::minPriceC()
{ 
  //Get the minimum Carbonprice where Forest=Argic (Eq.6)
  return((agrVal() * (1-pow(1+r.v(year),-rotInter())) +
	  plantingCosts() -
	  (priceTimber()
	   -priceHarvest())*woodHarvestVol()*pow(1+r.v(year), -rotInter()))
	 /(cUptake() * (1. - baseline.v(year)) * 
	  (((1. - pow(1.+r.v(year),-rotInter()) ) /r.v(year)) -
	    rotInter() * (1.-beta()) * pow(1.+r.v(year), -rotInter()))));
}*/

#endif
