wget ftp://geo10.elie.ucl.ac.be/CCI/ESACCI-LC-L4-WB-Map-150m-P13Y-2000-v4.0.tif

#This is somehow slow -> make it in Grass
#gdal_calc.py -A ./orig/ESACCI-LC-L4-WB-Map-150m-P13Y-2000-v4.0.tif --outfile=waterLandEsaV4.tif --calc="A-1" --co "BIGTIFF=YES" --co "COMPRESS=CCITTRLE" --co "NBITS=1"


#grass 30Sec
r.in.gdal input=./orig/ESACCI-LC-L4-WB-Map-150m-P13Y-2000-v4.0.tif output=tmp --overwrite
g.region raster=tmp
r.mapcalc "tmp = tmp - 1" --overwrite
r.out.gdal input=tmp format=GTiff type=Byte output=waterLandEsaV4.tif createopt=COMPRESS=CCITTRLE,BIGTIFF=YES,NBITS=1

g.region n=84:0:00n s=90:00:00s w=180w e=180e res=0:00:30
r.resamp.stats input=tmp output=del method=sum
r.mapcalc "del = round(36 - del)" --overwrite

r.info del
echo "0 0:0:255
1 0:0:0
36 240:240:240" | r.colors map=del rules=-

r.out.gdal input=del format=GTiff type=Byte output=waterLandEsaV4_30Sec.tif createopt=COMPRESS=LZMA,NUM_THREADS=7

g.remove -f type=raster name=del,tmp





