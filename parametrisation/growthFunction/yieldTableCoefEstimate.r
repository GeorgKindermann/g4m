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






