--psql tmp

create table tmp (country text, name text, year int, forest numeric);
\copy tmp from 'areaYear.csv' csv header

select distinct year from tmp order by year;

create table t1 as select distinct country, name, null as forest from tmp;
update t1 set forest = tmp.forest from tmp where t1.country = tmp.country and tmp.year = 2000 and t1.forest is null and tmp.forest > 0;
update t1 set forest = tmp.forest from tmp where t1.country = tmp.country and tmp.year = 2005 and t1.forest is null and tmp.forest > 0;
update t1 set forest = tmp.forest from tmp where t1.country = tmp.country and tmp.year = 2010 and t1.forest is null and tmp.forest > 0;
update t1 set forest = tmp.forest from tmp where t1.country = tmp.country and tmp.year = 1990 and t1.forest is null and tmp.forest > 0;
update t1 set forest = tmp.forest from tmp where t1.country = tmp.country and tmp.year = 2015 and t1.forest is null and tmp.forest > 0;
update t1 set forest = tmp.forest from tmp where t1.country = tmp.country and tmp.year = 9999 and t1.forest is null and tmp.forest > 0;

\copy (select country, name, forest from t1 where forest is not null) to '/tmp/forestAreaFra2000.csv' csv header

drop table t1;
drop table tmp;

