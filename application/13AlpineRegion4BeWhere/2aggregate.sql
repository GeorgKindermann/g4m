--psql tmp

create table t1 (lat double precision, lon double precision, col int, row int, country smallint, gridarea real, waterLand smallint, irrigation real, forest smallint, forestIrrigated smallint, EvergreenNeedleleaf smallint, EvergreenBroadleaf smallint, DeciduousNeedleleaf smallint, DeciduousBroadleaf smallint, nppEvergreenNeedleleaf real, nppEvergreenBroadleaf real, nppDeciduousNeedleleaf real, nppDeciduousBroadleaf real, nppEvergreenNeedleleafI real, nppEvergreenBroadleafI real, nppDeciduousNeedleleafI real, nppDeciduousBroadleafI real, slp3 smallint, slp6 smallint, slp10 smallint, slp15 smallint, slp30 smallint, slp50 smallint, slpl50 smallint, cAbove_ha real);
\copy t1 from '/tmp/baseData.csv' csv
alter table t1 ADD PRIMARY KEY (col,row);

create table tmp (country smallint, coef real);
\copy tmp from '/home/georg/13/g4mData/npp/calibFactorsS.csv' csv header

create table t2 as select col, row, lat, lon, round(gridarea * (1. - waterLand/16.) * (forest/100./(1. - waterLand/16.))^coef) as forestArea_m2, case when forest>0. and waterLand<16 then cast(evergreenneedleleaf+deciduousneedleleaf as smallint) else 0 end as conif_share, cast(evergreenbroadleaf+deciduousbroadleaf as smallint) nonConif_share from t1 left outer join tmp using(country);
alter table t2 ADD PRIMARY KEY (col,row);

\copy (select col, row, t1.lat, t1.lon, forestarea_m2, conif_share, nonconif_share, case when conif_share > 0 then cast(0.35 * (evergreenneedleleaf*nppevergreenneedleleaf+deciduousneedleleaf*nppdeciduousneedleleaf) as real) / conif_share else null end as mai_conif_tc_ha_year, case when nonconif_share > 0 then cast(0.35 * (evergreenbroadleaf*nppevergreenbroadleaf + deciduousbroadleaf*nppdeciduousbroadleaf) as real) / nonconif_share else null end as mai_nonConif_tc_ha_year, slp3 as slope0_3deg, slp6 as slope3_6deg, slp10 as slope6_10deg, slp15 as slope10_15deg, slp30 as slope15_30deg, slp50 as slope30_50deg, slpl50 as slopelager50deg, cAbove_ha as cAboveTCperha from t1 left join t2 using (col, row)) to '/tmp/g4mAlps.csv' csv header


drop table t1;
drop table t2;
drop table tmp;
