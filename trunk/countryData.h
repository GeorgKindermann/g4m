#ifndef COUNTRYDATA_H_
#define COUNTRYDATA_H_

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include "misc.h"

const int countriesNum = 242;

using namespace std;

class countryData {
private:
    
    vector< g4m::ipol<double,double> > countries;
public:
    countryData();
    ~countryData();
    void reset(void);
    void set(int, int, double);
    void inc(int, int, double);
    double get(int, int);
};

void countryData::reset(void)
{
  int ind = 0;
  do {
    countries[ind].clear();
    ind++;
  } while (ind <countriesNum);
}


void countryData::set(int ind, int year, double val)
{
  countries[ind].insert(year,val);
}

void countryData::inc(int ind, int year, double val)
{
  countries[ind].inc(year,val);
}

double countryData::get(int ind, int year)
{
  return(countries[ind][year]);
}

countryData::countryData()
{
  countries.resize(countriesNum);
}

countryData::~countryData()
{
  
}

#endif
