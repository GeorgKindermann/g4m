grass --tmp-location EPSG:4326 --text

#Elevation-0
#m
r.in.gdal /data/griddata/global/topography/elevation/gtopo30/demGtopo30.tif output=del0

##Soil
#Phosphorus-1
r.in.gdal /data/griddata/global/soil/phosphorus/soilPhosphorusSec.tif output=del1

#wise-2
#Wasserspeicherfaehigkeit des Bodens
r.in.gdal -l /data/griddata/global/soil/wise/h2oMmPerM.tif output=del2
r.region -a map=del2
g.region raster=del2
r.mapcalc "del2 = if(isnull(del0), null(), del2)" --overwrite

#wise-3
#Stickstoff
r.in.gdal -l /data/griddata/global/soil/wise/ntPerHa.tif output=del3
r.region -a map=del3
g.region raster=del3
r.mapcalc "del3 = if(isnull(del0), null(), del3)" --overwrite

#soilgrids-4
#Soil pH in H2O * 10
r.in.gdal -l /data/griddata/global/soil/soilgrids/PHIHOX1m.tif output=del4
r.region -a map=del4
g.region raster=del4
r.mapcalc "del4 = del4 / 10." --overwrite

#Iiasa-5
#Salinity
r.in.gdal /data/griddata/global/soil/IIASA/soilSalinitySub.tif output=del5_1
r.in.gdal /data/griddata/global/soil/IIASA/soilSalinityTop.tif output=del5_2
g.region raster=del5_1
r.mapcalc "del5 = if(isnull(del0), null(), (del5_1 + del5_2) / 2)"
g.remove -f type=raster name=del5_1
g.remove -f type=raster name=del5_2

#prec6_1-12
cdo -f nc4c -z zip ymonmean /data/griddata/global/weather/terraClimate/TerraClimate_ppt.nc /tmp/tmp.nc
ncpdq --unpack /tmp/tmp.nc /tmp/ppt.nc
rm /tmp/tmp.nc
r.in.gdal -o input=NETCDF:"/tmp/ppt.nc":ppt output=del6 --overwrite

#temp7_1-12
cdo -f nc4c -z zip ymonmean /data/griddata/global/weather/terraClimate/TerraClimate_t.nc /tmp/tmp.nc
ncpdq --unpack /tmp/tmp.nc /tmp/t.nc
rm /tmp/tmp.nc
r.in.gdal -o input=NETCDF:"/tmp/t.nc":t output=del7 --overwrite

#radi8_1-12
cdo -f nc4c -z zip ymonmean /data/griddata/global/weather/terraClimate/TerraClimate_srad.nc /tmp/tmp.nc
ncpdq --unpack /tmp/tmp.nc /tmp/srad.nc
rm /tmp/tmp.nc
r.in.gdal -o input=NETCDF:"/tmp/srad.nc":srad output=del8 --overwrite

g.region n=84N s=56S w=180W e=180E
g.region res=0:30:00
r.region -a map=del4
r.resamp.stats input=del4 output=del4 --overwrite

g.region raster=del1
g.region res=0:30:00
for i in {0..5}; do
    r.resamp.stats input=del$i output=del$i --overwrite 
done
for i in {6..8}; do
    for mon in {1..12}; do
	r.resamp.stats input=del${i}.$mon output=del${i}.$mon --overwrite
    done
done
for i in {1..5}; do
    r.grow input=del$i output=del$i radius=360 metric=maximum --overwrite
    r.mapcalc "del$i = if(isnull(del0), null(), if(isnull(del$i), 0, del$i))" --overwrite
done
r.mapcalc "del4 = if(del4 < 1, 7, del4)" --overwrite
for i in {6..8}; do
    for mon in {1..12}; do
	r.grow input=del${i}.$mon output=del${i}.$mon radius=360 metric=maximum --overwrite
	r.mapcalc "del${i}_$mon = if(isnull(del0), null(), del${i}.$mon)" --overwrite
    done
done

g.region n=90n s=60s w=180w e=180e res=0:30

r.out.xyz input=del0,del1,del2,del3,del4,del5,del6.1,del6.2,del6.3,del6.4,del6.5,del6.6,del6.7,del6.8,del6.9,del6.10,del6.11,del6.12,del7.1,del7.2,del7.3,del7.4,del7.5,del7.6,del7.7,del7.8,del7.9,del7.10,del7.11,del7.12,del8.1,del8.2,del8.3,del8.4,del8.5,del8.6,del8.7,del8.8,del8.9,del8.10,del8.11,del8.12 separator="\\t" >/tmp/baseData30.txt
