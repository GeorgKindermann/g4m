--psql tmp

create table tmp (country text, name text, year int, forest numeric, primforest numeric);
\copy tmp from 'primFor.csv' csv header

select distinct year from tmp order by year;

create table t1 as select distinct country, name, null as forest, null as primforest from tmp;
update t1 set forest = tmp.forest from tmp where t1.country = tmp.country and tmp.year = 2000 and t1.forest is null and tmp.forest > 0;
update t1 set forest = tmp.forest from tmp where t1.country = tmp.country and tmp.year = 2005 and t1.forest is null and tmp.forest > 0;
update t1 set forest = tmp.forest from tmp where t1.country = tmp.country and tmp.year = 2010 and t1.forest is null and tmp.forest > 0;
update t1 set forest = tmp.forest from tmp where t1.country = tmp.country and tmp.year = 1990 and t1.forest is null and tmp.forest > 0;
update t1 set forest = tmp.forest from tmp where t1.country = tmp.country and tmp.year = 2015 and t1.forest is null and tmp.forest > 0;
update t1 set forest = tmp.forest from tmp where t1.country = tmp.country and tmp.year = 9999 and t1.forest is null and tmp.forest > 0;

update t1 set primforest = tmp.primforest from tmp where t1.country = tmp.country and tmp.year = 2000 and t1.primforest is null and tmp.primforest >= 0;
update t1 set primforest = tmp.primforest from tmp where t1.country = tmp.country and tmp.year = 2005 and t1.primforest is null and tmp.primforest >= 0;
update t1 set primforest = tmp.primforest from tmp where t1.country = tmp.country and tmp.year = 2010 and t1.primforest is null and tmp.primforest >= 0;
update t1 set primforest = tmp.primforest from tmp where t1.country = tmp.country and tmp.year = 1990 and t1.primforest is null and tmp.primforest >= 0;
update t1 set primforest = tmp.primforest from tmp where t1.country = tmp.country and tmp.year = 2015 and t1.primforest is null and tmp.primforest >= 0;
update t1 set primforest = tmp.primforest from tmp where t1.country = tmp.country and tmp.year = 9999 and t1.primforest is null and tmp.primforest >= 0;

create table t2 (gadmid int, country text, x1 text, x2 text, x3 text, x4 text);
\copy t2 from '/data/vectordata/global/adminstrativeBoundaries/gadm/v1/countrynames.csv' csv header

\copy (select gadmid, forest, primforest from t1 left join t2 using (country) where forest is not null and primforest is not null) to '/tmp/faoFra2000.txt' csv delimiter ' '


drop table t1;
drop table t2;
drop table tmp;

