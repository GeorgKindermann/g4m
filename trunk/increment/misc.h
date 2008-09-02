#ifndef G4M_MISC_H
#define G4M_MISC_H

#include <map>
#include <vector>
#include <cmath>

#include <iostream>

namespace g4m {

  template <class IDX, class VAL>
    class ipol {  //Interpolate: IDX .. Index, VAL .. Value
  public:
    void insert(IDX, VAL);
    VAL g(const IDX);   //returns the value
    void clear();
    VAL operator[](const IDX);
    void operator*=(const double x);
  private:
    std::map<IDX, VAL> aMap;
    VAL ip(const IDX i, const IDX i0, const IDX i1, const VAL v0, const VAL v1);
  };

  template <class IDX, class VAL>
  void ipol<IDX, VAL>::clear() {
    aMap.clear();
  }

  template <class IDX, class VAL>
  void ipol<IDX, VAL>::insert(IDX i, VAL v) {
    aMap.insert(make_pair(i, v));
  }
  
  template <class IDX, class VAL>
  VAL ipol<IDX, VAL>::ip(const IDX i, const IDX i0, const IDX i1, const VAL v0, const VAL v1) {
    //interpolate/extrapolate the value for index i
    VAL y;
    if(i0 == i1) {y = (v0 + v1)/2.;}
    else {
      y = v0 + (i-i0)/(i1-i0) * (v1-v0);
    }
    return(y);
  }

  template <class IDX, class VAL>
    VAL ipol<IDX, VAL>::g(const IDX i) {
    typename std::map<IDX,VAL>::iterator lo, up;
    VAL y = 0;
    if(aMap.size() > 0) {
      up = aMap.lower_bound(i);
      if(up == aMap.end()) {--up; lo = up;}
      else {
	lo = up;
	--lo;
	if(up == aMap.begin()) {lo = aMap.begin();}
      }
      y = ip(i, lo->first, up->first, lo->second, up->second);
    }
    return(y);
  }

  template <class IDX, class VAL>
    void ipol<IDX, VAL>::operator*=(const double x) {
    typename std::map<IDX,VAL>::iterator iter;
    iter = aMap.begin();
    while(iter != aMap.end() ) {
      iter->second *= x;
      ++iter;
    }
  }

  template <class IDX, class VAL>
    VAL ipol<IDX, VAL>::operator[](const IDX i) {
    return(g(i));
  }


  //Multidimensional interpolation
  template <class IDX, class VAL>
    class ipol<std::vector<IDX>, VAL> {
  public:
    void insert(std::vector<IDX>, VAL);
    VAL g(const std::vector<IDX>);   //returns the value
    void clear();
    VAL operator[](const std::vector<IDX> i);
   private:
    std::map<std::vector<IDX>, VAL> aMap; //Data Map
  };

  template <class IDX, class VAL>
  void ipol<std::vector<IDX>, VAL>::clear() {
    aMap.clear();
  }

  template <class IDX, class VAL>
  void ipol<std::vector<IDX>, VAL>::insert(std::vector<IDX> i, VAL v) {
    aMap.insert(make_pair(i, v));
  }

  template <class IDX, class VAL>
    VAL ipol<std::vector<IDX>, VAL>::g(const std::vector<IDX> i) {
    typename std::map<std::vector<IDX>, VAL>::iterator iter;
    unsigned int regions = 1;
    for(unsigned int j=0; j<i.size(); ++j) {regions *= 2;}
    std::vector<VAL>* y = new std::vector<VAL>[regions];
    double* dist = new double[regions]; //shortest distance in region
    for(unsigned int j=0; j < regions; ++j) {dist[j] = -1.;}
    for(iter = aMap.begin(); iter != aMap.end(); ++iter) {
      double d=0.;
      int pos=0;
      int mul=1;
      for(unsigned int j=0; j < iter->first.size() && j < i.size(); ++j) {
	double tmp = iter->first[j] - i[j];
	if(tmp > 0.) {pos += mul;}
	//d += tmp * tmp; //Geometric interpolation
	d += std::fabs(tmp);  //Manhaten distance
	mul *= 2;
      }
      if(d <= dist[pos] || dist[pos]<0.) {
	if(d < dist[pos] || dist[pos]<0.) {y[pos].clear(); dist[pos] = d;}
	y[pos].push_back(iter->second); 
      }
    }
    for(unsigned int j=0; j < regions; ++j) {
      if(dist[j] > 0.) {
	//dist[j] = std::sqrt(dist[j]); //Geometric
	dist[j] = dist[j]; //Manhaten distance
      }
    }
    double ip = 0.;
    double distsum = 0.;
    int n = 0;
    for(unsigned int j=0; j < regions; ++j) {
      if(dist[j] > 0. && n==0) {
	for(unsigned int k=0; k<y[j].size(); ++k) {
	  ip += y[j][k] * 1./dist[j];
	  distsum += 1./dist[j];
	}
      }
      else if(dist[j] == 0.) {
	if(n==0) {ip = 0.;}
	for(unsigned int k=0; k<y[j].size(); ++k) {
	  ip += y[j][k];
	  ++n;
	}
      }
    }
    if(n > 0) {ip /= n;}
    else if(distsum > 0.) {ip /= distsum;}
    else {ip = 0.;}
    VAL ret = ip;
    delete[] y;
    delete[] dist;
    return(ret);
  }

  template <class IDX, class VAL>
    VAL ipol<std::vector<IDX>, VAL>::operator[](const std::vector<IDX> i) {
    return(g(i));
  }


  //Fast Interploation where the steps of the index are 1 and starting at 0
  //and the size of the data array is known in advance
  //and complete filled with values
  //Interpolate: IDX .. Index, VAL .. Value
  template <class VAL>
    class fipol {
  public:
    bool insert(unsigned int, VAL);
    bool insert(unsigned int*, VAL);
    bool insert(std::vector<unsigned int>, VAL);
    VAL g(const double);   //returns the value
    VAL g(double*);
    VAL g(std::vector<double>);
    void fill(VAL);
    VAL operator[](const double);
    void operator*=(const double x);
    fipol();
    fipol(unsigned int);
    fipol(std::vector<unsigned int>);
    fipol(const unsigned int*, const unsigned int dim);
    ~fipol();
    void clear(unsigned int);
    void clear(unsigned int*, unsigned int dim);
    void clear(std::vector<unsigned int>);
    unsigned int gs();
  private:
    VAL* aMap;
    unsigned int* n;
    unsigned int dim;
    VAL ip(const double i, const double i0, const double i1, const double v0, const double v1);
  };

  template <class VAL>
    unsigned int fipol<VAL>::gs() {
    return(n[0]);
  }

  template <class VAL>
    fipol<VAL>::fipol() : dim(1) {
    n = new unsigned int[1];
    n[0] = 1;
    aMap = new VAL[n[0]];
  }

  template <class VAL>
    fipol<VAL>::fipol(unsigned int an) : dim(1) {
    n = new unsigned int[dim];
    n[0] = an;
    aMap = new VAL[n[0]];
  }

  template <class VAL>
    fipol<VAL>::fipol(const unsigned int* an, const unsigned int adim) {
    dim = adim;
    n = new unsigned int[dim];
    unsigned int slots = n[0] = an[0];
    for(unsigned int i=1; i<dim; ++i) {n[i] = an[i]; slots *= n[i];}
    aMap = new VAL[slots];
  }

  template <class VAL>
    fipol<VAL>::fipol(std::vector<unsigned int> an) : dim(an.size()) {
    n = new unsigned int[dim];
    unsigned int slots = n[0] = an[0];
    for(unsigned int i=1; i<dim; ++i) {n[i] = an[i]; slots *= n[i];}
    aMap = new VAL[slots];
  }

  template <class VAL>
    fipol<VAL>::~fipol() {
    delete[] n;
    delete[] aMap;
  }

  template <class VAL>
    void fipol<VAL>::clear(unsigned int an) {
    dim = 1;
    delete[] n;
    delete[] aMap;
    n = new unsigned int[dim];
    n[0] = an;
    aMap = new VAL[n[0]];
  }

  template <class VAL>
    void fipol<VAL>::clear(unsigned int* an, unsigned int adim) {
    dim = adim;
    delete[] n;
    delete[] aMap;
    n = new unsigned int[dim];
    unsigned int slots = n[0] = an[0];
    for(unsigned int i=1; i<dim; ++i) {n[i] = an[i]; slots *= n[i];}
    aMap = new VAL[slots];
  }

  template <class VAL>
    void fipol<VAL>::clear(std::vector<unsigned int> an) {
    dim = an.size();
    delete[] n;
    delete[] aMap;
    n = new unsigned int[dim];
    unsigned int slots = n[0] = an[0];
    for(unsigned int i=1; i<dim; ++i) {n[i] = an[i]; slots *= n[i];}
    aMap = new VAL[slots];
  }

  template <class VAL>
  void fipol<VAL>::fill(VAL x) {
    unsigned int slots = n[0];
    for(unsigned int i=1; i<dim; ++i) {slots *= n[i];}
    for(unsigned int i=0; i<slots; ++i) {aMap[i] = x;}
  }

  template <class VAL>
  bool fipol<VAL>::insert(unsigned int i, VAL v) {
    bool ret = false;
    if(i < n[0]) {aMap[i] = v; ret = true;}
    return(ret);
  }

  template <class VAL>
  bool fipol<VAL>::insert(unsigned int* i, VAL v) {
    bool ret = true;
    unsigned int idx = i[0];
    unsigned int mul = n[0];
    for(unsigned int j=1; j<dim; ++j) {
      if(i[j] >= n[j]) {ret = false;}
      idx += i[j] * mul;
      mul *= n[j];
    }
    if(ret == true) {aMap[idx] = v;}
    return(ret);
  }

  template <class VAL>
    bool fipol<VAL>::insert(std::vector<unsigned int> i, VAL v) {
    bool ret = true;
    unsigned int idx = i[0];
    unsigned int mul = n[0];
    for(unsigned int j=1; j<dim && j<i.size(); ++j) {
      if(i[j] >= n[j]) {ret = false;}
      idx += i[j] * mul;
      mul *= n[j];
    }
    if(ret == true) {aMap[idx] = v;}
    return(ret);
  }
  
  template <class VAL>
  VAL fipol<VAL>::ip(const double i, const double i0, const double i1, const double v0, const double v1) {
    //interpolate/extrapolate the value for index i
    VAL y;
    if(i0 == i1) {y = (v0 + v1)/2.;}
    else {
      y = v0 + (i-i0)/(i1-i0) * (v1-v0);
    }
    return(y);
  }

  template <class VAL>
    VAL fipol<VAL>::g(const double i) {
    VAL y;
    if(i >= n[0]) {y = aMap[n[0]-1];}
    else if(i <= 0) {y = aMap[0];}
    else {
      unsigned int i0 = std::floor(i);
      unsigned int i1 = std::ceil(i);
      y = ip(i, i0, i1, aMap[i0], aMap[i1]);
    }
    return(y);
  }

  template <class VAL>
    VAL fipol<VAL>::g(double* i) {
    //Test if index is in the possible range
    for(unsigned j = 1; j < dim; ++j) {
      if(i[j] >= n[j]) {i[j] = n[j]-1;}
      if(i[j] < 0) {i[j] = 0;}
    }
    unsigned int sur = std::ceil(std::pow(2,dim));
    unsigned int* idx = new unsigned int[sur];
    double* dist = new double[sur];
    for(unsigned j = 0; j < sur; ++j) {
      idx[j] = -1; dist[j] = -1;
    }
    idx[0] = std::floor(i[0]);
    idx[1] = std::ceil(i[0]);
    //dist[0] = std::pow(i[0] - std::floor(i[0]), 2); //Geometric
    //dist[1] = std::pow(i[0] - std::ceil(i[0]), 2);  //Geometric
    dist[0] = std::fabs(i[0] - std::floor(i[0])); //Manhaten distance
    dist[1] = 1. - dist[0];                       //Manhaten distance
    unsigned int mul = n[0];
    for(unsigned j = 1; j < dim; ++j) {
      unsigned int t = std::ceil(std::pow(2,j));
      unsigned int uc = std::ceil(i[j]) * mul;
      unsigned int uf = std::floor(i[j]) * mul;
      //double dc = std::pow(i[j] - std::ceil(i[j]), 2);  //Geometric
      //double df = std::pow(i[j] - std::floor(i[j]), 2); //Geometric
      double dc = std::fabs(i[j] - std::ceil(i[j]));  //Manhaten distance
      double df = 1. - dc;                            //Manhaten distance
      for(unsigned int k=0; k<t; ++k) {
	idx[k+t] = idx[k] + uc;
	idx[k] += uf;
	dist[k+t] = dist[k] + dc;
	dist[k] += df;
      }
      mul *= n[j];
    }
    //for(unsigned j = 0; j < sur; ++j) { //Geometric
    //  if(dist[j] > 0.) {dist[j] = std::sqrt(dist[j]);}
    //}
    double sdist = 0.;
    double sval = 0.;
    for(unsigned j=0; j<sur; ++j) {
      if(idx[j] >= 0) {
	if(dist[j] > 0.) {
	  sval += aMap[idx[j]] / dist[j];
	  sdist += 1./dist[j];
	} else {
	  sval = aMap[idx[j]];
	  sdist = 1.;
	  break;
	}
      }
    }
    VAL ret = 0.;
    if(sdist > 0.) {ret = sval / sdist;}
    delete[] idx;
    delete[] dist;
    return(ret);
  }

  template <class VAL>
    VAL fipol<VAL>::g(std::vector<double> i) {
    //Test if index is in the possible range
    for(unsigned j = 1; j < dim && j < i.size(); ++j) {
      if(i[j] >= n[j]) {i[j] = n[j]-1;}
      if(i[j] < 0) {i[j] = 0;}
    }
    unsigned int sur = std::ceil(std::pow(2,dim));
    unsigned int* idx = new unsigned int[sur];
    double* dist = new double[sur];
    for(unsigned j = 0; j < sur; ++j) {
      idx[j] = -1; dist[j] = -1;
    }
    idx[0] = std::floor(i[0]);
    idx[1] = std::ceil(i[0]);
    //dist[0] = std::pow(i[0] - std::floor(i[0]), 2); //Geometric
    //dist[1] = std::pow(i[0] - std::ceil(i[0]), 2);  //Geometric
    dist[0] = std::fabs(i[0] - std::floor(i[0])); //Manhaten distance
    dist[1] = 1. - dist[0];                       //Manhaten distance
    unsigned int mul = n[0];
    for(unsigned j = 1; j < dim && j < i.size(); ++j) {
      unsigned int t = std::ceil(std::pow(2,j));
      unsigned int uc = std::ceil(i[j]) * mul;
      unsigned int uf = std::floor(i[j]) * mul;
      //double dc = std::pow(i[j] - std::ceil(i[j]), 2);  //Geometric
      //double df = std::pow(i[j] - std::floor(i[j]), 2); //Geometric
      double dc = std::fabs(i[j] - std::ceil(i[j]));  //Manhaten distance
      double df = 1. - dc;                            //Manhaten distance
      for(unsigned int k=0; k<t; ++k) {
	idx[k+t] = idx[k] + uc;
	idx[k] += uf;
	dist[k+t] = dist[k] + dc;
	dist[k] += df;
      }
      mul *= n[j];
    }
    //for(unsigned j = 0; j < sur; ++j) { //Geometric
    //  if(dist[j] > 0.) {dist[j] = std::sqrt(dist[j]);}
    //}
    double sdist = 0.;
    double sval = 0.;
    for(unsigned j=0; j<sur; ++j) {
      if(idx[j] >= 0) {
	if(dist[j] > 0.) {
	  sval += aMap[idx[j]] / dist[j];
	  sdist += 1./dist[j];
	} else {
	  sval = aMap[idx[j]];
	  sdist = 1.;
	  break;
	}
      }
    }
    VAL ret = 0.;
    if(sdist > 0.) {ret = sval / sdist;}
    delete[] idx;
    delete[] dist;
    return(ret);
  }

  /*
  template <class IDX, class VAL>
    void ipol<IDX, VAL>::operator*=(const double x) {
    typename std::map<IDX,VAL>::iterator iter;
    iter = aMap.begin();
    while(iter != aMap.end() ) {
      iter->second *= x;
      ++iter;
    }
  }

  template <class IDX, class VAL>
    VAL ipol<IDX, VAL>::operator[](const IDX i) {
    return(g(i));
  }
  */
}

#endif
