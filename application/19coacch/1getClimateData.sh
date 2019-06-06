RCP=${1-rcp45}

cdo setmissval,-1 -mulc,2592000 -monmean -mergetime "/data/tmp/pr_*historical*2???????.nc4 /data/tmp/pr_*${RCP}*2???????.nc4" /tmp/dat_pr.nc
cdo setmissval,-1 -monmean -mergetime "/data/tmp/rsds_*historical*2???????.nc4 /data/tmp/rsds_*${RCP}*2???????.nc4" /tmp/dat_rsds.nc
cdo setmissval,-273 -subc,273.15 -monmean -mergetime "/data/tmp/tas_*historical*2???????.nc4 /data/tmp/tas_*${RCP}*2???????.nc4" /tmp/dat_tas.nc

cdo seldate,2000-01-01,2100-12-31 -merge '/tmp/dat_pr.nc /tmp/dat_rsds.nc /tmp/dat_tas.nc' /tmp/dat.nc
rm /tmp/dat_pr.nc /tmp/dat_rsds.nc /tmp/dat_tas.nc

for climVar in pr tas rsds
do
    cdo outputtab,lon,lat,nohead -selname,${climVar} -seltimestep,1 /tmp/dat.nc | awk '{printf "%s\t%s\n", $1, $2}' >/tmp/xgk00000.txt
    cdo outputtab,timestep,value,nohead -selname,${climVar} /tmp/dat.nc | awk '{print $2 > "/tmp/xgk"sprintf("%05i", $1)".txt"}'
    ls -1 /tmp/xgk?????.txt | split -l $(($(ulimit -n)-10)) -d - /tmp/lists
    for list in /tmp/lists*; do paste $(cat $list) > /tmp/merge${list##/tmp/lists}; done
    printf "#lon\tlat" >/tmp/${climVar}MonMean.txt
    cdo outputtab,year,month,nohead -selname,${climVar} -selindexbox,1,1,1,1 /tmp/dat.nc | awk '{printf "\t%i%02i", $1, $2 }' >>/tmp/${climVar}MonMean.txt
    printf "\n" >>/tmp/${climVar}MonMean.txt
    paste /tmp/merge* >>/tmp/${climVar}MonMean.txt
    rm /tmp/xgk?????.txt /tmp/lists* /tmp/merge*
done
rm /tmp/dat.nc

