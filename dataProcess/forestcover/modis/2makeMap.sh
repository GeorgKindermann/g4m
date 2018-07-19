mkdir ~/tmp/grass/
grass -e -c EPSG:4326 ~/tmp/grass/mylocation
grass -c ~/tmp/grass/mylocation/mymapset
#nohup grass ~/tmp/grass/mylocation/mymapset --exec bash 2makeMap.sh &

r.in.gdal input=/data/griddata/global/waterLand/modis/latLon/waterLandModis2000.tif output=del --overwrite
r.in.gdal input=forestCoverModisV62000LatLon.tif output=del1 --overwrite
r.in.gdal input=forestCoverModisV62010LatLon.tif output=del2 --overwrite

g.region raster=del1
r.mapcalc "del3 = if(del1>100, null(), if(del1==0 && del==1, null(), del1))" --overwrite &
r.mapcalc "del4 = if(del2>100, null(), if(del2==0 && del==1, null(), del2))" --overwrite &
wait

r.mapcalc "del5 = if(isnull(del3) && isnull(del4), 0, 1)" --overwrite

r.mapcalc "del3 = if(del5==0, 0, if(del1>100, 0, del1))" --overwrite &
r.mapcalc "del4 = if(del5==0, 0, if(del2>100, 0, del1))" --overwrite &
wait

r.mapcalc "del6 = max(del3, del4)" --overwrite

g.region n=89:59:30n s=89:59:30s w=180w e=180e res=0:00:30
r.resamp.stats input=del5 output=del7 method=sum --overwrite &
r.resamp.stats input=del6 output=del8 method=sum --overwrite &
r.resamp.stats input=del5 output=del9 method=count --overwrite &
wait

r.mapcalc "del10 = if(del7>0, round(del8/del7), 0)" --overwrite &
r.mapcalc "del11 = round(100.*del7/del9)" --overwrite &
wait

r.out.gdal -f input=del10 format=GTiff type=Byte output=forestShare2000Modis30Sec.tif createopt=COMPRESS=LZMA,BIGTIFF=YES,NUM_THREADS=7,TILED=YES
r.out.gdal -f input=del11 format=GTiff type=Byte output=landShareModis30Sec.tif createopt=COMPRESS=LZMA,BIGTIFF=YES,NUM_THREADS=7,TILED=YES

g.remove -f type=raster pattern=del*,tmp*
