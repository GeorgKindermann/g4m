#Need account to download global phosphorus map
#https://daac.ornl.gov/SOILS/guides/Global_Phosphorus_Dist_Map.html
#pforms_den.nc

wget https://files.isric.org/public/af250m_nutrient/af250m_nutrient_p_t_m_agg30cm.tif


gdal_translate -a_srs EPSG:4326 -co "TILED=YES" -co "COMPRESS=DEFLATE" -co "BIGTIFF=YES" NETCDF:"pforms_den.nc":tot soilPhosphorusTot.tif
gdal_translate -a_srs EPSG:4326 -co "TILED=YES" -co "COMPRESS=DEFLATE" -co "BIGTIFF=YES" NETCDF:"pforms_den.nc":lab soilPhosphorusLab.tif
gdal_translate -a_srs EPSG:4326 -co "TILED=YES" -co "COMPRESS=DEFLATE" -co "BIGTIFF=YES" NETCDF:"pforms_den.nc":org soilPhosphorusOrg.tif
gdal_translate -a_srs EPSG:4326 -co "TILED=YES" -co "COMPRESS=DEFLATE" -co "BIGTIFF=YES" NETCDF:"pforms_den.nc":occ soilPhosphorusOcc.tif
gdal_translate -a_srs EPSG:4326 -co "TILED=YES" -co "COMPRESS=DEFLATE" -co "BIGTIFF=YES" NETCDF:"pforms_den.nc":sec soilPhosphorusSec.tif
gdal_translate -a_srs EPSG:4326 -co "TILED=YES" -co "COMPRESS=DEFLATE" -co "BIGTIFF=YES" NETCDF:"pforms_den.nc":apa soilPhosphorusApa.tif

gdal_translate -co "TILED=YES" -co "COMPRESS=DEFLATE" -co "BIGTIFF=YES" af250m_nutrient_p_t_m_agg30cm.tif soilPhosphorusAfrica.tif

