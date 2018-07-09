mdb-tables ./orig/HWSD.mdb
mdb-export ./orig/HWSD.mdb HWSD_DATA >/tmp/HWSD_DATA.csv

mdb-export ./orig/HWSD.mdb D_SYMBOL90 >D_SYMBOL90.csv
mdb-export ./orig/HWSD.mdb D_AWC >D_AWC.csv
mdb-export ./orig/HWSD.mdb D_SWR >D_SWR.csv

