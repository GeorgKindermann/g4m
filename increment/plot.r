c0 <- c(0.06179233,0.06448844,0.07000044,0.07867775,0.06506758,0.08137664,0.06192571,0.07169721,0.07110523,0.06381677,0.05441309,0.06347873,0.07584091,0.07330926,0.05766713,0.07205265,0.05055277,0.06077571,0.07759581,0.05685617,0.06527024,0.05558023,0.06699292)
c <- c(1.29460152,-0.09012495,0.17332495, 35, -1, 0.66170523, 2.8, 0.4138984, -54.4741443, -1.4, 1.155907e+00, 2.154028e-04, -3.733458e-01, 2.335792e-05, 2, 0.5, 0.841, -22.131)
tMinJ <- -12.0 - 2
tMaxJ <- 28.0 + 2.;
pMinJ <- 203.;
pMaxJ <- 5900. + 4000.;
tMinM <- -43.1 - 4;
tMaxM <- 32.5 + 5.;
pMinM <- 0;
pMaxM <- 1045 + 1100;
minNpp <- 1.86;

npp <- function(co2, soilType, t, p, sw, altitude, latitude) {
walterLieth <- c[15] * (c[16] + 1./(1. + exp(c[17] + c[18]*co2)))
t3 <- c[7] / (1. + exp(c[8] + c[9]*co2)) + c[10]
days <- c(31.,28.25,31.,30.,31.,30.,31.,31.,30.,31.,30.,31.)
month <- 1
ret <- 0
for(month in 1:12) {
  t1 <- c0[soilType]/(1. + exp(c[1] + c[2]*t[month])) - c0[soilType]*c[3] - c0[soilType]/(1. + exp(c[4]+c[5]*t[month]))
  if(t1<0.) {t1 <- 0.}
  t2 <- 1. - 2./(1. + exp( (sw[month]+p[month]-pmax(0.,t[month])*walterLieth)/(pmax(1.,t[month]*c[6]))))
  if(t2<0.) {t2 <- 0.}
  ret <- ret + t1 * t2 * t3 * days[month]
}
ret <- ret * c[11] + c[12]*altitude + c[13]*cos(latitude) + c[14]*altitude*cos(latitude)
if(ret < minNpp) {ret <- 0.}
if(ret<0.) {ret <- 0.}
ret
}

co2 <- 0.038
soilType <- 1
t <- rep(10,12)
p <- rep(40,12)
sw <- rep(0,12)
altitude <- 400
latitude <- 45*pi/90

npp(co2, soilType, t, p, sw, altitude, latitude)


x <- matrix(0,5,171)
for(j in (1:5)*5) {
  t <- rep(j,12)
  for(i in 0:170) {
    p <- rep(i,12)
    x[j/5,i+1] <- npp(co2, soilType, t, p, sw, altitude, latitude)
  }
}
pdf("/tmp/p.pdf", width=5, height=5)
plot(0:170, x[1,], type="l", ylim=range(x), lwd=3, xlab="Precipitation [mm]", ylab="NPP [tC/ha/Yera]")
lines(0:170, x[2,], col=2, lwd=3)
lines(0:170, x[3,], col=3, lwd=3)
lines(0:170, x[4,], col=4, lwd=3)
lines(0:170, x[5,], col=5, lwd=3)
legend("topleft", legend=c("5°C", "10°C", "15°C", "20°C", "25°C"), lwd=3, col=1:5, bty="n")
dev.off()

prc <- c(25,50,100,200,300)
x <- matrix(0,5,41)
for(j in 1:5) {
  p <- rep(prc[j],12)
  for(i in 0:40) {
    t <- rep(i,12)
    x[j,i+1] <- npp(co2, soilType, t, p, sw, altitude, latitude)
  }
}
pdf("/tmp/t.pdf", width=5, height=5)
plot(0:40, x[1,], type="l", ylim=range(x), lwd=3, xlab="Temperature [°C]", ylab="NPP [tC/ha/Yera]")
lines(0:40, x[2,], col=2, lwd=3)
lines(0:40, x[3,], col=3, lwd=3)
lines(0:40, x[4,], col=4, lwd=3)
lines(0:40, x[5,], col=5, lwd=3)
legend("topleft", legend=c("25 mm", "50 mm", "100 mm", "200 mm", "300 mm"), lwd=3, col=1:5, bty="n")
dev.off()

x <- matrix(0,41,231)
for(i in 0:230) {
  for(j in 0:40) {
    p <- rep(i,12)
    t <- rep(j,12)
    x[j+1,i+1] <- npp(co2, soilType, t, p, sw, altitude, latitude)
   }
}
pdf("/tmp/contour.pdf", width=5, height=5)
contour(0:40, 0:230, x, xlab="Temperature [°C]", ylab="Precipitation [mm]")
dev.off()
