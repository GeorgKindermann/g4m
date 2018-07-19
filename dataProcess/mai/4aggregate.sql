--psql tmp

create table tmp (x real, y real, country smallint, gridarea real, waterLand smallint, irrigation real, forest smallint, forestIrrigated smallint, EvergreenNeedleleaf smallint, EvergreenBroadleaf smallint, DeciduousNeedleleaf smallint, DeciduousBroadleaf smallint, EvergreenNeedleleafI smallint, EvergreenBroadleafI smallint, DeciduousNeedleleafI smallint, DeciduousBroadleafI smallint, nppEvergreenNeedleleaf real, nppEvergreenBroadleaf real, nppDeciduousNeedleleaf real, nppDeciduousBroadleaf real, nppEvergreenNeedleleafI real, nppEvergreenBroadleafI real, nppDeciduousNeedleleafI real, nppDeciduousBroadleafI real);
\copy tmp from 'baseData.csv' csv

CREATE INDEX tmp_idx_cntry ON tmp (country);

create table t1 (GADMID smallint,ISO char(3),NAME_ENGLI text,NAME_ISO text,NAME_FAO text,NAME_LOCAL text);
\copy t1 from '/data/vectordata/global/adminstrativeBoundaries/gadm/v1/countrynames.csv' csv header

--Flaechen in m2
select country, sum(gridarea) as land, sum(gridarea * forest/100.) as forest, sum(gridarea * forestIrrigated/100. * irrigation/100.) as irrigatedForest from tmp group by country order by country;

--npp stammholz (35% von gesamt npp) ohne bewaesserung
select country, sum(gridarea * forest * 0.35 * (nppEvergreenNeedleleaf*EvergreenNeedleleaf/100. + nppEvergreenBroadleaf*EvergreenBroadleaf/100. + nppDeciduousNeedleleaf*DeciduousNeedleleaf/100. + nppDeciduousBroadleaf*DeciduousBroadleaf/100.)) / sum(gridarea * forest) mai, sum(gridarea * forest) forest, sum(gridarea * (16.-waterLand)/16.) land, sum(gridarea) total from tmp where country=17 group by country ;


\copy (select country, gridarea, waterLand, forest from tmp where forest > 0) to calibArea.csv csv
--damit dann die Waldflaechen in 5calibArea.r kalibrieren


create table t2 (country smallint, coef real);
\copy t2 from 'calibFactorsS.csv' csv header
create table calib as select distinct country, coef from tmp left join t2 using(country);
update calib set coef=1 where coef is null;
drop table t2;

drop index calib_idx_cntry;
drop index t1_idx_cntry;
create index calib_idx_cntry ON calib using hash (country);
create index t1_idx_cntry ON t1 using hash (gadmid);
--btree, hash

select country, sum(gridarea * forest) forest, sum(gridarea * (1. - waterLand/16.) * (forest/100./(1. - waterLand/16.))^1.31528702547956)  from tmp where country=17 group by country;

select country, sum(gridarea * forest) forest, sum(gridarea * (1. - waterLand/16.) * (forest/100./(1. - waterLand/16.))^coef) from tmp left join calib using(country) where country=17 group by country;

--drop table t2;
create table t2 as select x as lat, y as lon, iso, round(gridarea * (1. - waterLand/16.) * (forest/100./(1. - waterLand/16.))^coef) as forestArea_m2, EvergreenNeedleleaf, EvergreenBroadleaf, DeciduousNeedleleaf, DeciduousBroadleaf, nppEvergreenNeedleleaf, nppEvergreenBroadleaf, nppDeciduousNeedleleaf, nppDeciduousBroadleaf, cast(evergreenneedleleaf+deciduousneedleleaf as smallint) neadle_share, cast(evergreenbroadleaf+deciduousbroadleaf as smallint) broad_share, cast(0.35 * (evergreenneedleleaf*nppevergreenneedleleaf+deciduousneedleleaf*nppdeciduousneedleleaf) as real) mai_neadle_tc_ha_year, cast(0.35 * (evergreenbroadleaf*nppevergreenbroadleaf + deciduousbroadleaf*nppdeciduousbroadleaf) as real) mai_broad_tc_ha_year from tmp left outer join calib using(country) left outer join t1 on country=GADMID;

--alter table t2 ADD PRIMARY KEY (lat,lon);
--CREATE INDEX t2_idx_ns ON t2 (neadle_share);
--CREATE INDEX t2_idx_bs ON t2 (broad_share);

\copy (select lat, lon, iso, forestarea_m2, neadle_share, broad_share, case when neadle_share > 0 then mai_neadle_tc_ha_year / neadle_share else null end as mai_neadle_tc_ha_year, case when broad_share > 0 then mai_broad_tc_ha_year / broad_share else null end as mai_broad_tc_ha_year from t2) to g4mData.csv csv header

--optische datentests in grass
--tail -n+2 g4mData.csv | r.in.xyz -i input=- output=del fs=, x=1 y=2 z=7 --overwrite
--r.colors map=del color=rainbow

--select iso, sum(forestarea_m2) as forest from t2 group by iso;




drop table t2;
drop table tmp;
drop table t1;
drop table calib;





