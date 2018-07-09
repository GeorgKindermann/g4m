countryGadm <- read.csv("/data/vectordata/global/adminstrativeBoundaries/gadm/v1/countrynames.csv")
fra2015 <- read.csv("primaryForestAreaFra2015.csv")
x <- read.csv("../forestCover/forestAreaFra2015.csv")
fra2015 <- merge(fra2015, x)

me <- merge(fra2015, countryGadm[,c("ISO", "GADMID")], by.y="ISO", by.x="Country", all.x=T)
me[is.na(me$GADMID),]
me <- me[me$Country!="BLM",]

write.table(me[,c("GADMID", "Forest", "PrimFor")], "/tmp/faoFra2015.txt", col.names=F, row.names=F)


