countryGadm <- read.csv("/data/vectordata/global/adminstrativeBoundaries/gadm/v1/countrynames.csv")
fra2015 <- read.csv("fra2015Carbon.csv")

me<- merge(fra2015, countryGadm[,c("ISO", "GADMID")], by.y="ISO", by.x="Country", all.x=T)

me[is.na(me$GADMID),]
me <- me[me$Country!="BLM",]
me$Forest[me$Country=="GLP"] <- me$Forest[me$Country=="GLP"] + me$Forest[me$Country=="MAF"]
me <- me[me$Country!="MAF",]
me$Forest[me$Country=="SDN"] <- me$Forest[me$Country=="SDN"] + me$Forest[me$Country=="SSD"]
me <- me[me$Country!="SSD",]

g4m <- read.table("countrysum.txt", header=T)
me2 <- merge(g4m, me, by.x="country", by.y="GADMID", all.x=T)
me2$bm <- with(me2, bmManNoirri+bmUmanNoirri+bmManIrri+bmUmanIrri)

me2$fcSo <- with(me2, ( ForSoilCarb/Forest*1000) / (bm/area))
me2$fcSo[me2$bm<=0] <- NA

tmp <- with(me2[!is.na(me2$fcSo) & !is.na(me2$bm),], sum(fcSo*bm)) / with(me2[!is.na(me2$fcSo) & !is.na(me2$bm),], sum(bm))
me2$fcSo[is.na(me2$fcSo)] <- tmp

write.table(me2[me2$country>0,c("country","fcSo")], "/tmp/calibFactor.txt", row.names = F, col.names = F)


### FRA 2015 werte fuer jahr 2000 ###
me <- read.csv("/home/georg/16/g4m/biomassmap/fra20152000Carbon.csv")

me[is.na(me$gadmid),]
me$forest[me$country=="GLP"] <- me$forest[me$country=="GLP"] + me$forest[me$country=="MAF"]
me <- me[me$country!="MAF",]
me$forest[me$country=="SDN"] <- me$forest[me$country=="SDN"] + me$forest[me$country=="SSD"]
me <- me[me$country!="SSD",]
me$country <- NULL

g4m <- read.table("countrysum.txt", header=T)
me2 <- merge(g4m, me, by.x="country", by.y="gadmid", all.x=T)
me2$bm <- with(me2, bmManNoirri+bmUmanNoirri+bmManIrri+bmUmanIrri)

me2$fcSo <- with(me2, ( forsoilcarb/forest*1000) / (bm/area))
me2$fcSo[me2$bm<=0] <- NA

tmp <- with(me2[!is.na(me2$fcSo) & !is.na(me2$bm),], sum(fcSo*bm)) / with(me2[!is.na(me2$fcSo) & !is.na(me2$bm),], sum(bm))
me2$fcSo[is.na(me2$fcSo)] <- tmp

write.table(me2[me2$country>0,c("country","fcSo")], "/tmp/calibFactor.txt", row.names = F, col.names = F)
