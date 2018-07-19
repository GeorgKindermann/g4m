x <- read.csv("tables.csv")
x <- x[x$sp=="pine",]
x$sp <- factor(x$sp)
x$si <- factor(x$si)

xs <- read.csv("stocking.csv")
xs <- xs[xs$sp=="pine",]
xs$si <- factor(xs$si)

xd <- read.csv("dbhH.csv")
xd <- xd[xd$sp=="pine",]
xd$si <- factor(xd$si)

erg <- matrix(data=NA, nrow = nlevels(x$si), ncol=6)
for(yield in 1:nlevels(x$si)) {
  y <- x[x$si == levels(x$si)[yield],]
  #a <- nls(totste ~ a * exp(b * log(age/c)**2), data=y, start = list(a=470, b=-0.5, c=220))
  a <- nls(tot ~ a * exp(b * log(age/c)**2), data=y, start = list(a=2000, b=-0.2, c=1500))
  erg[yield,] <- c(coef(a), summary(a)$coef[,4])
}
y <- x[x$si == levels(x$si)[1],]
plot(totste~age, data=y, xlim = c(0,max(erg[,3])+10), ylim = c(0,max(erg[,1])))
tmp <- data.frame(age = 0:erg[1,3])
lines(tmp$age, erg[1,1]*exp(erg[1,2]*log(tmp/erg[1,3])**2), lty=2)
text(x = erg[1,3], y = erg[1,1], labels = levels(x$si)[1], pos = 4)
for(i in 2:nlevels(x$si)) {
  y <- x[x$si == levels(x$si)[i],]
  points(totste~age, data=y)
  tmp <- data.frame(age = 0:erg[i,3])
  lines(tmp$age, erg[i,1]*exp(erg[i,2]*log(tmp/erg[i,3])**2), lty=2)
  text(x = erg[i,3], y = erg[i,1], labels = levels(x$si)[i], pos = 4)
}
#tOpt
tOpt <- erg[,3] * exp(1/(2*erg[,2]))
#MAI
MAItot <- erg[,1]*exp(erg[,2]*log(tOpt/erg[,3])**2)/tOpt

erg <- matrix(data=NA, nrow = nlevels(x$si), ncol=6)
for(yield in 1:nlevels(x$si)) {
  y <- x[x$si == levels(x$si)[yield],]
  a <- nls(totste ~ a * exp(b * log(age/c)**2), data=y, start = list(a=470, b=-0.5, c=220))
  erg[yield,] <- c(coef(a), summary(a)$coef[,4])
}
y <- x[x$si == levels(x$si)[1],]
plot(totste~age, data=y, xlim = c(0,max(erg[,3])+10), ylim = c(0,max(erg[,1])))
tmp <- data.frame(age = 0:erg[1,3])
lines(tmp$age, erg[1,1]*exp(erg[1,2]*log(tmp/erg[1,3])**2), lty=2)
text(x = erg[1,3], y = erg[1,1], labels = levels(x$si)[1], pos = 4)
for(i in 2:nlevels(x$si)) {
  y <- x[x$si == levels(x$si)[i],]
  points(totste~age, data=y)
  tmp <- data.frame(age = 0:erg[i,3])
  lines(tmp$age, erg[i,1]*exp(erg[i,2]*log(tmp/erg[i,3])**2), lty=2)
  text(x = erg[i,3], y = erg[i,1], labels = levels(x$si)[i], pos = 4)
}
#tOpt
tOpt <- erg[,3] * exp(1/(2*erg[,2]))
#MAI
MAI <- erg[,1]*exp(erg[,2]*log(tOpt/erg[,3])**2)/tOpt
names(MAI) <- levels(x$si)

#Transformation from MAI to MAItot
tmp <- data.frame(MAI, MAItot)
plot(tmp$MAI, tmp$MAI/tmp$MAItot, xlim=c(0,10), ylim=c(0,1))
a <- nls(tmp$MAI/tmp$MAItot ~ exp(c0*MAI**c1), data=tmp, start=list(c0=-1.1845110, c1=-0.4027689), trace=T)
lines(0.1*0:100, predict(a, newdata=data.frame(MAI=0.1*0:100)))
#
plot(tmp$MAItot, tmp$MAI, xlim=c(0,20), ylim=c(0,10))
abline(0,1)
a <- nls(MAI ~ MAItot*exp(c0*MAItot**c1), data=tmp, start=list(c0=-3.248214, c1=-0.809247), trace=T)
lines(0.2*0:100, predict(a, newdata=data.frame(MAItot=0.2*0:100)))

#Calculate k from MAI
plot(erg[,2]~MAI, xlim = c(0,10), ylim = c(-1.6, 0))
tmp <- data.frame(k=erg[,2], mai=MAI)
tmp <- tmp[tmp$mai < 2,]
a <- -0.65
a <- nls(k ~ a*exp(b*mai**c), start = list(b=-0.5, c=0.5), trace=T,control=nls.control(maxit=999,minFactor=1/2**32), data=tmp, algorithm="port")
a <- nls(k ~ d + a*exp(b*mai**c), start = list(a=-0.65, b=-0.4, c=0.57, d=0), trace=T,control=nls.control(maxit=999,minFactor=1/2**32), data=tmp, algorithm="port")
del <- data.frame(mai = 0:100/10)
lines(del$mai, predict(a, newdata=del))
CK <- a

#Calculate tMax from MAI
plot(erg[,3]~MAI, xlim = c(0,10), ylim = c(80, 300))
tmp <- data.frame(tMax=erg[,3], MAI=MAI)
tmp <- tmp[tmp$MAI > 0.5,]
d <- 170
a <- nls(tMax ~ d + c/(1 + exp(a + b*MAI)), data=tmp, trace = T
         , start = list(a=-8.778685, b=2.504171, c=150)
         ,control=nls.control(maxit=9999,minFactor=1/2**32))
#a <- nls(tMax ~ d + c/(1 + exp(a + b*MAI)), data=tmp, trace = T
#         , start = list(a=-2.5391998, b=0.7020712, c=194.0527073, d=100)
#         ,control=nls.control(maxit=9999,minFactor=1/2**32))
del <- data.frame(MAI = 0:100/10)
lines(del$MAI, predict(a, newdata=del))
CTM <- a

#Calculate TCPMax from MAI
plot(erg[,1]~MAI)
points(MAI, erg[,3] * exp(0.25/erg[,2]) * MAI, col=2)

#Mortality
erg2 <- matrix(data=NA, nrow = nlevels(x$si), ncol=6)
x$actste[x$actste/x$totste > 1] <- x$totste[x$actste/x$totste > 1]
plot(x$totste,x$actste/x$totste)
plot(NA,NA,xlim=c(0,7.5), ylim = c(0,1), xlab = "Age/TOpt", ylab ="VMax/GWL")
x$relage <- NA
for(i in 1:nlevels(x$si)) {
  x$relage[x$si == levels(x$si)[i]] <- x$age[x$si == levels(x$si)[i]]/tOpt[i]
  tmp <- x[x$si == levels(x$si)[i],]
  tmp2 <- tmp
  idx <- sort(tmp$relage, index.return = T)
  for(j in 1:NROW(tmp)) {
    tmp[j,] <- tmp2[idx$ix[j],]
  }
  tmp <- rbind(tmp[1,],tmp)
  tmp$actste[1] <- 0.001; tmp$totste[1] <- 0.001; tmp$age[1] <- 1; tmp$relage[1] <- tmp$age[1]/tOpt[i]
  tmp <- tmp[tmp$actste[2:NROW(tmp)]/tmp$totste[2:NROW(tmp)] < tmp$actste[1:NROW(tmp)-1]/tmp$totste[1:NROW(tmp)-1],]
  points(I(tmp$actste/tmp$totste) ~ I(tmp$age/tOpt[i]), col = i, pch=1, type="l")
  a <- nls(actste/totste ~pmax(0, pmin(1, (c0 + c1*log(relage)) * (1-c2*relage)**0.5))
  , trace = T, data=tmp[-1,], start = list(c0=0.8, c1=-0.006, c2=1/4.5)
  , nls.control(maxiter=999))
  erg2[i,] <- c(coef(a), summary(a)$coef[,4])
d <- data.frame(relage = (0:650)/100)
lines(d$relage, predict(a, newdata=d), type="l", lw=2, col=i)
}

plot(NA,NA,xlim=c(0,7.5), ylim = c(0,1), xlab = "Age/TOpt", ylab ="VMax/GWL")
fu <- function (relage) {
  c0 <- 0.7; c1 <- -.1; c2 <- 0.10;
  pmax(0, pmin(1, (c0 + c1*log(relage)) * (1-c2*relage)**0.5))}
lines(0:70/10,fu(0:70/10))

tmp <- data.frame(MAI = 0:100/10)
plot(erg2[,1]~MAI, pch=19, cex=2*(min(erg2[,4])/erg2[,4])**0.3, xlim=c(0,10), ylim=c(0.45,0.95))
a <- nls(erg2[,1] ~ 0.9/(1 + exp(c0+c1*MAI)), weight=2*(min(erg2[,4])/erg2[,4])**0.3,
         start = list(c0=-0.8242255, c1=-0.4273254), trace=T
         , nls.control(maxiter=999))
lines(tmp$MAI, predict(a, newdata=tmp))

plot(erg2[,2]~MAI, pch=19, cex=2*(min(erg2[,5])/erg2[,5])**0.1, xlim=c(0,10), ylim=c(-1,0))
del <- data.frame(val = erg2[,2], MAI=MAI)
a <- nls(val ~ -0.4/(1+exp(c1 + c2*MAI)) -0.3/(1+exp(c3 + c4*MAI))
         , trace = T, nls.control(maxiter=999), data=del
         , start = list(c1=0.3850213, c2=2.9970913, c3=4.9501050, c4=-1.0023533))
lines(tmp$MAI, predict(a, newdata=tmp))

plot(erg2[,3]~MAI, pch=19, cex=2*(min(erg2[,6])/erg2[,6])**0.3, xlim=c(0,10))
a <- nls(erg2[,3] ~ 0.10 + exp(c0+c1*MAI), weight=2*(min(erg2[,6])/erg2[,6])**0.3
         , trace=T, start = list(c0=-1.5, c1=-0.15))
lines(tmp$MAI, predict(a, newdata=tmp))

#Managed Stocking degree
xs$mai <- MAI[xs$si]
names(tOpt) <- names(MAI)
xs$tOpt <- tOpt[xs$si]
xs$r1 <- xs$sd1/pmax(xs$sd1, xs$sd08, xs$sd06, xs$sd04)
xs$r08 <- xs$sd08/pmax(xs$sd1, xs$sd08, xs$sd06, xs$sd04)
xs$r06 <- xs$sd06/pmax(xs$sd1, xs$sd08, xs$sd06, xs$sd04)
xs$r04 <- xs$sd04/pmax(xs$sd1, xs$sd08, xs$sd06, xs$sd04)

plot(NA, NA, type="n", xlim=c(1,4), ylim=c(0,1))
for(i in 1:NROW(xs)) {
  lines(1:4, c(xs$r1[i], xs$r08[i], xs$r06[i], xs$r04[i]), col=xs$age[i]/10)
}

tdat <- data.frame(mai=xs$mai, age=xs$age, sd=rep(c(1, 0.8, 0.6, 0.4), each=NROW(xs)), inc = c(xs$r1, xs$r08, xs$r06, xs$r04), relage=xs$age/xs$tOpt)

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

nd <- data.frame(sd=0:10/10)
plot(nd$sd, di(nd$sd,4.3,1))
age <- 80
mai <- 10
#1 + (age**1.5)/150 * 1/(0.01 + 0.5*mai**0.5)
#1 + 0.8/(1 + (1/500) * age**2 * 1/(0.01 + mai**0.5))
di(nd$sd, 1 + (age**1.5)/150 * 1/(0.01 + 0.5*mai**0.5), 1 + 0.8/(1 + (1/500) * age**2 * 1/(0.01 + mai**0.5)))

nd <- data.frame(sd = 0:10/10, age=20, mai=1)
plot(nd$sd, di(nd$sd, 1 + (nd$age**1.5)/150 * 1/(0.01 + 0.5*nd$mai**0.5), 1 + 0.8/(1 + (1/500) * nd$age**2 * 1/(0.01 + nd$mai**0.5))), type="l", xlim=c(0,1), ylim=c(0,1))
nd <- data.frame(sd = 0:10/10, age=60, mai=1)
lines(nd$sd, di(nd$sd, 1 + (nd$age**1.5)/150 * 1/(0.01 + 0.5*nd$mai**0.5), 1 + 0.8/(1 + (1/500) * nd$age**2 * 1/(0.01 + nd$mai**0.5))), col=2)
nd <- data.frame(sd = 0:10/10, age=120, mai=1)
lines(nd$sd, di(nd$sd, 1 + (nd$age**1.5)/150 * 1/(0.01 + 0.5*nd$mai**0.5), 1 + 0.8/(1 + (1/500) * nd$age**2 * 1/(0.01 + nd$mai**0.5))), col=3)

tdat$mod <- di(tdat$sd, 1 + (tdat$age**1.5)/150 * 1/(0.01 + 0.5*tdat$mai**0.5), 1 + 0.8/(1 + (1/500) * tdat$age**2 * 1/(0.01 + tdat$mai**0.5)))

plot(NA, NA, type="n", xlim=c(0,1), ylim=c(0,1))
for(i in levels(as.factor(tdat$mai))) {
  t1 <- tdat[tdat$mai == i,]
  for(j in levels(as.factor(t1$age))) {
    t2 <- t1[t1$age==j,]
    #lines(t2$sd, t2$inc, col=t2$age/10, type="l")
    lines(t2$sd, t2$mod, col=t2$age/10, type="l")
  }
}

cor(tdat$mod, tdat$inc)
plot(tdat$mod, tdat$inc)

#Managed stand density - Where increment is 0.95 of max increment
#Create with interpolation table

#Height
xd$mai <- MAI[xd$si]
xd$tOpt <- tOpt[xd$si]
plot(NA,NA,xlim=c(0,max(xd$age)), ylim = c(0,max(xd$h)), xlab="Age", ylab ="Height")
for(i in 1:nlevels(xd$si)) {
  tmp <- xd[xd$si == levels(xd$si)[i],]
  points(tmp$h ~ tmp$age, col = i, pch=1)
}
a <- nls(h ~ c0*mai**c3* (1-exp(c1*age))**(c2*mai**c4)
         , data = xd, trace=T
         , start = list(c0=17, c1=-0.03, c2=1.13, c3=0.8, c4=1))
for(i in 1:nlevels(xd$si)) {
  d <- data.frame(age = 0:450, mai = MAI[i])
  lines(d$age, predict(a, newdata=d), type="l", col = i)
}
plot(NA,NA, type="n", xlim=c(0,450), ylim=c(0,100), xlab="Age", ylab ="Height")
for(i in 1:10) {
  d <- data.frame(age = 0:450, mai = i)
  lines(d$age, predict(a, newdata=d), type="l", col = i)
}
c <- coef(a)
mai <- 2
Age <- 9.766196
c[1]*mai**c[4]* (1-exp(c[2]*Age))**(c[3]*mai**c[5])
h <- 1.3
(log(mai**(-c[4]*mai**-c[5]/c[3])*(mai**(c[4]*mai**-c[5]/c[3])-(h/c[1])**(mai**-c[5]/c[3]))))/c[2]
#They are equal but the second is shorter
#xd$td0 <- (log(xd$mai**(-c[4]*xd$mai**-c[5]/c[3])*(xd$mai**(c[4]*xd$mai**-c[5]/c[3])-(h/c[1])**(xd$mai**-c[5]/c[3]))))/c[2]
xd$td0 <- log(1-(1.3/(c[1]*xd$mai**c[4]))**(1/(c[3]*xd$mai**c[5])))/c[2]

#DBH
plot(NA,NA,xlim=c(0,max(xd$age)), ylim = c(0,max(xd$d, na.rm=T))
     , xlab="Age", ylab ="DBH")
erg <- matrix(data=NA, nrow = nlevels(xd$si), ncol=3)
for(i in 1:nlevels(xd$si)) {
  tmp <- xd[xd$si == levels(xd$si)[i],]
  points(tmp$d ~ tmp$age, col = i, pch=1)
  #a <- nls(d ~ a*(age-td0)**b, data = tmp, start = list(a=1, b=1))
  d <- data.frame(age = 0:450, mai = MAI[i])
  d$td0 <- (log(d$mai**(-c[4]*d$mai**-c[5]/c[3])*(d$mai**(c[4]*d$mai**-c[5]/c[3])-(h/c[1])**(d$mai**-c[5]/c[3]))))/c[2]
  #lines(d$age, predict(a, newdata=d), type="l", col = i)
  #erg[i,] <- c(coef(a), tmp$mai[1])
  a <- nls(d ~ a * (1-exp(b*(age-td0)))**c
           , data = tmp, trace=T
           , start = list(a=100, b=-0.003, c=1))
  erg[i,] <- coef(a)
  #lines(d$age, predict(a, newdata=d), type="l", col = i)
}
plot(erg[,1] ~ MAI)
a <- nls(erg[,1] ~ c0 + c1*MAI, start = list(c0=22, c1=16), trace=T)
lines(MAI, predict(a))
plot(erg[,2] ~ MAI)
a <- nls(erg[,2] ~ c2/(1+c3*MAI**c4), start = list(c2=-0.011, c3=0.25, c4=-1.8), trace=T)
lines(MAI, predict(a))
plot(erg[,3] ~ MAI)
a <- nls(erg[,3] ~ c5/(1+c6*MAI**c7), start = list(c5=1, c6=0.1, c7=-1.6),
 trace=T)
lines(MAI, predict(a))

a <- nls(d ~ (c0 + c1*mai) * (1-exp((c2/(1+c3*mai**c4))*(age-td0)))**(c5/(1+c6*mai**c7))
         , data = xd[xd$age > xd$td0,], trace=T
         , start = list(c0=25, c1=10, c2=-0.011, c3=0.25, c4=-1.8, c5=1, c6=0.1, c7=-1.6))

for(i in 1:nlevels(xd$si)) {
  d <- data.frame(age = 0:450, mai = MAI[i])
  h <- 1.3
  d$td0 <- (log(d$mai**(-c[4]*d$mai**-c[5]/c[3])*(d$mai**(c[4]*d$mai**-c[5]/c[3])-(h/c[1])**(d$mai**-c[5]/c[3]))))/c[2]
  lines(d$age, predict(a, newdata=d), type="l", col = i)
}

del <- data.frame(age = 10*0:45, mai=10)
h <- 1.3
del$td0 <- (log(del$mai**(-c[4]*del$mai**-c[5]/c[3])*(del$mai**(c[4]*del$mai**-c[5]/c[3])-(h/c[1])**(del$mai**-c[5]/c[3]))))/c[2]
plot(del$age, predict(a, newdata=del), type="l")

#DBH-Increment dependency to stocking degree
#bhdSD = dbh * (c0-c1*sd**c1) #c0=2, c1=1, c2=1.6
nd <- 0:10/10
plot(nd, 2-1*nd**1.6)


