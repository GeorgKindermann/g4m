wget https://edcintl.cr.usgs.gov/downloads/sciweb1/shared/gtc/downloads/WaterMask2010_UMD.zip
unzip WaterMask2010_UMD.zip

nohup nice -19 gdal_merge.py -co BIGTIFF=YES -co COMPRESS=LZW -o del_waterLandHansenGFC2013.tif Hansen_GFC2013_datamask_*.tif &

#grass 30Sec
r.in.gdal input=del_waterLandHansenGFC2013.tif output=tmp --overwrite
g.region raster=tmp
r.mapcalc "tmp = if(tmp==1,0,1)" --overwrite  #Water=1, Land=0
r.out.gdal input=tmp format=GTiff type=Byte output=waterLandHansenGFC2013.tif createopt=COMPRESS=CCITTRLE,BIGTIFF=YES,NBITS=1

#Correct wrong region extend
gdal_translate -a_ullr -180.0001389 80.0001389 180.0001389 -56.9998611 -co COMPRESS=CCITTRLE -co BIGTIFF=YES -co NBITS=1 waterLandHansenGFC2013.tif waterLandHansenGFC2013g.tif
mv waterLandHansenGFC2013g.tif waterLandHansenGFC2013.tif

#Correct wrong region extend of the map also in grass
r.region map=tmp e=180 w=180w
r.info map=tmp
#Resample to 0:00:01 by nearest neighbor
g.region raster=tmp
g.region res=0:00:01
r.resample input=tmp output=del --overwrite
g.region res=0:00:30
r.resamp.stats input=del output=tmp method=sum --overwrite
r.mapcalc "tmp = round(900 - tmp)" --overwrite

r.info tmp
echo "0 0:0:255
1 0:0:0
900 240:240:240" | r.colors map=tmp rules=-

r.out.gdal input=tmp format=GTiff type=UInt16 output=waterLandHansenGFC2013_30Sec.tif createopt=COMPRESS=LZMA,NUM_THREADS=7,TILED=YES

g.remove -f type=raster name=del,tmp

