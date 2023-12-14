#~/17/nppModel/1getData.grass

mkdir ~/tmp/grass/
grass -e -c EPSG:4326 ~/tmp/grass/mylocation
grass -c ~/tmp/grass/mylocation/mymapset
#nohup grass ~/tmp/grass/mylocation/mymapset --exec bash 1getData.sh &

#Forestype-0 Landcover
#valid_range:    0  254
#_FillValue:    255
#water:    0
###evergreen needleleaf forest:    1
###evergreen broadleaf forest:    2
###deciduous needleleaf forest:    3
###deciduous broadleaf forest:    4
##mixed forests:    5
#closed shrublands:    6
#open shrublands:    7
###woody savannas:    8
#savannas:    9
#grasslands:    10
#croplands:    12
#urban and built-up:    13
#barren or sparsely vegetated:    16
#unclassfied:    254
r.in.gdal /data/griddata/global/npp/runningSteve/MOD12Q1_UMD.tif output=del0 --overwrite
g.region raster=del0
r.mapcalc "del0 = if((del0>0 & del0<6) | del0==8, del0, null())" --overwrite #only forest area

#Ecoregion-1
###1..Tropical
###2..Subtropical
###3..Temperate
###4..Boreal
#5..Polar
#9..Water
#10..No data
r.in.gdal /data/griddata/global/ecoRegion/fra2000/ecoRegionFao4Regions.tif output=del1 --overwrite
r.mapcalc "del1 = if(del1>0 & del1<5, del1, null())" --overwrite

#Elevation-2
#m
r.in.gdal /data/griddata/global/topography/elevation/gtopo30/demGtopo30.tif output=del2 --overwrite

##Soil
#Phosphorus-3
r.in.gdal /data/griddata/global/soil/phosphorus/soilPhosphorusApa.tif output=del3_0 --overwrite
r.in.gdal /data/griddata/global/soil/phosphorus/soilPhosphorusLab.tif output=del3_1 --overwrite
r.in.gdal /data/griddata/global/soil/phosphorus/soilPhosphorusOcc.tif output=del3_2 --overwrite
r.in.gdal /data/griddata/global/soil/phosphorus/soilPhosphorusOrg.tif output=del3_3 --overwrite
r.in.gdal /data/griddata/global/soil/phosphorus/soilPhosphorusSec.tif output=del3_4 --overwrite
r.in.gdal /data/griddata/global/soil/phosphorus/soilPhosphorusTot.tif output=del3_5 --overwrite

#wise-4
r.in.gdal /data/griddata/global/soil/wise/ctPerHa.tif output=del4_0 --overwrite
r.in.gdal /data/griddata/global/soil/wise/h2oMmPerM.tif output=del4_1 --overwrite
r.in.gdal /data/griddata/global/soil/wise/ntPerHa.tif output=del4_2 --overwrite

#soilgrids-5
r.in.gdal /data/griddata/global/soil/soilgrids/OCSTHA_sum.tif output=del5_0 --overwrite #Soil carbon
r.in.gdal /data/griddata/global/soil/soilgrids/WWP1m.tif output=del5_1 --overwrite      #water content
r.in.gdal /data/griddata/global/soil/soilgrids/PHIHOX1m.tif output=del5_2 --overwrite   #Soil pH in H2O
r.in.gdal /data/griddata/global/soil/soilgrids/PHIKCL1m.tif output=del5_3 --overwrite   #Soil pH in KCl
r.in.gdal /data/griddata/global/soil/soilgrids/CEC1m.tif output=del5_4 --overwrite      #Cation-exchange capacity
g.region n=89:59:30N
for i in {0..4}; do
    r.resamp.stats input=del5_$i output=del5_$i --overwrite 
done

#Iiasa-6
g.region n=90:0:00N
r.in.gdal /data/griddata/global/soil/IIASA/soilAWC.tif output=del6_0 --overwrite
r.in.gdal /data/griddata/global/soil/IIASA/soilCACO3Sub.tif output=del6_1 --overwrite
r.in.gdal /data/griddata/global/soil/IIASA/soilCACO3Top.tif output=del6_2 --overwrite
r.in.gdal /data/griddata/global/soil/IIASA/soilCASO4Sub.tif output=del6_3 --overwrite
r.in.gdal /data/griddata/global/soil/IIASA/soilCASO4Top.tif output=del6_4 --overwrite
r.in.gdal /data/griddata/global/soil/IIASA/soilSalinitySub.tif output=del6_5 --overwrite
r.in.gdal /data/griddata/global/soil/IIASA/soilSalinityTop.tif output=del6_6 --overwrite
r.in.gdal /data/griddata/global/soil/IIASA/soilSodicitySub.tif output=del6_7 --overwrite
r.in.gdal /data/griddata/global/soil/IIASA/soilSodicityTop.tif output=del6_8 --overwrite
r.in.gdal /data/griddata/global/soil/IIASA/soilSWR.tif output=del6_9 --overwrite

r.mapcalc "del7 = row()" --overwrite
r.mapcalc "del8 = col()" --overwrite

nl -v0 -n'ln' /data/griddata/global/weather/terraClimate/MonMeanP.txt | tail -n +2 | sort -k 1b,1 >/data/tmp/MonMeanP.txt 
g.remove -f type=raster name=del9
g.region res=0:02:30
cut -f 1-3 /data/tmp/MonMeanP.txt | r.in.xyz input=- output=del9 separator=tab x=2 y=3 z=1
#r.grow input=del9 output=del9 radius=4.01 metric=maximum --overwrite

r.out.xyz input=del9,del7,del8,del0,del1,del2,del3_0,del3_1,del3_2,del3_3,del3_4,del3_5,del4_0,del4_1,del4_2,del5_0,del5_1,del5_2,del5_3,del5_4,del6_0,del6_1,del6_2,del6_3,del6_4,del6_5,del6_6,del6_7,del6_8,del6_9 separator="\\t" | sort -k 3b,3 >/data/tmp/g4mDataSite.txt

paste /data/griddata/global/weather/terraClimate/MonMeanP.txt <(cut -f 3- /data/griddata/global/weather/terraClimate/MonMeanR.txt) <(cut -f 3- /data/griddata/global/weather/terraClimate/MonMeanT.txt) <(cut -f 3- /data/griddata/global/weather/terraClimate/MonMeanTmin.txt) <(cut -f 3- /data/griddata/global/weather/terraClimate/MonMeanTmax.txt) | nl -v0 -n'ln' | tail -n +2  | sort -k 1b,1 -T /data/tmp/ >/data/tmp/MonMean.txt 

join -1 3 /data/tmp/g4mDataSite.txt <(cut -f 1,4- /data/tmp/MonMean.txt) >/data/tmp/g4mDataSiteClim.txt

g.remove -f type=raster pattern=del*

exit

rm -rf ~/tmp/grass/
rm /data/tmp/MonMean.txt
rm /data/tmp/g4mDataSite.txt

cut -d ' ' -f 2,3 /data/tmp/g4mDataSiteClim.txt > /tmp/xy.txt
for filename in /data/griddata/global/npp/modis/MOD17A2HGF006/psnNetMOD17A2HGF006*.tif; do
    cat /tmp/xy.txt | gdallocationinfo -wgs84 -valonly $filename >/data/tmp/npp_${filename: -11:7}
done
for filename in /data/griddata/global/npp/modis/MOD17A3HGF006/nppMOD17A3HGF006_*Sin.tif; do
    cat /tmp/xy.txt | gdallocationinfo -wgs84 -valonly $filename >/data/tmp/npp_${filename: -11:4}
done

paste /data/tmp/g4mDataSiteClim.txt /data/tmp/npp_???? /data/tmp/npp_????-?? >/data/tmp/g4mDataSiteClimNpp.txt

rm /data/tmp/g4mDataSiteClim.txt /data/tmp/npp_???? /data/tmp/npp_????-??


head /data/tmp/g4mDataSiteClimNpp.txt >/tmp/in.txt
R
x <- read.table("/tmp/in.txt")
colnames(x)[1:32] <- c("id","lon","lat","x","y","lc","er","nn","pApa","pLab","pOcc","pOrg","pSec","pTot","wc","ww","wn","sc","sw","sphh20","sphkcl","scec","iawc","icaco3Sub","icaco3Top","icaso4Sub","icaso4Top","isalinitySub","isalinityTop","isodicitySub","isodicityTop","iswr")
tt <- t(outer(2000:2019, sprintf("%02d", 1:12), paste0))
colnames(x)[33:272] <- paste0("p", tt)
colnames(x)[273:512] <- paste0("r", tt)
colnames(x)[513:752] <- paste0("t", tt)
colnames(x)[753:992] <- paste0("tMin", tt)
colnames(x)[993:1232] <- paste0("tMax", tt)
colnames(x)[1233:1252] <- paste0("npp", 2000:2019)
colnames(x)[1253:1492] <- paste0("npp", tt)

cat(colnames(x), "\n", file="/data/tmp/header.txt")
quit()


cat /data/tmp/header.txt /data/tmp/g4mDataSiteClimNpp.txt > /data/tmp/g4mNppModelData.txt
bzip2 g4mNppModelData.txt


awk '{ print > "/data/tmp/lc" $6 "er" $7 ".txt" }' /data/tmp/g4mDataSiteClimNpp.txt


get_fixed_random()
{
  openssl enc -aes-256-ctr -pass pass:"$1" -nosalt </dev/zero 2>/dev/null
}
shuf -n 1000000 --random-source=<(get_fixed_random 42) /data/tmp/lc2er1.txt >/tmp/tmp.tmp; cat /data/tmp/header.txt /tmp/tmp.tmp



#x <- read.table("/data/tmp/g4mDataSiteClimNpp.txt")
read.table(pipe("cut -d ' ' -f 1-5 /data/tmp/g4mDataSiteClimNpp.txt")

x <- read.table(pipe("head /data/tmp/g4mNppModelData.txt"), header=TRUE)
