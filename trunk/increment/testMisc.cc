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
  idx.push_back(0.);
  idx.push_back(0.);
  idx.push_back(0.);
  
  vd.insert(idx, 10.);

  idx[0] = 10; idx[1] = 5; idx[2] = 0;
  vd.insert(idx, 20);

  cout << vd.g(idx) << endl;
  idx[0] = 5; idx[1] = 2.5; idx[2] = 0;
  cout << vd[idx] << endl;
  idx[0] = 7.5; idx[1] = 3.75; idx[2] = 0;
  cout << vd[idx] << endl;
  idx[0] = 0; idx[1] = 0; idx[2] = 0;
  cout << vd.g(idx) << endl;

  vd.clear();
  idx.clear();

  //Test fast intepolation - one dimesion
  g4m::fipol<double> fip(10);
  fip.fill(10.);
  fip.insert(4, 15.5);
  fip.insert(5, 25.5);
  fip.insert(6, 35.5);
  fip.insert(16, 35.5);
  cout << fip.g(3.) << endl;
  cout << fip.g(3.5) << endl;
  cout << fip.g(4.) << endl;
  cout << fip.g(4.5) << endl;
  cout << fip.g(5.) << endl;
  cout << fip.g(16.) << endl;

  cout << "fiplo Multidim" << endl;
  //Test fast intepolation - multi dimesional
  vector<unsigned int> fidx;
  fidx.push_back(5);
  fidx.push_back(10);
  fidx.push_back(30);
  g4m::fipol<double> fipm(fidx);
  fipm.fill(0.);
  fidx[0] = 3; fidx[1] = 5; fidx[2] = 15; fipm.insert(fidx, 10.5);
  fidx[0] = 2; fidx[1] = 5; fidx[2] = 15; fipm.insert(fidx, 20.5);
  fidx[0] = 2; fidx[1] = 4; fidx[2] = 15; fipm.insert(fidx, 30.5);
  fidx[0] = 2; fidx[1] = 4; fidx[2] = 14; fipm.insert(fidx, 40.5);
  fidx[0] = 2; fidx[1] = 4; fidx[2] = 14; fipm.insert(fidx, 40.5);
  vector<double> gfidx;
  gfidx.push_back(2.5);
  gfidx.push_back(4.5);
  gfidx.push_back(14.5);
  cout << fipm.g(gfidx) << endl;
  gfidx[0] = 2; gfidx[1] = 4; gfidx[2] = 14;
  cout << fipm.g(gfidx) << endl;

  //2dim
  unsigned int fidxx[] = {5, 10};
  fipm.clear(fidxx, 2);
  fipm.fill(0.);
  fidxx[0] = 3; fidxx[1] = 5; fipm.insert(fidxx, 10.5);
  fidxx[0] = 2; fidxx[1] = 5; fipm.insert(fidxx, 20.5);
  fidxx[0] = 3; fidxx[1] = 4; fipm.insert(fidxx, 30.5);
  fidxx[0] = 2; fidxx[1] = 4; fipm.insert(fidxx, 40.5);
  double gfidxx[] = {2.75, 4.5};
  cout << fipm.g(gfidxx) << endl;
  gfidxx[0] = 2; gfidxx[1] = 4;
  cout << fipm.g(gfidxx) << endl;

  fidx.clear();
  fidx.push_back(5);
  fidx.push_back(10);
  fipm.clear(fidx);
  fipm.fill(0.);
  fidx[0] = 3; fidx[1] = 5; fipm.insert(fidx, 10.5);
  fidx[0] = 2; fidx[1] = 5; fipm.insert(fidx, 20.5);
  fidx[0] = 3; fidx[1] = 4; fipm.insert(fidx, 30.5);
  fidx[0] = 2; fidx[1] = 4; fipm.insert(fidx, 40.5);
  gfidx.clear();
  gfidx.push_back(2.75);
  gfidx.push_back(4.5);
  cout << fipm.g(gfidx) << endl;
  gfidx[0] = 2; gfidx[1] = 4;
  cout << fipm.g(gfidx) << endl;

  return(0);
}
