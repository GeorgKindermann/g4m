//   Name:      g4m
//   Author:    Andriy Bun, based on works of ...
//   Date:      9.06.2008

#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <ctime>

#include "misc.h"                       // ipol class for interpolation
#include "dataStruct.h"                 // data structures
#include "griddata3.h"                  // GridData class                       (v1.1, v1.2 or v1.3)
#include "forest.h"                     // definitions
#include "readInput.cpp"                // code for reading from files
#include "forestCalculations.cpp"       // code for calculations
#include "countryData.h"                // data by countries

//******************************************************************************
//***********************************  MAIN  ***********************************
//******************************************************************************
int main ()
 {
  time_t start=time(NULL);
// resolution of model  
  ResLatitude = int(floor(180/GridStepLat));
  ResLongitude = int(floor(360/GridStepLon));
// Setting years for output
  set<int> years;
  years.insert(2010);
  years.insert(2020);
  years.insert(2030);
  years.insert(2040);
  years.insert(2050);
//*******************************
//** Reading coefficients file **
//*******************************
  readCoeff(coeff);
// starting and ending years of calculations
  byear = coeff.bYear;
  eyear = coeff.eYear;
//*****************************************************
//** Reading input files (with resolution 0.5 x 0.5) **
//*****************************************************
  readInput("BUILTUP",data05x_BUILTUP);
  readInput("AREA",data05x_AREA);
  readInput("CROP",data05x_CROP);
  readInput("PRICEINDEX",data05x_PRICEINDEX);
  readInput("POPDENS",data05x_POPDENS);
  readInput("DISCRATE",data05x_DISCRATE);
  readInput("GDP",data05x_GDP);
  readInput("COUNTRY",data05x_COUNTRY);
//******************************************************************************
//**  data05x_COUNTRY[x][y][0]  - country index
//**  data05x_COUNTRY[x][y][4]  - IIASA region 4
//**  data05x_COUNTRY[x][y][11] - IIASA region 11
//******************************************************************************
//*******************************************
//** Reading detailed input data from file **
//*******************************************
  dataDetStruct plots;                         // structure with data
  readInputDet(plots);                         // plots[<elNum>].<variable>[year]
  cout << "> Time is " << difftime(time(NULL),start) << " sec." << endl;
//******************************************************************************
//***************************** start calculations *****************************
//******************************************************************************
//** Initializing forest cover array by gridcells **
//**************************************************
  griddata forestCover = griddata(ResLongitude,ResLatitude,0);
  griddata forestArea = griddata(ResLongitude,ResLatitude,0);
  dataDetStruct::iterator it = plots.begin();
  while (it != plots.end())
   {
    forestCover.set(it->x,it->y,it->FOREST[2000]);
    it++;
   }
      
      {
      double totFor1=0;
      for (int j = 0; j < 360; j++)
        for (int i = 0; i < 720; i++) {
          double ar = forestCover.get(i,j) * data05x_AREA[i][j][0];
          totFor1 += ar;
          forestArea.set(i,j,ar);
        }
      cout << "> Total forest area = " << totFor1 << endl;    
      }

//**************************************************
//** Initializing forest cover array by countries **
//**************************************************
  countryData allCountries = countryData();
//************************
//**** loop by years *****
//************************
  int year = byear;
  do {
//************************************
//** processing data from all plots **
//************************************
    cout << "Processing year " << year << endl;
    dataDetStruct::iterator it = plots.begin();
    forestCover.update();                                                       // update forest data
    while (it != plots.end())
     {
      double forShare = procPlots(*it,forestCover.GetMin(it->x,it->y),year);
      forestCover.set(it->x,it->y,forShare);                                     // updating forest data
      if (years.find(year)!=years.end()) {
        allCountries.inc(data05x_COUNTRY[it->x][it->y][0],year,forShare);
      }
      it++;
     }
/*
    if (year < 2012) {
      double totFor = 0;
      for (int j = 0; j < 360; j++)
        for (int i = 0; i < 720; i++) {
          double ar = forestCover.get(i,j) * data05x_AREA[i][j][0];
          totFor += ar;
          forestArea.set(i,j,ar);
        }
      stringstream ss;
      ss << year;
      string tmp = "forestArea_" + ss.str();
      forestArea.PrintToFile(tmp);
      cout << "Total forest area = " << totFor << endl;    
    }
*/
    if (years.find(year)!=years.end()) {
//******************************************************************************
        for (int j = 0; j < 360; j++)
          for (int i = 0; i < 720; i++) {
            forestCover.setPrev(i,j,forestCover.get(i,j)-forestCover.getPrev(i,j));
          }
//******************************************************************************
      stringstream ss;
      ss << year;
      string tmp = "forestCoverChange_" + ss.str();
      forestCover.PrintToFilePrev(tmp);
    }
    year++;
  } while (year <= eyear);
  cout << "> Working time is " << difftime(time(NULL),start) << " sec." << endl;
  system("pause");
 }
//******************************************************************************
// end main
//******************************************************************************
