head -n -1 /data/tmp/baseData.txt | cut -f 3,4 |sort |uniq |sed 's/\t/_/g' |sort -k 1b,1 >/tmp/idx.txt
split -l$((`wc -l < /tmp/idx.txt`/7 + 1)) /tmp/idx.txt /tmp/part -da 1 --additional-suffix=".txt"
head -n -1 /data/tmp/baseData.txt |awk '{print $3"_"$4,$0}' |sort -k 1b,1 >/data/tmp/bd.txt
head -n -1 /data/tmp/baseDataClima.txt |awk '{print $1"_"$2,$0}' |sort -k 1b,1 >/data/tmp/bdc.txt

for prt in {0..6}
do
    join /tmp/part$prt.txt /data/tmp/bd.txt |cut -f 2- -d ' ' >/data/tmp/bd$prt.txt
    join /tmp/part$prt.txt /data/tmp/bdc.txt |cut -f 2- -d ' ' >/data/tmp/bdc$prt.txt
    head -n1 /data/tmp/bd$prt.txt >>/data/tmp/bd$prt.txt
    head -n1 /data/tmp/bdc$prt.txt >>/data/tmp/bdc$prt.txt
done

./4g4mSiteVal /data/tmp/baseData.txt /data/tmp/baseDataClima.txt

#filenamen implementieren und dann 8 (??4-7??) Instanzen starten
./4g4mSiteVal /data/tmp/bd0.txt /data/tmp/bdc0.txt >/data/tmp/g4m0.txt &
./4g4mSiteVal /data/tmp/bd1.txt /data/tmp/bdc1.txt >/data/tmp/g4m1.txt &
./4g4mSiteVal /data/tmp/bd2.txt /data/tmp/bdc2.txt >/data/tmp/g4m2.txt &
./4g4mSiteVal /data/tmp/bd3.txt /data/tmp/bdc3.txt >/data/tmp/g4m3.txt &
./4g4mSiteVal /data/tmp/bd4.txt /data/tmp/bdc4.txt >/data/tmp/g4m4.txt &
./4g4mSiteVal /data/tmp/bd5.txt /data/tmp/bdc5.txt >/data/tmp/g4m5.txt &
./4g4mSiteVal /data/tmp/bd6.txt /data/tmp/bdc6.txt >/data/tmp/g4m6.txt &

./4g4mSiteVal /data/tmp/bd5.txt /data/tmp/bdc5.txt

wc /tmp/idx.txt
wc g4m?.txt
#12891


cp /data/tmp/g4m0.txt /data/tmp/g4mUs.txt
for prt in {1..6}
do
    tail -n +2 /data/tmp/g4m$prt.txt >>/data/tmp/g4mUs.txt
done




#R
x <- read.table("/data/tmp/g4mUs.txt", header=T)
with(x, plot(y,x))
summary(with(x, s2020/area))
summary(with(x, h2020/area))
summary(with(x, h2020/s2020))
summary(with(x, c2020/h2020))
with(x, plot(y,x, col=rainbow(10)[10*h2020/area]))
with(x, plot(y,x, col=rainbow(30)[s2020/area]))
with(x, plot(y,x, col=rainbow(30)[s2021/area]))


t1 <- grep("h....", names(x))
res <- matrix(NA,NROW(x),7)
for(i in 2:8) {
       res[,i-1] <- rowMeans(x[,t1[1:5 + i*5]])
}
write.table(data.frame(x$x, x$y, res), file="/tmp/agg.txt", row.names = F, col.names = F)

	      


