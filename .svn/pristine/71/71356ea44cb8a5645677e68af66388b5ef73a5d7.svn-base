//   Name:          GridData class 1.2
//   Author:        Andriy Bun
//   Date:          28.05.2008

#ifndef griddata_h_
#define griddata_h_

#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;

class griddata
 {
  private:
    int HorResolution;
    int VerResolution;
    int HorNeigh;
    int VerNeigh;
    double GridRows[];
    double *grid, *gridPrev;  
  public:
      griddata(int VR, int HR, double val);
      griddata();
      griddata(const griddata& g);
      ~griddata();
      void ShowArray();                       // prints array
      void ShowArrayPrev();                   // prints array for the previous year
      void update();                          // updates values for previous year to current values
      void set(int x, int y, double val);     // assigns value val to cell [x][y]
      void setPrev(int x, int y, double val); // assigns previous year value val to cell [x][y]
      void inc(int x, int y, double val);     // adds value val to the existing value in cell [x][y]
      double get(int x, int y);               // returns value stored in cell [x][y]
      double getPrev(int x, int y);           // returns value for the previous year stored in cell [x][y]
      void SetNeighNum(int n, int m);         // sets number of neighbour cells to be considered
      double GetMax(int x, int y);            // returns maximum value of all neighbours for the previous year
      double GetMin(int x, int y);            // returns minimum value of all neighbours for the previous year
      double GetAvg(int x, int y);            // returns average value for the previous year
 };

void griddata::ShowArray()
 {
  for (int j = 0; j < VerResolution; j++)
   {
    cout << j << "|\t";
    for (int i = 0; i < HorResolution; i++)
     {
      cout << grid[j*HorResolution+i] << "\t";
     }
    cout << endl;  
   }   
 }

void griddata::ShowArrayPrev()
 {
  for (int j = 0; j < VerResolution; j++)
   {
    cout << j << "|\t";
    for (int i = 0; i < HorResolution; i++)
     {
      cout << gridPrev[j*HorResolution+i] << "\t";
     }
    cout << endl;  
   }   
 }
 
void griddata::set(int x, int y, double val)
 {
  grid[y*HorResolution+x] = val;
 }
 
void griddata::setPrev(int x, int y, double val)
 {
  gridPrev[y*HorResolution+x] = val;
 }
 
void griddata::inc(int x, int y, double val)
 {
  grid[y*HorResolution+x] += val;
 }
 
double griddata::get(int x, int y)
 {
  return (grid[y*HorResolution+x]);
 }
 
double griddata::getPrev(int x, int y)
 {
  return (gridPrev[y*HorResolution+x]);
 }
 
void griddata::update()
 {
  memcpy(gridPrev,grid,VerResolution*HorResolution*sizeof(double));
 }

void griddata::SetNeighNum(int n, int m)
 {
  HorNeigh = n;
  VerNeigh = m;
 }

double griddata::GetMax(int x, int y)
 {
  int tmpx = x - HorNeigh;
  if (tmpx < 0) tmpx = HorResolution + tmpx;
  int tmpy = y - VerNeigh;
  if (tmpy < 0) tmpy = 0;
  double maxv = gridPrev[tmpy*HorResolution+tmpx];
  for (int j = tmpy; j <= y + VerNeigh; j++)
   {
    if (j >= VerResolution) break;
    for (int i = -HorNeigh; i <= HorNeigh; i++)
     {
      int ii = x + i;
      if (ii >= HorResolution) ii -= HorResolution;
      else if (ii < 0) ii += HorResolution;
      if ((gridPrev[j*HorResolution+ii] > maxv) && !((ii == x) && (j == y)))
        maxv = gridPrev[j*HorResolution+ii];
     }
   }  
  return(maxv);
 }

double griddata::GetMin(int x, int y)
 {
  int tmpx = x - HorNeigh;
  if (tmpx < 0) tmpx = HorResolution + tmpx;
  int tmpy = y - VerNeigh;
  if (tmpy < 0) tmpy = 0;
  double minv = gridPrev[tmpy*HorResolution+tmpx];
  for (int j = tmpy; j <= y + VerNeigh; j++)
   {
    if (j >= VerResolution) break;
    for (int i = -HorNeigh; i <= HorNeigh; i++)
     {
      int ii = x + i;
      if (ii >= HorResolution) ii -= HorResolution;
      else if (ii < 0) ii += HorResolution;
      if ((gridPrev[j*HorResolution+ii] < minv) && !((ii == x) && (j == y)))
        minv = gridPrev[j*HorResolution+ii];
     }
   }  
  return(minv);
 }

double griddata::GetAvg(int x, int y)
 {
  int count = 0;
  int tmpx = x - HorNeigh;
  if (tmpx < 0) tmpx = HorResolution + tmpx;
  int tmpy = y - VerNeigh;
  if (tmpy < 0) tmpy = 0;
  double sumv = 0;
  for (int j = tmpy; j <= y + VerNeigh; j++)
   {
    if (j >= VerResolution) break;
    for (int i = -HorNeigh; i <= HorNeigh; i++)
     {
      int ii = x + i;
      if (ii >= HorResolution) ii -= HorResolution;
      else if (ii < 0) ii += HorResolution;
      count++;
      sumv += gridPrev[j*HorResolution+ii];
     }
   }  
  return(sumv/count);
 }

// Class constructor
griddata::griddata(int HR, int VR, double val)
 {
  HorResolution = HR;
  VerResolution = VR;
  HorNeigh = 1;
  VerNeigh = 1;
  grid = new double[HorResolution*VerResolution];
  gridPrev = new double[HorResolution*VerResolution];
  memset(grid,val,HorResolution*VerResolution*sizeof(double));
  memset(gridPrev,val,HorResolution*VerResolution*sizeof(double));
 }

// Default constructor
griddata::griddata()
 {
  HorResolution = 720;
  VerResolution = 360;
  HorNeigh = 1;
  VerNeigh = 1;
  grid = new double[HorResolution*VerResolution];
  gridPrev = new double[HorResolution*VerResolution];
  memset(grid,0,HorResolution*VerResolution*sizeof(double));
  memset(gridPrev,0,HorResolution*VerResolution*sizeof(double));
 }

// Copy constructor
griddata::griddata(const griddata& g)
 {
  HorResolution = g.HorResolution;
  VerResolution = g.VerResolution;
  HorNeigh = g.HorNeigh;
  VerNeigh = g.VerNeigh;
  grid = new double[HorResolution*VerResolution];
  gridPrev = new double[HorResolution*VerResolution];
  memcpy(grid,g.grid,HorResolution*VerResolution*sizeof(double));
  memcpy(gridPrev,g.gridPrev,HorResolution*VerResolution*sizeof(double));
 }

// Destructor
griddata::~griddata()
 {
  delete []grid;
  delete []gridPrev;
 }

#endif
