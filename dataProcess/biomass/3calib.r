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
write.table(me2, "/tmp/fra.csv", sep=",", row.names = F, col.names = F, na="")

#Coefficients from:
#Timing of carbon emissions from global forest clearance
#Nature Climate Change, 682–685 (2012), doi:10.1038/nclimate1535
m3totc <- read.csv("tCperM3.csv", header=F)
names(m3totc) <- c("Country", "fractionTemperate", "fractionTropical","tCperM3")

me2 <- merge(me2, m3totc[,c("Country","tCperM3")], by.x="Country", by.y="Country", all.x=T)

with(me2, plot(tCperM3*ForGrow/ForAbovCarb))
with(me2, plot(bm/area ,tCperM3*ForGrow/Forest*1000))
abline(0,1)

tmp <- with(me2[!is.na(me2$tCperM3) & !is.na(me2$ForGrow),], sum(tCperM3*ForGrow)) / with(me2[!is.na(me2$tCperM3) & !is.na(me2$ForGrow),], sum(ForGrow))
me2$tCperM3[is.na(me2$tCperM3)] <- tmp


me2$fcAgS <- with(me2, (tCperM3*ForGrow/Forest*1000) / (bm/area))
me2$fcAgS[me2$bm<=0] <- NA
me2$fMcAg <- with(me2, (tCperM3*ForGrow/Forest*1000) / (bmMax/area))
me2$fcMAg[me2$bmMax<=0] <- NA
me2$fcAgS[!is.na(me2$fMcAg) & me2$fMcAg > 1] <- with(me2[!is.na(me2$fMcAg) & me2$fMcAg > 1,], bmMax/bm)

me2[,c("Country","fMcAg","fcAgS","area")]
summary(me2[,c("Country","fMcAg","fcAgS","area")])

tmp <- with(me2[!is.na(me2$fcAgS) & !is.na(me2$ForGrow),], sum(fcAgS*ForGrow)) / with(me2[!is.na(me2$fcAgS) & !is.na(me2$ForGrow),], sum(ForGrow))
me2$fcAgS[is.na(me2$fcAgS)] <- tmp

write.table(me2[me2$country>0,c("country","fcAgS")], "/tmp/calibFactor.txt", row.names = F, col.names = F)




me2$ceil <- with(me2, fcAgS / ((tCperM3*ForGrow/Forest*1000) / (bm/area)))
me2$ceil[is.na(me2$ceil)] <- 1
me2[me2$ceil<1,]

me2$fstock <- with(me2, ceil*(ForGrow/Forest*1000) / (bm/area))
me2$fbmAg <- with(me2, ceil*(ForAbovBiom/Forest*1000) / (bm/area))
me2$fbmBg <- with(me2, ceil*(ForBelBiom/Forest*1000) / (bm/area))
me2$fbmDe <- with(me2, ceil*(ForDeadW/Forest*1000) / (bm/area))
me2$fcAg <- with(me2, ceil*(ForAbovCarb/Forest*1000) / (bm/area))
me2$fcBg <- with(me2, ceil*(ForBelCarb/Forest*1000) / (bm/area))
me2$fcDe <- with(me2, ceil*(DeadForCarb/Forest*1000) / (bm/area))
me2$fcLi <- with(me2, ceil*(ForLittCarb/Forest*1000) / (bm/area))
me2$fcLv <- with(me2, ceil*(ForSubLiv/Forest*1000) / (bm/area))
me2$fcSo <- with(me2, ceil*(ForSoilCarb/Forest*1000) / (bm/area))

me2$fstock[me2$bm<=0] <- NA
me2$fbmAg[me2$bm<=0] <- NA
me2$fbmBg[me2$bm<=0] <- NA
me2$fbmDe[me2$bm<=0] <- NA
me2$fcAg[me2$bm<=0] <- NA
me2$fcBg[me2$bm<=0] <- NA
me2$fcDe[me2$bm<=0] <- NA
me2$fcLi[me2$bm<=0] <- NA
me2$fcLv[me2$bm<=0] <- NA
me2$fcSo[me2$bm<=0] <- NA

tmp <- with(me2[!is.na(me2$fstock) & !is.na(me2$ForGrow),], sum(fstock*ForGrow)) / with(me2[!is.na(me2$fstock) & !is.na(me2$ForGrow),], sum(ForGrow))
me2$fstock[is.na(me2$fstock)] <- tmp
tmp <- with(me2[!is.na(me2$fbmAg) & !is.na(me2$ForAbovBiom),], sum(fbmAg*ForAbovBiom)) / with(me2[!is.na(me2$fbmAg) & !is.na(me2$ForAbovBiom),], sum(ForAbovBiom))
me2$fbmAg[is.na(me2$fbmAg)] <- tmp
tmp <- with(me2[!is.na(me2$fbmBg) & !is.na(me2$ForBelBiom),], sum(fbmBg*ForBelBiom)) / with(me2[!is.na(me2$fbmBg) & !is.na(me2$ForBelBiom),], sum(ForBelBiom))
me2$fbmBg[is.na(me2$fbmBg)] <- tmp
tmp <- with(me2[!is.na(me2$fbmDe) & !is.na(me2$ForDeadW),], sum(fbmDe*ForDeadW)) / with(me2[!is.na(me2$fbmDe) & !is.na(me2$ForDeadW),], sum(ForDeadW))
me2$fbmDe[is.na(me2$fbmDe)] <- tmp
tmp <- with(me2[!is.na(me2$fcAg) & !is.na(me2$ForAbovCarb),], sum(fcAg*ForAbovCarb)) / with(me2[!is.na(me2$fcAg) & !is.na(me2$ForAbovCarb),], sum(ForAbovCarb))
me2$fcAg[is.na(me2$fcAg)] <- tmp
tmp <- with(me2[!is.na(me2$fcBg) & !is.na(me2$ForBelCarb),], sum(fcBg*ForBelCarb)) / with(me2[!is.na(me2$fcBg) & !is.na(me2$ForBelCarb),], sum(ForBelCarb))
me2$fcBg[is.na(me2$fcBg)] <- tmp
tmp <- with(me2[!is.na(me2$fcDe) & !is.na(me2$DeadForCarb),], sum(fcDe*DeadForCarb)) / with(me2[!is.na(me2$fcDe) & !is.na(me2$DeadForCarb),], sum(DeadForCarb))
me2$fcDe[is.na(me2$fcDe)] <- tmp
tmp <- with(me2[!is.na(me2$fcLi) & !is.na(me2$ForLittCarb),], sum(fcLi*ForLittCarb)) / with(me2[!is.na(me2$fcLi) & !is.na(me2$ForLittCarb),], sum(ForLittCarb))
me2$fcLi[is.na(me2$fcLi)] <- tmp
tmp <- with(me2[!is.na(me2$fcLv) & !is.na(me2$ForSubLiv),], sum(fcLv*ForSubLiv)) / with(me2[!is.na(me2$fcLv) & !is.na(me2$ForSubLiv),], sum(ForSubLiv))
me2$fcLv[is.na(me2$fcLv)] <- tmp
tmp <- with(me2[!is.na(me2$fcSo) & !is.na(me2$ForSoilCarb),], sum(fcSo*ForSoilCarb)) / with(me2[!is.na(me2$fcSo) & !is.na(me2$ForSoilCarb),], sum(ForSoilCarb))
me2$fcSo[is.na(me2$fcSo)] <- tmp

write.table(me2[me2$country>0,c("country","fstock","fbmAg","fbmBg","fbmDe","fcAg","fcBg","fcDe","fcLi","fcLv","fcSo")], "/tmp/calibFactor2.txt", row.names = F, col.names = F)



with(me2, plot(stock/bmAg))
with(me2, plot(bmBg/bmAg))
with(me2, plot(bmDe/bmAg))
with(me2, plot(cAg/bmAg))
with(me2, plot(cBg/cAg))
with(me2, plot(cDe/cAg))
with(me2, plot(cLi/cAg))
with(me2, plot(cLv/cAg))
with(me2, plot(cSo/cAg))

with(me2, plot(1000*stock/fraForest))
with(me2, plot(1000*bmAg/fraForest))
with(me2, plot(1000*bmBg/fraForest))
with(me2, plot(1000*bmDe/fraForest))
with(me2, plot(1000*cAg/fraForest))
with(me2, plot(1000*cBg/fraForest))
with(me2, plot(1000*cDe/fraForest))
with(me2, plot(1000*cLi/fraForest))
with(me2, plot(1000*cLv/fraForest))
with(me2, plot(1000*cSo/fraForest))




### 2000 ###
me <- read.csv("fra20152000Carbon.csv")

me[is.na(me$gadmid),]
me$forest[me$country=="GLP"] <- me$forest[me$country=="GLP"] + me$forest[me$country=="MAF"]
me <- me[me$country!="MAF",]
me$forest[me$country=="SDN"] <- me$forest[me$country=="SDN"] + me$forest[me$country=="SSD"]
me <- me[me$country!="SSD",]

names(me)[2] <- "Country"
g4m <- read.table("countrysum.txt", header=T)
me2 <- merge(g4m, me, by.x="country", by.y="gadmid", all.x=T)
write.table(me2, "/tmp/fra.csv", sep=",", row.names = F, col.names = F, na="")

#Coefficients from:
#Timing of carbon emissions from global forest clearance
#Nature Climate Change, 682–685 (2012), doi:10.1038/nclimate1535
m3totc <- read.csv("tCperM3.csv", header=F)
names(m3totc) <- c("Country", "fractionTemperate", "fractionTropical","tCperM3")

me2 <- merge(me2, m3totc[,c("Country","tCperM3")], by.x="Country", by.y="Country", all.x=T)

with(me2, plot(tCperM3*forgrow/forabovcarb))
with(me2, plot(bm/area ,tCperM3*forgrow/forest*1000))
abline(0,1)

tmp <- with(me2[!is.na(me2$tCperM3) & !is.na(me2$forgrow),], sum(tCperM3*forgrow)) / with(me2[!is.na(me2$tCperM3) & !is.na(me2$forgrow),], sum(forgrow))
me2$tCperM3[is.na(me2$tCperM3)] <- tmp


me2$fcAgS <- with(me2, (tCperM3*forgrow/forest*1000) / (bm/area))
me2$fcAgS[me2$bm<=0] <- NA
me2$fMcAg <- with(me2, (tCperM3*forgrow/forest*1000) / (bmMax/area))
me2$fcMAg[me2$bmMax<=0] <- NA
me2$fcAgS[!is.na(me2$fMcAg) & me2$fMcAg > 1] <- with(me2[!is.na(me2$fMcAg) & me2$fMcAg > 1,], bmMax/bm)

me2[,c("Country","fMcAg","fcAgS","area")]
summary(me2[,c("Country","fMcAg","fcAgS","area")])

tmp <- with(me2[!is.na(me2$fcAgS) & !is.na(me2$forgrow),], sum(fcAgS*forgrow)) / with(me2[!is.na(me2$fcAgS) & !is.na(me2$forgrow),], sum(forgrow))
me2$fcAgS[is.na(me2$fcAgS)] <- tmp

write.table(me2[me2$country>0,c("country","fcAgS")], "/tmp/calibFactor.txt", row.names = F, col.names = F)




me2$ceil <- with(me2, fcAgS / ((tCperM3*forgrow/forest*1000) / (bm/area)))
me2$ceil[is.na(me2$ceil)] <- 1
me2[me2$ceil<1,]

me2$fstock <- with(me2, ceil*(forgrow/forest*1000) / (bm/area))
me2$fbmAg <- with(me2, ceil*(forabovbiom/forest*1000) / (bm/area))
me2$fbmBg <- with(me2, ceil*(forbelbiom/forest*1000) / (bm/area))
me2$fbmDe <- with(me2, ceil*(fordeadw/forest*1000) / (bm/area))
me2$fcAg <- with(me2, ceil*(forabovcarb/forest*1000) / (bm/area))
me2$fcBg <- with(me2, ceil*(forbelcarb/forest*1000) / (bm/area))
me2$fcDe <- with(me2, ceil*(deadforcarb/forest*1000) / (bm/area))
me2$fcLi <- with(me2, ceil*(forlittcarb/forest*1000) / (bm/area))
me2$fcLv <- with(me2, ceil*(forsubliv/forest*1000) / (bm/area))
me2$fcSo <- with(me2, ceil*(forsoilcarb/forest*1000) / (bm/area))

me2$fstock[me2$bm<=0] <- NA
me2$fbmAg[me2$bm<=0] <- NA
me2$fbmBg[me2$bm<=0] <- NA
me2$fbmDe[me2$bm<=0] <- NA
me2$fcAg[me2$bm<=0] <- NA
me2$fcBg[me2$bm<=0] <- NA
me2$fcDe[me2$bm<=0] <- NA
me2$fcLi[me2$bm<=0] <- NA
me2$fcLv[me2$bm<=0] <- NA
me2$fcSo[me2$bm<=0] <- NA

tmp <- with(me2[!is.na(me2$fstock) & !is.na(me2$forgrow),], sum(fstock*forgrow)) / with(me2[!is.na(me2$fstock) & !is.na(me2$forgrow),], sum(forgrow))
me2$fstock[is.na(me2$fstock)] <- tmp
tmp <- with(me2[!is.na(me2$fbmAg) & !is.na(me2$forabovbiom),], sum(fbmAg*forabovbiom)) / with(me2[!is.na(me2$fbmAg) & !is.na(me2$forabovbiom),], sum(forabovbiom))
me2$fbmAg[is.na(me2$fbmAg)] <- tmp
tmp <- with(me2[!is.na(me2$fbmBg) & !is.na(me2$forbelbiom),], sum(fbmBg*forbelbiom)) / with(me2[!is.na(me2$fbmBg) & !is.na(me2$forbelbiom),], sum(forbelbiom))
me2$fbmBg[is.na(me2$fbmBg)] <- tmp
tmp <- with(me2[!is.na(me2$fbmDe) & !is.na(me2$fordeadw),], sum(fbmDe*fordeadw)) / with(me2[!is.na(me2$fbmDe) & !is.na(me2$fordeadw),], sum(fordeadw))
me2$fbmDe[is.na(me2$fbmDe)] <- tmp
tmp <- with(me2[!is.na(me2$fcAg) & !is.na(me2$forabovcarb),], sum(fcAg*forabovcarb)) / with(me2[!is.na(me2$fcAg) & !is.na(me2$forabovcarb),], sum(forabovcarb))
me2$fcAg[is.na(me2$fcAg)] <- tmp
tmp <- with(me2[!is.na(me2$fcBg) & !is.na(me2$forbelcarb),], sum(fcBg*forbelcarb)) / with(me2[!is.na(me2$fcBg) & !is.na(me2$forbelcarb),], sum(forbelcarb))
me2$fcBg[is.na(me2$fcBg)] <- tmp
tmp <- with(me2[!is.na(me2$fcDe) & !is.na(me2$deadforcarb),], sum(fcDe*deadforcarb)) / with(me2[!is.na(me2$fcDe) & !is.na(me2$deadforcarb),], sum(deadforcarb))
me2$fcDe[is.na(me2$fcDe)] <- tmp
tmp <- with(me2[!is.na(me2$fcLi) & !is.na(me2$forlittcarb),], sum(fcLi*forlittcarb)) / with(me2[!is.na(me2$fcLi) & !is.na(me2$forlittcarb),], sum(forlittcarb))
me2$fcLi[is.na(me2$fcLi)] <- tmp
tmp <- with(me2[!is.na(me2$fcLv) & !is.na(me2$forsubliv),], sum(fcLv*forsubliv)) / with(me2[!is.na(me2$fcLv) & !is.na(me2$forsubliv),], sum(forsubliv))
me2$fcLv[is.na(me2$fcLv)] <- tmp
tmp <- with(me2[!is.na(me2$fcSo) & !is.na(me2$forsoilcarb),], sum(fcSo*forsoilcarb)) / with(me2[!is.na(me2$fcSo) & !is.na(me2$forsoilcarb),], sum(forsoilcarb))
me2$fcSo[is.na(me2$fcSo)] <- tmp

write.table(me2[me2$country>0,c("country","fstock","fbmAg","fbmBg","fbmDe","fcAg","fcBg","fcDe","fcLi","fcLv","fcSo")], "/tmp/calibFactor2.txt", row.names = F, col.names = F)
