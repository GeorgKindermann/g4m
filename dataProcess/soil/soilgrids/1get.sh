#Soil organic carbon stock in tonnes per ha 
curl "ftp://ftp.soilgrids.org/data/recent/OCSTHA_M_sd[1-6]_250m.tif" -o "OCSTHA_M_sd#1_250m.tif"

#Volumetric water content at wilting point
curl "ftp://ftp.soilgrids.org/data/recent/WWP_M_sl[1-7]_250m.tif" -o "WWP_M_sl#1_250m.tif"

#Soil pH in H2O
curl "ftp://ftp.soilgrids.org/data/recent/PHIHOX_M_sl[1-7]_250m.tif" -o "PHIHOX_M_sl#1_250m.tif"

#Soil pH in KCl
curl "ftp://ftp.soilgrids.org/data/recent/PHIKCL_M_sl[1-7]_250m.tif" -o "PHIKCL_M_sl#1_250m.tif"

#Bulk density (kg m−3) of the fine earth fraction
curl "ftp://ftp.soilgrids.org/data/recent/BLDFIE_M_sl[1-7]_250m.tif" -o "BLDFIE_M_sl#1_250m.tif"

#Cation-exchange capacity (cmol + /kg)
curl "ftp://ftp.soilgrids.org/data/recent/CECSOL_M_sl[1-7]_250m.tif" -o "CECSOL_M_sl#1_250m.tif"

#Coarse fragments (volumetric %),
curl "ftp://ftp.soilgrids.org/data/recent/CRFVOL_M_sl[1-7]_250m.tif" -o "CRFVOL_M_sl#1_250m.tif"

#Depth to bedrock (cm)
curl "ftp://ftp.soilgrids.org/data/recent/BDRICM_M_250m.tif" -o "BDRICM_M_250m.tif"


#Deepths
#0 m
#0.05 m
#0.15 m
#0.30 m
#0.60 m
#1 m
#2 m

#https://files.isric.org/soilgrids/data/recent/

#Soil carbon
gdal_calc.py --co="BIGTIFF=YES" --co="COMPRESS=DEFLATE" --co="TILED=YES" -A OCSTHA_M_sd1_250m.tif -B OCSTHA_M_sd2_250m.tif -C OCSTHA_M_sd3_250m.tif -D OCSTHA_M_sd4_250m.tif -E OCSTHA_M_sd5_250m.tif -F OCSTHA_M_sd6_250m.tif --outfile=OCSTHA_sum.tif --calc="A+B+C+D+E+F"

#Cation-exchange capacity [cmol/m3]
for DEEPTH in {1..7}
do
    gdal_calc.py --co="BIGTIFF=YES" --co="COMPRESS=DEFLATE" --co="TILED=YES" -A CECSOL_M_sl${DEEPTH}_250m.tif -B BLDFIE_M_sl${DEEPTH}_250m.tif -C CRFVOL_M_sl${DEEPTH}_250m.tif --outfile=CEC_M_sl${DEEPTH}_250m.tif --calc="B * (1. - C/100.)*A" &
done
wait

#WWP..water content, PHIHOX..Soil pH in H2O, PHIKCL..Soil pH in KCl, CEC..Cation-exchange capacity
for VAL in WWP PHIHOX PHIKCL CEC
do
nice -19 gdal_calc.py --co="BIGTIFF=YES" --co="COMPRESS=DEFLATE" --co="TILED=YES" -A ${VAL}_M_sl1_250m.tif -B ${VAL}_M_sl2_250m.tif -C ${VAL}_M_sl3_250m.tif -D ${VAL}_M_sl4_250m.tif -E ${VAL}_M_sl5_250m.tif -F ${VAL}_M_sl6_250m.tif -G ${VAL}_M_sl7_250m.tif --outfile=${VAL}2m.tif --calc="(A*.05+B*.15+C*.25+D*.45+E*.7+F*1.4+G*1.)/4." &
nice -19 gdal_calc.py --co="BIGTIFF=YES" --co="COMPRESS=DEFLATE" --co="TILED=YES" -A ${VAL}_M_sl1_250m.tif -B ${VAL}_M_sl2_250m.tif -C ${VAL}_M_sl3_250m.tif -D ${VAL}_M_sl4_250m.tif -E ${VAL}_M_sl5_250m.tif -F ${VAL}_M_sl6_250m.tif --outfile=${VAL}1m.tif --calc="(A*.05+B*.15+C*.25+D*.45+E*.7+F*0.4)/2." &
nice -19 gdal_calc.py --co="BIGTIFF=YES" --co="COMPRESS=DEFLATE" --co="TILED=YES" -A ${VAL}_M_sl1_250m.tif -B ${VAL}_M_sl2_250m.tif -C ${VAL}_M_sl3_250m.tif -D ${VAL}_M_sl4_250m.tif -E ${VAL}_M_sl5_250m.tif --outfile=${VAL}06m.tif --calc="(A*.05+B*.15+C*.25+D*.45+E*.3)/1.2" &
nice -19 gdal_calc.py --co="BIGTIFF=YES" --co="COMPRESS=DEFLATE" --co="TILED=YES" -A ${VAL}_M_sl1_250m.tif -B ${VAL}_M_sl2_250m.tif -C ${VAL}_M_sl3_250m.tif -D ${VAL}_M_sl4_250m.tif --outfile=${VAL}03m.tif --calc="(A*.05+B*.15+C*.25+D*.15)/.6" &
nice -19 gdal_calc.py --co="BIGTIFF=YES" --co="COMPRESS=DEFLATE" --co="TILED=YES" -A ${VAL}_M_sl1_250m.tif -B ${VAL}_M_sl2_250m.tif -C ${VAL}_M_sl3_250m.tif --outfile=${VAL}015m.tif --calc="(A*.05+B*.15+C*.1)/.3" &
nice -19 gdal_calc.py --co="BIGTIFF=YES" --co="COMPRESS=DEFLATE" --co="TILED=YES" -A ${VAL}_M_sl1_250m.tif -B ${VAL}_M_sl2_250m.tif --outfile=${VAL}005m.tif --calc="(A+B)/2." &
wait
done

