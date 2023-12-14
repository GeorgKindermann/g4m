#include <cstdlib>
#include <iostream>
#include <cmath>
#include <Rcpp.h>
#include <algorithm>
//#include <omp.h>

static const double nTage = 30;

using namespace std;
// [[Rcpp::export]]
double nppMod(Rcpp::NumericVector cI,
			   Rcpp::NumericVector daysI,
			   Rcpp::NumericVector nppI,
			   Rcpp::NumericVector pI,
			   Rcpp::NumericVector tI,
			   Rcpp::NumericVector rI,
			   double nnI) {
  double aux=0.;
  double sum = 0.;
  for(int i=0; i<12; ++i) {
    sum += pow(cI[0]*exp(-nnI/7990.)
	       * daysI[i]
	       * rI[i]
	       * pow(fmax(0., cI[1]+tI[i]), 2)
	       * pow(fmax(0., 1. - cI[3]*exp(17.62*tI[i]/(243.12 + tI[i])) / tanh(cI[4]*(aux + pI[i]))), cI[5])
	       - nppI[i], 2);
  }
  return(sum);
}

// [[Rcpp::export]]
double nppMod2(Rcpp::NumericVector cI,
	       Rcpp::NumericVector daysI,
	       Rcpp::NumericVector nppI,
	       Rcpp::NumericVector pI,
	       Rcpp::NumericVector tI,
	       Rcpp::NumericVector rI,
	       double nnI,
	       double whcI) {
  double soilWater = whcI/2.;
  for(int i=0; i<12; ++i) {
    double aux = rI[i] - daysI[i] * cI[3]*exp(17.62*tI[i]/(243.12 + tI[i]));
    soilWater += aux;
    if(soilWater > whcI) {soilWater = whcI;}
    if(soilWater < 0.) {soilWater = 0.;}
  }
  double sum = 0.;
  for(int i=0; i<12; ++i) {
    sum += pow(cI[0]*exp(-nnI/7990.)
	       * daysI[i]
	       * rI[i]
	       * pow(fmax(0., cI[1]+tI[i]), 2)
	       * pow(fmax(0., 1. - cI[3]*exp(17.62*tI[i]/(243.12 + tI[i])) / tanh(cI[4]*(soilWater + pI[i]))), cI[5])
	       - nppI[i], 2);
    double aux = rI[i] - daysI[i] * cI[3]*exp(17.62*tI[i]/(243.12 + tI[i]));
    soilWater += aux;
    if(soilWater > whcI) {soilWater = whcI;}
    if(soilWater < 0.) {soilWater = 0.;}
  }
  return(sum);
}

// [[Rcpp::export]]
double nppMod3(Rcpp::NumericVector cI,
	       Rcpp::NumericVector daysI,
	       Rcpp::NumericVector nppI,
	       Rcpp::NumericVector pI,
	       Rcpp::NumericVector tI,
	       Rcpp::NumericVector rI,
	       double nnI,
	       double whcI) {
  double soilWater = whcI/2.;
  for(int i=0; i<12; ++i) {
    double aux = rI[i] - daysI[i] * cI[6]*exp(17.62*tI[i]/(243.12 + tI[i]));
    soilWater += aux;
    if(soilWater > whcI) {soilWater = whcI;}
    if(soilWater < 0.) {soilWater = 0.;}
  }
  double sum = 0.;
  for(int i=0; i<12; ++i) {
    sum += pow(cI[0]*exp(-nnI/7990.)
	       * daysI[i]
	       * rI[i]
	       * pow(fmax(0., cI[1]+tI[i]), 2)
	       * pow(fmax(0., 1. - cI[3]*exp(17.62*tI[i]/(243.12 + tI[i])) / tanh(cI[4]*(soilWater + pI[i]))), cI[5])
	       - nppI[i], 2);
    double aux = rI[i] - daysI[i] * cI[6]*exp(17.62*tI[i]/(243.12 + tI[i]));
    soilWater += aux;
    if(soilWater > whcI) {soilWater = whcI;}
    if(soilWater < 0.) {soilWater = 0.;}
  }
  return(sum);
}

// [[Rcpp::export]]
double nppMod4(Rcpp::NumericVector cI,
	       Rcpp::NumericVector daysI,
	       Rcpp::NumericVector tI,
	       Rcpp::NumericVector pI,
	       Rcpp::NumericVector rI,
	       double nnI,
	       double whcI) {
  double soilWater = whcI/2.;
  for(int i=0; i<12; ++i) {
    soilWater += rI[i] - daysI[i] * cI[6]*exp(17.62*tI[i]/(243.12 + tI[i]));
    if(soilWater > whcI) {soilWater = whcI;}
    if(soilWater < 0.) {soilWater = 0.;}
  }
  double sumTage = 0.;
  double sum = 0.;
  for(int i=0; i<12; ++i) {
    sumTage += daysI[i];
    sum += daysI[i]
      * rI[i]
      * pow(fmax(0., cI[1]+tI[i]), 2)
      * pow(fmax(0., 1. - cI[3]*exp(17.62*tI[i]/(243.12 + tI[i])) / tanh(cI[4]*(soilWater + pI[i]))), cI[5]);
    soilWater += rI[i] - daysI[i] * cI[6]*exp(17.62*tI[i]/(243.12 + tI[i]));
    if(soilWater > whcI) {soilWater = whcI;}
    if(soilWater < 0.) {soilWater = 0.;}
  }
  sum *= cI[0]*exp(-nnI/7990.);
  sum *= 365.25 / sumTage;
  return(sum);
}

// [[Rcpp::export]]
Rcpp::NumericVector nppMod5(Rcpp::NumericVector cI,
	       Rcpp::NumericVector daysI,
	       Rcpp::NumericVector tI,
	       Rcpp::NumericVector pI,
	       Rcpp::NumericVector rI,
	       Rcpp::NumericVector nnI,
	       Rcpp::NumericVector whcI) {
  int n = tI.size()/12;
  Rcpp::NumericVector npp(n);
  for(int j=0; j<n; ++j) {
    double soilWater = whcI[j]/2.;
    for(int i=0; i<12; ++i) {
      soilWater += pI[j*12+i] - daysI[i] * cI[6]*exp(17.62*tI[j*12+i]/(243.12 + tI[j*12+i]));
      if(soilWater > whcI[j]) {soilWater = whcI[j];}
      if(soilWater < 0.) {soilWater = 0.;}
    }
    double sumTage = 0.;
    npp[j] = 0.;
    for(int i=0; i<12; ++i) {
      sumTage += daysI[i];
      npp[j] += daysI[i]
	* rI[j*12+i]
      	* pow(fmax(0., cI[1]+tI[j*12+i]), cI[2])
      	* pow(fmax(0., 1. - cI[3]*exp(17.62*tI[j*12+i]/(243.12 + tI[j*12+i])) / tanh(cI[4]*(soilWater + pI[j*12+i]))), cI[5]);
      soilWater += pI[j*12+i] - daysI[i] * cI[6]*exp(17.62*tI[j*12+i]/(243.12 + tI[j*12+i]));
      if(soilWater > whcI[j]) {soilWater = whcI[j];}
      if(soilWater < 0.) {soilWater = 0.;}
    }
    npp[j] *= cI[0]*exp(-nnI[j]/7990.);
    npp[j] *= 365.25 / sumTage;
  }
  return(npp);
}

// [[Rcpp::export]]
Rcpp::NumericVector nppMod6(Rcpp::NumericVector cI,
	       Rcpp::NumericVector daysI,
	       Rcpp::NumericVector tI,
	       Rcpp::NumericVector pI,
	       Rcpp::NumericVector rI,
	       Rcpp::NumericVector nnI,
	       Rcpp::NumericVector whcI) {
  int n = tI.size()/12;
  Rcpp::NumericVector npp(n);
  for(int j=0; j<n; ++j) {
    double soilWater = 0.;
    for(int i=0; i<12; ++i) {
      double potEvapotrans = 30.*exp(17.62*tI[j*12+i]/(243.12 + tI[j*12+i]));
      double realEvapotrans = cI[6] * potEvapotrans;
      double interception = min({abs(cI[7]), pI[j*12+i], realEvapotrans});
      double precipToSoil = pI[j*12+i] - interception;
      soilWater +=  daysI[i]/30. * (precipToSoil - (realEvapotrans - interception));
      if(soilWater > whcI[j]) {soilWater = whcI[j];}
      if(soilWater < 0.) {soilWater = 0.;}
    }
    //soilWater = 0.; //Damit geht die Regression recht gut
    double sumTage = 0.;
    npp[j] = 0.;
    for(int i=0; i<12; ++i) {
      double potEvapotrans = 30.*exp(17.62*tI[j*12+i]/(243.12 + tI[j*12+i]));
      double realEvapotrans = cI[6] * potEvapotrans;
      double interception = min({abs(cI[7]), pI[j*12+i], realEvapotrans});
      //double interception = fmin(abs(cI[7]), pI[j*12+i]);
      double precipToSoil = pI[j*12+i] - interception;
      //Veraendern, damit Soilwater nicht 1:1 uebernommen werden kann
      //double plantAvailabelWater = soilWater * pow(soilWater/whcI[j],2) + precipToSoil;
      double plantAvailabelWater = soilWater + precipToSoil;
      soilWater +=  daysI[i]/30. * (precipToSoil - (realEvapotrans - interception));
      if(soilWater > whcI[j]) {soilWater = whcI[j];}
      if(soilWater < 0.) {soilWater = 0.;}
      sumTage += daysI[i];
      npp[j] += daysI[i]
	* rI[j*12+i]
      	* pow(fmax(0., cI[1]+tI[j*12+i]), cI[2])
	* pow(fmin(1., cI[8] * plantAvailabelWater / potEvapotrans), cI[3])
	* pow(fmax(0., 1. - fmax(0., cI[1]+tI[j*12+i]) / (cI[1]+cI[4])), cI[5]);
    }
    npp[j] *= cI[0]*exp(-nnI[j]/7990.);
    npp[j] *= 365.25 / sumTage;
  }
  return(npp);
}

// [[Rcpp::export]]
Rcpp::NumericVector nppMod7(Rcpp::NumericVector cI,
	       Rcpp::NumericVector daysI,
	       Rcpp::NumericVector tI,
	       Rcpp::NumericVector pI,
	       Rcpp::NumericVector rI,
	       Rcpp::NumericVector nnI,
	       Rcpp::NumericVector whcI) {
  int n = tI.size()/12;
  Rcpp::NumericVector npp(n);
  for(int j=0; j<n; ++j) {
    double bodenwasser = 0.;
    for(int i=0; i<12; ++i) {
      double niederschlag = pI[j*12+i];
      double temperatur = tI[j*12+i];
      double verdunstungPot = 30. * exp(17.62*temperatur/(243.12 + temperatur));
      double evapotrans = tanh(cI[6]) * verdunstungPot;
      double interceptionPot = abs(cI[10]) + fmin(abs(cI[7]) * tanh(abs(cI[8]) * verdunstungPot), evapotrans);
      double interception = fmin(niederschlag, interceptionPot);
      double niederschlagBoden = niederschlag - interception;
      double verfuegbaresBodenwasser = bodenwasser * pow((bodenwasser / whcI[j]), cI[9]);
      double nutzbaresWasser = niederschlagBoden + verfuegbaresBodenwasser;
      double genutztesWasser = fmin(nutzbaresWasser, evapotrans - interception);
      bodenwasser += (niederschlagBoden - genutztesWasser) * daysI[i]/30.;
      if(bodenwasser > whcI[j]) {bodenwasser = whcI[j];}
      if(bodenwasser < 0.) {bodenwasser = 0.;}
    }
    double sumTage = 0.;
    npp[j] = 0.;
    for(int i=0; i<12; ++i) {
      double niederschlag = pI[j*12+i];
      double temperatur = tI[j*12+i];
      double verdunstungPot = 30. * exp(17.62*temperatur/(243.12 + temperatur));
      double evapotrans = tanh(cI[6]) * verdunstungPot;
      double interceptionPot = abs(cI[10]) + fmin(abs(cI[7]) * tanh(abs(cI[8]) * verdunstungPot), evapotrans);
      double interception = fmin(niederschlag, interceptionPot);
      double niederschlagBoden = niederschlag - interception;
      double verfuegbaresBodenwasser = bodenwasser * pow((bodenwasser / whcI[j]), cI[9]);
      double nutzbaresWasser = niederschlagBoden + verfuegbaresBodenwasser;
      double genutztesWasser = fmin(nutzbaresWasser, evapotrans - interception);
      bodenwasser += (niederschlagBoden - genutztesWasser) * daysI[i]/30.;
      if(bodenwasser > whcI[j]) {bodenwasser = whcI[j];}
      if(bodenwasser < 0.) {bodenwasser = 0.;}
      sumTage += daysI[i];
      npp[j] += daysI[i]
	* rI[j*12+i]
	* pow(fmax(0., (cI[1]+temperatur)), cI[2])
	* pow(fmax(0, 1. - verdunstungPot/tanh(cI[5]*nutzbaresWasser)/cI[3]), cI[4]);
    }
    npp[j] *= cI[0]*exp(-nnI[j]/7990.);
    npp[j] *= 365.25 / sumTage;
    if(npp[j] != npp[j]) {npp[j] = 0.;}
  }
  return(npp);
}

// [[Rcpp::export]]
Rcpp::NumericVector nppMod8(Rcpp::NumericVector cI,
	       Rcpp::NumericVector daysI,
	       Rcpp::NumericVector tI,
	       Rcpp::NumericVector pI,
	       Rcpp::NumericVector rI,
	       Rcpp::NumericVector nnI,
	       Rcpp::NumericVector whcI) {
  int n = tI.size()/12;
  Rcpp::NumericVector npp(tI.size());
  for(int j=0; j<n; ++j) {
    double bodenwasser = 0.;
    for(int i=0; i<12; ++i) {
      double niederschlag = pI[j*12+i];
      double temperatur = tI[j*12+i];
      double verdunstungPot = 30. * exp(17.62*temperatur/(243.12 + temperatur));
      double evapotrans = tanh(cI[6]) * verdunstungPot;
      double interceptionPot = abs(cI[10]) + fmin(abs(cI[7]) * tanh(abs(cI[8]) * verdunstungPot), evapotrans);
      double interception = fmin(niederschlag, interceptionPot);
      double niederschlagBoden = niederschlag - interception;
      double verfuegbaresBodenwasser = bodenwasser * pow((bodenwasser / whcI[j]), cI[9]);
      double nutzbaresWasser = niederschlagBoden + verfuegbaresBodenwasser;
      double genutztesWasser = fmin(nutzbaresWasser, evapotrans - interception);
      bodenwasser += (niederschlagBoden - genutztesWasser) * daysI[i]/30.;
      if(bodenwasser > whcI[j]) {bodenwasser = whcI[j];}
      if(bodenwasser < 0.) {bodenwasser = 0.;}
    }
    npp[j] = 0.;
    for(int i=0; i<12; ++i) {
      double niederschlag = pI[j*12+i];
      double temperatur = tI[j*12+i];
      double verdunstungPot = 30. * exp(17.62*temperatur/(243.12 + temperatur));
      double evapotrans = tanh(cI[6]) * verdunstungPot;
      double interceptionPot = abs(cI[10]) + fmin(abs(cI[7]) * tanh(abs(cI[8]) * verdunstungPot), evapotrans);
      double interception = fmin(niederschlag, interceptionPot);
      double niederschlagBoden = niederschlag - interception;
      double verfuegbaresBodenwasser = bodenwasser * pow((bodenwasser / whcI[j]), cI[9]);
      double nutzbaresWasser = niederschlagBoden + verfuegbaresBodenwasser;
      double genutztesWasser = fmin(nutzbaresWasser, evapotrans - interception);
      bodenwasser += (niederschlagBoden - genutztesWasser) * daysI[i]/30.;
      if(bodenwasser > whcI[j]) {bodenwasser = whcI[j];}
      if(bodenwasser < 0.) {bodenwasser = 0.;}
      npp[j*12+i] = 30
	* cI[0]*exp(-nnI[j]/7990.)
	* rI[j*12+i]
	* pow(fmax(0., (cI[1]+temperatur)), cI[2])
	* pow(fmax(0, 1. - verdunstungPot/tanh(cI[5]*nutzbaresWasser)/cI[3]), cI[4]);
    }
  }
  return(npp);
}
