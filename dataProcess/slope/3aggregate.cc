#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>

using namespace std;

class number {
public:
  number();
  ~number();
  int mfc(void);  //most frequent class
  unsigned long long int n(int cl);
  int in(const char &min, const char &max);
  static int setma(int cl, int max);
  int clear();
private:
  static int size;  //number of classes
  unsigned long long int *nn; //counts of a class
  static int *ma; //max of a class
  static double noval; //Value of no value
};

int number::size = 7;
double number::noval = 255;
int* number::ma = new int[size];

int number::clear() {
  for(int i=0; i < size+2; ++i) {nn[i] = 0;}
  return(0);
}

int number::mfc(void) {
  int cl=0;
  for(int i=1; i<size+2; ++i) {
    if(nn[cl] <= nn[i]) {cl = i;}
  }
  return(cl);
}

unsigned long long int number::n(int cl) {
  unsigned long long int val=0;
  if(cl < size+2 && cl >= 0) {val = nn[cl];}
  return(val);
}

int number::setma(int cl, int max) {
  if(cl < size && cl >= 0) {
    ma[cl]=max;
  }
  return(cl);
}

number::~number() {
  delete[] nn;
}

number::number() {
  nn = new unsigned long long int[size+2];
}

int number::in(const char &min, const char &max) {
  if(min < 0 || max > 90) {nn[size+1]+=100;}
  else if(min==max) {
    int i=0;
    while(i < size && min > ma[i]) {++i;}
    nn[i]+=100;
  } else {
    int i=0;
    while(i < size && min > ma[i]) {++i;}
    int j=i;
    while(j < size && max > ma[j]) {++j;}
    if(i==j) {nn[i]+=100;}
    else {
      double weight = 100. / (max-min);
      nn[i] += weight * (ma[i] - min);
      nn[j] += weight * (max - ma[j-1]);
      for(int k=i+1; k<j; ++k) {
	nn[k] += weight * (ma[k] - ma[k-1]);
      }
    }
  }
  return(max-min);
}

int compare_puchar(const void* a, const void* b) {
  return ( *(*(int**)b) - *(*(int**)a) );
}

int main(int argc, char *argv[]) {
  unsigned int ncolIn = atoi(argv[1]); //Number of columns IN
  unsigned int nrowIn = atoi(argv[2]); //Number of rows IN
  unsigned int ncolOut = atoi(argv[3]); //Number of columns OUT
  unsigned int nrowOut = atoi(argv[4]); //Number of rows OUT
  ifstream fpiSlpMin(argv[5], ios::out | ios::binary);
  ifstream fpiSlpMax(argv[6], ios::out | ios::binary);

  //unsigned int ncolIn = 863998;
  //unsigned int nrowIn = 287998;
  //unsigned int nrowIn = 335998;
  //unsigned int ncolOut = 43200;
  //unsigned int nrowOut = 14400;
  //unsigned int nrowOut = 16800;

  //ifstream fpiSlpMin("/data/tmp/slpmin.bin", ios::out | ios::binary);
  //ifstream fpiSlpMax("/data/tmp/slpmax.bin", ios::out | ios::binary);

  number *cnt = new number[ncolOut]; //cout
  cnt[0].setma(0, 1);
  cnt[0].setma(1, 3);
  cnt[0].setma(2, 6);
  cnt[0].setma(3, 10);
  cnt[0].setma(4, 15);
  cnt[0].setma(5, 30);
  cnt[0].setma(6, 50);

  ofstream fpsp[9]; //Slope percentge
  fpsp[0].open("slope1.bin", ios::out | ios::binary);
  fpsp[1].open("slope3.bin", ios::out | ios::binary);
  fpsp[2].open("slope6.bin", ios::out | ios::binary);
  fpsp[3].open("slope10.bin", ios::out | ios::binary);
  fpsp[4].open("slope15.bin", ios::out | ios::binary);
  fpsp[5].open("slope30.bin", ios::out | ios::binary);
  fpsp[6].open("slope50.bin", ios::out | ios::binary);
  fpsp[7].open("slopeL50.bin", ios::out | ios::binary);
  fpsp[8].open("slopeNULL.bin", ios::out | ios::binary);

  char slpMin, slpMax;
  unsigned long long int nx=0;
  unsigned long long int ny=0;
  unsigned int writtenLines = 0;
  fpiSlpMin.get(slpMin);
  fpiSlpMax.get(slpMax);
  do {
    cnt[static_cast<int>(static_cast<double>(nx)*static_cast<double>(ncolOut)/static_cast<double>(ncolIn))].in(slpMin, slpMax);
    ++nx;
    if(nx>=ncolIn) {
      nx=0;
      ++ny;
      if(static_cast<int>(static_cast<double>(ny-1)*static_cast<double>(nrowOut)/static_cast<double>(nrowIn)) < static_cast<int>(static_cast<double>(ny)*static_cast<double>(nrowOut)/static_cast<double>(nrowIn))) {
	//Write out and rest cnt
	++writtenLines;
	unsigned char tmp=0;
	for(unsigned int x=0; x<ncolOut; ++x) {
	  double sum = 0;
	  for(int j=0; j<7; ++j) {
	    sum += cnt[x].n(j);
	  }
	  if(sum + cnt[x].n(7) > 0) {
	    tmp = (unsigned char)(100*cnt[x].n(7) / (sum + cnt[x].n(7)));
	  } else {
	    tmp = 0;
	  }
	  fpsp[7].put(tmp);
	  if(sum > 0) {
	    int share[7];
	    int sumInt=0;
	    for(int j=0; j<7; ++j) {
	      share[j] = static_cast<int>(0.5 + 100*cnt[x].n(j) / sum);
	      sumInt += share[j];
	    }
	    sumInt -= 100;
	    if(sumInt != 0) {
	      int *pshare[7];
	      for(int j=0; j<7; ++j) {
		pshare[j] = &share[j];
	      }
	      qsort(pshare, 7, sizeof(int*), compare_puchar);
	      int diff = 1;
	      if(sumInt > 0) {diff = -1;}
	      sumInt = abs(sumInt);
	      for(int i=0; i<sumInt; ++i) {
		*pshare[i] += diff;
	      }
	    }
	    for(int j=0; j<7; ++j) {
	      fpsp[j].put(static_cast<unsigned char>(share[j]));
	    }
	  } else {
	    for(int j=0; j<7; ++j) {fpsp[j].put(0);}
	  }
	  cnt[x].clear();
	}
      }
    }
    fpiSlpMin.get(slpMin);
    fpiSlpMax.get(slpMax);
  } while ((!fpiSlpMin.eof() && !fpiSlpMax.eof()) || writtenLines < nrowOut);
  return(0);
}

