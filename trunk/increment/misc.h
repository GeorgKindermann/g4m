#ifndef G4M_MISC_H
#define G4M_MISC_H

#include <map>

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

}

#endif
