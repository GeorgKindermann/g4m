#https://github.com/AndGem/osm_extract_polygon

./osm_extract_polygon -x 2 -m 2 -f planet-220328.osm.pbf

mkdir countrys
grass --tmp-location -c epsg:4326 --text

for COUNTRY in ./planet-220328.osm.pbf_polygons/*.poly
do
rm tmpShp*.txt
awk -F'[_\t]' '/^area/{file="tmpShp"$2".txt"} !/[a-zA-Z]/ && NR>1{print > file}' "$COUNTRY"

for FILE in tmpShp[0-9]*.txt
do
    echo -n "B " >tmpShp.txt
    wc -l <$FILE >>tmpShp.txt
    cat $FILE >>tmpShp.txt
    v.in.ascii -zn in=tmpShp.txt out=${FILE%.txt} format=standard --overwrite
    v.centroids input=${FILE%.txt} output=C${FILE%.txt} --overwrite
done

MAPS=($(g.list type=vector pattern=CtmpShp*))
g.rename vector=${MAPS[0]},country --overwrite
unset MAPS[0]
for MAP in ${MAPS[@]}
do
    v.overlay -t ainput=$MAP binput=country operator=xor output=tmp --overwrite
    g.rename vector=tmp,country --overwrite
done

v.category input=country output=tmp option=del cat=-1 --overwrite
v.category input=tmp output=country option=add cat=1 step=0 --overwrite

v.db.addtable map=country columns="name varchar(80)"
TCOUNTRY=$(basename "$COUNTRY" .poly)
v.db.update map=country layer=1 column=name value="${TCOUNTRY/"'"/" "}"
#'
v.out.ogr -s input=country output=tmp.gpkg format=GPKG --overwrite

ogr2ogr "./countrys/$(basename "$COUNTRY" .poly).gpkg" tmp.gpkg -dialect sqlite -sql "SELECT ST_Union(geom), name FROM country GROUP BY name"

g.remove -f type=vector pattern=*
done

exit

rm tmpShp*.txt
rm tmp.gpkg

ogrmerge.py -f GeoJSON -o countryBorders.geojson -single -nln country ./countrys/*.gpkg
ogr2ogr countryBorders.gpkg countryBorders.geojson

sqlite3 countryBorders.gpkg
CREATE TABLE tmp (
  name TEXT NOT NULL,
  iso2 TEXT NOT NULL,
  iso3 TEXT NOT NULL,
  isoNr int,
  namee TEXT NOT NULL
);
.separator \t
.import countriesTable.txt tmp

create table t1 as select iso2, iso3, isoNr, country.name, namee, geom from country left join tmp using(name);
drop table tmp;
drop table country;
ALTER TABLE t1 RENAME TO country;
.quit

rm countryBorders.geojson
ogr2ogr -f GeoJSON countryBorders.geojson countryBorders.gpkg
rm countryBorders.gpkg
ogr2ogr -f GPKG countryBorders.gpkg countryBorders.geojson
