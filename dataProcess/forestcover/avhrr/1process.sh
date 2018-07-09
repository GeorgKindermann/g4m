#Start as background job with:
#nohup grass ~/tmp/grass/mylocation/mymapset --exec bash 1process.sh &

wget ftp://ftp.glcf.umd.edu/glcf/Continuous_Fields_Tree_Cover/Global/gl-latlong-treecover/gl-latlong-treecover.bin.gz
wget ftp://ftp.glcf.umd.edu/glcf/Continuous_Fields_Tree_Cover/Global/gl-latlong-treecover/gl-latlong-broadleaf.bin.gz
wget ftp://ftp.glcf.umd.edu/glcf/Continuous_Fields_Tree_Cover/Global/gl-latlong-treecover/gl-latlong-deciduous.bin.gz
wget ftp://ftp.glcf.umd.edu/glcf/Continuous_Fields_Tree_Cover/Global/gl-latlong-treecover/gl-latlong-evergreen.bin.gz
wget ftp://ftp.glcf.umd.edu/glcf/Continuous_Fields_Tree_Cover/Global/gl-latlong-treecover/gl-latlong-needleleaf.bin.gz
wget ftp://ftp.glcf.umd.edu/glcf/Continuous_Fields_Tree_Cover/Global/gl-latlong-treecover/gl-latlong-treecover.glcf

gunzip *.gz

mkdir ~/tmp/grass/
grass -e -c EPSG:4326 ~/tmp/grass/mylocation
grass -c ~/tmp/grass/mylocation/mymapset

r.in.bin north=90 south=-90 east=180 west=-180 rows=21600 cols=43200 bytes=1 input=gl-latlong-treecover.bin output=tmp --overwrite
g.region raster=tmp
r.mapcalc "tmp = round(if(tmp==254,0,if(tmp==255,5,tmp/0.8)))" --overwrite
r.out.gdal input=tmp format=GTiff type=Byte output=avhrrForestCover30Sec.tif createopt=COMPRESS=LZMA,NUM_THREADS=7,TILED=YES

r.in.bin north=90 south=-90 east=180 west=-180 rows=21600 cols=43200 bytes=1 input=gl-latlong-broadleaf.bin output=tmp1 --overwrite &
r.in.bin north=90 south=-90 east=180 west=-180 rows=21600 cols=43200 bytes=1 input=gl-latlong-needleleaf.bin output=tmp2 --overwrite &

wait

r.mapcalc "tmp1 = round(if(tmp1==254,0,if(tmp1==255,5,tmp1/0.8)))" --overwrite &
r.mapcalc "tmp2 = round(if(tmp2==254,0,if(tmp2==255,5,tmp2/0.8)))" --overwrite &

wait

r.mapcalc "tmp = tmp1 + tmp2" --overwrite

r.mapcalc "tmp1 = if(tmp>0, round(double(tmp1)/double(tmp)*100.), 0)" --overwrite &
r.mapcalc "tmp2 = if(tmp>0, round(double(tmp2)/double(tmp)*100.), 0)" --overwrite &

wait

r.mapcalc "tmp = tmp1 + tmp2" --overwrite
r.info map=tmp
r.mapcalc "tmp = if(tmp > 0, 100 - tmp, 0)" --overwrite

r.mapcalc "tmp1x = if(tmp != 0 && tmp1 >= tmp2, tmp1 + tmp, tmp1)" --overwrite &
r.mapcalc "tmp2x = if(tmp != 0 && tmp2 > tmp1, tmp2 + tmp, tmp2)" --overwrite &

wait

r.mapcalc "tmp = tmp1x + tmp2x" --overwrite
r.info map=tmp

r.out.gdal input=tmp1x format=GTiff type=Byte output=avhrrForestCoverBroadleaf30Sec.tif createopt=COMPRESS=LZMA,NUM_THREADS=7,TILED=YES
r.out.gdal input=tmp2x format=GTiff type=Byte output=avhrrForestCoverNeedleleaf.tif createopt=COMPRESS=LZMA,NUM_THREADS=7,TILED=YES


r.in.bin north=90 south=-90 east=180 west=-180 rows=21600 cols=43200 bytes=1 input=gl-latlong-deciduous.bin output=tmp1 --overwrite &
r.in.bin north=90 south=-90 east=180 west=-180 rows=21600 cols=43200 bytes=1 input=gl-latlong-evergreen.bin output=tmp2 --overwrite &
wait

r.mapcalc "tmp1 = round(if(tmp1==254,0,if(tmp1==255,5,tmp1/0.8)))" --overwrite &
r.mapcalc "tmp2 = round(if(tmp2==254,0,if(tmp2==255,5,tmp2/0.8)))" --overwrite &

wait

r.mapcalc "tmp = tmp1 + tmp2" --overwrite

r.mapcalc "tmp1 = if(tmp>0, round(double(tmp1)/double(tmp)*100.), 0)" --overwrite &
r.mapcalc "tmp2 = if(tmp>0, round(double(tmp2)/double(tmp)*100.), 0)" --overwrite &

wait

r.mapcalc "tmp = tmp1 + tmp2" --overwrite
r.info map=tmp
r.mapcalc "tmp = if(tmp > 0, 100 - tmp, 0)" --overwrite

r.mapcalc "tmp1x = if(tmp != 0 && tmp1 >= tmp2, tmp1 + tmp, tmp1)" --overwrite &
r.mapcalc "tmp2x = if(tmp != 0 && tmp2 > tmp1, tmp2 + tmp, tmp2)" --overwrite &

wait

r.mapcalc "tmp = tmp1x + tmp2x" --overwrite
r.info map=tmp

r.out.gdal input=tmp1x format=GTiff type=Byte output=avhrrForestCoverDeciduous.tif createopt=COMPRESS=LZMA,NUM_THREADS=7,TILED=YES
r.out.gdal input=tmp2x format=GTiff type=Byte output=avhrrForestCoverEvergreen.tif createopt=COMPRESS=LZMA,NUM_THREADS=7,TILED=YES
