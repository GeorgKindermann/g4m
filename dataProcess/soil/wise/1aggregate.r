x <- read.csv("./Interchangeable_format/HW30s_FULL.txt")

summary(aggregate(PROP ~ NEWSUID + Layer, data=x, FUN=sum))

x$cKgPerM2 <- with(x, BULK * (1 - CFRAG/100) * 100 * 100 * (BotDep - TopDep) / 1000 * ORGC / 1000)
t1 <- aggregate(cKgPerM2 * PROP/100 ~ NEWSUID, data=x, FUN=sum) #kgC/m2 fuer 2m
names(t1)[2] <- "cKgPerM2"

x$nKgPerM2 <- with(x, BULK * (1 - CFRAG/100) * 100 * 100 * (BotDep - TopDep) / 1000 * TOTN / 1000)
t2 <- aggregate(nKgPerM2 * PROP/100 ~ NEWSUID, data=x, FUN=sum) #kgN/m2 fuer 2m
names(t2)[2] <- "nKgPerM2"

t3 <- aggregate(TAWC * (BotDep - TopDep) / 20 * PROP / 100 ~ NEWSUID, data=x, FUN=sum) #watercapacity mm/m
names(t3)[2] <- "h2oMmPerM"
t3$h2oMmPerM[t3$h2oMmPerM<0] <- 0

me <- merge(merge(t1,t2),t3)
me$id <- as.integer(substr(me$NEWSUID, 5, 10))
me$ctPerHa <- round(me$cKgPerM2*10,1)
me$ntPerHa <- round(me$nKgPerM2*10,2)
me$h2oMmPerM <- round(me$h2oMmPerM)

write.table(me[,c("id","ctPerHa","ntPerHa","h2oMmPerM")], "/tmp/soilData.txt", row.names = F)
