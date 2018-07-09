cd ~
touch .netrc
echo "machine urs.earthdata.nasa.gov login uid_goes_here password password_goes_here" > .netrc
chmod 0600 .netrc
cd ~
touch .urs_cookies

for i in $(curl -s https://e4ftl01.cr.usgs.gov/MOLT/MOD44B.006/ | grep -Po '(?<=href=")[^"]*' | grep -P "^\d{4}\.\d{2}\.\d{2}/"); do #Go to all years
    echo $i
    for j in $(curl -s https://e4ftl01.cr.usgs.gov/MOLT/MOD44B.006/$i | grep -Po '(?<=href=")[^"]*' | grep ".hdf$"); do #Get all files of this year
	curl -b ~/.urs_cookies -c ~/.urs_cookies -L -n -O https://e4ftl01.cr.usgs.gov/MOLT/MOD44B.006/$i/$j
    done
    find . -name '*.hdf' -print0 | xargs -0 -I {} -P 10 gdal_translate -of GTiff "HDF4_EOS:EOS_GRID:{}:MOD44B_250m_GRID:Percent_Tree_Cover" {}.tif
    rm MOD44B.*.hdf
    gdal_merge.py -o forestCoverModisV6${i:0:4}Sin.tif -co "BIGTIFF=YES" -co "COMPRESS=LZMA" -co "NUM_THREADS=7" -co "TILED=YES" MOD44B.*.tif
    gdalwarp -t_srs EPSG:4326 -co "BIGTIFF=YES" -co "COMPRESS=LZMA" -co "NUM_THREADS=7" -co "TILED=YES" forestCoverModisV6${i:0:4}Sin.tif forestCoverModisV6${i:0:4}LatLon.tif
    rm MOD44B.*.tif
done
