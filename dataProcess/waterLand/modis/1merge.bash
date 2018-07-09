gdal_merge.py -co BIGTIFF=YES -co COMPRESS=LZW -o del_waterLandModis2000.tif -a_nodata 255 -init 255 ./orig/*.tif

gdal_translate -co "BIGTIFF=YES" -co "COMPRESS=CCITTRLE" -co "NBITS=1" del_waterLandModis2000.tif waterLandModis2000.tif

rm del_waterLandModis2000.tif

#Compress original datsets with bzip
#bzip2 ./orig/*.tif
#
#maxprocesses=7
#for file in ./orig/*.tif ; do
# actualprocesses=$(pgrep -c bzip2)
#  while [ $actualprocesses -ge $maxprocesses ]; do
#    sleep 1
#    actualprocesses=$(pgrep -c bzip2)
#  done
#  nice -19 bzip2 ${file} &
#done
#
#find ./ -name "./orig/*.tif" -type f -print | xargs -n 1 -P7 bzip2
#
parallel -j 7 bzip2 ::: ./orig/*.tif


#grass 30Sec
r.in.gdal input=waterLandModis2000.tif output=waterLandModis

r.info waterLandModis
g.region n=84:0:00n s=64:00:00s w=180w e=180e res=0:00:30
r.resamp.stats input=waterLandModis output=waterLandModis30Sec method=sum
r.mapcalc "waterLandModis30Sec = round(waterLandModis30Sec)" --overwrite

r.info waterLandModis30Sec
echo "0 0:0:0
15 240:240:240
16 0:0:255" | r.colors map=waterLandModis30Sec rules=-

r.out.gdal input=waterLandModis30Sec format=GTiff type=Byte output=waterLandModis30Sec.tif createopt=COMPRESS=LZMA,NUM_THREADS=7
