#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>

using namespace std;

int main() {

  vector<string> columns {"SU_CODE90", "AWC_CLASS", "SWR"};  //Need to be in the same order like the columns in HWSD_DATA.csv
  vector<string> filenames {"/tmp/soilFAO90.bil", "/tmp/soilAWC.bil", "/tmp/soilSWR.bil"};

  vector<map<unsigned int, unsigned char> > look;
  ifstream fin( "/tmp/HWSD_DATA.csv" );
  string s;
  getline( fin, s );
  vector<int> colNr;
  {
    stringstream ss(s);
    int col=0;
    while (ss) {
      string ts;
      if(!getline(ss, ts, ',' )) break;
      for(vector<string>::iterator it=columns.begin(); it!=columns.end(); ++it) {
	if(ts == *it) {colNr.push_back(col); break;}
      }
      ++col;
    }
  }
  look.resize(colNr.size());
  while(getline( fin, s )) {
    unsigned int ID;
    stringstream ss(s);
    vector<string> tokens;
    while (ss) {
      string ts;
      if(!getline( ss, ts, ',' )) break;
      tokens.push_back(ts);
    }
    ID = (unsigned int)atoi(tokens[0].c_str());
    for(unsigned int i=0; i<colNr.size(); ++i) {
      look[i][ID] = static_cast<unsigned char>(atoi(tokens[colNr[i]].c_str()));
    }
  }
  fin.close();
  fin.open("./orig/hwsd.bil");
  ofstream fo[colNr.size()];
  for(unsigned int i=0; i<colNr.size(); ++i) {fo[i].open(filenames[i]);}
  int val1, val2;
  while (val1 = fin.get(), val2 = fin.get(), !fin.eof()) {
    unsigned int val3 = val2;
    val3 <<= 8;
    unsigned int val = val1 + val3;
    for(unsigned int i=0; i<colNr.size(); ++i) {fo[i].put(look[i][val]);}
  }
  for(unsigned int i=0; i<colNr.size(); ++i) {fo[i].close();}
  fin.close();
  return(0);
}
