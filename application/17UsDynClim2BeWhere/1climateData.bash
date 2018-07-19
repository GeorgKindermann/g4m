cdo subc,273.15 -selindexbox,377,674,203,332 -remapbil,r720x360 -mergetime "/data/griddata/usa/climate/cordex/tas_NAM-44_ICHEC-EC-EARTH_rcp26_r12i1p1_SMHI-RCA4_v1_mon_20[0-4]*.nc" /tmp/tas.nc
cdo mulc,2592000. -selindexbox,377,674,203,332 -remapbil,r720x360 -mergetime "/data/griddata/usa/climate/cordex/pr_NAM-44_ICHEC-EC-EARTH_rcp26_r12i1p1_SMHI-RCA4_v1_mon_20[0-4]*.nc" /tmp/pr.nc
cdo selindexbox,377,674,203,332 -remapbil,r720x360 -mergetime "/data/griddata/usa/climate/cordex/rsds_NAM-44_ICHEC-EC-EARTH_rcp26_r12i1p1_SMHI-RCA4_v1_mon_20[0-4]*.nc" /tmp/rsds.nc
