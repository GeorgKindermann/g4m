#Read in the data from the yield table
data <- read.table("assmann.csv", header = T)
datb <- read.table("assmann2.csv", header = T)
#what do the colums show (they are from the Yield table from Assmann and Franz 1963)
#Yield .. Identifier for the yield level
#Age .. Age [years]
#HO .. average height of the 100 lagest trees per hectare [m]
#HM .. average height of all trees [m]
#NHA .. nuber of trees per hectare [n/ha]
#DM .. average diameter [cm]
#GOPT .. basal area where the forest shows highest increment [m2/ha]
#VSHA .. volume stocking stemmwood [m3/ha]
#VDHA .. volume stocking mercantable wood [m3/ha]
#Naus .. number of stemms which have been removed during the last 5 years [n/ha/5years]
#VSAus .. stemm volume of trees which have been removed in the last 5 years [m3/ha/5years]
#GWLS .. total increment (stock+removals) stemm wood [m3/ha]
#VDVBEfm .. Harvestable ammount of VDHA [m3/ha]
#GWLDEfm .. total increment of harvestable mercantable wood [m3/ha]
#
#s04 .. relativ increment if the stand density is 40% of GOPT
#s05 .. relativ increment if the stand density is 50% of GOPT
#s06 .. relativ increment if the stand density is 60% of GOPT
#s07 .. relativ increment if the stand density is 70% of GOPT
#s08 .. relativ increment if the stand density is 80% of GOPT
#s09 .. relativ increment if the stand density is 90% of GOPT
#s1 .. relativ increment if the stand density is 100% of GOPT (should be 1)
#s11 .. relativ increment if the stand density is 110% of GOPT
#s12 .. relativ increment if the stand density is 120% of GOPT
#s13 .. relativ increment if the stand density is 130% of GOPT
#sMax .. relativ increment if the basal area is MaxGha
#MaxGha .. maximum possible basal area [m2/ha]
#Gha .. basal area where the forest shows highest increment [m2/ha] (should be identical with column GOPT)

#join the two tables to one table
x <- merge(data, datb, all=T)

#The tables have 3 levels of increment. They can be identified with the starting charagter in the column Yield (U..low, M..average and O..high)
#I selcet now to use only data from the average "Ertragsniveau"
x <- x[substring(x$Yield,1,1) == "M",]
#The following is done to eliminate the now empty classes in column Yield
x$Yield <- as.character(x$Yield)
x$Yield <- as.factor(x$Yield)

#now I have to convert the 3m wood to carbon
#1m3 fresh wood has a dry weight of 390 kg (here it is important to use the converson from 1m3 FRESH (wet) wood to DRY weight and not to use 1m3 DRY wood to DRY weight as the wood volume is changing with the water content) (in the past I have used 430 instead of 390)
#1kg wood consists by 50%C (in the past I have used 44% or 72/162)
#So 1m3wood = 0.39 * 0.5 tC = 0.195 tC
#With this knoledge calculate the Total Carbon Production in the stemm (TCP) using GWLS
x$TCP <- x$GWLS * 0.195

#create a matrix to store resulting coeficients of the used growth curve for each yield level
erg <- matrix(data=NA, nrow = nlevels(x$Yield), ncol=6)
#fill the matrix with the coeficients
for(yield in 1:nlevels(x$Yield)) {
  y <- x[x$Yield == levels(x$Yield)[yield],]
  a <- nls(TCP ~ a * exp(b * log(Age/c)**2), data=y, start = list(a=470, b=-0.5, c=220))
  erg[yield,] <- c(coef(a), summary(a)$coef[,4])
}
#and show how the curves fir to the observed data
y <- x[x$Yield == levels(x$Yield)[1],]
plot(TCP~Age, data=y, xlim = c(0,max(erg[,3])+10), ylim = c(0,max(erg[,1])))
tmp <- data.frame(age = 0:erg[1,3])
lines(tmp$age, erg[1,1]*exp(erg[1,2]*log(tmp/erg[1,3])**2), lty=2)
text(x = erg[1,3], y = erg[1,1], labels = levels(x$Yield)[1], pos = 4)
for(i in 2:nlevels(x$Yield)) {
  y <- x[x$Yield == levels(x$Yield)[i],]
  points(TCP~Age, data=y)
  tmp <- data.frame(age = 0:erg[i,3])
  lines(tmp$age, erg[i,1]*exp(erg[i,2]*log(tmp/erg[i,3])**2), lty=2)
  text(x = erg[i,3], y = erg[i,1], labels = levels(x$Yield)[i], pos = 4)
}
#now we have for each yield level individual coefficients
#in the following lines I try to describe these individual curves with one rule

#Get the optimal (highest increment) rotation time (tOpt) for each yield level
tOpt <- erg[,3] * exp(1/(2*erg[,2]))
names(tOpt) <- levels(x$Yield)
x$tOpt <- tOpt[x$Yield]
#Get the highest possible mean annual increment (MAI) for each yield level
MAI <- erg[,1]*exp(erg[,2]*log(tOpt/erg[,3])**2)/tOpt
names(MAI) <- levels(x$Yield)
x$mai <- MAI[x$Yield]

#Calculate the shape factor k using MAI
plot(erg[,2]~MAI, xlim = c(0,10), ylim = c(-1.6, 0), ylab="k")
tmp <- data.frame(k=erg[,2], mai=MAI)
#a <- nls(k ~ d + a*exp(b*mai**c), start = list(a=-1.5, b=-0.1, c=1, d=0), trace=T,control=nls.control(maxit=999,minFactor=1/2**32), data=tmp, algorithm="port")
#the regression does not convergate for this yield table so reduce it to 3 parameters and make a "good" hand made estimate for the last parameter
d <- -0.25
a <- nls(k ~ d + a*exp(b*mai**c), start = list(a=-2.2, b=-1, c=0.58), trace=T,control=nls.control(maxit=999,minFactor=1/2**32), data=tmp, algorithm="port")
del <- data.frame(mai = 0:100/10)
lines(del$mai, predict(a, newdata=del))
CK <- a
#now get the coeficients c0=d(-0.25), c1=a(-2.2), c2=b(-0.99), c3=c(0.58)
coef(a)

#Calculate tMax using MAI
plot(erg[,3]~MAI, xlim = c(0,10), ylim = c(80, 300), ylab="tMax")
tmp <- data.frame(tMax=erg[,3], MAI=MAI)
a <- nls(tMax ~ d + c/(1 + exp(a + b*MAI)), data=tmp, trace = T
         , start = list(a=4.7, b=-1.3, c=100, d=170)
         ,control=nls.control(maxit=9999,minFactor=1/2**32))
del <- data.frame(MAI = 0:100/10)
lines(del$MAI, predict(a, newdata=del))
CTM <- a
#now get the coeficients c4=d(169), c5=c(81), c6=a(4.5), c7=b(-1.4)
coef(a)

#Calculate TCPMax from MAI
#There is no need for a regression -> test if it works OK
plot(erg[,1]~MAI)
points(MAI, erg[,3] * exp(0.25/erg[,2]) * MAI, col=2)


#Mortality
x$Vmax <- x$VSHA * x$MaxGha / x$Gha
erg2 <- matrix(data=NA, nrow = nlevels(x$Yield), ncol=6)
x$Vmax[x$Vmax/x$GWLS > 1] <- x$GWLS[x$Vmax/x$GWLS > 1]
plot(x$GWLS,x$Vmax/x$GWLS)
plot(NA,NA,xlim=c(0,7.5), ylim = c(0,1), xlab = "Age/TOpt", ylab ="VMax/GWL")
x$relage <- NA
for(i in 1:nlevels(x$Yield)) {
  x$relage[x$Yield == levels(x$Yield)[i]] <- x$Age[x$Yield == levels(x$Yield)[i]]/tOpt[i]
  tmp <- x[x$Yield == levels(x$Yield)[i],]
  tmp2 <- tmp
  idx <- sort(tmp$relage, index.return = T)
  for(j in 1:NROW(tmp)) {
    tmp[j,] <- tmp2[idx$ix[j],]
  }
  tmp <- rbind(tmp[1,],tmp)
  tmp$Vmax[1] <- 0.001; tmp$GWLS[1] <- 0.001; tmp$Age[1] <- 1; tmp$relage[1] <- tmp$Age[1]/tOpt[i]
  del <- c(FALSE, tmp$Vmax[2:NROW(tmp)]/tmp$GWLS[2:NROW(tmp)] < tmp$Vmax[1:NROW(tmp)-1]/tmp$GWLS[1:NROW(tmp)-1])
  del[is.na(del)] <- FALSE
  tmp <- tmp[del,]
  points(I(tmp$Vmax/tmp$GWLS) ~ I(tmp$Age/tOpt[i]), col = i, pch=1, type="l")
  c2 <- 1/6
  #c2 <- 0.25 + 1.*exp(-2 -0.5*MAI[i])
  a <- nls(Vmax/GWLS ~pmax(0, pmin(1, (c0 + c1*log(relage)) * (1-c2*relage)**0.5))
  , trace = T, data=tmp, start = list(c0=0.8, c1=-0.006)
  , nls.control(maxiter=999))
  #erg2[i,] <- c(coef(a), summary(a)$coef[,4])
  erg2[i,] <- c(coef(a), c2, summary(a)$coef[,4], 1)
d <- data.frame(relage = (0:650)/100)
#lines(d$relage, predict(a, newdata=d), type="l", lw=2, col=i)
}

c2 <- 1/6
a <- nls(Vmax/GWLS ~pmax(0, pmin(1, (c0 + c1*log(relage)) * (1-c2*relage)**0.5))
         , trace = T, data=x, start = list(c0=0.8, c1=-0.006)
         , nls.control(maxiter=999))
d <- data.frame(relage = (0:650)/100)
lines(d$relage, predict(a, newdata=d), type="l", lw=3, col=1)

## tmp <- data.frame(MAI = 0:100/10)
## plot(erg2[,1]~MAI, pch=19, cex=2*(min(erg2[,4])/erg2[,4])**0.3, xlim=c(0,10), ylim=c(0.45,0.95))
## points(erg2[,1]~MAI, pch=".", col=2)
## a <- nls(erg2[,1] ~ 0.9/(1 + exp(c0+c1*MAI)), weight=2*(min(erg2[,4])/erg2[,4])**0.3,
##          start = list(c0=-0.8242255, c1=-0.4273254), trace=T
##          , nls.control(maxiter=999))
## lines(tmp$MAI, predict(a, newdata=tmp))

## plot(erg2[,2]~MAI, pch=19, cex=2*(min(erg2[,5])/erg2[,5])**0.1, xlim=c(0,10), ylim=c(-1,0))
## points(erg2[,2]~MAI, pch=".", col=2)
## del <- data.frame(val = erg2[,2], MAI=MAI)
## a <- nls(val ~ -0.4/(1+exp(c1 + c2*MAI)) -0.3/(1+exp(c3 + c4*MAI))
##          , trace = T, nls.control(maxiter=999), data=del
##          , start = list(c1=0.3850213, c2=2.9970913, c3=4.9501050, c4=-1.0023533)
##          , algo="port")
## lines(tmp$MAI, predict(a, newdata=tmp))

## plot(erg2[,3]~MAI, pch=19, cex=2*(min(erg2[,6])/erg2[,6])**0.3, xlim=c(0,10))
## c3 <- +1
## a <- nls(erg2[,3] ~ 0.10 + c3*exp(c0+c1*MAI), weight=2*(min(erg2[,6])/erg2[,6])**0.3
##          , trace=T, start = list(c0=-1.5, c1=-0.15), algo="port")
## lines(tmp$MAI, predict(a, newdata=tmp))


#Managed Stocking degree - growth response to stand density
xs <- data.frame(mai = x$mai[1], age = x$Age[1], sd = 0.4 * x$Gha[1]/x$MaxGha[1], rinc = x$s04[1])[-1,]
for(i in 1:NROW(x)) {
  xs <- rbind(xs, data.frame(mai = x$mai[i], age= x$Age[i], sd = 0.4 * x$Gha[i]/x$MaxGha[i], rinc = x$s04[i]))
  xs <- rbind(xs, data.frame(mai = x$mai[i], age= x$Age[i], sd = 0.5 * x$Gha[i]/x$MaxGha[i], rinc = x$s05[i]))
  xs <- rbind(xs, data.frame(mai = x$mai[i], age= x$Age[i], sd = 0.6 * x$Gha[i]/x$MaxGha[i], rinc = x$s06[i]))
  xs <- rbind(xs, data.frame(mai = x$mai[i], age= x$Age[i], sd = 0.7 * x$Gha[i]/x$MaxGha[i], rinc = x$s07[i]))
  xs <- rbind(xs, data.frame(mai = x$mai[i], age= x$Age[i], sd = 0.8 * x$Gha[i]/x$MaxGha[i], rinc = x$s08[i]))
  xs <- rbind(xs, data.frame(mai = x$mai[i], age= x$Age[i], sd = 0.9 * x$Gha[i]/x$MaxGha[i], rinc = x$s09[i]))
  xs <- rbind(xs, data.frame(mai = x$mai[i], age= x$Age[i], sd = 1.0 * x$Gha[i]/x$MaxGha[i], rinc = x$s1[i]))
  xs <- rbind(xs, data.frame(mai = x$mai[i], age= x$Age[i], sd = 1.1 * x$Gha[i]/x$MaxGha[i], rinc = x$s11[i]))
  xs <- rbind(xs, data.frame(mai = x$mai[i], age= x$Age[i], sd = 1.2 * x$Gha[i]/x$MaxGha[i], rinc = x$s12[i]))
  xs <- rbind(xs, data.frame(mai = x$mai[i], age= x$Age[i], sd = 1.3 * x$Gha[i]/x$MaxGha[i], rinc = x$s13[i]))
  xs <- rbind(xs, data.frame(mai = x$mai[i], age= x$Age[i], sd = 1., rinc = x$sMax[i]))
}
xs <- xs[!is.na(xs$sd),]
xs <- xs[!is.na(xs$rinc),]
xs$sd[xs$sd > 1] <- 1

plot(xs$sd, xs$rinc)

coplot(rinc ~ sd |age * mai, data=xs, overlap=c(0,0), panel = panel.smooth)
#no peak
row.names(xs) <- 1:NROW(xs)
idx <- order(xs$mai, xs$age, xs$sd)
dmai <- xs$mai[idx[1]]
dage <- xs$age[idx[1]]
dric <- xs$rinc[idx[1]]
for(i in 1:NROW(xs)) {
  if(dmai == xs$mai[idx[i]] & dage == xs$age[idx[i]]) {
    if(dric > xs$rinc[idx[i]]) {xs$rinc[idx[i]] <- dric}
    if(dric < xs$rinc[idx[i]]) {dric <- xs$rinc[idx[i]]}
  } else {
    dmai <- xs$mai[idx[i]]
    dage <- xs$age[idx[i]]
    dric <- xs$rinc[idx[i]]
  }
}
coplot(rinc ~ sd |age * mai, data=xs, overlap=c(0,0), panel = panel.smooth)


di <- function(std=1, c0=2, c1=1) {
  std[std > 1] <- 1
  std[std < 0] <- 0
  c0[c0==1] <- 1.00001;
  c1[c1<1] <- 1;
  top <- (1/c0)**(1/(c0-1))
  top2 <- top - top^c0
  top4 <- top * c1
  top3 <- pmin(top, std*top4)
  (top3 - top3^c0)/top2
}

#Ohne gerade am Anfang
c1 <- 0.582198
c2 <- 150
c3 <- 0.01
c4 <- 0.00202676
c5 <- 2.11138
a <- nls(rinc ~ di(sd, 1 + (age**c1)/c2 * 1/(c3 + c4*mai**c5), 1)
         , data=xs, trace=T, algo="default"
         , start = list(c1=0.690023, c3=0.0170579, c4=0.00243725, c5=2.32822))

if(!is.na(coef(a)["c1"])) {c1 <- coef(a)["c1"]}
if(!is.na(coef(a)["c2"])) {c2 <- coef(a)["c2"]}
if(!is.na(coef(a)["c3"])) {c3 <- coef(a)["c3"]}
if(!is.na(coef(a)["c4"])) {c4 <- coef(a)["c4"]}
if(!is.na(coef(a)["c5"])) {c5 <- coef(a)["c5"]}
c6 <- 0

xs$erinc <- di(xs$sd,1 + (xs$age**c1)/c2 * 1/(c3 + c4*xs$mai**c5), 1 + c6/(1 + (c7) * xs$age**c8 * 1/(c9 + xs$mai**c10)))
coplot(rinc ~ sd |age * mai, data=xs, overlap=c(0,0), panel = panel.smooth, col=3, pch=".", col.smooth=3)
par(new=T)
coplot(erinc ~ sd |age * mai, data=xs, overlap=c(0,0), panel = panel.smooth, col=2, pch=".", col.smooth=2, ylim=range(xs$rinc))

#
c6 <- 0.8
c7 <- 1/500
c8 <- 2
c9 <- 0.01
c10 <- 0.5
a <- nls(rinc ~ di(sd, 1 + (age**c1)/c2 * 1/(c3 + c4*mai**c5), 1 + c6/(1 + (c7) * age**c8 * 1/(c9 + mai**c10))), data=xs, trace=T, algo="port"
         , control=nls.control(maxit=999, warnOnly=T)
         , start = list(c6=0.136606, c8=3.41300, c10=5.59232)
         )

if(!is.na(coef(a)["c6"])) {c6 <- coef(a)["c6"]}
if(!is.na(coef(a)["c7"])) {c7 <- coef(a)["c7"]}
if(!is.na(coef(a)["c8"])) {c8 <- coef(a)["c8"]}
if(!is.na(coef(a)["c9"])) {c9 <- coef(a)["c9"]}
if(!is.na(coef(a)["c10"])) {c10 <- coef(a)["c10"]}

xs$erinc <- di(xs$sd,1 + (xs$age**c1)/c2 * 1/(c3 + c4*xs$mai**c5), 1 + c6/(1 + (c7) * xs$age**c8 * 1/(c9 + xs$mai**c10)))
coplot(rinc ~ sd |age * mai, data=xs, overlap=c(0,0), panel = panel.smooth, col=3, pch=".", col.smooth=3)
par(new=T)
coplot(erinc ~ sd |age * mai, data=xs, overlap=c(0,0), panel = panel.smooth, col=2, pch=".", col.smooth=2, ylim=range(xs$rinc))

#
c2 <- 150
c7 <- 1/500
c9 <- 0.01
a <- nls(rinc ~ di(sd, 1 + (age**c1)/c2 * 1/(c3 + c4*mai**c5), 1 + c6/(1 + (c7) * age**c8 * 1/(c9 + mai**c10))), data=xs, trace=T, algo="port"
         , control=nls.control(maxit=999, warnOnly=T)
         #, start = list(c1=0.690023, c3=0.0170579, c4=0.00243725, c5=2.32822, c6=0.136606, c8=3.41300, c10=5.59232)
         , start = list(c1=-0.266008, c3=0.000253965, c4=2.49563e-09, c5=8.20519, c6=0.436421, c8=2.16923, c10=2.17820)
         )

if(!is.na(coef(a)["c1"])) {c1 <- coef(a)["c1"]}
if(!is.na(coef(a)["c2"])) {c2 <- coef(a)["c2"]}
if(!is.na(coef(a)["c3"])) {c3 <- coef(a)["c3"]}
if(!is.na(coef(a)["c4"])) {c4 <- coef(a)["c4"]}
if(!is.na(coef(a)["c5"])) {c5 <- coef(a)["c5"]}
if(!is.na(coef(a)["c6"])) {c6 <- coef(a)["c6"]}
if(!is.na(coef(a)["c7"])) {c7 <- coef(a)["c7"]}
if(!is.na(coef(a)["c8"])) {c8 <- coef(a)["c8"]}
if(!is.na(coef(a)["c9"])) {c9 <- coef(a)["c9"]}
if(!is.na(coef(a)["c10"])) {c10 <- coef(a)["c10"]}

xs$erinc <- di(xs$sd,1 + (xs$age**c1)/c2 * 1/(c3 + c4*xs$mai**c5), 1 + c6/(1 + (c7) * xs$age**c8 * 1/(c9 + xs$mai**c10)))
coplot(rinc ~ sd |age * mai, data=xs, overlap=c(0,0), panel = panel.smooth, col=3, pch=".", col.smooth=3)
par(new=T)
coplot(erinc ~ sd |age * mai, data=xs, overlap=c(0,0), panel = panel.smooth, col=2, pch=".", col.smooth=2, ylim=range(xs$rinc))



nd <- data.frame(sd=0:10/10)
age <- 60
mai <- 2
plot(nd$sd, di(nd$sd,1 + (age**c1)/c2 * 1/(c3 + c4*mai**c5), 1 + c6/(1 + (c7) * age**c8 * 1/(c9 + mai**c10))))


cor(xs$rinc, xs$erinc)
plot(xs$rinc, xs$erinc)
abline(0,1)

#Managed stand density - Where increment is 0.95 of max increment
#Create with interpolation table

#Height
plot(NA,NA,xlim=c(0,max(x$Age)), ylim = c(0,max(x$HM)), xlab="Age", ylab ="Height")
for(i in 1:nlevels(x$Yield)) {
  tmp <- x[x$Yield == levels(x$Yield)[i],]
  points(tmp$HM ~ tmp$Age, col = i, pch=1)
}
a <- nls(HM ~ c0*mai**c3* (1-exp(c1*Age))**(c2*mai**c4)
         , data = x, trace=T
         , start = list(c0=17, c1=-0.03, c2=1.13, c3=0.8, c4=1))
for(i in 1:nlevels(x$Yield)) {
  d <- data.frame(Age = 0:450, mai = MAI[i])
  lines(d$Age, predict(a, newdata=d), type="l", col = i)
}
plot(NA,NA, type="n", xlim=c(0,450), ylim=c(0,100), xlab="Age", ylab ="Height")
for(i in 1:10) {
  d <- data.frame(Age = 0:450, mai = i)
  lines(d$Age, predict(a, newdata=d), type="l", col = i)
}
c <- coef(a)
mai <- 2
Age <- 9.766196
c[1]*mai**c[4]* (1-exp(c[2]*Age))**(c[3]*mai**c[5])
h <- 1.3
(log(mai**(-c[4]*mai**-c[5]/c[3])*(mai**(c[4]*mai**-c[5]/c[3])-(h/c[1])**(mai**-c[5]/c[3]))))/c[2]
#They are equal but the second is shorter
#xd$td0 <- (log(xd$mai**(-c[4]*xd$mai**-c[5]/c[3])*(xd$mai**(c[4]*xd$mai**-c[5]/c[3])-(h/c[1])**(xd$mai**-c[5]/c[3]))))/c[2]
x$td0 <- log(1-(1.3/(c[1]*x$mai**c[4]))**(1/(c[3]*x$mai**c[5])))/c[2]

#DBH
plot(NA,NA,xlim=c(0,max(x$Age)), ylim = c(0,max(x$DM, na.rm=T))
     , xlab="Age", ylab ="DBH")
erg <- matrix(data=NA, nrow = nlevels(x$Yield), ncol=3)
for(i in 1:nlevels(x$Yield)) {
  tmp <- x[x$Yield == levels(x$Yield)[i],]
  points(tmp$DM ~ tmp$Age, col = i, pch=1)
  #a <- nls(d ~ a*(age-td0)**b, data = tmp, start = list(a=1, b=1))
  d <- data.frame(Age = 0:450, mai = MAI[i])
  d$td0 <- (log(d$mai**(-c[4]*d$mai**-c[5]/c[3])*(d$mai**(c[4]*d$mai**-c[5]/c[3])-(h/c[1])**(d$mai**-c[5]/c[3]))))/c[2]
  #lines(d$age, predict(a, newdata=d), type="l", col = i)
  #erg[i,] <- c(coef(a), tmp$mai[1])
  c0 <- 775
  a <- nls(DM ~ c0 * (1-exp(c1*(Age-td0)))**c2
           , data = tmp, trace=T, control=nls.control(maxit=999)
           , start = list(c1=-0.0005, c2=0.85), algo="port")
  erg[i,] <- c(c0,coef(a))
  lines(d$Age, predict(a, newdata=d), type="l", col = i)
}
plot(erg[,1] ~ MAI)
a <- nls(erg[,1] ~ c0 + c1*MAI, start = list(c0=22, c1=16), trace=T, algo="port")
lines(MAI, predict(a))
plot(erg[,2] ~ MAI)
#a <- nls(erg[,2] ~ c2/(1+c3*MAI**c4), start = list(c2=-0.011, c3=0.25, c4=-1.8), trace=T, algo="port", control=nls.control(warnOnly=T))
c2 <- -0.011
a <- nls(erg[,2] ~ c2/(1+c3*MAI**c4), start = list(c3=0.25, c4=-1.8), trace=T, algo="port", control=nls.control(warnOnly=T, maxit=999))
lines(MAI, predict(a))
plot(erg[,3] ~ MAI)
#a <- nls(erg[,3] ~ c5/(1+c6*MAI**c7), start = list(c5=1, c6=0.1, c7=-1.6),
#         trace=T, algo="port", control=nls.control(warnOnly=T))
c5 <- 2
a <- nls(erg[,3] ~ c5/(1+c6*MAI**c7), start = list(c6=0.1, c7=-1.6),
 trace=T, algo="port", control=nls.control(warnOnly=T))
lines(MAI, predict(a))

c1 <- 0
c2 <- -0.011
c5 <- 2
a <- nls(DM ~ (c0 + pmax(0,c1*mai)) * (1-exp((c2/(1+c3*mai**c4))*(Age-td0)))**(c5/(1+c6*mai**c7))
         , data = x[x$Age > x$td0,], trace=T, algo="port"
         , control=nls.control(maxit=999, warnOnly=T)
         #, start = list(c0=775, c3=42.7, c4=-0.2359, c6=1.159, c7=0.165)
         , start = list(c0=192.206, c2=-0.011, c3=7.68561, c4=-0.501452, c6=1.12110, c7=0.0958744)
         , start = list(c0=700.613, c2=-0.000509960, c3=0.969293, c4=-0.885478, c6=1.21276, c7=0.102969)
         )
c0 <- 700
c1 <- 0
c5 <- 2
a <- nls(DM ~ (c0 + pmax(0,c1*mai)) * (1-exp((c2/(1+c3*mai**c4))*(Age-td0)))**(c5/(1+c6*mai**c7))
         , data = x[x$Age > x$td0,], trace=T, algo="port"
         , control=nls.control(maxit=999, warnOnly=T)
         , start = list(c2=-0.000509960, c3=0.969293, c4=-0.885478, c6=1.21276, c7=0.102969)
         )

plot(NA,NA,xlim=c(0,max(x$Age)), ylim = c(0,max(x$DM, na.rm=T))
     , xlab="Age", ylab ="DBH")
for(i in 1:nlevels(x$Yield)) {
  tmp <- x[x$Yield == levels(x$Yield)[i],]
  points(tmp$DM ~ tmp$Age, col = i, pch=1)
  d <- data.frame(Age = 0:450, mai = MAI[i])
  h <- 1.3
  d$td0 <- (log(d$mai**(-c[4]*d$mai**-c[5]/c[3])*(d$mai**(c[4]*d$mai**-c[5]/c[3])-(h/c[1])**(d$mai**-c[5]/c[3]))))/c[2]
  lines(d$Age, predict(a, newdata=d), type="l", col = i)
}

del <- data.frame(Age = 10*0:45, mai=1)
h <- 1.3
del$td0 <- (log(del$mai**(-c[4]*del$mai**-c[5]/c[3])*(del$mai**(c[4]*del$mai**-c[5]/c[3])-(h/c[1])**(del$mai**-c[5]/c[3]))))/c[2]
plot(del$Age, predict(a, newdata=del), type="l")

#DBH-Increment dependency to stocking degree
#bhdSD = dbh * (c0-c1*sd**c1) #c0=2, c1=1, c2=1.6
nd <- 0:10/10
plot(nd, 2-1*nd**1.6)


