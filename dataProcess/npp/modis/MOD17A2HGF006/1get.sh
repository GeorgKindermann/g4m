touch .netrc
echo "machine urs.earthdata.nasa.gov login uid_goes_here password password_goes_here" > .netrc
chmod 0600 .netrc
touch .urs_cookies

LINK=https://e4ftl01.cr.usgs.gov/MOLT/MOD17A2HGF.006/
for i in $(curl -s $LINK/ | grep -Po '(?<=href=")[^"]*' | grep -P "^\d{4}\.\d{2}\.\d{2}/"); do #Go to all folders
    if ((${i:0:4} >= 2000 && ${i:0:4} <= 2019));
    then
	echo $i
	for j in $(curl -s $LINK/$i | grep -Po '(?<=href=")[^"]*' | grep ".hdf$"); do #Get all files of this year
	    curl -s -b .urs_cookies -c .urs_cookies -L -n -O $LINK/$i/$j
	done
	find . -name '*.hdf' -print0 | xargs -0 -I {} -P 10 gdal_translate -co "COMPRESS=ZSTD" -co "PREDICTOR=2" -of GTiff "HDF4_EOS:EOS_GRID:{}:MOD_Grid_MOD17A2H:PsnNet_500m" {}.tif
	rm MOD17A2HGF.*.hdf
	gdal_merge.py -init 32767 -o psnNetMOD17A2HGF006_${i:0:10}Sin.tif -co "BIGTIFF=YES" -co "COMPRESS=ZSTD" -co "TILED=YES" -co "PREDICTOR=2"  MOD17A2HGF.*.tif
	rm MOD17A2HGF.*.tif
    fi
done
