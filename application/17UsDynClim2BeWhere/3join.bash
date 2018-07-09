./2getTempPrec /tmp/tas.nc tas |sort -k 1b,1 >/tmp/s1.txt
./2getTempPrec /tmp/pr.nc pr |sort -k 1b,1 >/tmp/s2.txt

#echo "lat lon year month t p" >/tmp/jo.txt
join /tmp/s1.txt /tmp/s2.txt | sed 's/_/ /g' | awk '{printf "%s %s %i %i %.1f %.0f\n", $1, $2-360, 2006 + $3/12, 1 + $3%12, $4, $5}' | sort -k 1g,1  -k 2g,2 -k 3g,3 -k 4g,4 >/tmp/jo.txt
cut -d ' ' -f 1,2 /tmp/jo.txt |uniq | awk '{print $1, $2, $1}' >/tmp/positions1.txt
cut -d ' ' -f 1,2 /tmp/jo.txt |uniq | awk '{print $1, $2, $2}' >/tmp/positions2.txt

#grass #halfDeg
#r.in.xyz input=/tmp/positions.txt x=2 y=1 z=3 output=del separator=" " skip=1 --overwrite
#g.region zoom=del



grass #30sec

g.region n=90n s=90s w=180w e=180e res=0:00:30

#Forestype-0
r.in.gdal input=/home/georg/13/g4mData/forestType/type_DeciduousBroadleafForest_g4m_base.tif output=del0_0 --overwrite
r.in.gdal input=/home/georg/13/g4mData/forestType/type_DeciduousNeedleleafForest_g4m_base.tif output=del0_1 --overwrite
r.in.gdal input=/home/georg/13/g4mData/forestType/type_EvergreenBroadleafForest_g4m_base.tif output=del0_2 --overwrite
r.in.gdal input=/home/georg/13/g4mData/forestType/type_EvergreenNeedleleafForest_g4m_base.tif output=del0_3 --overwrite

#soilIiasaFao90-4
r.in.gdal input=/data/griddata/global/soil/IIASA/soilFAO90.tif output=del4 --overwrite

#soilIiasaSwr-5
r.in.gdal input=/data/griddata/global/soil/IIASA/soilSwr.tif output=del5 --overwrite
r.null map=del5 null=-1

#soilIiasaAwc-6
r.in.gdal input=/data/griddata/global/soil/IIASA/soilAwc.tif output=del6 --overwrite
r.null map=del6 null=-1

#elevation-7
r.in.gdal input=/data/griddata/global/topography/elevation/gtopo30/demGtopo30.tif output=del7 --overwrite

#Forest area-8
g.region w=0 e=0:00:30E n=90n s=90s
r.mapcalc "del = row()" --overwrite
r.report -e -h map=del units=me | tail -n+5 | head -n-3 | cut -f 4 -d \| >/tmp/tmp.txt
sed -i '1i north: 90N\nsouth: 90S\neast: 180E\nwest: 180W\nrows: 21600\ncols: 1' /tmp/tmp.txt
r.in.ascii input=/tmp/tmp.txt output=del --overwrite
g.region n=90n s=90s w=180w e=180e res=0:00:30
r.mapcalc "del8 = round(del * (16 - waterLandModis30Sec)/16.) * forestCoverG4mCalib2015/100."

#Slope-9
r.in.gdal input=/data/griddata/global/topography/slope/gls/slope3.tif output=del9_0 --overwrite &
r.in.gdal input=/data/griddata/global/topography/slope/gls/slope6.tif output=del9_1 --overwrite &
r.in.gdal input=/data/griddata/global/topography/slope/gls/slope10.tif output=del9_2 --overwrite &
r.in.gdal input=/data/griddata/global/topography/slope/gls/slope15.tif output=del9_3 --overwrite &
r.in.gdal input=/data/griddata/global/topography/slope/gls/slope30.tif output=del9_4 --overwrite &
r.in.gdal input=/data/griddata/global/topography/slope/gls/slope50.tif output=del9_5 --overwrite &
r.in.gdal input=/data/griddata/global/topography/slope/gls/slopeL50.tif output=del9_6 --overwrite &

#cStem_ha-10
r.in.gdal input=/home/georg/16/g4m/biomassmap/cStem_ha.tif output=del10 --overwrite

g.region n=90n s=90s w=180w e=180e res=0:30
r.in.xyz input=/tmp/positions1.txt x=2 y=1 z=3 output=del1 separator=" " skip=0 --overwrite
r.in.xyz input=/tmp/positions2.txt x=2 y=1 z=3 output=del2 separator=" " skip=0 --overwrite
g.region zoom=del1
g.region res=0:00:30

r.mapcalc "del3 = if(waterLandModis30Sec == 16, null(), waterLandModis30Sec)" --overwrite

r.out.xyz input=del1,del2,del3,del0_0,del0_1,del0_2,del0_3,del4,del5,del6,del7,del8,del9_0,del9_1,del9_2,del9_3,del9_4,del9_5,del9_6,del10 separator="\\t" | sort -k 3g,3 -k 4g,4  >/data/tmp/baseData.txt

g.remove -f type=raster pattern=del*



#Climate Data only on Points with land information
join <(cut -f 3,4 /data/tmp/baseData.txt |uniq |sed 's/\t/_/' | sort -k 1,1) <(cat /tmp/jo.txt |sed 's/ /_/' | sort -k 1,1) |sed 's/_/ /' | sort -k 1g,1  -k 2g,2 -k 3g,3 -k 4g,4 >/data/tmp/baseDataClima.txt


#Bring first line to end of file to process all grids
head /data/tmp/baseDataClima.txt -n1 >>/data/tmp/baseDataClima.txt
head /data/tmp/baseData.txt -n1 >>/data/tmp/baseData.txt
