split -nl/8 /tmp/datG4mIn.txt /data/tmp/datG4mIn
find /data/tmp/ -name "datG4mIn*" -print0 | xargs -0 -I {} -P 8 ./2getUMaiBm {} {}out.txt
head -n 3 /data/tmp/datG4mInaaout.txt >/data/tmp/datG4m4Fulvio2018.txt
tail -q -n +4 /data/tmp/datG4mIna?out.txt >>/data/tmp/datG4m4Fulvio2018.txt
./3aggregate.pl </data/tmp/datG4m4Fulvio2018.txt >/data/tmp/datG4m4Fulvio2018IsoAgg.txt
pbzip2 -k /data/tmp/datG4m4Fulvio2018.txt
