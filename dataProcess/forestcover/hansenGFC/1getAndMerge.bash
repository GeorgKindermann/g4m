#treecover 2010 from Hansen Verson 3
wget https://edcintl.cr.usgs.gov/downloads/sciweb1/shared/gtc/downloads/treecover2010.tar.gz
nohup nice -9 tar -xzvf treecover2010.tar.gz &
nohup nice -9 gdal_merge.py -pct -co "BIGTIFF=YES" -co "COMPRESS=LZMA" -co "NUM_THREADS=7" -o treecover2010Hansenv3m.tif ./treecover2010_v3/*_treecover2010_v3.tif &
nohup gdal_translate -co "TILED=YES" -co "BIGTIFF=YES" -co "COMPRESS=LZMA" -co "NUM_THREADS=7" treecover2010Hansenv3m.tif treecover2010Hansenv3.tif &

#treecover 2000 from GFC-2016-v1.4
wget https://storage.googleapis.com/earthenginepartners-hansen/GFC-2016-v1.4/treecover2000.txt
wget -i treecover2000.txt
nohup nice -9 gdal_merge.py -pct -co "BIGTIFF=YES" -co "COMPRESS=LZMA" -co "NUM_THREADS=7" -o treecover2000Hansenv14m.tif Hansen_GFC-2016-v1.4_treecover2000_*.tif &
nohup gdal_translate -co "TILED=YES" -co "BIGTIFF=YES" -co "COMPRESS=LZMA" -co "NUM_THREADS=7" treecover2000Hansenv14m.tif treecover2000Hansenv14.tif

#Gain: 1 (gain) or 0 (no gain) in 2000–2012
wget https://storage.googleapis.com/earthenginepartners-hansen/GFC-2016-v1.4/gain.txt
wget -i gain.txt
gdal_merge.py -pct -co "BIGTIFF=YES" -co "COMPRESS=CCITTFAX4" -o treecoverGain20002012Hansenv14.tif Hansen_GFC-2016-v1.4_gain_*.tif
nohup nice -9 gdal_merge.py -pct -co "BIGTIFF=YES" -co "COMPRESS=LZMA" -co "NUM_THREADS=7" -o treecoverGain20002012Hansenv14m.tif Hansen_GFC-2016-v1.4_gain_*.tif &
rm Hansen_GFC-2016-v1.4_gain_*.tif
nohup gdal_translate -co "NBITS=1" -co "BIGTIFF=YES" -co "COMPRESS=CCITTFAX4" treecoverGain20002012Hansenv14m.tif treecoverGain20002012Hansenv14.tif &
rm treecoverGain20002012Hansenv14m.tif

#Loss: 1–16 Year of forest cover loss (2001–2016) 0..no loss
wget https://storage.googleapis.com/earthenginepartners-hansen/GFC-2016-v1.4/lossyear.txt
wget -i lossyear.txt
nohup nice -9 gdal_merge.py -pct -co "BIGTIFF=YES" -co "COMPRESS=LZMA" -co "NUM_THREADS=7" -o treecoverlossyearHansenv14m.tif Hansen_GFC-2016-v1.4_lossyear_*.tif &
nohup gdal_translate -co "TILED=YES" -co "BIGTIFF=YES" -co "COMPRESS=LZMA" -co "NUM_THREADS=7" treecoverlossyearHansenv14.tif treecoverlossyearHansenv14.tif
rm Hansen_GFC-2016-v1.4_lossyear_*.tif

