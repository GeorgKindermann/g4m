#ifndef FORESTCALCULATIONS_CPP
#define FORESTCALCULATIONS_CPP
#include "dima.cpp"

//#ifdef unix
//#include "./dima/dima.cpp"
//#else
//#include "dima\dima.cpp"
//#endif

const double coeffAgr = 2;

//******************************************************************************
// compressing data, sending them for further calculations
//******************************************************************************
double procPlots(g4m::dataStruct &plot, double minForest, int year)
{
  double buf[40];
//********************************
//** compressing data to buffer **
//********************************  
  buf[0] =  minForest;
// from separate files
  buf[1] =  data05x_BUILTUP[plot.x][plot.y][year];
  buf[2] =  data05x_AREA[plot.x][plot.y][0];
  buf[3] =  data05x_CROP[plot.x][plot.y][year];
  buf[4] =  data05x_PRICEINDEX[plot.x][plot.y][year];
  buf[5] =  data05x_POPDENS[plot.x][plot.y][year];
  buf[6] =  data05x_DISCRATE[plot.x][plot.y][year];
  buf[7] =  data05x_GDP[plot.x][plot.y][year];  
// from the combined file
  buf[8] =  plot.NPP[year];
  buf[9] =  plot.AGRSUIT[year]; 
  buf[10] = plot.BIOMASS[year];
  buf[11] = plot.FOREST[year];
  buf[12] = plot.PROTECT[year];
// coefficients
  buf[13] = coeff.PriceIndexE[0];
  buf[14] = coeff.PriceLandMinR[0];
  buf[15] = coeff.PriceLandMaxR[0];
  buf[16] = coeff.FCuptake[0];
  buf[17] = coeff.FTimber[0];
  buf[18] = coeff.HarvLoos[0];
  buf[19] = coeff.PriceC[0];
  buf[20] = coeff.FracLongProd[0];
  buf[21] = coeff.decRateL[0];
  buf[22] = coeff.decRateS[0];
  buf[23] = coeff.SlashBurn[0];
  buf[24] = coeff.FreqAid[0];
  buf[25] = coeff.PriceCAid[0];
  buf[26] = coeff.MaxRotInter[0];
  buf[27] = coeff.MinRotInter[0];
  buf[28] = coeff.baseline[0];
  buf[29] = coeff.PriceTimberMaxR[0];
  buf[30] = coeff.PriceTimberMinR[0];
  buf[31] = coeff.PlantingCostsR[0];
  buf[32] = coeff.sPopDens[0];
  buf[33] = coeff.PriceIndexE[0];
// model run options
  buf[34] = coeff.cellsInteract;
  buf[35] = coeff.inclAffor;
  buf[36] = coeff.noPay;
  buf[37] = coeff.uBiomass;
  buf[38] = coeff.litter;
  buf[39] = coeff.SOC;
//************************************
//** sending buffer and calculating **
//************************************
  double res = calcPlots(buf,year);
  return (res);
}  

double calcPlots(double compr[],int year)
{
//************************
//** decompressing data **
//************************
  double minForest = compr[0];                 // minimum forest share among neighbouring cells
// from separate files
  double builtup       = compr[1];
  double landarea      = compr[2];
  double crop          = compr[3];
  double priceIndex    = compr[4];
  double popdens       = compr[5];
  double r             = compr[6];
  double GDPdat        = compr[7];
// from the combined file
  double NPP           = compr[8];
  double agrsuit       = compr[9];
  double biomass       = compr[10];
  double forestShare   = compr[11];                                             // has to be calculated and cumulated every year
  double protcost      = compr[12];
// coefficients
  double priceIndexE   = compr[13];
  double priceLandMinR = compr[14];
  double priceLandMaxR = compr[15];  
  double FCUptake      = compr[16];
  double ftimber       = compr[17];
  double harvloos      = compr[18];
  double pricec        = compr[19];
  double fraclongprod  = compr[20];
  double decratel      = compr[21];
  double decrates      = compr[22];
  double slashburn     = compr[23];
  double freqaid       = compr[24];
  double pricecaid     = compr[25];
// model run options
  int cellsInteract    = compr[34];
  int inclAffor        = compr[35];                                             // need to include code for afforestation
  int noPay            = compr[36];
  int uBiomass         = compr[37];                                             // need to include code for belowground biomass
  int litter           = compr[38];                                             // need to include code for belowground biomass
  int SOC              = compr[39];                                             // need to include code for belowground biomass
//***************************
//** starting calculations **
//***************************
  double aval, rotInter, priceTimber, forValNC;
  double refForShare = forestShare;            // forest share of ref. year
  double fsNoPay = forestShare;                              // Forest share without payment
  double fval = forVal(compr,rotInter,priceTimber,forValNC);
// calculating agricultural value with/without interaction
  if (!cellsInteract)
    aval = agrVal(compr);                               // without interaction
  else
    aval = coeffAgr * agrVal(compr) * (1 - minForest);  // with interaction

  double freeLand = 1 - (forestShare + crop + builtup);
// 0..Land has min Landprice, 1..Land has 2*minLandprice
  double landvalue = aval / (priceLandMinR * priceIndex / priceIndexE) - 1;
// deforestation speed
  double defShare = 0;
   {
    double gdp = 1644.;
    if(popdens > 0)
      gdp = 200000 * GDPdat / popdens;
    if((agrsuit > 0) && (forestShare > 0))
      defShare = 0.05/(1. + exp(1.799e+00 + 2.200e-01 / forestShare + 1.663e-01 
                 / agrsuit + ( -4.029e-02 + 5.305e-04 * popdens) * popdens + 1.282e-04 * gdp));
	defShare *= 3.52;           //Corection to come to the defor rate of FAO-FRA
   }
// Wood consumption per capita
  double woodConCapita = 5. /(1. + exp(2.221 - 2.242 * forestShare + 0.01227 * popdens -0.00005103 * 8500. * GDPdat));
  double woodCon = woodConCapita * popdens * landarea;
// Sustainable wood suply
  double woodProd = 10. * NPP * FCUptake * landarea * 100. * forestShare * ftimber * (1. - harvloos);
// Wood-suply deficit
  double woodDef = woodProd - woodCon;
  if (woodDef < 0.) {
    woodDef *= -1.;
	double addition = defShare * landarea * 100. * forestShare * biomass * ftimber;
    if (woodDef > addition) woodDef = addition;
    if (addition > 0) woodDef /= addition; else woodDef = 0.;
  } else woodDef = 0.;
  double defIncome = 0.;
// Pay if Carbon get's to air (Harvest goes to products)
   {
    double pDefIncome = biomass * (priceTimber * ftimber
		              * (1. - harvloos) - pricec * (1 + r)
		              * (fraclongprod * decratel / (decratel + r)
		              + fraclongprod * decrates / (decrates + r)));
// Immediate Pay if deforested (Slash and Burn)
    double sDefIncome = biomass *
		                (priceTimber * ftimber
		              * (1. - harvloos)
		              - pricec);
    defIncome = pDefIncome * (1. - slashburn) + sDefIncome * slashburn;
   }
  double defIncomeNoCp = 0.;  //Income with no carbon price
  defIncomeNoCp = biomass * priceTimber * ftimber * (1. - harvloos);
// Aid
  double aid = 0;
  if ((biomass > 0) && (pricecaid > 0))
   {
    double tmpR = pow(r + 1, freqaid);
    if (tmpR != 1.) aid = biomass * pricecaid * tmpR / (tmpR - 1.);
   }
  double protectionCosts = 0.;
  if (r > .07) protectionCosts =(r-.07) * protcost * priceIndex / priceIndexE * (r + 1.) / r;
  if ((aval + defIncomeNoCp) <= (fval * 1.5)) protectionCosts = 0.;
// Deforestation
  if ((aval + defIncome) > (fval * 1.5 + aid - protectionCosts))
    forestShare -= defShare; //Decrease Forest share
// Aforestation [HAS TO BE INCLUDED]
//Forest growth
  double maxfor = 1. - (builtup + crop);
  if (forestShare > maxfor) forestShare = maxfor;
  if (forestShare < 0.) forestShare = 0.;
  if (noPay) {
//Deforestation without Payment
    if (((aval + defIncomeNoCp) > (forValNC * 1.5 - protectionCosts)) && (NPP > 0.3)) {
      fsNoPay -= defShare;
      if (fsNoPay > maxfor) fsNoPay = maxfor;
      if (fsNoPay < 0.) fsNoPay = 0.;
    }
  }
  if(year <= 2000) {
    refForShare = forestShare;
    fsNoPay = forestShare;
  }
  double forestArea;
  double forestBiomass;
  double income;
  double costs;
  double costsb;
  double costsc;
  double forDec = refForShare - forestShare;
  forestArea = forDec * landarea;
  forestBiomass = forDec * landarea * 100. * biomass;
  if (year > 2000) {
    if (freqaid > 0.) {
      costs = forestShare * landarea * 100. * biomass * pricecaid / freqaid;
// Could we pay less and have the same result
      if ((aval + defIncome) <= (fval * 1.5 + aid - protectionCosts) && (aval + defIncome) > (fval * 1.5)) {
        double tmpR = pow(r+1, freqaid);
        if((tmpR != 1.) && (freqaid > 0.) && (biomass > 0)) {
          double pay = (aval + defIncome - fval * 1.5 + protectionCosts) / (biomass * tmpR / (r-1.));
		  costsb = forestShare * landarea * 100. * biomass * pay / freqaid;
// We don't pay for the whol grid, only for the saved forest
          costsc = forestShare * landarea * 100. * biomass * pay / freqaid * (forestShare - fsNoPay);
        }
      }
    }
	if(forDec > 0.) income = forDec * landarea * 100. * biomass * pricec;
  }
  return (forestShare);
}

#endif
