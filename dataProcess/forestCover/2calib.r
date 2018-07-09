files <- list.files(path = "/tmp/t/", pattern = "*.txt")

countryGadm <- read.csv("/data/vectordata/global/adminstrativeBoundaries/gadm/v1/countrynames.csv")

fra2015 <- read.csv("forestAreaFra2015.csv")
#fra2015 <- read.csv("forestAreaFra2000.csv")

me <- merge(fra2015, countryGadm[,c("ISO", "GADMID")], by.y="ISO", by.x="Country", all.x=T)
me[is.na(me$GADMID),]
me <- me[me$Country!="BLM",]
me$Forest[me$Country=="GLP"] <- me$Forest[me$Country=="GLP"] + me$Forest[me$Country=="MAF"]
me <- me[me$Country!="MAF",]
me$Forest[me$Country=="SDN"] <- me$Forest[me$Country=="SDN"] + me$Forest[me$Country=="SSD"]
me <- me[me$Country!="SSD",]

me$coef <- NA
me$calibArea <- NA
me$forAreaUncalib <- NA

f <- function(z) {abs(sum((x$forShare * x$gridarea / x$landarea)^z * x$landarea, na.rm=T) - fo)}

filename <- "17.txt"
for(filename in files) {
    
  country <- as.integer(gsub("(.*).txt", "\\1", filename))
  
  x <- read.table(paste("/tmp/t/", filename, sep=""), comment.char="")
  names(x) <- c("forestCoverG4mBase","forestCoverG4mCut","countryGadmV1","gridarea","waterLandModis30Sec","irrigation","nppG4m","nppG4mMaxIrri","g4mcol","g4mrow")

  #Flaechen auf Hektar bringen
  x$gridarea <- x$gridarea / 10000.
  x$landarea <- (1. - x$waterLandModis30Sec/16.) * x$gridarea

  #forshare von 0-100 auf 0-1 bringen
  x$forShare <- x$forestCoverG4mCut / 100.

  me$forAreaUncalib[me$GADMID==country] <- sum(x$forShare * x$gridarea, na.rm=T)
  
  fo <- me$Forest[me$GADMID==country]*1000

  if(length(fo) > 0) {
    #coef <- optimize(f, interval = c(0.2,3), tol=1e-9)$minimum
    coef <- optimize(f, interval = c(0.5,2), tol=1e-9)$minimum
  } else {coef <- 1}

  me$coef[me$GADMID==country] <- coef
  me$calibArea[me$GADMID==country] <- sum((x$forShare * x$gridarea / x$landarea)^coef * x$landarea, na.rm=T)

  print(paste(country, me$Country[me$GADMID==country], coef, me$Forest[me$GADMID==country], me$calibArea[me$GADMID==country]))
  
}

me[is.na(me$coef),]
me <- me[!is.na(me$coef),]


#Use Irrigated areas if there is a need
me$delta <- me$Forest*1000 - me$calibArea
me[me$delta < -5,]
me$delta[me$delta < 5] <- 0 #less than 5ha are missing
me$useIrri <- NA
me$irriArea <- NA
dline <- 192
for(dline in 1:NROW(me)) {
  if(me$delta[dline] > 0) {
    x <- read.table(paste("/tmp/t/", me$GADMID[dline], ".txt", sep=""), comment.char="")
    names(x) <- c("forestCoverG4mBase","forestCoverG4mCut","countryGadmV1","gridarea","waterLandModis30Sec","irrigation","nppG4m","nppG4mMaxIrri","g4mcol","g4mrow")
    x$gridarea <- x$gridarea / 10000.
    x$landarea <- (1. - x$waterLandModis30Sec/16.) * x$gridarea

    x$irriPlus <- pmin(100,x$irrigation + 100*x$waterLandModis30Sec/4)
    
    t1 <- with(x[x$nppG4m <= 0 & x$nppG4mMaxIrri>0,], sum(forestCoverG4mBase/100 * landarea * irriPlus/100.))
    if(t1 > 0) {t1 <- me$delta[dline] / t1}
    if(t1 < 0) {t1 <- 0}
    if(t1 > 1) {t1 <- 1}
    me$irriArea[dline] <- with(x[x$nppG4m <= 0 & x$nppG4mMaxIrri>0,], sum(forestCoverG4mBase/100 * landarea * irriPlus/100.)) * t1
  } else {
      t1 <- 0
      me$irriArea[dline] <- 0
  }
  me$useIrri[dline] <- t1
  
  print(paste(dline, t1))
}

me$delta <- NULL

me[me$useIrri > 0,]

me$multi <- with(me, Forest*1000/(calibArea + irriArea) )
me$multi[me$multi > 0.999] <- 1
me$multi[me$Forest<=0] <- 0
me$coef[me$Forest<=0] <- 1

me$Name <- gsub(",", "", me$Name)

write.csv(me, "/tmp/calibFactors.csv", na="", row.names = F)
#write.csv(me, "/tmp/calibFactors2000.csv", na="", row.names = F)
