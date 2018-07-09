#include <fstream>
#include <iostream>

using namespace std;

int main() {
  ifstream fi;
  fi.open("/tmp/res.txt");
  ofstream of[2];
  of[0].open("/tmp/area.asc");
  of[1].open("/tmp/irri.asc");
  for(int i=0; i<2; ++i) {
    of[i] << "north: 90N\nsouth: 90S\neast: 180E\nwest: 180W\nrows: 21600\ncols: 43200\n";
  }
  int drow, dcol, area, irri;
  //int drow, dcol;
  //double area, irri;
  fi >> drow >> dcol >> area >> irri;
  for(int row=1; row<21601; ++row) {
    for(int col=1; col<43201; ++col) {
      if(row==drow && col==dcol) {
	of[0] << " " << area;
	of[1] << " " << irri;
	fi >> drow >> dcol >> area >> irri;
      } else {
	for(int i=0; i<2; ++i) {of[i] << " 0";}
      }
    }
    for(int i=0; i<2; ++i) {of[i] << endl;}
  }    
}
