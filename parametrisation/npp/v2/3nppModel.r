x <- read.table("/data/tmp/baseData10.txt")
names(x) <- c("x","y","landcover","npp","npp01","npp02","npp03","npp04","npp05","npp06","npp07","npp08","npp09","npp10","npp11","npp12","ecoRegionFao","soilIiasaFao90","soilIiasaSwr","soilIiasaAwc","elevation","prec01","prec02","prec03","prec04","prec05","prec06","prec07","prec08","prec09","prec10","prec11","prec12","tmean01","tmean02","tmean03","tmean04","tmean05","tmean06","tmean07","tmean08","tmean09","tmean10","tmean11","tmean12","radi01","radi02","radi03","radi04","radi05","radi06","radi07","radi08","radi09","radi10","radi11","radi12")

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

x$radi01 <- x$radi01/3.6/1000 #(kJ m-2 day-1) to kw/m2/day
x$radi02 <- x$radi02/3.6/1000
x$radi03 <- x$radi03/3.6/1000
x$radi04 <- x$radi04/3.6/1000
x$radi05 <- x$radi05/3.6/1000
x$radi06 <- x$radi06/3.6/1000
x$radi07 <- x$radi07/3.6/1000
x$radi08 <- x$radi08/3.6/1000
x$radi09 <- x$radi09/3.6/1000
x$radi10 <- x$radi10/3.6/1000
x$radi11 <- x$radi11/3.6/1000
x$radi12 <- x$radi12/3.6/1000
gc()

whc <- c(150,125,100,75,50,15,0)*1.5  #Water holding capacity
x$whc <- whc[x$soilIiasaAwc]

library(minpack.lm)
nTage <- c(31,28.25,31,30,31,30,31,31,30,31,30,31)

tfun <- function(c0, c1, c2, c3, c4, c5, t, p, radi, h) {
    aux <- 0
    nT <- rep(nTage, each=floor(length(t)/12))
    c0*exp(-h/7990)*tapply(pmax(0, nT*radi*(c1+t))^c2 * pmax(0, 1 - c3*exp(17.62*t/(243.12 + t)) / tanh(c4*(aux + p)))^c5, rep(1:floor(length(t)/12), 12), sum)
}
a <- nlsLM(npp ~ tfun(c0, c1, c2, c3, c4, c5, c(tmean01,tmean02,tmean03,tmean04,tmean05,tmean06,tmean07,tmean08,tmean09,tmean10,tmean11,tmean12), c(prec01,prec02,prec03,prec04,prec05,prec06,prec07,prec08,prec09,prec10,prec11,prec12), c(radi01,radi02,radi03,radi04,radi05,radi06,radi07,radi08,radi09,radi10,radi11,radi12), elevation), data=x, start=list(c0=0.18, c1=-5, c2=0.3, c3=0.1, c4=0.01, c5=0.6), trace=T, control = nls.lm.control(maxiter=999))
#482169: 0.177963   -4.99095   0.295135   0.127134  0.0829582   0.622811

aux <- 0
h <- 400
t <- -10:40
radi <- 2
c0 <- 0.177963
c1 <- -4.99095
c2 <- 0.295135
c3 <- 0.127134
c4 <- 0.0829582
c5 <- 0.622811

c0 <- 0.006
c1 <- -5
c2 <- 0.2
c3 <- 0.15
c4 <- 0.05
c5 <- 0.4
p <- 100
plot(t,365.25*c0*exp(-h/7990)*pmax(0, nT*radi*(c1+t))^c2 * pmax(0, 1 - c3*exp(17.62*t/(243.12 + t)) / tanh(c4*(aux + p)))^c5, type="l")
p <- 20
lines(t,365.25*c0*exp(-h/7990)*pmax(0, nT*radi*(c1+t))^c2 * pmax(0, 1 - c3*exp(17.62*t/(243.12 + t)) / tanh(c4*(aux + p)))^c5, col=2)

x$sumP <- rowSums(x[,grep("prec", colnames(x))])
x$meanT <- rowMeans(x[,grep("tmean", colnames(x))])
x$wgt <- (x$sumP/30 - x$meanT) - 10
x$wgt[x$wgt < 0] <- 0
x$wgt[x$wgt > 1] <- 1
plot(x$meanT, x$sumP, pch=".")
with(x[x$wgt < 1,], points(meanT, sumP, col=2, pch="."))
with(x[x$wgt <= 0,], points(meanT, sumP, col=3, pch="."))

library(minpack.lm)
library(Rcpp)
Sys.setenv("PKG_CXXFLAGS"="-Wall -O3 -ffast-math -march=native -funroll-loops -flto -std=c++11")
sourceCpp("nppModel.cc")

nppMod7(c(0.004, -0.5, 1, 190, 0.85, 0.06, 0.9, 32, 0.0023, 0.37, 32), nTage, rep(12,12), rep(80,12), rep(2,12), 400, 100)
nppMod8(c(0.004, -0.5, 1, 190, 0.85, 0.06, 0.9, 32, 0.0023, 0.37, 32), nTage, rep(12,12), rep(80,12), rep(2,12), 400, 100)

#Low precipitation
t2 <- x[x$wgt < 1,]
t1 <- with(t2, c(rbind(npp01,npp02,npp03,npp04,npp05,npp06,npp07,npp08,npp09,npp10,npp11,npp12)))
a <- nlsLM(t1 ~ nppMod8(c(c0,c1,c2,300,c4,0.04,c6,50,0.002,1,20), nTage, rbind(tmean01,tmean02,tmean03,tmean04,tmean05,tmean06,tmean07,tmean08,tmean09,tmean10,tmean11,tmean12), rbind(prec01,prec02,prec03,prec04,prec05,prec06,prec07,prec08,prec09,prec10,prec11,prec12), rbind(radi01,radi02,radi03,radi04,radi05,radi06,radi07,radi08,radi09,radi10,radi11,radi12), elevation, whc), data=t2, start=list(c0=0.002, c1=3, c2=1, c4=1, c6=0.9), trace=T, control = nls.lm.control(maxiter=999))
#
a <- nlsLM(t1 ~ nppMod8(c(c0,c1,c2,300,c4,0.04,c6,50,0.002,c9,20), nTage, rbind(tmean01,tmean02,tmean03,tmean04,tmean05,tmean06,tmean07,tmean08,tmean09,tmean10,tmean11,tmean12), rbind(prec01,prec02,prec03,prec04,prec05,prec06,prec07,prec08,prec09,prec10,prec11,prec12), rbind(radi01,radi02,radi03,radi04,radi05,radi06,radi07,radi08,radi09,radi10,radi11,radi12), elevation, whc), data=t2, start=list(c0=0.0024, c1=-1.8, c2=0.5, c4=1, c6=0.26, c9=1), trace=T, control = nls.lm.control(maxiter=999))
#
t1 <- with(x, c(rbind(npp01,npp02,npp03,npp04,npp05,npp06,npp07,npp08,npp09,npp10,npp11,npp12)))
a <- nlsLM(t1 ~ nppMod8(c(c0,c1,c2,300,c4,0.06,0.35,50,0.002,1,20), nTage, rbind(tmean01,tmean02,tmean03,tmean04,tmean05,tmean06,tmean07,tmean08,tmean09,tmean10,tmean11,tmean12), rbind(prec01,prec02,prec03,prec04,prec05,prec06,prec07,prec08,prec09,prec10,prec11,prec12), rbind(radi01,radi02,radi03,radi04,radi05,radi06,radi07,radi08,radi09,radi10,radi11,radi12), elevation, whc), data=x, start=list(c0=0.0024, c1=-1.8, c2=0.5, c4=1), trace=T, control = nls.lm.control(maxiter=999))

c0 <- 0.00113115
c1 <- -0.324885
c2 <- 0.881869
c3 <- 300
c4 <- 1.42346
c5 <- 0.06
c6 <- 0.35
c7 <- 50
c8 <- 0.002
c9 <- 1
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


#Low precipitation
t1 <- x[x$wgt < 1,]
a <- nlsLM(npp ~ nppMod7(c(c0,c1,c2,c3,c4,c5,c6,c7,c8,0.4,c10), nTage, rbind(tmean01,tmean02,tmean03,tmean04,tmean05,tmean06,tmean07,tmean08,tmean09,tmean10,tmean11,tmean12), rbind(prec01,prec02,prec03,prec04,prec05,prec06,prec07,prec08,prec09,prec10,prec11,prec12), rbind(radi01,radi02,radi03,radi04,radi05,radi06,radi07,radi08,radi09,radi10,radi11,radi12), elevation, whc), data=t1, start=list(c0=0.006, c1=-2, c2=0.5, c3=187, c4=1.5, c5=0.08, c6=0.77, c7=100, c8=0.00088, c10=18), trace=T, control = nls.lm.control(maxiter=999))

a <- nlsLM(npp ~ nppMod7(c(c0, -0.5, c2, 190, 0.85, c5, 0.9, 32, 0.0023, 0.37, 32), nTage, rbind(tmean01,tmean02,tmean03,tmean04,tmean05,tmean06,tmean07,tmean08,tmean09,tmean10,tmean11,tmean12), rbind(prec01,prec02,prec03,prec04,prec05,prec06,prec07,prec08,prec09,prec10,prec11,prec12), rbind(radi01,radi02,radi03,radi04,radi05,radi06,radi07,radi08,radi09,radi10,radi11,radi12), elevation, whc), data=x, start=list(c0=0.004, c2=1, c5=0.06), trace=T, control = nls.lm.control(maxiter=999))



a <- nlsLM(npp ~ nppMod6(c(c0,c1,c2,c3,c4,c5,c6,c7,c8), nTage, rbind(tmean01,tmean02,tmean03,tmean04,tmean05,tmean06,tmean07,tmean08,tmean09,tmean10,tmean11,tmean12), rbind(prec01,prec02,prec03,prec04,prec05,prec06,prec07,prec08,prec09,prec10,prec11,prec12), rbind(radi01,radi02,radi03,radi04,radi05,radi06,radi07,radi08,radi09,radi10,radi11,radi12), elevation, whc), data=x, start=list(c0=0.002, c1=-3, c2=1, c3=1, c4=30, c5=1, c6=0.5, c7=10,c8=1), trace=T, control = nls.lm.control(maxiter=999))
#507798: 0.00529007 -5.08529 0.292881 0.114555 29.7999 0.32801 0.831027 12.6577 0.410344

a <- nlsLM(npp ~ nppMod6(c(c0,c1,c2,c3,c4,c5,c6,c7,c8), nTage, rbind(tmean01,tmean02,tmean03,tmean04,tmean05,tmean06,tmean07,tmean08,tmean09,tmean10,tmean11,tmean12), rbind(prec01,prec02,prec03,prec04,prec05,prec06,prec07,prec08,prec09,prec10,prec11,prec12), rbind(radi01,radi02,radi03,radi04,radi05,radi06,radi07,radi08,radi09,radi10,radi11,radi12), elevation, whc), data=x, start=list(c0=0.005, c1=-3, c2=0.3, c3=0.11, c4=30, c5=0.3, c6=0.8, c7=12, c8=0.4), trace=T, control = nls.lm.control(maxiter=999))



a <- nlsLM(npp ~ nppMod5(c(c0,c1,c2,c3,c4,c5,c6), nTage, rbind(tmean01,tmean02,tmean03,tmean04,tmean05,tmean06,tmean07,tmean08,tmean09,tmean10,tmean11,tmean12), rbind(prec01,prec02,prec03,prec04,prec05,prec06,prec07,prec08,prec09,prec10,prec11,prec12), rbind(radi01,radi02,radi03,radi04,radi05,radi06,radi07,radi08,radi09,radi10,radi11,radi12), elevation, whc), data=x, start=list(c0=0.00003, c1=3, c2=1, c3=0.15, c4=0.02, c5=1, c6=0.7), trace=T, control = nls.lm.control(maxiter=999))
#521614: 0.00609688 -5.49181 0.212137 0.142299 0.0457305 0.370809 0.905398
a <- nls(npp ~ nppMod5(c(c0,c1,c2,c3,c4,c5,c6), nTage, rbind(tmean01,tmean02,tmean03,tmean04,tmean05,tmean06,tmean07,tmean08,tmean09,tmean10,tmean11,tmean12), rbind(prec01,prec02,prec03,prec04,prec05,prec06,prec07,prec08,prec09,prec10,prec11,prec12), rbind(radi01,radi02,radi03,radi04,radi05,radi06,radi07,radi08,radi09,radi10,radi11,radi12), elevation, whc), data=x, start=list(c0=0.006, c1=-5, c2=0.2, c3=0.15, c4=0.05, c5=0.4, c6=0.9), trace=T, control = nls.control(maxiter=999))
a <- nls(npp ~ nppMod5(c(c0,c1,c2,c3,c4,c5,c6), nTage, rbind(tmean01,tmean02,tmean03,tmean04,tmean05,tmean06,tmean07,tmean08,tmean09,tmean10,tmean11,tmean12), rbind(prec01,prec02,prec03,prec04,prec05,prec06,prec07,prec08,prec09,prec10,prec11,prec12), rbind(radi01,radi02,radi03,radi04,radi05,radi06,radi07,radi08,radi09,radi10,radi11,radi12), elevation, whc), data=x, start=list(c0=0.006, c1=-5, c2=0.2, c3=0.15, c4=0.05, c5=0.4, c6=0.9), trace=T, control = nls.control(maxiter=999),algo="port")


with(x, plot(npp, nppMod5(c(0.006,-5,0.2,0.15,0.05,0.4,0.9), nTage, rbind(tmean01,tmean02,tmean03,tmean04,tmean05,tmean06,tmean07,tmean08,tmean09,tmean10,tmean11,tmean12), rbind(prec01,prec02,prec03,prec04,prec05,prec06,prec07,prec08,prec09,prec10,prec11,prec12), rbind(radi01,radi02,radi03,radi04,radi05,radi06,radi07,radi08,radi09,radi10,radi11,radi12), elevation, whc),pch=".", ylab=""))

#Danach Multiplikator fuer den Standort suchen


