#include <cstdlib>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <map>
#include <Rcpp.h>

static const double nTage = 30;

using namespace std;

// [[Rcpp::export]]
Rcpp::NumericVector soilWater(Rcpp::NumericVector cI
			      , Rcpp::NumericVector feldkapaI
			      , Rcpp::NumericVector tageI
			      , Rcpp::NumericVector tI
			      , Rcpp::NumericVector pI) {
  int datenzeilen = feldkapaI.size();
  int wetterZeitraum = tI.size() / datenzeilen;
  Rcpp::NumericVector res(tI.size());
  for(int j=0; j<datenzeilen; ++j) {
    double bodenwasser=feldkapaI[j]/2.; //Start mit halb gefuelltem Boden
    for(int i=0; i<wetterZeitraum; ++i) {
      int monat = i%12;
      double niederschlag = pI[j*wetterZeitraum+i];
      double temperatur = tI[j*wetterZeitraum+i];
      double verdunstungPot = 30. * exp(17.62*temperatur/(243.12 + temperatur));
      double evapotrans = tanh(cI[0]) * verdunstungPot;
      double interceptionPot = abs(cI[1]) + fmin(abs(cI[2]) * tanh(abs(cI[3]) * verdunstungPot), evapotrans);
      double interception = fmin(niederschlag, interceptionPot);
      double niederschlagBoden = niederschlag - interception;
      double verfuegbaresBodenwasser = bodenwasser * pow((bodenwasser / feldkapaI[j]), abs(cI[4]));
      double nutzbaresWasser = niederschlagBoden + verfuegbaresBodenwasser;
      double genutztesWasser = fmin(nutzbaresWasser, evapotrans - interception);
      bodenwasser += (niederschlagBoden - genutztesWasser) * tageI[monat]/30.;
      if(bodenwasser > feldkapaI[j]) {bodenwasser = feldkapaI[j];}
      if(bodenwasser < 0.) {bodenwasser = 0.;}
      //res[j*wetterZeitraum+i] = bodenwasser;
      //res[j*wetterZeitraum+i] = genutztesWasser;
      res[j*wetterZeitraum+i] = genutztesWasser / evapotrans;
    }
  }
  return(res);
}
