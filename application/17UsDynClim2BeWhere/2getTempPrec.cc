//./2getTempPrec /tmp/tas.nc tas |sort >/tmp/s1
//./2getTempPrec /tmp/pr.nc pr |sort >/tmp/s2
#include <fstream>
#include <iostream>
#include <netcdfcpp.h>
#include <cmath>
#include <vector>
#include <map>
#include <limits>
#include <iomanip>

using namespace std;

static const int NLON = 298; //lon NX
static const int NLAT = 130; //lat NY


int main(int argc, char *argv[], char *env[]) {
  float dat[1][NLAT][NLON];
  double lon[NLON];
  double lat[NLAT];

  int ntime;
  {
    int recid, ncid;
    size_t recs;
    nc_open(argv[1], NC_NOWRITE, &ncid);
    nc_inq_unlimdim(ncid, &recid);
    nc_inq_dim(ncid, recid, NULL, &recs);
    ntime = recs;
    nc_close(ncid);
  }
  
  NcFile dataFile(argv[1], NcFile::ReadOnly);
  if (!dataFile.is_valid()) {cerr << "Couldn't open file!\n"; return(0);}

  NcVar *data = dataFile.get_var("lon");
  data->get(lon,NLON);
  data = dataFile.get_var("lat");
  data->get(lat,NLAT);
  data = dataFile.get_var(argv[2]);
  for(int j=0; j<ntime; ++j) {
    data->set_cur(j, 0, 0);
    data->get(&dat[0][0][0],1,NLAT,NLON);
    for(int y=0; y<NLAT; ++y) {
      for(int x=0; x<NLON; ++x) {
	if(dat[0][y][x] < 1e+19) {
	  cout << lat[y] << "_" << lon[x] << "_" << j << " " << dat[0][y][x] << endl;
	}
      }
    }
  }
  
  return(0);
}

