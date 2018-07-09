#include <fstream>
#include <iostream>
#include <map>
#include <map>
#include <sstream>
#include <vector>

using namespace std;

class keys {
public:
  int country;
  int humAct;
  bool operator< (const keys& param) const;
  friend ostream& operator<<(ostream&, const keys&);
};
bool keys::operator< (const keys& param) const {
  bool ret = false;
  if(country < param.country) {ret = true;}
  else if(country == param.country) {
    if(humAct < param.humAct) {ret = true;}
  }
  return(ret);
}
ostream& operator<<(ostream& out, const keys& c) {
  out << c.country << "\t" << c.humAct;
  return(out);
}

int main() {
  ifstream fp[4];
  int val[4];
  map<int, double> shareUnmanaged;
  map<int, double> forestArea;

  map<keys, double> result;


  fp[0].open("faoFra2015.txt", ios::in);
  //fp[0].open("faoFra2000.txt", ios::in);
  while(!fp[0].eof()) {
    string line;
    getline(fp[0], line);
    if(line[0] != '#' && line.size()>0) {
      stringstream ss(line);
      int t1; double t2, t3;
      ss >> t1 >> t2 >> t3;
      if(t2 > 0.) {shareUnmanaged[t1] = t3 / t2;}
      forestArea[t1] = t2;
    }
  }
  fp[0].close();
  //cout << shareUnmanaged[11] << endl;

  fp[0].open("/tmp/humanFoot.bin", ios::in | ios::binary);
  fp[1].open("/tmp/landarea.bin", ios::in | ios::binary);
  fp[2].open("/tmp/forest.bin", ios::in | ios::binary);
  fp[3].open("/tmp/country.bin", ios::in | ios::binary);
  while (val[0] = fp[0].get(), val[1] = fp[1].get(), val[2] = fp[2].get()
	 , val[3] = fp[3].get(),
	 !fp[0].eof() && !fp[1].eof() && !fp[2].eof() && !fp[3].eof()) {
    int byte1 = fp[1].get();
    int byte2 = fp[1].get();
    int byte3 = fp[1].get();
    val[1] += (byte1<<8) + (byte2<<16) + (byte3<<24);

    keys t; //Store total forest area for the classes: Country and humanActivity
    t.country = val[3];
    t.humAct = val[0]; 
    result[t] += val[2] * val[1]/100.; 
  }
  fp[0].close();
  fp[1].close();
  fp[2].close();
  fp[3].close();

  map<int, int> hinfmax;
  map<int, double> hinfmul;
  {
    double vec[101];
    for(int i=0; i<101; ++i) {vec[i] = 0.;}
    map<keys, double>::iterator iter;
    bool first = true;
    int ccountry = 0;
    for(iter=result.begin(); iter!=result.end(); ++iter) {
      if(first == true) {first=false; ccountry=iter->first.country;}
      if(ccountry != iter->first.country) {
	double target = 0.;
	if(shareUnmanaged.find(ccountry) != shareUnmanaged.end()) {
	  target = shareUnmanaged[ccountry];
	}
	double forarea=0.;
	for(int i=0; i<101; ++i) {forarea += vec[i];}
	if(forarea > 0) {
	  double factor = 1.;
	  int hit=0;
	  if(target > 0) {
	    for(int i=100; i>0; --i) {
	      double sum=0;
	      for(hit=0; hit<i; ++hit) {
		sum += vec[hit]*(1-hit/double(i));
	      }
	      if(sum/forarea <= target) {hit=i; break;}
	    }
	    if(hit < 100) {++hit;}
	    for(int j=0; j<1000; ++j) {
	      double sum=0;
	      for(int i=0; i<hit; ++i) {
		double perc = i/double(hit) * factor;
		if(perc<0.) {perc=0.;}
		if(perc>1.) {perc=1.;}
		sum += vec[i]*(1.-perc);
	      }
	      factor += (sum-target*forarea)/(target*forarea)/10.;
      //cout << factor << "\t" <<  target*forarea << "\t" << sum << endl;
	    }
	  }

	//double sum=0;
	//int hit;
	//for(hit=0; hit<101 && sum/forarea >= target; ++hit) {sum += vec[hit];}
	//double hitperc=vec[hit] / (target*forarea - (sum-vec[hit]));
	  double sum = 0;
	  for(int i=0; i<101; ++i) {
	    double perc = 1.;
	    if(i<hit) {
	      perc = i/double(hit) * factor;
	      if(perc<0.) {perc=0.;}
	      if(perc>1.) {perc=1.;}
	      sum += vec[i] * (1. - perc);
	    }
	    //cout << ccountry << "\t" << i << "\t" << vec[i] << "\t";
	    //cout << perc;
	    //cout << "\t" << sum/forarea << "\t" << target << endl;
	    hinfmax[ccountry] = hit;
	    hinfmul[ccountry] = factor;
	  }

	}
	for(int i=0; i<101; ++i) {vec[i] = 0.;}
	ccountry=iter->first.country;
      }
      vec[iter->first.humAct] = iter->second;
    }
  }

  { //Set a value to those countries without a number in FRA
    double sumForestArea = 0.;
    double sumHinfmax = 0.;
    double sumHinfmul = 0.;
    for(map<int, double>::iterator iter = forestArea.begin(); iter!=forestArea.end(); ++iter) {
      if(hinfmax.find(iter->first) != hinfmax.end()) {
	if(hinfmul[iter->first] > 0.5 && hinfmul[iter->first] < 4.) {
	  sumForestArea += iter->second;
	  sumHinfmax += hinfmax[iter->first] * iter->second;
	  sumHinfmul += hinfmul[iter->first] * iter->second;
	}
      }
    }
    sumHinfmax /= sumForestArea;
    sumHinfmul /= sumForestArea;
    for(map<int, int>::iterator iter = hinfmax.begin(); iter!=hinfmax.end(); ++iter) {
      if(forestArea.find(iter->first) == forestArea.end()) {
	hinfmax[iter->first] = sumHinfmax;
	hinfmul[iter->first] = sumHinfmul;
      }
    }
  }

  ofstream fpo;
  fpo.open("/tmp/manFor.bin", ios::out | ios::binary);
  fp[0].open("/tmp/humanFoot.bin", ios::in | ios::binary);
  fp[1].open("/tmp/landarea.bin", ios::in | ios::binary);
  fp[2].open("/tmp/forest.bin", ios::in | ios::binary);
  fp[3].open("/tmp/country.bin", ios::in | ios::binary);
  while (val[0] = fp[0].get(), val[1] = fp[1].get(), val[2] = fp[2].get()
	 , val[3] = fp[3].get(),
	 !fp[0].eof() && !fp[1].eof() && !fp[2].eof() && !fp[3].eof()) {
    int byte1 = fp[1].get();
    int byte2 = fp[1].get();
    int byte3 = fp[1].get();
    val[1] += (byte1<<8) + (byte2<<16) + (byte3<<24);
    double perc = 1;
    if(val[1]>0 && val[2]>0) {
    //if(val[2]>0) {
      int hit = hinfmax[val[3]];
      double factor = hinfmul[val[3]];
      if(val[0] < hit) {
	perc = val[0]/double(hit) * factor;
	if(perc<0.) {perc=0.;}
	if(perc>1.) {perc=1.;}
      }
      perc *= 100.;
    } else {perc = 255;}
    unsigned char caout = (unsigned char)(perc);
    fpo.put(caout);
  }
  fpo.close();
  fp[0].close();
  fp[1].close();
  fp[2].close();
  fp[3].close();
  return(0);
}
