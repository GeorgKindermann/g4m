mkdir /tmp/grass/
grass -e -c EPSG:4326 /tmp/grass/mylocation
grass -e -c /tmp/grass/mylocation/mymapset

for RCPScen in 26 45 60 85
do
    bash 1getClimateData.sh rcp${RCPScen}
    grass /tmp/grass/mylocation/mymapset --exec bash 2getG4mData.grass
    ./3estimateNpp >/tmp/mai.txt
    grass /tmp/grass/mylocation/mymapset --exec bash 4getStock.grass
    ./5useAgestruct >/tmp/g4mOut${RCPScen}.txt
done
