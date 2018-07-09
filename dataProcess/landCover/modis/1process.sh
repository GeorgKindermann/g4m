cd ~
touch .netrc
echo "machine urs.earthdata.nasa.gov login uid_goes_here password password_goes_here" > .netrc
chmod 0600 .netrc
cd ~
touch .urs_cookies

for i in $(curl -s https://e4ftl01.cr.usgs.gov/MOTA/MCD12Q1.006/ | grep -Po '(?<=href=")[^"]*' | grep -P "^\d{4}\.\d{2}\.\d{2}/"); do #Go to all years
    echo $i
    for j in $(curl -s https://e4ftl01.cr.usgs.gov/MOTA/MCD12Q1.006/$i | grep -Po '(?<=href=")[^"]*' | grep ".hdf$"); do #Get all files of this year
	curl -b ~/.urs_cookies -c ~/.urs_cookies -L -n -O https://e4ftl01.cr.usgs.gov/MOTA/MCD12Q1.006/$i/$j
    done
    find . -name '*.hdf' -print0 | xargs -0 -I {} -P 10 gdal_translate -of GTiff "HDF4_EOS:EOS_GRID:{}:MCD12Q1:LC_Type4" {}.tif
    rm MCD12Q1.*.hdf
    gdal_merge.py -o landCoverModisBgcV6${i:0:4}Sin.tif -co "BIGTIFF=YES" -co "COMPRESS=LZMA" -co "NUM_THREADS=7" -co "TILED=YES" MCD12Q1.*.tif
    gdalwarp -t_srs EPSG:4326 -co "BIGTIFF=YES" -co "COMPRESS=LZMA" -co "NUM_THREADS=7" -co "TILED=YES" landCoverModisBgcV6${i:0:4}Sin.tif landCoverModisBgcV6${i:0:4}LatLon.tif
    rm MCD12Q1.*.tif
done

0: Water Bodies -- At least 60% of area is covered by permanent water bodies.
1: Evergreen Needleleaf Vegetation -- Dominated by evergreen conifer trees and shrubs (>1m). Woody vegetation cover >10%.
2: Evergreen Broadleaf Vegetation -- Dominated by evergreen broadleaf and palmate trees and shrubs (>1m). Woody vegetation cover >10%.
3: Deciduous Needleleaf Vegetation -- Dominated by deciduous needleleaf (larch) trees and shrubs (>1m). Woody vegetation cover >10%.
4: Deciduous Broadleaf Vegetation -- Dominated by deciduous broadleaf trees and shrubs (>1m). Woody vegetation cover >10%.
5: Annual Broadleaf Vegetation -- Dominated by herbaceous annuals (<2m). At least 60% cultivated broadleaf crops.
6: Annual Grass Vegetation -- Dominated by herbaceous annuals (<2m) including cereal croplands.
7: Non-Vegetated Lands -- At least 60% of area is non-vegetated barren (sand, rock, soil) or permanent snow/ice with less than 10% vegetation.
8: Urban and Built-up Lands -- At least 30% impervious surface area including building materials, asphalt, and vehicles.
255: Unclassifed -- Has not received a map label because of missing inputs.
