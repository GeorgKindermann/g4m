gdal_translate -of AAIGrid ./Interchangeable_format/wise_30sec_v1.tif /tmp/out.asc
./3createMap.pl 0 >/tmp/ctPerHa.asc
./3createMap.pl 1 >/tmp/ntPerHa.asc
./3createMap.pl 2 >/tmp/h2oMmPerM.asc
cp /tmp/out.prj /tmp/ctPerHa.prj
cp /tmp/out.prj /tmp/ntPerHa.prj
cp /tmp/out.prj /tmp/h2oMmPerM.prj
gdal_translate -of GTiff -co "BIGTIFF=YES" -co "COMPRESS=DEFLATE" -co "TILED=YES" /tmp/ctPerHa.asc /tmp/ctPerHa.tif
gdal_translate -of GTiff -co "BIGTIFF=YES" -co "COMPRESS=DEFLATE" -co "TILED=YES" /tmp/ntPerHa.asc /tmp/ntPerHa.tif
gdal_translate -of GTiff -co "BIGTIFF=YES" -co "COMPRESS=DEFLATE" -co "TILED=YES" /tmp/h2oMmPerM.asc /tmp/h2oMmPerM.tif

