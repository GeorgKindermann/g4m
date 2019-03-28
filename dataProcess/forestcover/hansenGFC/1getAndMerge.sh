#treecover 2010 from Hansen Verson 3
wget https://edcintl.cr.usgs.gov/downloads/sciweb1/shared/gtc/downloads/treecover2010.tar.gz
tar -xzvf treecover2010.tar.gz
gdal_merge.py -pct -co "BIGTIFF=YES" -co "COMPRESS=DEFLATE" -co "TILED=YES" -o treecover2010HansenV3.tif ./treecover2010_v3/*_treecover2010_v3.tif
rm ./treecover2010_v3/ -r

#treecover 2000 from GFC-2018-v1.6
wget https://storage.googleapis.com/earthenginepartners-hansen/GFC-2018-v1.6/treecover2000.txt
wget -i treecover2000.txt
gdal_merge.py -pct -co "TILED=YES" -co "BIGTIFF=YES" -co "COMPRESS=DEFLATE" -o treecover2000HansenV16.tif Hansen_GFC-2018-v1.6_treecover2000_*.tif
rm Hansen_GFC-2018-v1.6_treecover2000_*.tif
rm treecover2000.txt

#Gain: 1 (gain) or 0 (no gain) in 2000–2012
wget https://storage.googleapis.com/earthenginepartners-hansen/GFC-2018-v1.6/gain.txt
wget -i gain.txt
gdal_merge.py -pct -co "TILED=YES" -co "BIGTIFF=YES" -co "COMPRESS=DEFLATE" -o treecoverGain20002012HansenV16.tif Hansen_GFC-2018-v1.6_gain_*.tif
rm Hansen_GFC-2018-v1.6_gain_*.tif
rm gain.txt

#Loss: 1–18 Year of forest cover loss (2001–2018) 0..no loss
wget https://storage.googleapis.com/earthenginepartners-hansen/GFC-2018-v1.6/lossyear.txt
wget -i lossyear.txt
gdal_merge.py -pct -co "TILED=YES" -co "BIGTIFF=YES" -co "COMPRESS=DEFLATE" -o treecoverlossyearHansenV16.tif Hansen_GFC-2018-v1.6_lossyear_*.tif
rm Hansen_GFC-2018-v1.6_lossyear_*.tif
rm lossyear.txt

#Mask: 0..no data, 1..mapped land surface, 2..permanent water bodies
wget https://storage.googleapis.com/earthenginepartners-hansen/GFC-2018-v1.6/datamask.txt
wget -i datamask.txt
gdal_merge.py -pct -co "TILED=YES" -co "BIGTIFF=YES" -co "COMPRESS=DEFLATE" -o landWaterHansenV16.tif Hansen_GFC-2018-v1.6_datamask_*.tif
rm Hansen_GFC-2018-v1.6_datamask_*.tif
rm datamask.txt
