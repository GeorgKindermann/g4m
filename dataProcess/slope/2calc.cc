#include <iostream>
#include <cmath>
#include <vector>
#include <utility>
#include <cstdlib>
#include <fstream>

using namespace std;

void laloh2xyz(const double &lat, const double &lon, const double &h, double &x, double &y, double &z) {
  //lat,lon .. position in radians
  //h .. Height in meters
  //x,y,z .. position in meters

  static const double a=6378137.000;
  static const double b=6356752.3142;
  //double f=298.257223563;
  static const double e=(a*a-b*b)/(a*a);

  double sinLat = sin(lat);

  double n=a/sqrt(1.-e*sinLat*sinLat);

  z=(n*(1.-e)+h)*sinLat;
  
  n += h;
  n *= cos(lat);

  x=n*cos(lon);
  y=n*sin(lon);
}

double dist(const double &lat, const double &lon, const double &dlat, const double &dlon) {
  double x0,y0,z0,x1,y1,z1;
  laloh2xyz(lat,lon,0,x0,y0,z0);
  laloh2xyz(lat+dlat,lon+dlon,0,x1,y1,z1);
  return(sqrt(pow(x0-x1,2) + pow(y0-y1,2) + pow(z0-z1,2)));
}

class fileio {
public:
  bool getRow(int *alti);
  fileio(const char *filename, const int anoval, const unsigned int ncol);
private:
  ifstream fp;
  int noval;
  unsigned int ncol;
};

fileio::fileio(const char *filename, const int anoval, const unsigned int ancol) {
  fp.open(filename, ios::in | ios::binary);
  noval = anoval;
  ncol = ancol;
}
bool fileio::getRow(int *alti) {
  for(unsigned int col=0; col<ncol; ++col) {
    int data = noval;
    if(fp.good()) {
      int byte0 = fp.get();
      int byte1 = fp.get();
      short nn = byte1<<8;
      nn += byte0;
      data = nn;
    }
    alti[col] = data;
  }
  return(fp.good() && !fp.eof());
}

void gradEW(const int *alti, double *grad, const double &dist, unsigned int n, const int &noval) { //Calculate gradient
  --n;
  for(unsigned int i=0; i<n; ++i) {
    if(alti[i] != noval && alti[i+1] != noval) {
      grad[i] = (alti[i]-alti[i+1])/dist;
    } else {grad[i] = NAN;}
  }
}

void gradNS(const int *alti0, const int *alti1, double *grad, const double &dist, const unsigned int n, const int &noval) { //Calculate gradient
  for(unsigned int i=0; i<n; ++i) {
    if(alti0[i] != noval && alti1[i] != noval) {
      grad[i] = (alti0[i]-alti1[i])/dist;
    } else {grad[i] = NAN;}
  }
}

void gradEWmin(const int *alti, double *grad, const double &dist, unsigned int n, const int &noval, const double &hnois) {
  --n;
  for(unsigned int i=0; i<n; ++i) {
    if(alti[i] != noval && alti[i+1] != noval) {
      double dh = alti[i]-alti[i+1];
      if(fabs(dh) < hnois) {grad[i] = 0.;}
      else {
	if(dh > 0) {dh -= hnois;}
	else {dh += hnois;}
	grad[i] = dh/dist;
      }
    } else {grad[i] = NAN;}
  }
}

void gradEWmax(const int *alti, double *grad, const double &dist, unsigned int n, const int &noval, const double &hnois) {
  --n;
  for(unsigned int i=0; i<n; ++i) {
    if(alti[i] != noval && alti[i+1] != noval) {
      double dh = alti[i]-alti[i+1];
      if(dh > 0) {dh += hnois;}
      else {dh -= hnois;}
      grad[i] = dh/dist;
    } else {grad[i] = NAN;}
  }
}

void gradNSmin(const int *alti0, const int *alti1, double *grad, const double &dist, const unsigned int n, const int &noval, const double &hnois) {
  for(unsigned int i=0; i<n; ++i) {
    if(alti0[i] != noval && alti1[i] != noval) {
      double dh = alti0[i]-alti1[i];
      if(fabs(dh) < hnois) {grad[i] = 0.;}
      else {
	if(dh > 0) {dh -= hnois;}
	else {dh += hnois;}
	grad[i] = dh/dist;
      }
    } else {grad[i] = NAN;}
  }
}

void gradNSmax(const int *alti0, const int *alti1, double *grad, const double &dist, const unsigned int n, const int &noval, const double &hnois) {
  for(unsigned int i=0; i<n; ++i) {
    if(alti0[i] != noval && alti1[i] != noval) {
      double dh = alti0[i]-alti1[i];
      if(dh > 0) {dh += hnois;}
      else {dh -= hnois;}
      grad[i] = dh/dist;
    } else {grad[i] = NAN;}
  }
}

void slp(const double *grad0, const double *grad1, unsigned int n, ofstream *fp) {
  --n;
  for(unsigned int i=0; i<n; ++i) {
    fp->put(static_cast<char>(floor(0.5 + atan(sqrt(grad0[i]*grad0[i] + grad1[i]*grad1[i]))*180./M_PI)));
    fp->put(static_cast<char>(floor(0.5 + atan(sqrt(grad0[i]*grad0[i] + grad1[i+1]*grad1[i+1]))*180./M_PI)));
  }
}

void aspect(const double *gradEW, const double *gradNS, unsigned int n, ofstream *fp) {
  --n;
  for(unsigned int i=0; i<n; ++i) {
    fp->put(static_cast<char>(floor(0.5 + atan(gradEW[i]/gradNS[i])*128./M_PI))); //full circle is 255 degree!!!
    fp->put(static_cast<char>(floor(0.5 + atan(gradEW[i]/gradNS[i+1])*128./M_PI)));
  }
}

int main(int argc, char *argv[]) {
  unsigned int ncol = atoi(argv[1]); //Number of columns
  unsigned int nrow = atoi(argv[2]); //Number of rows
  double lonMin = atof(argv[3]);
  double lonMax = atof(argv[4]);
  double latMin = atof(argv[5]);
  double latMax = atof(argv[6]);
  int NODATAIN = atoi(argv[7]);
  double hnois = atof(argv[8]) * 2.;

  fileio dataFile(argv[9], NODATAIN, ncol); //name of dem input file
  ofstream fpoSlp(argv[10], ios::out | ios::binary); //name of slope output file
  ofstream fpoAsp(argv[11], ios::out | ios::binary); //name of Aspect output file
  ofstream fpoSlpMin(argv[12], ios::out | ios::binary);
  ofstream fpoSlpMax(argv[13], ios::out | ios::binary);
  
  // static const unsigned int ncol = 432000; //Number of columns
  // static const unsigned int nrow = 144000; //Number of rows
  // static const double lonMin = -180.;
  // static const double lonMax = 180.;
  // static const double latMin = -60.;
  // static const double latMax = 60.;
  // static const int NODATAIN = -9999;

  // fileio dataFile("/data/tmp/srtm.bin", NODATAIN, ncol);
  // ofstream fpoSlp("/data/tmp/slope.bin", ios::out | ios::binary);

  int *alti0 = new int[ncol];
  int *alti1 = new int[ncol];
  double *grad0 = new double[ncol-1]; //gradient EW
  double *grad1 = new double[ncol]; //gradient NS
  double *grad0min = new double[ncol-1];
  double *grad1min = new double[ncol];
  double *grad0max = new double[ncol-1];
  double *grad1max = new double[ncol];

  int *palti0;
  int *palti1;
  int *paltix;

  //Horizontal distance Degree
  double distHorDeg = (lonMax - lonMin) / static_cast<double>(ncol) / 180. * M_PI;
  //Vertical distance Degree
  double distVerDeg = (latMax - latMin) / static_cast<double>(nrow) / 180. * M_PI;

  double distHorM; //Horizontal distance Meter
  double distVerM; //Vertical distance Meter

  unsigned int row=0;
  double lat=(latMax - (0.5 + static_cast<double>(row)) * (latMax - latMin) / static_cast<double>(nrow)) / 180. * M_PI;
  distHorM = dist(lat, 0., 0., distHorDeg);
  palti1 = alti1;
  palti0 = alti0;
  dataFile.getRow(alti1);
  gradEW(palti1, grad0, distHorM, ncol, NODATAIN);
  gradEWmin(palti1, grad0min, distHorM, ncol, NODATAIN, hnois);
  gradEWmax(palti1, grad0max, distHorM, ncol, NODATAIN, hnois);
  bool fileOk = true;
  do {
    paltix = palti0;
    palti0 = palti1;
    palti1 = paltix;
    fileOk = dataFile.getRow(palti1);
    distVerM = dist(lat, 0., -distVerDeg, 0.);
    gradNS(palti0, palti1, grad1, distVerM, ncol, NODATAIN);
    gradNSmin(palti0, palti1, grad1min, distVerM, ncol, NODATAIN, hnois);
    gradNSmax(palti0, palti1, grad1max, distVerM, ncol, NODATAIN, hnois);
    slp(grad0, grad1, ncol, &fpoSlp);
    aspect(grad0, grad1, ncol, &fpoAsp);
    slp(grad0min, grad1min, ncol, &fpoSlpMin);
    slp(grad0max, grad1max, ncol, &fpoSlpMax);
    ++row;
    lat=(latMax - (0.5 + static_cast<double>(row)) * (latMax - latMin) / static_cast<double>(nrow)) / 180. * M_PI;
    distHorM = dist(lat, 0., 0., distHorDeg);
    gradEW(palti1, grad0, distHorM, ncol, NODATAIN);
    gradEWmin(palti1, grad0min, distHorM, ncol, NODATAIN, hnois);
    gradEWmax(palti1, grad0max, distHorM, ncol, NODATAIN, hnois);
    slp(grad0, grad1, ncol, &fpoSlp);
    aspect(grad0, grad1, ncol, &fpoAsp);
    slp(grad0min, grad1min, ncol, &fpoSlpMin);
    slp(grad0max, grad1max, ncol, &fpoSlpMax);
  } while(fileOk && row<nrow-1);

  return(0);
}
