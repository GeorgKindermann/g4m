x <- read.table("/data/tmp/lcEr1_3.txt", header = T)

names(x)[names(x)=="nnp01"] <- "npp01"

plot(x$x, x$y, pch=".")

table(x$landcover)
#evergreen needleleaf forest:    1
#evergreen broadleaf forest:    2
#deciduous needleleaf forest:    3
#deciduous broadleaf forest:    4
#mixed forests:    5

table(x$ecoRegionFao)
#1..Tropical
#2..Subtropical
#3..Temperate
#4..Boreal
#5..Polar
#9..Water
#10..No data
x <- x[x$ecoRegionFao < 5,]

table(x$soilIiasaAwc)
table(x$soilIiasaSwr)
x$soilIiasaAwc[x$soilIiasaAwc < 0] <- NA
x$soilIiasaSwr[x$soilIiasaSwr < 0] <- NA
#estimate awc and swr from soiltype
modalwert <- function(a) {max(0,as.integer(names(which.max(table(a)))))}
t1 <- with(x, tapply(soilIiasaAwc, soilIiasaFao90 , modalwert))
x$soilIiasaAwc[is.na(x$soilIiasaAwc)] <- t1[as.character(x$soilIiasaFao90[is.na(x$soilIiasaAwc)])]
x$soilIiasaAwc[x$soilIiasaAwc == 0] <- NA
t1 <-modalwert(x$soilIiasaAwc)
x$soilIiasaAwc[is.na(x$soilIiasaAwc)] <- t1
t1 <- with(x, tapply(soilIiasaSwr, soilIiasaFao90 , modalwert))
x$soilIiasaSwr[is.na(x$soilIiasaSwr)] <- t1[as.character(x$soilIiasaFao90[is.na(x$soilIiasaSwr)])]
x$soilIiasaSwr[x$soilIiasaSwr == 0] <- NA
t1 <-modalwert(x$soilIiasaSwr)
x$soilIiasaSwr[is.na(x$soilIiasaSwr)] <- t1
rm(t1)
gc()

#Soil groups
sort(table(x$soilIiasaFao90))
createSoilGroups <- function() {
  t1 <- rep(NA, NROW(x))
  t1[x$soilIiasaFao90 >= 1 & x$soilIiasaFao90 < 9] <- 1
  t1[x$soilIiasaFao90 >= 9 & x$soilIiasaFao90 < 18] <- 2
  t1[x$soilIiasaFao90 >= 18 & x$soilIiasaFao90 < 24] <- 3
  t1[x$soilIiasaFao90 >= 24 & x$soilIiasaFao90 < 31] <- 4
  t1[x$soilIiasaFao90 >= 31 & x$soilIiasaFao90 < 38] <- 5
  t1[x$soilIiasaFao90 >= 38 & x$soilIiasaFao90 < 46] <- 6
  t1[x$soilIiasaFao90 >= 46 & x$soilIiasaFao90 < 51] <- 7
  t1[x$soilIiasaFao90 >= 51 & x$soilIiasaFao90 < 57] <- 8
  t1[x$soilIiasaFao90 >= 57 & x$soilIiasaFao90 < 61] <- 9
  t1[x$soilIiasaFao90 >= 61 & x$soilIiasaFao90 < 71] <- 10
  t1[x$soilIiasaFao90 >= 71 & x$soilIiasaFao90 < 78] <- 11
  t1[x$soilIiasaFao90 >= 78 & x$soilIiasaFao90 < 81] <- 12
  t1[x$soilIiasaFao90 >= 81 & x$soilIiasaFao90 < 86] <- 13
  t1[x$soilIiasaFao90 >= 86 & x$soilIiasaFao90 < 92] <- 14
  t1[x$soilIiasaFao90 >= 92 & x$soilIiasaFao90 < 97] <- 15
  t1[x$soilIiasaFao90 >= 97 & x$soilIiasaFao90 < 105] <- 16
  t1[x$soilIiasaFao90 >= 105 & x$soilIiasaFao90 < 114] <- 17
  t1[x$soilIiasaFao90 >= 114 & x$soilIiasaFao90 < 121] <- 18
  t1[x$soilIiasaFao90 >= 121 & x$soilIiasaFao90 < 125] <- 19
  t1[x$soilIiasaFao90 >= 125 & x$soilIiasaFao90 < 131] <- 20
  t1[x$soilIiasaFao90 >= 131 & x$soilIiasaFao90 < 137] <- 21 
  t1[x$soilIiasaFao90 >= 137 & x$soilIiasaFao90 < 143] <- 22
  t1[x$soilIiasaFao90 >= 143 & x$soilIiasaFao90 < 148] <- 23
  t1[x$soilIiasaFao90 >= 148 & x$soilIiasaFao90 < 155] <- 24
  t1[x$soilIiasaFao90 >= 155 & x$soilIiasaFao90 < 162] <- 25
  t1[x$soilIiasaFao90 >= 162 & x$soilIiasaFao90 < 170] <- 26
  t1[x$soilIiasaFao90 >= 170 & x$soilIiasaFao90 < 177] <- 27
  t1[x$soilIiasaFao90 >= 177 & x$soilIiasaFao90 < 182] <- 28
  t1[x$soilIiasaFao90 >= 182] <- 29
  t1[x$soilIiasaFao90 == 63] <- 30
  t1[x$soilIiasaFao90 == 65] <- 31
  t1[x$soilIiasaFao90 == 12] <- 32
  t1[x$soilIiasaFao90 == 196] <- 33
  t1[x$soilIiasaFao90 == 103] <- 34
  t1[x$soilIiasaFao90 == 153] <- 35
  t1[x$soilIiasaFao90 == 106] <- 36
  t1[x$soilIiasaFao90 == 161] <- 37
  t1[x$soilIiasaFao90 == 17] <- 38
  t1[x$soilIiasaFao90 == 62] <- 39
  t1[x$soilIiasaFao90 == 151] <- 40
#
#sort(table(t1))
  t2 <- rep(1, NROW(x))
  t2[t1 == 24] <- 2
  t2[t1 == 6] <- 3
  t2[t1 == 30] <- 4
  t2[t1 == 20] <- 5
  t2[t1 == 31] <- 6
  t2[t1 == 1] <- 7
  t2[t1 == 32] <- 8
  t2[t1 == 25] <- 9
  t2[t1 == 33] <- 10
  t2[t1 == 2] <- 11
  t2[t1 == 21] <- 12
  t2[t1 == 34] <- 13
  t2[t1 == 35] <- 14
  t2[t1 == 16] <- 15
  t2[t1 == 14] <- 16
  t2[t1 == 36] <- 17
  t2[t1 == 17] <- 18
  t2[t1 == 37] <- 19
  t2[t1 == 10] <- 20
  t2[t1 == 38] <- 21
  t2[t1 == 39] <- 22
  t2[t1 == 40] <- 23
  t2
}
x$soil <- createSoilGroups()
gc()
table(x$soil)

#Ausreisser weglassen
#plot(density(x$npp))
#quantile(x$npp, probs = c(0.025, 0.975))
#plot(density(x$npp[x$npp > quantile(x$npp, probs = 0.025) & x$npp < quantile(x$npp, probs = 0.975)]))
x <- x[x$npp > quantile(x$npp, probs = 0.025) & x$npp < quantile(x$npp, probs = 0.975),]
gc()
#plot(density(x$npp))

x$npp <- x$npp/1000 #tC/ha/Year
x$npp01 <- x$npp01/1000 #tC/ha/month
x$npp02 <- x$npp02/1000
x$npp03 <- x$npp03/1000
x$npp04 <- x$npp04/1000
x$npp05 <- x$npp05/1000
x$npp06 <- x$npp06/1000
x$npp07 <- x$npp07/1000
x$npp08 <- x$npp08/1000
x$npp09 <- x$npp09/1000
x$npp10 <- x$npp10/1000
x$npp11 <- x$npp11/1000
x$npp12 <- x$npp12/1000
gc()

x$radi01 <- x$radi01*1000/(24*60*60) #(kJ m-2 day-1) to W/m2
x$radi02 <- x$radi02*1000/(24*60*60)
x$radi03 <- x$radi03*1000/(24*60*60)
x$radi04 <- x$radi04*1000/(24*60*60)
x$radi05 <- x$radi05*1000/(24*60*60)
x$radi06 <- x$radi06*1000/(24*60*60)
x$radi07 <- x$radi07*1000/(24*60*60)
x$radi08 <- x$radi08*1000/(24*60*60)
x$radi09 <- x$radi09*1000/(24*60*60)
x$radi10 <- x$radi10*1000/(24*60*60)
x$radi11 <- x$radi11*1000/(24*60*60)
x$radi12 <- x$radi12*1000/(24*60*60)
gc()

whc <- c(150,125,100,75,50,15,0)*1.5  #Water holding capacity
x$whc <- whc[x$soilIiasaAwc]

library(minpack.lm)
nTage <- c(31,28.25,31,30,31,30,31,31,30,31,30,31)

x$sumP <- rowSums(x[,grep("prec", colnames(x))])
x$meanT <- rowMeans(x[,grep("tmean", colnames(x))])
x$wgt <- (x$sumP/30 - x$meanT) - 10
x$wgt[x$wgt < 0] <- 0
x$wgt[x$wgt > 1] <- 1
plot(x$meanT, x$sumP, pch=".")
with(x[x$wgt < 1,], points(meanT, sumP, col=2, pch="."))
with(x[x$wgt <= 0,], points(meanT, sumP, col=3, pch="."))

#Climate Boundaries
quantile(x$meanT, p=c(0.02,0.98))
quantile(x$sumP, p=c(0.02))
apply(as.data.frame(t(x[,grep("tmean", colnames(x))])), 1, quantile, probs = c(0.02, 0.98),  na.rm = TRUE) 
apply(as.data.frame(t(x[,grep("prec", colnames(x))])), 1, quantile, probs = c(0.02),  na.rm = TRUE) 

library(minpack.lm)
library(Rcpp)
Sys.setenv("PKG_CXXFLAGS"="-Wall -O3 -ffast-math -march=native -funroll-loops -flto -std=c++11")
sourceCpp("../nppModel.cc")

#Low precipitation
t2 <- x[x$wgt < 1,]
t1 <- with(t2, c(rbind(npp01,npp02,npp03,npp04,npp05,npp06,npp07,npp08,npp09,npp10,npp11,npp12)))
a <- nlsLM(t1 ~ nppMod8(c(c0,c1,c2,300,c4,0.06,c6,50,0.002,0.85,20), nTage, rbind(tmean01,tmean02,tmean03,tmean04,tmean05,tmean06,tmean07,tmean08,tmean09,tmean10,tmean11,tmean12), rbind(prec01,prec02,prec03,prec04,prec05,prec06,prec07,prec08,prec09,prec10,prec11,prec12), rbind(radi01,radi02,radi03,radi04,radi05,radi06,radi07,radi08,radi09,radi10,radi11,radi12), elevation, whc), data=t2, start=list(c0=0.002, c1=3, c2=1, c4=1, c6=0.9), trace=T, control = nls.lm.control(maxiter=999))
#159950, Par. =  1.70168e-06    4.97851    1.90225    3.66556   0.159555
a <- nlsLM(t1 ~ nppMod8(c(c0,c1,c2,300,c4,c5,c6,50,0.002,0.85,20), nTage, rbind(tmean01,tmean02,tmean03,tmean04,tmean05,tmean06,tmean07,tmean08,tmean09,tmean10,tmean11,tmean12), rbind(prec01,prec02,prec03,prec04,prec05,prec06,prec07,prec08,prec09,prec10,prec11,prec12), rbind(radi01,radi02,radi03,radi04,radi05,radi06,radi07,radi08,radi09,radi10,radi11,radi12), elevation, whc), data=t2, start=list(c0=1.70168e-06, c1=4.97851, c2=1.90225, c4=3.66556, c5=0.06, c6=0.159555), trace=T, control = nls.lm.control(maxiter=999))
#158090, Par. =  1.39708e-09    13.8965    3.98748     5.5003  0.0828284   0.180158

#
set.seed(1)
t2 <- x[sample(NROW(x), 100000),]
t1 <- with(t2, c(rbind(npp01,npp02,npp03,npp04,npp05,npp06,npp07,npp08,npp09,npp10,npp11,npp12)))
a <- nlsLM(t1 ~ nppMod8(c(c0,c1,c2,300,c4,0.0828,0.18,50,0.002,0.85,20), nTage, rbind(tmean01,tmean02,tmean03,tmean04,tmean05,tmean06,tmean07,tmean08,tmean09,tmean10,tmean11,tmean12), rbind(prec01,prec02,prec03,prec04,prec05,prec06,prec07,prec08,prec09,prec10,prec11,prec12), rbind(radi01,radi02,radi03,radi04,radi05,radi06,radi07,radi08,radi09,radi10,radi11,radi12), elevation, whc), data=t2, start=list(c0=1.39708e-09, c1=13.9, c2=4, c4=5.5), trace=T, control = nls.lm.control(maxiter=999))
#73068.7, Par. =  1.95866e-11    16.6774    5.30899    7.00127
#t1 <- with(x, c(rbind(npp01,npp02,npp03,npp04,npp05,npp06,npp07,npp08,npp09,npp10,npp11,npp12)))
#a <- nlsLM(t1 ~ nppMod8(c(c0,c1,c2,300,c4,0.0828,0.18,50,0.002,0.85,20), nTage, rbind(tmean01,tmean02,tmean03,tmean04,tmean05,tmean06,tmean07,tmean08,tmean09,tmean10,tmean11,tmean12), rbind(prec01,prec02,prec03,prec04,prec05,prec06,prec07,prec08,prec09,prec10,prec11,prec12), rbind(radi01,radi02,radi03,radi04,radi05,radi06,radi07,radi08,radi09,radi10,radi11,radi12), elevation, whc), data=x, start=list(c0=1.95866e-11, c1=16.6774, c2=5.30899, c4=7.00127), trace=T, control = nls.lm.control(maxiter=999))

#plot(t1, predict(a), pch=".")
#cor(t1, predict(a))

#x$nppEst0 <- rowSums(matrix(t1, ncol=12, byrow=T))
t2$nppEst0 <- rowSums(matrix(predict(a), ncol=12, byrow=T))
plot(t2$npp, t2$nppEst0, pch=".")
cor(t2$npp, t2$nppEst0)

#a0 <- lm(npp ~ soil * nppEst0) - 1, data=x)
a0 <- nls(npp ~ c0[soil] * nppEst0, data=t2, start=list(c0=rep(1,23)))
t2$nppEst1 <- predict(a0)
plot(t2$npp, t2$nppEst1, pch=".")
cor(t2$npp, t2$nppEst1)

summary(a0)
###
#plot(t1, predict(a), pch=".")
#cor(t1, predict(a))

#x$nppEst0 <- rowSums(matrix(t1, ncol=12, byrow=T))
x$nppEst0 <- rowSums(matrix(predict(a), ncol=12, byrow=T))
plot(x$npp, x$nppEst0, pch=".")
cor(x$npp, x$nppEst0)

#a0 <- lm(npp ~ soil * nppEst0) - 1, data=x)
a0 <- nls(npp ~ c0[soil] * nppEst0, data=x, start=list(c0=rep(1,23)))
x$nppEst1 <- predict(a0)
plot(x$npp, x$nppEst1, pch=".")
cor(x$npp, x$nppEst1)

summary(a0)
###
     Estimate Std. Error t value Pr(>|t|)    
c01  1.054684   0.007907  133.38   <2e-16 ***
c02  1.099922   0.010203  107.81   <2e-16 ***
c03  1.075196   0.010903   98.62   <2e-16 ***
c04  0.980570   0.008049  121.83   <2e-16 ***
c05  1.002155   0.005113  196.00   <2e-16 ***
c06  1.044522   0.013298   78.55   <2e-16 ***
c07  1.134524   0.010083  112.52   <2e-16 ***
c08  1.073864   0.004305  249.47   <2e-16 ***
c09  1.000548   0.009629  103.91   <2e-16 ***
c010 1.070339   0.004247  252.00   <2e-16 ***
c011 1.068615   0.006136  174.16   <2e-16 ***
c012 1.086483   0.008892  122.18   <2e-16 ***
c013 1.054495   0.008949  117.83   <2e-16 ***
c014 1.036821   0.006477  160.07   <2e-16 ***
c015 1.095323   0.006459  169.58   <2e-16 ***
c016 1.008207   0.004632  217.67   <2e-16 ***
c017 1.094867   0.011379   96.22   <2e-16 ***
c018 1.031270   0.009903  104.14   <2e-16 ***
c019 0.987843   0.005634  175.33   <2e-16 ***
c020 1.035130   0.005366  192.91   <2e-16 ***
c021 0.950606   0.003804  249.89   <2e-16 ***
c022 1.074587   0.007048  152.47   <2e-16 ***
c023 1.008381   0.002976  338.87   <2e-16 ***
#

c0 <- 1.95866e-11
c1 <- 16.6774
c2 <- 5.30899
c3 <- 300
c4 <- 7.00127
c5 <- 0.0828
c6 <- 0.180158
c7 <- 50
c8 <- 0.002
c9 <- 0.85
c10 <- 20

h <- 400
nT <- 365
rad <- 3
t <- -10:35
aux <- 0
p <- 999
plot(t, c0*exp(-h/7990)*nT*rad*pmax(0, (c1+t))^c2 * pmax(0, 1 - 30*exp(17.62*t/(243.12 + t)) / tanh(c5*p) / c3)^c4, type="l")
for(i in 1:8) {
  p <- pmax(0, i*10 - c10 - c7 * tanh(c8 * 30*exp(17.62*t/(243.12 + t))))
  lines(t, c0*exp(-h/7990)*nT*rad*pmax(0, (c1+t))^c2 * pmax(0, 1 - 30*exp(17.62*t/(243.12 + t)) / tanh(c5*p) / c3)^c4, col=i)
}



