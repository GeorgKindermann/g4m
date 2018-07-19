mkdir ~/tmp/grass/
grass -e -c EPSG:4326 ~/tmp/grass/mylocation
grass -c ~/tmp/grass/mylocation/mymapset
#nohup grass ~/tmp/grass/mylocation/mymapset --exec bash 2makeMap.sh &

r.in.gdal input=/data/griddata/global/waterLand/esa/waterLandEsaV4.tif output=del --overwrite
r.in.gdal input=forestCoverESA2000.tif output=del1 --overwrite
r.in.gdal input=forestCoverESA2010.tif output=del2 --overwrite

g.region raster=del1
r.resamp.stats input=del output=tmp method=sum
r.mapcalc "del = if(tmp==4, null(), (4-tmp)/4)" --overwrite

r.mapcalc "del1 = min(del*100,del1)" --overwrite
r.mapcalc "del2 = min(del*100,del2)" --overwrite

r.mapcalc "del3 = max(del1, del2)" --overwrite
g.region n=90n s=90:00:00s w=180w e=180e res=0:00:30
r.resamp.stats input=del3 output=del4 method=sum --overwrite
r.resamp.stats input=del output=del5 method=sum --overwrite
r.null map=del4 null=0
r.null map=del5 null=0
r.mapcalc "del6 = if(del5>0, round(del4/del5), 0)" --overwrite
r.mapcalc "del7 = round(100*del5/9)" --overwrite
r.out.gdal -f input=del6 format=GTiff type=Byte output=forestShare2000Esa30Sec.tif createopt=COMPRESS=LZMA,BIGTIFF=YES,NUM_THREADS=7,TILED=YES
r.out.gdal -f input=del7 format=GTiff type=Byte output=landShareEsa30Sec.tif createopt=COMPRESS=LZMA,BIGTIFF=YES,NUM_THREADS=7,TILED=YES

g.remove -f type=raster pattern=del*,tmp*
