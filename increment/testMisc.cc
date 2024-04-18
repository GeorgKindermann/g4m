#include <iostream>
#include <ctime>
#include <cstdlib>

#include "misc.h"

using namespace std;


int main() {

  {
    g4m::ipol<vector<double>, double> vd;
    vector<double> idx;
    idx.push_back(3.);
    idx.push_back(3.);
    vd.insert(idx, 0.);
    idx[0] = 8; idx[1] = 13;
    vd.insert(idx, 10.);
    cout << " ipol";
    for(int j=0; j<=15; ++j) cout << ' ' << j;
    cout << "\n";
    for(int i=0; i<=10; ++i) {
      cout << i;
      idx[0] = i;
      for(int j=0; j<=15; ++j) {
	idx[1] = j;
	cout << ' ' << vd.g(idx);
      }
      cout << "\n";
    }
    cout << "fipol\n";
    g4m::fipolm<double> fipm(vd);
    for(int i=0; i<=10; ++i) {
      cout << i;
      idx[0] = i;
      for(int j=0; j<=15; ++j) {
	idx[1] = j;
	cout << ' ' << fipm.g(idx);
      }
      cout << "\n";
    }
    cout << "ffipol\n";
    g4m::ffipolm<double> ffipm(vd);
    for(int i=0; i<=10; ++i) {
      cout << i;
      idx[0] = i;
      for(int j=0; j<=15; ++j) {
	idx[1] = j;
	cout << ' ' << ffipm.g(idx);
      }
      cout << "\n";
    }
  }
  
  {
    g4m::ipol<double,double> sws;
    sws.insert(10, .0);
    sws.insert(30, .6);
    g4m::fipol<double> fsws(sws);
    g4m::ffipol<double> ffsws(sws, 5., .25);
    for (int i = 0; i < 55; ++i) {
      double x{9.7 + i*.05};
      std::cout << i << ' ' << x << ' ' << sws.g(x) << ' ' << fsws.g(x) << ' ' << ffsws.g(x) << '\n';
    }
    for (int i = 0; i <= 50; i++) {
      std::cout << i << ' ' << sws.g(i) << ' ' << fsws.g(i) << ' ' << ffsws.g(i) << '\n';
    }
  }

  {
    double ic = 20.5;
    vector<double> idx;
    idx.push_back(0+ic);
    idx.push_back(0+ic);
    g4m::ipol<vector<double>, double> vd;
    vd.insert(idx, 110);
    idx[0] = 1+ic; idx[1] = 0+ic; vd.insert(idx, 120.);
    idx[0] = 0+ic; idx[1] = 1+ic; vd.insert(idx, 210.);
    //idx[0] = 1; idx[1] = 2; vd.insert(idx, 320.);
    //idx[0] = 15; idx[1] = 30; vd.insert(idx, 999.);
    g4m::fipolm<double> fvd(vd);
    idx[0] = .5+ic; idx[1] = .5+ic;
    cout << vd.g(idx) << "\t" << fvd.g(idx) << endl;
    idx[0] = 1+ic; idx[1] = 1+ic;
    //cout << vd.g(idx) << "\t" << fvd.g(idx) << endl;
    idx[0] = 0+ic; idx[1] = 0+ic;
    //cout << vd.g(idx) << "\t" << fvd.g(idx) << endl;
  }

  {
    vector<double> idx;
    idx.push_back(0);
    idx.push_back(0);
    idx.push_back(0);
    g4m::ipol<vector<double>, double> vd;
    vd.insert(idx, 10.5);
    idx[0] = 30; idx[1] = 40; idx[2] = 14; vd.insert(idx, 20.5);
    idx[0] = 20; idx[1] = 50; idx[2] = 14; vd.insert(idx, 30.5);
    idx[0] = 30; idx[1] = 50; idx[2] = 14; vd.insert(idx, 40.5);
    idx[0] = 20; idx[1] = 40; idx[2] = 15; vd.insert(idx, 50.5);
    idx[0] = 30; idx[1] = 40; idx[2] = 15; vd.insert(idx, 60.5);
    idx[0] = 100; idx[1] = 50; idx[2] = 15; vd.insert(idx, 70.5);
    idx[0] = 30; idx[1] = 100; idx[2] = 100; vd.insert(idx, 80.5);
    g4m::fipolm<double> fvd(vd);
    idx[0] = 50; idx[1] = 50; idx[2] = 50;
    cout << vd.g(idx) << "\t" << fvd.g(idx) << "\t" << fvd.g(idx) << endl;
    idx[0] = 50; idx[1] = 50.25; idx[2] = 50;
    cout << vd.g(idx) << "\t" << fvd.g(idx) << "\t" << fvd.g(idx) << endl;
    idx[0] = 50; idx[1] = 50.5; idx[2] = 50;
    cout << vd.g(idx) << "\t" << fvd.g(idx) << "\t" << fvd.g(idx) << endl;
    idx[0] = 50; idx[1] = 50.75; idx[2] = 50;
    cout << vd.g(idx) << "\t" << fvd.g(idx) << "\t" << fvd.g(idx) << endl;
    idx[0] = 50; idx[1] = 51; idx[2] = 50;
    cout << vd.g(idx) << "\t" << fvd.g(idx) << "\t" << fvd.g(idx) << endl;
    idx[0] = 0; idx[1] = 0; idx[2] = 0;
    cout << vd.g(idx) << "\t" << fvd.g(idx) << "\t" << fvd.g(idx) << endl;
    idx[0] = -1; idx[1] = -1; idx[2] = -1;
    cout << vd.g(idx) << "\t" << fvd.g(idx) << "\t" << fvd.g(idx) << endl;
    idx[0] = 100; idx[1] = 100; idx[2] = 100;
    cout << vd.g(idx) << "\t" << fvd.g(idx) << "\t" << fvd.g(idx) << endl;
    idx[0] = 101; idx[1] = 101; idx[2] = 101;
    cout << vd.g(idx) << "\t" << fvd.g(idx) << "\t" << fvd.g(idx) << endl;
    idx[0] = 37.9598; idx[1] = 29.9711; idx[2] = 14.9924;
    cout << vd.g(idx) << "\t" << fvd.g(idx) << "\t" << fvd.g(idx) << endl;
    idx[0] = 9.08315; idx[1] = 99.9939; idx[2] = 3.01329;
    cout << vd.g(idx) << "\t" << fvd.g(idx) << "\t" << fvd.g(idx) << endl;
  }

  //Test one dimensional interpolation
  g4m::ipol<double, double> d;
  d.insert(3, 0.);
  d.insert(5, 10.);
  cout << d.g(4.5) << endl;
  cout << d[5.5] << endl;
  cout << "Min: " << d.min() << endl;
  cout << "Max: " << d.max() << endl;
  d *= 2.5;
  cout << d[5.5] << endl;

  g4m::fipol<double> copy(d);

  //Test multi dimensional interpolation
  g4m::ipol<vector<double>, double> vd;
  vector<double> idx;
  idx.push_back(0.);
  idx.push_back(0.);
  idx.push_back(0.);
  
  vd.insert(idx, 10.);

  idx[0] = 10; idx[1] = 5; idx[2] = 0;
  vd.insert(idx, 20);

  cout << "Min:";
  vector<double> idxMin(vd.min());
  for(unsigned int i=0; i<idxMin.size(); ++i) {
    cout << "\t" << idxMin[i];
  }
  cout << endl;
  cout << "Max:";
  vector<double> idxMax(vd.max());
  for(unsigned int i=0; i<idxMax.size(); ++i) {
    cout << "\t" << idxMax[i];
  }
  cout << endl;

  cout << vd.g(idx) << endl;
  idx[0] = 5; idx[1] = 2.5; idx[2] = 0;
  cout << vd[idx] << endl;
  idx[0] = 7.5; idx[1] = 3.75; idx[2] = 0;
  cout << vd[idx] << endl;
  idx[0] = 0; idx[1] = 0; idx[2] = 0;
  cout << vd.g(idx) << endl;

  vd *= 2.5;
  cout << vd[idx] << endl;

  vd.clear();
  idx.clear();

  idx.push_back(10.); idx.push_back(10.); vd.insert(idx, 110.);
  idx[0] = 20; idx[1] = 10; vd.insert(idx, 120.);
  idx[0] = 10; idx[1] = 20; vd.insert(idx, 210.);
  idx[0] = 20; idx[1] = 30; vd.insert(idx, 320.);
  //idx[0] = 15; idx[1] = 30; vd.insert(idx, 999.);
  idx[0] = 15; idx[1] = 15;
  cout << "mip: " << vd.g(idx) << endl;

  cout << "fipol one dimmension" << endl;
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
  cout << fip.g((unsigned int)3) << endl;
  cout << fip.g((unsigned int)4) << endl;
  cout << fip.g((unsigned int)16) << endl;

  fip *= 2.5;
  cout << fip.g(4.5) << endl;
  cout << fip.g((unsigned int)4) << endl;

  cout << "fipolm Multidim" << endl;
  //Test fast intepolation - multi dimesional
  vector<unsigned int> fidx;
  fidx.push_back(5);
  fidx.push_back(10);
  fidx.push_back(30);

  g4m::fipolm<double> fipm(fidx);
  fipm.fill(0.);
  fidx[0] = 2; fidx[1] = 4; fidx[2] = 14; fipm.insert(fidx, 10.5);
  fidx[0] = 3; fidx[1] = 4; fidx[2] = 14; fipm.insert(fidx, 20.5);
  fidx[0] = 2; fidx[1] = 5; fidx[2] = 14; fipm.insert(fidx, 30.5);
  fidx[0] = 3; fidx[1] = 5; fidx[2] = 14; fipm.insert(fidx, 40.5);
  fidx[0] = 2; fidx[1] = 4; fidx[2] = 15; fipm.insert(fidx, 50.5);
  fidx[0] = 3; fidx[1] = 4; fidx[2] = 15; fipm.insert(fidx, 60.5);
  fidx[0] = 2; fidx[1] = 5; fidx[2] = 15; fipm.insert(fidx, 70.5);
  fidx[0] = 3; fidx[1] = 5; fidx[2] = 15; fipm.insert(fidx, 80.5);
  vector<double> gfidx;
  gfidx.push_back(2.5);
  gfidx.push_back(4.5);
  gfidx.push_back(14.5);
  cout << fipm.g(gfidx) << endl;
  double key[] = {2.5, 4.5, 14.5};
  cout << fipm.g(key) << endl;
  gfidx[0] = 2; gfidx[1] = 4; gfidx[2] = 14;
  cout << fipm.g(gfidx) << endl;

  fipm *= 2.5;
  cout << fipm[key] << endl;

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

  { //Speed for 1 dim interpolation
    g4m::ipol<double, double> d;
    d.insert(0., 0.);
    d.insert(5., 10.);
    d.insert(15., 12.);
    d.insert(35., 13.4);
    d.insert(60., 16.2);
    d.insert(100., 20.);
    g4m::fipol<double> fd(d);
    g4m::ffipol<double> ffd(d,1,.5);
    double range = 100;
    clock_t time0, time1;
    double sum = 0.;
    unsigned int count = 0;
    //g4m::vipol<double, double> &vd = d;
    srand(1);
    time0 = clock();
    for(int i=0; i<10000; ++i) {
      for(int j=0; j<10000; ++j) {
	sum += rand()*range/RAND_MAX;
	++count;
      }
    }
    time1 = clock();
    cout << sum << "  n:" << count << "  Base Time: " << (time1 - time0)/double(CLOCKS_PER_SEC) << endl;
    double baseTime = (time1 - time0)/double(CLOCKS_PER_SEC);
    sum = 0.; count = 0;
    srand(1);
    time0 = clock();
    for(int i=0; i<10000; ++i) {
      for(int j=0; j<10000; ++j) {
	sum += d.g(rand()*range/RAND_MAX);
	//sum += vd.g(rand()*range/RAND_MAX); //Cal from virtual class is slower
	++count;
      }
    }
    time1 = clock();
    cout << sum << "  n:" << count << "  Used Time: " << (time1 - time0)/double(CLOCKS_PER_SEC) - baseTime << endl;
    sum = 0.; count = 0;
    srand(1);
    time0 = clock();
    for(int i=0; i<10000; ++i) {
      for(int j=0; j<10000; ++j) {
	//sum += ffd.g((unsigned int)(rand()*range/RAND_MAX));
	sum += ffd.g(rand()*range/RAND_MAX);
 	++count;
      }
    }
    time1 = clock();
    cout << sum << "  n:" << count << "  ff Used Time: " << (time1 - time0)/double(CLOCKS_PER_SEC) - baseTime << endl;
    sum = 0.; count = 0;
    srand(1);
    time0 = clock();
    for(int i=0; i<10000; ++i) {
      for(int j=0; j<10000; ++j) {
	sum += fd.g(rand()*range/RAND_MAX);
 	++count;
      }
    }
    time1 = clock();
    cout << sum << "  n:" << count << " f Used Time: " << (time1 - time0)/double(CLOCKS_PER_SEC) - baseTime << endl;
    //5.00011e+09  n:100000000  Base Time: 3.69
    //1.48303e+09  n:100000000  Used Time: 3.97
    //1.48303e+09  n:100000000  ff Used Time: 0.63
    //1.48303e+09  n:100000000 f Used Time: 1.69
  }

  {//Speed for multidimmensional interpolation
    vector<double> idx;
    idx.push_back(0);
    idx.push_back(0);
    idx.push_back(0);
    g4m::ipol<vector<double>, double> vd;
    vd.insert(idx, 10.5);
    idx[0] = 30; idx[1] = 40; idx[2] = 14; vd.insert(idx, 20.5);
    idx[0] = 20; idx[1] = 50; idx[2] = 14; vd.insert(idx, 30.5);
    idx[0] = 30; idx[1] = 50; idx[2] = 14; vd.insert(idx, 40.5);
    idx[0] = 20; idx[1] = 40; idx[2] = 15; vd.insert(idx, 50.5);
    idx[0] = 30; idx[1] = 40; idx[2] = 15; vd.insert(idx, 60.5);
    idx[0] = 100; idx[1] = 50; idx[2] = 15; vd.insert(idx, 70.5);
    idx[0] = 30; idx[1] = 100; idx[2] = 100; vd.insert(idx, 80.5);
    //vector<double> zoom;
    //zoom.push_back(1); zoom.push_back(2); zoom.push_back(1);
    //g4m::fipolm<double> fvd(vd, zoom);
    g4m::fipolm<double> fvd(vd);
    g4m::ffipolm<double> ffvd(vd);
    idx[0] = 50; idx[1] = 50; idx[2] = 50;
    cout << vd.g(idx) << "\t" << fvd.g(idx) << "\t" << ffvd.g(idx) << endl;
    idx[0] = 50; idx[1] = 50.25; idx[2] = 50;
    cout << vd.g(idx) << "\t" << fvd.g(idx) << "\t" << ffvd.g(idx) << endl;
    idx[0] = 50; idx[1] = 50.5; idx[2] = 50;
    cout << vd.g(idx) << "\t" << fvd.g(idx) << "\t" << ffvd.g(idx) << endl;
    idx[0] = 50; idx[1] = 50.75; idx[2] = 50;
    cout << vd.g(idx) << "\t" << fvd.g(idx) << "\t" << ffvd.g(idx) << endl;
    idx[0] = 50; idx[1] = 51; idx[2] = 50;
    cout << vd.g(idx) << "\t" << fvd.g(idx) << "\t" << ffvd.g(idx) << endl;
    idx[0] = 100; idx[1] = 100; idx[2] = 100;
    cout << vd.g(idx) << "\t" << fvd.g(idx) << "\t" << ffvd.g(idx) << endl;
    idx[0] = 0; idx[1] = 0; idx[2] = 0;
    cout << vd.g(idx) << "\t" << fvd.g(idx) << "\t" << ffvd.g(idx) << endl;
    idx[0] = 100; idx[1] = 0; idx[2] = 0;
    cout << vd.g(idx) << "\t" << fvd.g(idx) << "\t" << ffvd.g(idx) << endl;
    idx[0] = 0; idx[1] = 100; idx[2] = 0;
    cout << vd.g(idx) << "\t" << fvd.g(idx) << "\t" << ffvd.g(idx) << endl;
    idx[0] = 0; idx[1] = 0; idx[2] = 100;
    cout << vd.g(idx) << "\t" << fvd.g(idx) << "\t" << ffvd.g(idx) << endl;

    double range[3] = {100, 100, 100};
    clock_t time0, time1;
    double sum = 0.;
    unsigned int count = 0;
    srand(1);
    time0 = clock();
    for(int i=0; i<2000; ++i) {
      for(int j=0; j<2000; ++j) {
	idx[0] = rand()*range[0]/RAND_MAX; idx[1] = rand()*range[1]/RAND_MAX;
	idx[2] = rand()*range[2]/RAND_MAX;
	sum += idx[0] + idx[1] + idx[2];
	++count;
      }
    }
    time1 = clock();
    cout << sum << "  n:" << count << "  Base Time: " << (time1 - time0)/double(CLOCKS_PER_SEC) << endl;
    double baseTime = (time1 - time0)/double(CLOCKS_PER_SEC);
    sum = 0.; count = 0;
    srand(1);
    time0 = clock();
    for(int i=0; i<2000; ++i) {
      for(int j=0; j<2000; ++j) {
	idx[0] = rand()*range[0]/RAND_MAX; idx[1] = rand()*range[1]/RAND_MAX;
	idx[2] = rand()*range[2]/RAND_MAX;
	sum += vd.g(idx);
	++count;
      }
    }
    time1 = clock();
    cout << sum << "  n:" << count << "  Used Time: " << (time1 - time0)/double(CLOCKS_PER_SEC) - baseTime << endl;
    sum = 0.; count = 0;
    srand(1);
    time0 = clock();
    for(int i=0; i<2000; ++i) {
      for(int j=0; j<2000; ++j) {
	idx[0] = rand()*range[0]/RAND_MAX; idx[1] = rand()*range[1]/RAND_MAX;
	idx[2] = rand()*range[2]/RAND_MAX;
	sum += fvd.g(idx);
	++count;
      }
    }
    time1 = clock();
    cout << sum << "  n:" << count << "  f Used Time: " << (time1 - time0)/double(CLOCKS_PER_SEC) - baseTime << endl;
    sum = 0.; count = 0;
    srand(1);
    time0 = clock();
    for(int i=0; i<2000; ++i) {
      for(int j=0; j<2000; ++j) {
	idx[0] = rand()*range[0]/RAND_MAX; idx[1] = rand()*range[1]/RAND_MAX;
	idx[2] = rand()*range[2]/RAND_MAX;
	sum += ffvd.g(idx);
	++count;
      }
    }
    time1 = clock();
    cout << sum << "  n:" << count << "  ff Used Time: " << (time1 - time0)/double(CLOCKS_PER_SEC) - baseTime << endl;
    //6.00029e+08  n:4000000  Base Time: 0.45
    //2.17708e+08  n:4000000  Used Time: 5.78
    //2.18166e+08  n:4000000  f Used Time: 3.27
    //2.17705e+08  n:4000000  ff Used Time: 0.28
  }

  return(0);
}
