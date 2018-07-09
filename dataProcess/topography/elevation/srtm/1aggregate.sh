#Get srtm files - works for 1arcSec and 3acrSec
#https://earthexplorer.usgs.gov/
#https://lta.cr.usgs.gov/SRTM1Arc

#unzip using 10 cores
find . -name '*.zip' -print0 | xargs -0 -I {} -P 10 unzip {}

#Merge fist for all latitudes seperately using 8 threads
waitforjobs() {
    while test $(jobs -p | wc -w) -ge "$1"; do wait -n; done
}
for i in {00..90}
do
    echo $i
    waitforjobs 8
    gdal_merge.py -pct -co "BIGTIFF=YES" -co "COMPRESS=LZMA" -co "NUM_THREADS=7" -o srtmN${i}.tif N${i}*.hgt &
    gdal_merge.py -pct -co "BIGTIFF=YES" -co "COMPRESS=LZMA" -co "NUM_THREADS=7" -o srtmS${i}.tif S${i}*.hgt &
done

#merge the latitudes to one file
#For 1arcSec
nohup nice -9 gdal_merge.py -pct -co "TILED=YES" -co "BIGTIFF=YES" -co "COMPRESS=LZMA" -co "NUM_THREADS=7" -o srtm1ArcSec.tif srtm???.tif &

#For 3arcSec
nohup nice -9 gdal_merge.py -pct -co "TILED=YES" -co "BIGTIFF=YES" -co "COMPRESS=LZMA" -co "NUM_THREADS=7" -o srtm3ArcSec.tif srtm???.tif &
