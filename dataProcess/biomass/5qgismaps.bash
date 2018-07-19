unzip -d /tmp/ /data/vectordata/global/adminstrativeBoundaries/gadm/v1/gadm_v1_lev0_shp.zip
ogr2ogr -simplify 0.03 /tmp/gadm1_lev0_simp.shp /tmp/gadm1_lev0.shp

#rm gadm1_lev0_simp.sqlite
ogr2ogr -f SQLite gadm1_lev0_simp.sqlite /tmp/gadm1_lev0_simp.shp


sqlite3 gadm1_lev0_simp.sqlite
--PRAGMA table_info(gadm1_lev0_simp);

create table fra (country int,area real,areaManNoirri real,areaUmanNoirri real,areaManIrri real,areaUmanIrri real,bm real,bmManNoirri real,bmUmanNoirri real,bmManIrri real,bmUmanIrri real,bmMax real,NAME_FAO varchar(20),ISO char(3),fraForest real,stock real,bmAg real,bmBg real,bmDe real,cAg real,cBg real,cDe real,cLi real,cLv real,cSo real);
.separator ","
.import "fra.csv" fra

alter table gadm1_lev0_simp add column stock FLOAT;
update gadm1_lev0_simp set stock = (select 1000. * stock/fraForest from fra where gadm1_lev0_simp.iso=fra.iso);

alter table gadm1_lev0_simp add column bmAg FLOAT;
update gadm1_lev0_simp set bmAg = (select 1000. * bmAg/fraForest from fra where gadm1_lev0_simp.iso=fra.iso);

alter table gadm1_lev0_simp add column bmBg FLOAT;
update gadm1_lev0_simp set bmBg = (select 1000. * bmBg/fraForest from fra where gadm1_lev0_simp.iso=fra.iso);

alter table gadm1_lev0_simp add column bmDe FLOAT;
update gadm1_lev0_simp set bmDe = (select 1000. * bmDe/fraForest from fra where gadm1_lev0_simp.iso=fra.iso);

alter table gadm1_lev0_simp add column cAg FLOAT;
update gadm1_lev0_simp set cAg = (select 1000. * cAg/fraForest from fra where gadm1_lev0_simp.iso=fra.iso);

alter table gadm1_lev0_simp add column cBg FLOAT;
update gadm1_lev0_simp set cBg = (select 1000. * cBg/fraForest from fra where gadm1_lev0_simp.iso=fra.iso);

alter table gadm1_lev0_simp add column cDe FLOAT;
update gadm1_lev0_simp set cDe = (select 1000. * cDe/fraForest from fra where gadm1_lev0_simp.iso=fra.iso);

alter table gadm1_lev0_simp add column cLi FLOAT;
update gadm1_lev0_simp set cLi = (select 1000. * cLi/fraForest from fra where gadm1_lev0_simp.iso=fra.iso);

alter table gadm1_lev0_simp add column cLv FLOAT;
update gadm1_lev0_simp set cLv = (select 1000. * cLv/fraForest from fra where gadm1_lev0_simp.iso=fra.iso);

alter table gadm1_lev0_simp add column cSo FLOAT;
update gadm1_lev0_simp set cSo = (select 1000. * cSo/fraForest from fra where gadm1_lev0_simp.iso=fra.iso);


alter table gadm1_lev0_simp add column stockPbmag FLOAT;
update gadm1_lev0_simp set stockPbmag = (select 100*stock/bmAg from fra where gadm1_lev0_simp.iso=fra.iso);

alter table gadm1_lev0_simp add column bmbgPbmag FLOAT;
update gadm1_lev0_simp set bmbgPbmag = (select 100*bmBg/bmAg from fra where gadm1_lev0_simp.iso=fra.iso);

alter table gadm1_lev0_simp add column bmdePbmag FLOAT;
update gadm1_lev0_simp set bmdePbmag = (select 100*bmDe/bmAg from fra where gadm1_lev0_simp.iso=fra.iso);

alter table gadm1_lev0_simp add column bmagPcag FLOAT;
update gadm1_lev0_simp set bmagPcag = (select 100*bmAg/cAg from fra where gadm1_lev0_simp.iso=fra.iso);


alter table gadm1_lev0_simp add column cbgPcag FLOAT;
update gadm1_lev0_simp set cbgPcag = (select 100*cBg/cAg from fra where gadm1_lev0_simp.iso=fra.iso);

alter table gadm1_lev0_simp add column cdePcag FLOAT;
update gadm1_lev0_simp set cdePcag = (select 100*cDe/cAg from fra where gadm1_lev0_simp.iso=fra.iso);

alter table gadm1_lev0_simp add column bmagPcag FLOAT;
update gadm1_lev0_simp set cliPcag = (select 100*cLi/cAg from fra where gadm1_lev0_simp.iso=fra.iso);

alter table gadm1_lev0_simp add column cLvPcag FLOAT;
update gadm1_lev0_simp set cLvPcag = (select 100*cLv/cAg from fra where gadm1_lev0_simp.iso=fra.iso);

alter table gadm1_lev0_simp add column cSoPcag FLOAT;
update gadm1_lev0_simp set cSoPcag = (select 100*cSo/cAg from fra where gadm1_lev0_simp.iso=fra.iso);
