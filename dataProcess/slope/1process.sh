#Extends, and resolution needs to be adjusted to the used data
#This is an example to calculate the slopes with srtm3ArcSec

gdal_translate -of ENVI -ot Int16 -a_nodata -9999 srtm3ArcSec.tif dem.bin
nohup ./2calc 432001 139201 -180.0004167 180.0004167 -56.0004167 60.0004167 -9999 0.5 dem.bin slp.bin asp.bin slpmin.bin slpmax.bin &

echo "BANDS:      1
ROWS:    278400
COLS:    864000" | tee slp.hdr slpmin.hdr slpmax.hdr asp.hdr

nohup gdal_translate -ot Byte -a_ullr -180 60 180 -56 -a_srs EPSG:4326 -co COMPRESS=LZMA -co NUM_THREADS=7 -co TILED=YES slp.bin slpSrtm3.tif &
nohup gdal_translate -ot Byte -a_ullr -180 60 180 -56 -a_srs EPSG:4326 -co COMPRESS=LZMA -co NUM_THREADS=7 -co TILED=YES slpmin.bin slpMinSrtm3.tif &
nohup gdal_translate -ot Byte -a_ullr -180 60 180 -56 -a_srs EPSG:4326 -co COMPRESS=LZMA -co NUM_THREADS=7 -co TILED=YES slpmax.bin slpMaxSrtm3.tif &
nohup gdal_translate -ot Byte -a_ullr -180 60 180 -56 -a_srs EPSG:4326 -co COMPRESS=LZMA -co NUM_THREADS=7 -co TILED=YES asp.bin aspSrtm3.tif &


nohup ./3aggregate 864000 278400 43200 13920 slpmin.bin slpmax.bin &

echo "BANDS:      1
ROWS:    13920
COLS:    43200" | tee slope1.hdr slope3.hdr slope6.hdr slope10.hdr slope15.hdr slope30.hdr slope50.hdr slopeL50.hdr slopeNULL.hdr

for i in slope1 slope3 slope6 slope10 slope15 slope30 slope50 slopeL50 slopeNULL
do
    echo $i
    gdal_translate -ot Byte -a_ullr -180 60 180 -56 -a_srs EPSG:4326 -co COMPRESS=LZMA -co NUM_THREADS=7 -co TILED=YES ${i}.bin ${i}Srtm3.tif
done
