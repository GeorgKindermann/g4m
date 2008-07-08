#ifndef G4M_MISC_H
#define G4M_MISC_H

#include <map>
#include <vector>

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
	d += tmp * tmp;
	mul *= 2;
      }
      if(d <= dist[pos] || dist[pos]<0.) {
	if(d < dist[pos] || dist[pos]<0.) {y[pos].clear(); dist[pos] = d;}
	y[pos].push_back(iter->second); 
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


  //Fast Interploation where the steps of the index are 1
  //and the size of the data array is known in advance
  //and complete filled with values

}

#endif
