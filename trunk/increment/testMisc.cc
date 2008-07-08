#include <iostream>

#include "misc.h"

using namespace std;


int main() {
  //Test one dimensional interpolation
  g4m::ipol<double, double> d;
  d.insert(3, 0.);
  d.insert(5, 10.);
  cout << d.g(4.5) << endl;
  cout << d[5.5] << endl;

  //Test multi dimensional interpolation
  g4m::ipol<vector<double>, double> vd;
  vector<double> idx;
  idx.push_back(2.);
  idx.push_back(5.);
  idx.push_back(3.);
  
  vd.insert(idx, 7.);

  idx[0] = 7; idx[1] = -3; idx[2] = 5;
  vd.insert(idx, 2.2);

  cout << vd.g(idx) << endl;
  idx[0] = 5; idx[1] = -3; idx[2] = 5;
  cout << vd[idx] << endl;
  idx[0] = 2; idx[1] = 5; idx[2] = 3;
  cout << vd.g(idx) << endl;

  vd.clear();
  idx.clear();

  return(0);
}
