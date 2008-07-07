#include <iostream>

#include "misc.h"

using namespace std;


int main() {
  g4m::ipol<double, double> d;
  d.insert(3, 0.);
  d.insert(5, 10.);
  cout << d.g(4.5) << endl;
  cout << d[4.5] << endl;
  return(0);
}
