#ifndef G4M_MISC_H
#define G4M_MISC_H

#include <map>
#include <vector>
#include <cmath>
#include <limits>

#include <iostream>

namespace g4m {
  //Late binding with virtual class vipol needs some cpu-time!
  template <class IDX, class VAL>
    class vipol { //Virtual interpol class
  public:
    virtual VAL g(const IDX &) = 0;
    virtual VAL operator[](const IDX &) = 0;
  };

  template <class IDX, class VAL>  //Interpolate: IDX .. Index, VAL .. Value
    class ipol : virtual public vipol<IDX, VAL> {
  public:
    void insert(IDX, VAL);
    void inc(IDX, VAL); //MG: added to make it compatible with the previous version
    ipol(const ipol<IDX, VAL> & g);    //MG: added to make it compatible with the previous version
    ipol();  //MG: added to make it compatible with the previous version

    VAL g(const IDX &);   //returns the value
    void clear();
    VAL operator[](const IDX &);
    void operator*=(const double x);
    IDX min();  //returns the minimum index
    IDX max();  //returns the maximum index
    std::map<IDX, VAL> aMap; //MG: changed to make it compatible with the previous version (moved from private)
	private:
//    std::map<IDX, VAL> aMap;
    VAL ip(const IDX &i, const IDX &i0, const IDX &i1, const VAL &v0, const VAL &v1);
  };

//-------- MG: added to make it compatible with the previous version
           
// Class default constructor
  template <class IDX, class VAL>
  ipol<IDX, VAL>::ipol() { }

// Class copy constructor
  template <class IDX, class VAL>
  ipol<IDX, VAL>::ipol(const ipol<IDX, VAL> & g) {
    aMap = g.aMap;
  }    
  
 // template <class IDX, class VAL>
 //   VAL ipol<IDX, VAL>::v(const IDX i = 0) {
 //   typename std::map<IDX,VAL>::iterator lo, up;
 //   VAL y = 0;
 //   if(aMap.size() > 0) {
 //     up = aMap.lower_bound(i);
 //     if(up == aMap.end()) {--up; lo = up;}
 //     else {
	//lo = up;
	//--lo;
	//if(up == aMap.begin()) {lo = aMap.begin();}
 //     }
 //     y = ip(i, lo->first, up->first, lo->second, up->second);
 //   }
 //   return(y);
 // }  

  template <class IDX, class VAL> //MG: added to make it compatible with the previous version
  void ipol<IDX, VAL>::inc(IDX i, VAL v) {
    v += aMap[i];
    aMap.erase(i);
    aMap.insert(make_pair(i, v));
  }  

//---------------------------------

  template <class IDX, class VAL>
  void ipol<IDX, VAL>::clear() {
    aMap.clear();
  }

  template <class IDX, class VAL>
  void ipol<IDX, VAL>::insert(IDX i, VAL v) {
    aMap.insert(std::make_pair(i, v));
  }
  
  template <class IDX, class VAL>
  inline VAL ipol<IDX, VAL>::ip(const IDX &i, const IDX &i0, const IDX &i1, const VAL &v0, const VAL &v1) {
    //interpolate/extrapolate the value for index i
    //if(i0 == i1) {return((v0 + v1)/2.);}
    return(v0 + (i-i0)/(i1-i0) * (v1-v0));
  }

  template <class IDX, class VAL>
    VAL ipol<IDX, VAL>::g(const IDX &i) {
    if(aMap.size() > 0) {
      typename std::map<IDX,VAL>::iterator up;
      up = aMap.lower_bound(i);
      if(up == aMap.end()) {--up; return(up->second);}
      if(up == aMap.begin()) {return(up->second);}
      typename std::map<IDX,VAL>::iterator lo = up;
      --lo;
      return(ip(i, lo->first, up->first, lo->second, up->second));
    }
    return(std::numeric_limits<VAL>::quiet_NaN());
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
    VAL ipol<IDX, VAL>::operator[](const IDX &i) {
    return(g(i));
  }

  //template <class IDX, class VAL>
  //  IDX ipol<IDX, VAL>::min() {
  //  return(aMap.begin()->first);
  //}

  template <class IDX, class VAL>
    IDX ipol<IDX, VAL>::min() {
   if(aMap.size() > 0) {
	   return(aMap.begin()->first);}
   else
       {return(0);}
  }


  //template <class IDX, class VAL>
  //  IDX ipol<IDX, VAL>::max() {
  //  return(aMap.rbegin()->first);
  //}

  template <class IDX, class VAL>
    IDX ipol<IDX, VAL>::max() {
   if(aMap.size() > 0) {
	   return(aMap.rbegin()->first);}
   else {
	   return(0);}
  }

  //Multidimensional interpolation
  template <class IDX, class VAL>
    class ipol<std::vector<IDX>, VAL> :
  virtual public vipol<std::vector<IDX>, VAL>{
  public:
    void insert(std::vector<IDX>, VAL);
    virtual VAL g(const std::vector<IDX> &);   //returns the value
    void clear();
    virtual VAL operator[](const std::vector<IDX> &i);
    void operator*=(const double x);
    std::vector<IDX> min();  //returns the minimum index
    std::vector<IDX> max();  //returns the maximum index
   private:
    std::map<std::vector<IDX>, VAL> aMap; //Data Map
  };

  template <class IDX, class VAL>
    void ipol<std::vector<IDX>, VAL>::operator*=(const double x) {
    typename std::map<std::vector<IDX>,VAL>::iterator iter;
    iter = aMap.begin();
    while(iter != aMap.end() ) {
      iter->second *= x;
      ++iter;
    }
  }

  template <class IDX, class VAL>
  void ipol<std::vector<IDX>, VAL>::clear() {
    aMap.clear();
  }

  template <class IDX, class VAL>
  void ipol<std::vector<IDX>, VAL>::insert(std::vector<IDX> i, VAL v) {
    aMap.insert(make_pair(i, v));
  }

  template <class IDX, class VAL>
    VAL ipol<std::vector<IDX>, VAL>::g(const std::vector<IDX> &i) {
    typename std::map<std::vector<IDX>, VAL>::iterator iter;
//    unsigned int regions = 0.5 + pow(2,i.size()); //MG: to make it compatible with VS
	unsigned int regions = 0.5 + pow(2.,int(i.size()));
    std::vector<VAL>* y = new std::vector<VAL>[regions]; //Values of near points
    double* dist = new double[regions]; //shortest distance in region
    for(unsigned int j=0; j < regions; ++j) {dist[j] = -1.;}
    for(iter = aMap.begin(); iter != aMap.end(); ++iter) {
      double d=0.;  //Distance
      int pos=0; //Memory where to save the minimum distance
      int mul=1; //Multiplier to get right pos
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
    //for(unsigned int j=0; j < regions; ++j) {
    //  if(dist[j] > 0.) {
    //    dist[j] = std::sqrt(dist[j]); //Geometric distance
    //}}
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
    delete[] y;
    delete[] dist;
    if(n > 0) {ip /= n;}
    else if(distsum > 0.) {ip /= distsum;}
    else {ip = std::numeric_limits<double>::quiet_NaN();}
    VAL ret = ip;
    return(ret);
  }

  template <class IDX, class VAL>
    VAL ipol<std::vector<IDX>, VAL>::operator[](const std::vector<IDX> &i) {
    return(g(i));
  }

  //template <class IDX, class VAL>
  //  std::vector<IDX> ipol<std::vector<IDX>, VAL>::max() {
  //  typename std::map<std::vector<IDX>, VAL>::iterator iter = aMap.begin();
  //  std::vector<IDX> ret(iter->first);
  //  ++iter;
  //  while(iter != aMap.end()) {
  //    for(unsigned int i=0; i<ret.size(); ++i) {
  //      if(ret[i] < iter->first[i]) {ret[i] = iter->first[i];}
  //    }
  //    ++iter;
  //  }
  //  return(ret);
  //}
  template <class IDX, class VAL>
    std::vector<IDX> ipol<std::vector<IDX>, VAL>::max() {
    typename std::map<std::vector<IDX>, VAL>::iterator iter = aMap.begin();
	if (aMap.size()>0){					//MG: test
		std::vector<IDX> ret(iter->first);
		++iter;
		while(iter != aMap.end()) {
		 for(unsigned int i=0; i<ret.size(); ++i) {
			if(ret[i] < iter->first[i]) {ret[i] = iter->first[i];}
		}
		++iter;
		}
		return(ret);
	}
	else {
	    std::vector<IDX> ret(1);
		ret[0]=0;
		return(ret);
	}

  }


 // template <class IDX, class VAL>
 //   std::vector<IDX> ipol<std::vector<IDX>, VAL>::min() {
 //   typename std::map<std::vector<IDX>, VAL>::iterator iter = aMap.begin();
 //   std::vector<IDX> ret(iter->first);
 //   ++iter;
 //   while(iter != aMap.end()) {
 //     for(unsigned int i=0; i<ret.size(); ++i) {
	//if(ret[i] > iter->first[i]) {ret[i] = iter->first[i];}
 //     }
 //     ++iter;
 //   }
 //   return(ret);
 // }

  template <class IDX, class VAL>
    std::vector<IDX> ipol<std::vector<IDX>, VAL>::min() {
    typename std::map<std::vector<IDX>, VAL>::iterator iter = aMap.begin();
	if (aMap.size()>0){					//MG: test
		std::vector<IDX> ret(iter->first);
		++iter;
		while(iter != aMap.end()) {
		for(unsigned int i=0; i<ret.size(); ++i) {
			if(ret[i] > iter->first[i]) {ret[i] = iter->first[i];}
		}
		++iter;
		}
	    return(ret);
	}
	else {
	    std::vector<IDX> ret(1);
		ret[0]=0;
		return(ret);
	}
		
  }


  //Fast Interploation where the steps of the index are 1 and starting at 0
  //and the size of the data array is known in advance
  //and complete filled with values
  //Interpolate: IDX .. Index, VAL .. Value
  template <class VAL>
    class fipol : virtual public vipol<double, VAL> {
  public:
    fipol();
    fipol(unsigned int);
    fipol(ipol<double,VAL>, double zoom=1.);
    ~fipol();
    bool insert(unsigned int, VAL);
    VAL g(const double &);   //returns the value with interpolation
    void fill(VAL);
    VAL operator[](const double &);
    void operator*=(const double x);
    void clear(unsigned int);
    unsigned int gs();  //Get size of data array
  private:
    VAL* aMap;
    unsigned int n;  //Size of array
    const double intercept;
    const double zoom;
  };

  template <class VAL>
    unsigned int fipol<VAL>::gs() {
    return(n);
  }

  template <class VAL>
    fipol<VAL>::fipol() : intercept(0.), zoom(0.) {
    n = 1;
    aMap = new VAL[n];
  }

  template <class VAL>
    fipol<VAL>::fipol(unsigned int an) : intercept(0.), zoom(0.) {
    n = an;
    aMap = new VAL[n];
  }

  template <class VAL>
    fipol<VAL>::fipol(ipol<double,VAL> t, double zom) : intercept(zom*t.min()), zoom(zom) {
    n = static_cast<int>(1 + zoom * (std::ceil(t.max()) - std::floor(t.min())));
    aMap = new VAL[n];
    for(unsigned int i=0; i<n; ++i) {
      aMap[i] = t.g((intercept + i)/zoom);
    }
  }

  template <class VAL>
    fipol<VAL>::~fipol() {
    delete[] aMap;
  }

  template <class VAL>
    void fipol<VAL>::clear(unsigned int an) {
    delete[] aMap;
    n = an;
    aMap = new VAL[n];
  }

  template <class VAL>
  void fipol<VAL>::fill(VAL x) {
    for(unsigned int i=0; i<n; ++i) {aMap[i] = x;}
  }

  template <class VAL>
  bool fipol<VAL>::insert(unsigned int i, VAL v) {
    bool ret = false;
    if(i < n) {aMap[i] = v; ret = true;}
    return(ret);
  }

  template <class VAL>
    VAL fipol<VAL>::g(const double &i) {
    if(n > 1) {
      double zi = i*zoom + intercept;
      int i0 = static_cast<int>(zi);
      unsigned int i1 = i0+1;
      if(i0 < 0) {return(aMap[0]);}
      else if(i1 >= n) {return(aMap[n-1]);}
      else {
	return(aMap[i0] + (zi - static_cast<double>(i0)) * (aMap[i1] - aMap[i0]));
      }
    } else if(n == 1) {return(aMap[0]);}
    return(std::numeric_limits<VAL>::quiet_NaN());
  }

  template <class VAL>
    void fipol<VAL>::operator*=(const double x) {
    for(unsigned int i=0; i<n; ++i) {
      aMap[i] *= x;
    }
  }

  template <class VAL>
    VAL fipol<VAL>::operator[](const double &i) {
    return(g(i));
  }

  //Without interpolation
  template <class VAL>
    class ffipol : virtual public vipol<double, VAL> {
  public:
    ffipol();
    ffipol(unsigned int);
    //Zoom .. factor to multipy scale
    //Add .. Add this value to sclae to avoid bias caused by index roundind
    ffipol(ipol<double,VAL>, double zoom=1., double add=0.5);
    ~ffipol();
    bool insert(unsigned int, VAL);
    VAL g(const double &);   //returns the value
    void fill(VAL);
    VAL operator[](const double &);
    void operator*=(const double x);
    void clear(unsigned int);
    unsigned int gs();  //Get size of data array
    bool overwrite(ipol<double,VAL>, double zoom=1., double add=0.5);
  private:
    VAL* aMap;
    unsigned int n;  //Size of array
    //const double intercept;
    //const double zoom;
    double intercept;
    double zoom;
  };

  template <class VAL>
    unsigned int ffipol<VAL>::gs() {
    return(n);
  }

  template <class VAL>
    ffipol<VAL>::ffipol() : intercept(0.), zoom(0.) {
    n = 1;
    aMap = new VAL[n];
  }

  template <class VAL>
    ffipol<VAL>::ffipol(unsigned int an) : intercept(0.), zoom(0.) {
    n = an;
    aMap = new VAL[n];
  }

  template <class VAL>
    ffipol<VAL>::ffipol(ipol<double,VAL> t, double zom, double add) : intercept(zom*t.min() + add), zoom(zom) {
    n = static_cast<int>(1 + zoom * (std::ceil(t.max()) - std::floor(t.min())));
	if (n<2) n=2; //MG test
	aMap = new VAL[n];
    for(unsigned int i=0; i<n; ++i) {
      aMap[i] = t.g((intercept + i)/zoom - add);
    }
  }

  template <class VAL>
    ffipol<VAL>::~ffipol() {
    delete[] aMap;
  }

  template <class VAL>
    void ffipol<VAL>::clear(unsigned int an) {
    delete[] aMap;
    n = an;
    aMap = new VAL[n];
  }

  template <class VAL>
  void ffipol<VAL>::fill(VAL x) {
    for(unsigned int i=0; i<n; ++i) {aMap[i] = x;}
  }

  template <class VAL>
  bool ffipol<VAL>::insert(unsigned int i, VAL v) {
    bool ret = false;
    if(i < n) {aMap[i] = v; ret = true;}
    return(ret);
  }

  template <class VAL>
    VAL ffipol<VAL>::g(const double &i) {
    if(n > 1) {
      int i0 = static_cast<int>(i*zoom + intercept);
      if(i0 <= 0) {return(aMap[0]);}
      else if(i0 >= static_cast<int>(n)) {return(aMap[n-1]);}
      else {
	return(aMap[i0]);
      }
    } else if(n == 1) {return(aMap[0]);}
    return(std::numeric_limits<VAL>::quiet_NaN());
  }

  template <class VAL>
    void ffipol<VAL>::operator*=(const double x) {
    for(unsigned int i=0; i<n; ++i) {
      aMap[i] *= x;
    }
  }

  template <class VAL>
    VAL ffipol<VAL>::operator[](const double &i) {
    return(g(i));
  }

  template <class VAL>
    bool ffipol<VAL>::overwrite(ipol<double,VAL> t, double zom, double add) {
    bool ret = true;
    intercept = zom*t.min() + add;
    zoom = zom;
    n = static_cast<int>(1 + zoom * (std::ceil(t.max()) - std::floor(t.min())));
    delete[] aMap;
    aMap = new VAL[n];
    for(unsigned int i=0; i<n; ++i) {
      aMap[i] = t.g((intercept + i)/zoom - add);
    }
    return(ret);
  }



  //Fast interpolation Multidimmensional (only aproximation is possible!)
  template <class VAL>
    class fipolm {
  public:
    fipolm(std::vector<unsigned int>);
    fipolm(const unsigned int*, const unsigned int dim);
    fipolm(ipol<std::vector<double>, VAL> &);
    fipolm(ipol<std::vector<double>, VAL> &, std::vector<double> &zoom);
    ~fipolm();
    bool insert(unsigned int*, VAL);
    bool insert(std::vector<unsigned int> &, VAL);
    VAL g(double*);
    VAL g(std::vector<double>);
    void fill(VAL);
    VAL operator[](double*);
    VAL operator[](std::vector<double> &);
    void operator*=(const double x);
    void clear(unsigned int*, unsigned int dim);
    void clear(std::vector<unsigned int> &);
    std::vector<unsigned int> gs();  //Get size of array
  private:
    VAL* aMap;
    unsigned int* n;  //Size of array
    unsigned int dim;  //How many dimmensions does the index have
    double* intercept; //If data range does not start from 0
    double* zoom;
    int fill(ipol<std::vector<double>, VAL> &, unsigned int idx, unsigned int adim, unsigned int mul, std::vector<double> key);
    unsigned int sur; //n Surrounding points
    unsigned int* idx; //Index for surrounding points
    double* dist;  //Distance of surrounding points
  };

  template <class VAL>
    fipolm<VAL>::fipolm(const unsigned int* an, const unsigned int adim) {
    dim = adim;
    intercept = new double[dim];
    zoom = new double[dim];
    n = new unsigned int[dim];
    unsigned int slots = n[0] = an[0];
    for(unsigned int i=1; i<dim; ++i) {n[i] = an[i]; slots *= n[i];
      intercept[i]=0.; zoom[i] = 1.;}
    aMap = new VAL[slots];
    sur = std::ceil(std::pow(2,dim));
    idx = new unsigned int[sur];
    dist = new double[sur];
  }

  template <class VAL>
    fipolm<VAL>::fipolm(std::vector<unsigned int> an) : dim(an.size()) {
    intercept = new double[dim];
    zoom = new double[dim];
    n = new unsigned int[dim];
    unsigned int slots = n[0] = an[0];
    for(unsigned int i=1; i<dim; ++i) {n[i] = an[i]; slots *= n[i];
      intercept[i]=0.; zoom[i] = 1.;}
    aMap = new VAL[slots];
    sur = std::ceil(std::pow(2,dim));
    idx = new unsigned int[sur];
    dist = new double[sur];
  }

  template <class VAL>
    fipolm<VAL>::fipolm(ipol<std::vector<double>, VAL> &t) {
    std::vector<double> idxMin(t.min());
    std::vector<double> idxMax(t.max());
    dim = idxMin.size();
    intercept = new double[dim];
    zoom = new double[dim];
    n = new unsigned int[dim];
    zoom[0] = 1.;
    intercept[0]=zoom[0] * idxMin[0]; 
    unsigned int slots = n[0] = static_cast<int>(1 + zoom[0] * (std::ceil(idxMax[0]) - std::floor(idxMin[0])));
    for(unsigned int i=1; i<dim; ++i) {
      zoom[i] = 1.;
      n[i] = static_cast<int>(1 + zoom[i] * (std::ceil(idxMax[i]) - std::floor(idxMin[i])));
      slots *= n[i];
      intercept[i]=zoom[i] * idxMin[i]; 
    }
    aMap = new VAL[slots];
    std::vector<double> key;
    fill(t, 0, 0, 1, key);
    sur = std::ceil(std::pow(2,dim));
    idx = new unsigned int[sur];
    dist = new double[sur];
  }

  template <class VAL>
    fipolm<VAL>::fipolm(ipol<std::vector<double>, VAL> &t, std::vector<double> &azoom) {
     std::vector<double> idxMin(t.min());
    std::vector<double> idxMax(t.max());
    dim = idxMin.size();
    intercept = new double[dim];
    zoom = new double[dim];
    n = new unsigned int[dim];
    zoom[0] = azoom[0];
    intercept[0]=zoom[0] * idxMin[0]; 
    unsigned int slots = n[0] = static_cast<int>(1 + zoom[0] * (std::ceil(idxMax[0]) - std::floor(idxMin[0])));
    for(unsigned int i=1; i<dim; ++i) {
      zoom[i] = azoom[i];
      n[i] = static_cast<int>(1 + zoom[i] * (std::ceil(idxMax[i]) - std::floor(idxMin[i])));
      slots *= n[i];
      intercept[i]=zoom[i] * idxMin[i]; 
    }
    aMap = new VAL[slots];
    std::vector<double> key;
    fill(t, 0, 0, 1, key);
    sur = std::ceil(std::pow(2,dim));
    idx = new unsigned int[sur];
    dist = new double[sur];
  }

  template <class VAL>
    int fipolm<VAL>::fill(ipol<std::vector<double>, VAL> &t, unsigned int idx, unsigned int adim, unsigned int mul, std::vector<double> key) {
    key.push_back(0.);
    for(unsigned int i=0; i<n[adim]; ++i) {
      key[adim] = (intercept[adim] + i)/zoom[adim];
      if(adim+1 < dim) {fill(t, idx+i*mul, adim+1, mul*n[adim], key);}
      else {aMap[idx+i*mul] = t.g(key);}
    }
    return(0);
  }
 
  template <class VAL>
    fipolm<VAL>::~fipolm() {
    delete[] n;
    delete[] aMap;
    delete[] intercept;
    delete[] zoom;
    delete[] idx;
    delete[] dist;
  }

  template <class VAL>
  bool fipolm<VAL>::insert(unsigned int* i, VAL v) {
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
    bool fipolm<VAL>::insert(std::vector<unsigned int> &i, VAL v) {
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
    VAL fipolm<VAL>::g(double* i) {
    //Test if index is in the possible range
    for(unsigned j = 0; j < dim; ++j) {
      i[j] = i[j] * zoom[j] - intercept[j];
      if(i[j] >= n[j]) {i[j] = n[j]-1;}
      if(i[j] < 0) {i[j] = 0;}
    }
    for(unsigned j = 0; j < sur; ++j) {idx[j] = -1;}
    idx[0] = std::floor(i[0]);
    idx[1] = std::ceil(i[0]);
    dist[0] = std::fabs(i[0] - idx[0]); //Manhaten distance
    dist[1] = std::fabs(i[0] - idx[1]);
    unsigned int mul = n[0];  //Array size in dimmension 0
    for(unsigned j = 1; j < dim; ++j) {
      unsigned int t = std::ceil(std::pow(2,j));  //n/2 points used in this dim
      unsigned int uc = std::ceil(i[j]) * mul; //Index where of grid point
      unsigned int uf = std::floor(i[j]) * mul;
      double dc = std::fabs(i[j] - std::ceil(i[j])); //Manhaten distance
      double df = std::fabs(i[j] - std::floor(i[j]));
      for(unsigned int k=0; k<t; ++k) {
	idx[k+t] = idx[k] + uc;
	idx[k] += uf;
	dist[k+t] = dist[k] + dc;
	dist[k] += df;
      }
      mul *= n[j];
    }
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
    if(sdist > 0.) {return(sval / sdist);}
    return(std::numeric_limits<VAL>::quiet_NaN());
  }

  template <class VAL>
    VAL fipolm<VAL>::g(std::vector<double> i) {
    //Test if index is in the possible range
    if(i.size() != dim) {return(std::numeric_limits<VAL>::quiet_NaN());}
    for(unsigned j = 0; j < dim; ++j) {
      i[j] = i[j] * zoom[j] - intercept[j];
      if(i[j] >= n[j]) {i[j] = n[j]-1;}
      if(i[j] < 0) {i[j] = 0;}
    }
    for(unsigned j = 0; j < sur; ++j) {idx[j] = -1;}
    idx[0] = std::floor(i[0]);
    idx[1] = std::ceil(i[0]);
    dist[0] = std::fabs(i[0] - idx[0]); //Manhaten distance
    dist[1] = std::fabs(i[0] - idx[1]);
    unsigned int mul = n[0];  //Array size in dimmension 0
    for(unsigned j = 1; j < dim; ++j) {
      unsigned int t = std::ceil(std::pow(2,j));  //n/2 points used in this dim
      unsigned int uc = std::ceil(i[j]) * mul; //Index where of grid point
      unsigned int uf = std::floor(i[j]) * mul;
      double dc = std::fabs(i[j] - std::ceil(i[j])); //Manhaten distance
      double df = std::fabs(i[j] - std::floor(i[j]));
      for(unsigned int k=0; k<t; ++k) {
	idx[k+t] = idx[k] + uc;
	idx[k] += uf;
	dist[k+t] = dist[k] + dc;
	dist[k] += df;
      }
      mul *= n[j];
    }
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
    if(sdist > 0.) {return(sval / sdist);}
    return(std::numeric_limits<VAL>::quiet_NaN());
  }

  template <class VAL>
  void fipolm<VAL>::fill(VAL x) {
    unsigned int slots = n[0];
    for(unsigned int i=1; i<dim; ++i) {slots *= n[i];}
    for(unsigned int i=0; i<slots; ++i) {aMap[i] = x;}
  }

  template <class VAL>
    VAL fipolm<VAL>::operator[](double *i) {
    return(g(i));
  }

  template <class VAL>
    VAL fipolm<VAL>::operator[](std::vector<double> &i) {
    return(g(i));
  }

  template <class VAL>
    void fipolm<VAL>::operator*=(const double x) {
    unsigned int slots = n[0];
    for(unsigned int i=1; i<dim; ++i) {slots *= n[i];}
    for(unsigned int i=0; i<slots; ++i) {aMap[i] *= x;}
  }

  template <class VAL>
    void fipolm<VAL>::clear(unsigned int* an, unsigned int adim) {
    dim = adim;
    delete[] n;
    delete[] aMap;
    n = new unsigned int[dim];
    unsigned int slots = n[0] = an[0];
    for(unsigned int i=1; i<dim; ++i) {n[i] = an[i]; slots *= n[i];}
    aMap = new VAL[slots];
  }

  template <class VAL>
    void fipolm<VAL>::clear(std::vector<unsigned int> &an) {
    dim = an.size();
    delete[] n;
    delete[] aMap;
    n = new unsigned int[dim];
    unsigned int slots = n[0] = an[0];
    for(unsigned int i=1; i<dim; ++i) {n[i] = an[i]; slots *= n[i];}
    aMap = new VAL[slots];
  }

  template <class VAL>
    std::vector<unsigned int> fipolm<VAL>::gs() {
    std::vector<unsigned int> ret;
    for(unsigned int i=0; i<dim; ++i) {ret.push_back(n[i]);}
    return(ret);
  }


  //Without interpolation
  template <class VAL>
    class ffipolm {
  public:
    ffipolm(std::vector<unsigned int>);
    ffipolm(const unsigned int*, const unsigned int dim);
    ffipolm(ipol<std::vector<double>, VAL> &, double add=0.5);
    ffipolm(ipol<std::vector<double>, VAL> &, std::vector<double> &zoom, double add=0.5);
    ~ffipolm();
    bool insert(unsigned int*, VAL);
    bool insert(std::vector<unsigned int> &, VAL);
    VAL g(double *);
    VAL g(std::vector<double> &);
    void fill(VAL);
    VAL operator[](double *);
    VAL operator[](std::vector<double> &);
    void operator*=(const double x);
    void clear(unsigned int*, unsigned int dim);
    void clear(std::vector<unsigned int> &);
    std::vector<unsigned int> gs();
  private:
    VAL* aMap;
    unsigned int* n;  //Size of array
    unsigned int dim;  //How many dimmensions does the index have
    double* intercept; //If data range does not start from 0
    double* zoom;
    int fill(ipol<std::vector<double>, VAL> &, unsigned int idx, unsigned int adim, unsigned int mul, std::vector<double> key, double add);
  };

  template <class VAL>
    ffipolm<VAL>::ffipolm(const unsigned int* an, const unsigned int adim) {
    dim = adim;
    intercept = new double[dim];
    zoom = new double[dim];
    n = new unsigned int[dim];
    unsigned int slots = n[0] = an[0];
    for(unsigned int i=1; i<dim; ++i) {n[i] = an[i]; slots *= n[i];
      intercept[i]=0.; zoom[i] = 1.;}
    aMap = new VAL[slots];
  }

  template <class VAL>
    ffipolm<VAL>::ffipolm(std::vector<unsigned int> an) : dim(an.size()) {
    intercept = new double[dim];
    zoom = new double[dim];
    n = new unsigned int[dim];
    unsigned int slots = n[0] = an[0];
    for(unsigned int i=1; i<dim; ++i) {n[i] = an[i]; slots *= n[i];
      intercept[i]=0.; zoom[i] = 1.;}
    aMap = new VAL[slots];
  }

  template <class VAL>
    ffipolm<VAL>::ffipolm(ipol<std::vector<double>, VAL> &t, double add) {
    std::vector<double> idxMin(t.min());
    std::vector<double> idxMax(t.max());
    dim = idxMin.size();
    intercept = new double[dim];
    zoom = new double[dim];
    n = new unsigned int[dim];
    zoom[0] = 1.;
    intercept[0]=zoom[0] * idxMin[0]; 
    unsigned int slots = n[0] = static_cast<int>(1 + zoom[0] * (std::ceil(idxMax[0]) - std::floor(idxMin[0])));
    for(unsigned int i=1; i<dim; ++i) {
      zoom[i] = 1.;
      n[i] = static_cast<int>(1 + zoom[i] * (std::ceil(idxMax[i]) - std::floor(idxMin[i])));
      slots *= n[i];
      intercept[i]=zoom[i] * idxMin[i]; 
    }
    aMap = new VAL[slots];
    std::vector<double> key;
    fill(t, 0, 0, 1, key, add);
  }

  template <class VAL>
    ffipolm<VAL>::ffipolm(ipol<std::vector<double>, VAL> &t, std::vector<double> &azoom, double add) {
     std::vector<double> idxMin(t.min());
    std::vector<double> idxMax(t.max());
    dim = idxMin.size();
    intercept = new double[dim];
    zoom = new double[dim];
    n = new unsigned int[dim];
    zoom[0] = azoom[0];
    intercept[0]=zoom[0] * idxMin[0]; 
    unsigned int slots = n[0] = static_cast<int>(1 + zoom[0] * (std::ceil(idxMax[0]) - std::floor(idxMin[0])));
    for(unsigned int i=1; i<dim; ++i) {
      zoom[i] = azoom[i];
      n[i] = static_cast<int>(1 + zoom[i] * (std::ceil(idxMax[i]) - std::floor(idxMin[i])));
      slots *= n[i];
      intercept[i]=zoom[i] * idxMin[i]; 
    }
    aMap = new VAL[slots];
    std::vector<double> key;
    fill(t, 0, 0, 1, key, add);
  }

  template <class VAL>
    int ffipolm<VAL>::fill(ipol<std::vector<double>, VAL> &t, unsigned int idx, unsigned int adim, unsigned int mul, std::vector<double> key, double add) {
    key.push_back(0.);
    for(unsigned int i=0; i<n[adim]; ++i) {
      key[adim] = (intercept[adim] + i)/zoom[adim] + add;
      if(adim+1 < dim) {fill(t, idx+i*mul, adim+1, mul*n[adim], key, add);}
      else {aMap[idx+i*mul] = t.g(key);}
    }
    return(0);
  }
 
  template <class VAL>
    ffipolm<VAL>::~ffipolm() {
    delete[] n;
    delete[] aMap;
    delete[] intercept;
    delete[] zoom;
  }

  template <class VAL>
  bool ffipolm<VAL>::insert(unsigned int* i, VAL v) {
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
    bool ffipolm<VAL>::insert(std::vector<unsigned int> &i, VAL v) {
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
    VAL ffipolm<VAL>::g(std::vector<double> &i) {
    //Test if index is in the possible range
    if(i.size() != dim) {return(std::numeric_limits<VAL>::quiet_NaN());}
    unsigned int idx = 0;
    unsigned int mul = 1;
    for(unsigned j = 0; j < dim; ++j) {
      int k = static_cast<int>(i[j] * zoom[j] + intercept[j]);
      if(k < 0) {k = 0;}
      if(static_cast<unsigned int>(k) >= n[j]) {k = n[j]-1;}
      idx += k * mul;
      mul *= n[j];
    }
    return(aMap[idx]);
  }

  template <class VAL>
    VAL ffipolm<VAL>::g(double *i) {
    //Test if index is in the possible range
    unsigned int idx = 0;
    unsigned int mul = 1;
    for(unsigned j = 0; j < dim; ++j) {
      int k = static_cast<int>(i[j] * zoom[j] + intercept[j]);
      if(k < 0) {k = 0;}
      if(static_cast<unsigned int>(k) >= n[j]) {k = n[j]-1;}
      idx += k * mul;
      mul *= n[j];
    }
    return(aMap[idx]);
  }

  template <class VAL>
  void ffipolm<VAL>::fill(VAL x) {
    unsigned int slots = n[0];
    for(unsigned int i=1; i<dim; ++i) {slots *= n[i];}
    for(unsigned int i=0; i<slots; ++i) {aMap[i] = x;}
  }

  template <class VAL>
    VAL ffipolm<VAL>::operator[](double *i) {
    return(g(i));
  }

  template <class VAL>
    VAL ffipolm<VAL>::operator[](std::vector<double> &i) {
    return(g(i));
  }

  template <class VAL>
    void ffipolm<VAL>::operator*=(const double x) {
    unsigned int slots = n[0];
    for(unsigned int i=1; i<dim; ++i) {slots *= n[i];}
    for(unsigned int i=0; i<slots; ++i) {aMap[i] *= x;}
  }

  template <class VAL>
    void ffipolm<VAL>::clear(unsigned int* an, unsigned int adim) {
    dim = adim;
    delete[] n;
    delete[] aMap;
    n = new unsigned int[dim];
    unsigned int slots = n[0] = an[0];
    for(unsigned int i=1; i<dim; ++i) {n[i] = an[i]; slots *= n[i];}
    aMap = new VAL[slots];
  }

  template <class VAL>
    void ffipolm<VAL>::clear(std::vector<unsigned int> &an) {
    dim = an.size();
    delete[] n;
    delete[] aMap;
    n = new unsigned int[dim];
    unsigned int slots = n[0] = an[0];
    for(unsigned int i=1; i<dim; ++i) {n[i] = an[i]; slots *= n[i];}
    aMap = new VAL[slots];
  }

  template <class VAL>
    std::vector<unsigned int> ffipolm<VAL>::gs() {
    std::vector<unsigned int> ret;
    for(unsigned int i=0; i<dim; ++i) {ret.push_back(n[i]);}
    return(ret);
  }

}

#endif
