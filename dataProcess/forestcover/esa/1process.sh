wget ftp://geo10.elie.ucl.ac.be/v207/ESACCI-LC-L4-LCCS-Map-300m-P1Y-1992_2015-v2.0.7.zip
unzip ESACCI-LC-L4-LCCS-Map-300m-P1Y-1992_2015-v2.0.7.zip
mv ./scratch/ESACCI-LC-L4-LCCS-Map-300m-P1Y-1992_2015-v2.0.7.tif .
rm ESACCI-LC-L4-LCCS-Map-300m-P1Y-1992_2015-v2.0.7.zip
wget http://maps.elie.ucl.ac.be/CCI/viewer/download/ESACCI-LC-Legend.csv

gdalinfo ESACCI-LC-L4-LCCS-Map-300m-P1Y-1992_2015-v2.0.7.tif
#band: 1-24

mkdir ~/tmp/grass/
grass -e -c EPSG:4326 ~/tmp/grass/mylocation
grass -c ~/tmp/grass/mylocation/mymapset

#nohup grass ~/tmp/grass/mylocation/mymapset --exec bash 1process.sh &
r.in.gdal input=ESACCI-LC-L4-LCCS-Map-300m-P1Y-1992_2015-v2.0.7.tif output=del band=1 --overwrite
g.region raster=del

r.mapcalc --overwrite <<EOF
delA = if(del==50,1,0)
delB = if(del>=60 && del<=62,1,0)
delC = if(del>=70 && del<=72,1,0)
delD = if(del>=80 && del<=82,1,0)
EOF

for BAND in {2..24}; do
    echo $BAND
    r.in.gdal input=ESACCI-LC-L4-LCCS-Map-300m-P1Y-1992_2015-v2.0.7.tif output=del band=$BAND --overwrite
    r.mapcalc --overwrite <<EOF
delA = delA + if(del==50,1,0)
delB = delB + if(del>=60 && del<=62,1,0)
delC = delC + if(del>=70 && del<=72,1,0)
delD = delD + if(del>=80 && del<=82,1,0)
EOF
done

r.mapcalc "tmp = delA + delB + delC + delD" --overwrite
r.info map=tmp

r.mapcalc "delA = if(tmp>0, round(double(delA)/double(tmp)*100.), 0)" --overwrite &
r.mapcalc "delB = if(tmp>0, round(double(delB)/double(tmp)*100.), 0)" --overwrite &
r.mapcalc "delC = if(tmp>0, round(double(delC)/double(tmp)*100.), 0)" --overwrite &
r.mapcalc "delD = if(tmp>0, round(double(delD)/double(tmp)*100.), 0)" --overwrite &

wait

r.mapcalc "tmp = delA + delB + delC + delD" --overwrite
r.info map=tmp
r.mapcalc "tmp = if(tmp > 0, 100 - tmp, 0)" --overwrite

r.mapcalc "delAx = if(tmp != 0 && delA >= delB && delA >= delC && delA >= delD, delA + tmp, delA)" --overwrite &
r.mapcalc "delBx = if(tmp != 0 && delB > delA && delB >= delC && delB >= delD, delB + tmp, delB)" --overwrite &
r.mapcalc "delCx = if(tmp != 0 && delC > delA && delC > delD && delC >= delD, delC + tmp, delC)" --overwrite &
r.mapcalc "delDx = if(tmp != 0 && delD > delA && delD > delB && delD > delC, delD + tmp, delD)" --overwrite &

wait

r.out.gdal input=delA output=broadleavedEvergreenESA1992_2015V207.tif format=GTiff createopt=COMPRESS=LZMA,NUM_THREADS=7 --o
r.out.gdal input=delB output=broadleavedDeciduousESA1992_2015V207.tif format=GTiff createopt=COMPRESS=LZMA,NUM_THREADS=7 --o
r.out.gdal input=delC output=needleleavedEvergreenESA1992_2015V207.tif format=GTiff createopt=COMPRESS=LZMA,NUM_THREADS=7 --o
r.out.gdal input=delD output=needleleavedDeciduousESA1992_2015V207.tif format=GTiff createopt=COMPRESS=LZMA,NUM_THREADS=7 --o

wait

for BAND in {1..24}; do
    echo $BAND
    r.in.gdal input=ESACCI-LC-L4-LCCS-Map-300m-P1Y-1992_2015-v2.0.7.tif output=del band=$BAND --overwrite
    r.mapcalc "tmp = if(del==50 || del==60 || del==61 || del==70 || del==71 || del==72 || del==80 || del==81 || del==82 || del==90 || del==160 || del==170, 100, if(del==150, 5, if(del==151, 8, if(del==110 || del==30, 25, if(del==12, 50, if(del==40 || del==100, 75))))))" --overwrite
    YEAR=$(expr 1991 + $BAND)
    r.out.gdal input=tmp output=forestCoverESA$YEAR.tif format=GTiff createopt=COMPRESS=LZMA,NUM_THREADS=7 --o
done
