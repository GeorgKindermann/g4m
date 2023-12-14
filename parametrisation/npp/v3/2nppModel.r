lc <- 4
er <- 2

#Available water von Einzelbaumfunktion uebernehmen ## OK
#Bodenwasserspeicher nicht WC sondern WW  ## OK
#Salinity Mittelwert nicht Summe ## OK
#Salinity eventuell anderer Funktionsverlauf ## OK
#Bodenwasser - Irrigation map
#Stauhorizont, Bodentyp: Gley ? Pseudogley
#CO2 PPM -> Anteil (range 0-1) oder doch im PPM? Eigentlich egal - Multiplikator

#x <- read.table(pipe("cat /data/tmp/header.txt /data/tmp/lc1er3.txt"), header=TRUE)
#x <- read.table(pipe(paste("/bin/bash -c", shQuote("get_fixed_random() {
#  openssl enc -aes-256-ctr -pass pass:\"$1\" -nosalt </dev/zero 2>/dev/null
#}
#shuf -n 10000 --random-source=<(get_fixed_random 42) /data/tmp/lc1er3.txt >/tmp/tmp.tmp; cat /data/tmp/header.txt /tmp/tmp.tmp"))), header=TRUE)
x <- read.table(pipe(paste0("/bin/bash -c 'get_fixed_random() {
  openssl enc -aes-256-ctr -pass pass:\"$1\" -nosalt </dev/zero 2>/dev/null
}
shuf -n 10000 --random-source=<(get_fixed_random 42) /data/tmp/lc", lc, "er", er, ".txt >/tmp/tmp.tmp; cat /data/tmp/header.txt /tmp/tmp.tmp'")), header=TRUE)

Dw <- aperm(simplify2array(Map(function(y) {
    i <- grep(paste0("^",y,"\\d{6}$"), names(x))
    tt <- x[,i]
    array(t(tt), c(12, ncol(tt) / 12, nrow(tt)), list(month=NULL, year=substr(names(x)[i], 2, 5)[c(rep(FALSE, 11), TRUE)], point=NULL))}
  , c("t", "p", "r", "tMin", "tMax"))), c(1,2,4,3))

DnppY <- t(x[,grep("^npp\\d{4}$", names(x))])  #NppYear
DnppY[DnppY > 32760] <- NA
DnppY <- DnppY * 0.0001 #kg C/m²/Year
DnppY <- DnppY * 10 #tC/ha/Year

i <- grep("^npp\\d{6}$", names(x))  #NppMonth
tt <- x[,i]
DnppM <- array(t(tt), c(12, ncol(tt) / 12, nrow(tt)), list(month=NULL, year=substr(names(x)[i], 4, 7)[c(rep(FALSE, 11), TRUE)], point=NULL))
DnppM[DnppM > 32760] <- NA
DnppM <- DnppM * 0.0001 #kg C/m²/8 Day
DnppM <- DnppM * 10     #tC/ha/8 Day
DnppM <- DnppM / 8 * 30 #tC/ha/30 Days

D <- x[,1:32]
rm(x)
gc()

co2 <- data.frame(year = 1980:2020, mean = c(338.91, 340.11, 340.86, 342.53, 344.07, 345.54, 346.97, 348.68, 351.16, 352.78, 354.05, 355.39, 356.09, 356.84, 358.33, 360.17, 361.93, 363.05, 365.7, 367.8, 368.97, 370.57, 372.59, 375.15, 376.95, 378.98, 381.15, 382.9, 385.02, 386.5, 388.76, 390.63, 392.65, 395.4, 397.34, 399.65, 403.08, 405.22, 407.62, 410.07, 412.48))
co2 <- setNames(co2$mean[match(dimnames(Dw)$year, co2$year)], dimnames(Dw)$year)

library(Rcpp)
sourceCpp("soilWater.cc")

nTage <- c(31,28.25,31,30,31,30,31,31,30,31,30,31)
Uw <- array(soilWater(c(0.8, 0, 0, 0, .75), D$ww, nTage, Dw[,,"t",], Dw[,,"p",]), dim(Dw)[-3])

library(minpack.lm)

set.seed(42)
tt <- data.frame(
    nppM=c(DnppM)
  , radi = c(Dw[,,"r",])
  , t = c(Dw[,,"t",])
  , p = c(Dw[,,"p",])
  , uw = c(Uw)
  , co2 = rep(co2, each=12)
  , nn = rep(D$nn, each=prod(dim(Dw)[1:2]))
  , n = rep(D$wn, each=prod(dim(Dw)[1:2]))
#  , c = rep(D$wc, each=prod(dim(Dw)[1:2]))
  , po = rep(D$pSec, each=prod(dim(Dw)[1:2]))
  , s = rep((D$isalinityTop + D$isalinitySub)/2, each=prod(dim(Dw)[1:2]))
  , ph = rep(D$sphh20/10, each=prod(dim(Dw)[1:2]))
)
#tt <- tt[sample(nrow(tt), min(nrow(tt), 1e6)),]
rm(list=setdiff(ls(), "tt"))
gc()
i <- which(tt$s > quantile(tt$s, 0.95))
i <- i[sample.int(length(i), .2e4)]
i <- union(i, sample(nrow(tt), 1e4))


a <- nlsLM(nppM ~ c0 * exp(-nn/7990)    #Luftdruck
           * pmax(0,  cr0 + radi)^cr1   #Strahlung
           * pmax(0, tanh(ct0*pmax(0, ct1 + t)) - pmax(0, ct2 * t)^ct3) #Temp
           * pmax(0, (tanh(0.003 * (co2 - 50)) * uw - 0.1) / (1 - 0.1))^cp0 #Trockenheit
           * (1 - exp(cn0 * n))^cn1 #N - Mitscherlich
           * (1 - exp(cpo0 * po))^cpo1 #P
           * pmax(0, 1 - cs0^2*s^cs1) #Salz
           * exp(log(pmax(0, cph0 + ph)) + cph1*log(pmax(0, cph0 + ph))^2) #pH
         , data = tt[i,]
         , start=list(c0=0.6, cr0=-40, cr1=0.7, ct0=0.004, ct1=1.2, ct2=0.02, ct3=4.5, cp0=0.3, cn0=-0.05, cn1=0.2, cpo0=-0.32, cpo1=3.8, cs0=0.02, cs1=3, cph0=-4, cph1=-0.8)
         , trace=T, control = nls.lm.control(maxiter=999))
a <- nlsLM(nppM ~ c0 * exp(-nn/7990)    #Luftdruck
           * pmax(0,  cr0 + radi)^cr1   #Strahlung
           * pmax(0, tanh(ct0*pmax(0, ct1 + t)) - pmax(0, ct2 * t)^ct3) #Temp
           * pmax(0, (tanh(0.003 * (co2 - 50)) * uw - 0.1) / (1 - 0.1))^cp0 #Trockenheit
           * (1 - exp(cn0 * n))^cn1 #N - Mitscherlich
           * (1 - exp(cpo0 * po))^cpo1 #P
           * pmax(0, 1 - cs0^2*s^cs1) #Salz
           * exp(log(pmax(0, cph0 + ph)) + cph1*log(pmax(0, cph0 + ph))^2) #pH
         , data = tt
         , start=list(c0=0.6, cr0=-40, cr1=0.7, ct0=0.004, ct1=1.2, ct2=0.02, ct3=4.5, cp0=0.3, cn0=-0.05, cn1=0.2, cpo0=-0.32, cpo1=3.8, cs0=0.02, cs1=3, cph0=-4, cph1=-0.8)
         , trace=T, control = nls.lm.control(maxiter=999))

tt <- c(lc = lc, er = er, coef(a))
cat(names(tt), "\n", tt, file = paste0("coefLc", lc, "er", er, ".txt"))

plot(tt$radi, pmax(0,  coef(a)["cr0"] + tt$radi)^coef(a)["cr1"], pch=".")
plot(tt$t, pmax(0, tanh(coef(a)["ct0"]*pmax(0, coef(a)["ct1"] + tt$t)) - pmax(0, coef(a)["ct2"] * tt$t)^coef(a)["ct3"]), pch=".")
plot(tt$uw * tanh(0.003 * (tt$co2 - 50)), pmax(0, (tanh(0.003 * (tt$co2 - 50)) * tt$uw - 0.1) / (1 - 0.1))^coef(a)["cp0"], pch=".")
plot(tt$n, (1 - exp(coef(a)["cn0"] * tt$n))^coef(a)["cn1"], pch=".", log="x")
plot(tt$po, (1 - exp(coef(a)["cpo0"] * tt$po))^coef(a)["cpo1"], pch=".")
plot(tt$s, 1 - coef(a)["cs0"]^2*tt$s^coef(a)["cs1"], pch=".")
plot(tt$ph, exp(log(pmax(0, coef(a)["cph0"] + tt$ph)) + coef(a)["cph1"]*log(pmax(0, coef(a)["cph0"] + tt$ph))^2), pch=".")









a <- nlsLM(nppM ~ c0 * exp(-nn/7990)    #Luftdruck
           * pmax(0,  cr0 + radi)^cr1   #Strahlung
           * pmax(0, tanh(ct0*pmax(0, ct1 + t)) - pmax(0, ct2 * t)^ct3) #Temp
           * pmax(0, (tanh(0.003 * (co2 - 50)) * uw)^cp0) #Trockenheit
           * (1 - exp(cn0 * n))^cn1 #N - Mitscherlich
           * (1 - exp(cpo0 * po))^cpo1 #P
           * pmax(0, 1 - cs0^2*s^cs1) #Salz
           * exp(log(pmax(0, cph0 + ph)) + cph1*log(pmax(0, cph0 + ph))^2) #pH
         , data = tt[i,]
         , start=list(c0=0.6, cr0=-40, cr1=0.7, ct0=0.004, ct1=1.2, ct2=0.02, ct3=4.5, cp0=0.3, cn0=-0.05, cn1=0.2, cpo0=-0.32, cpo1=3.8, cs0=0.02, cs1=3, cph0=-4, cph1=-0.8)
         , trace=T, control = nls.lm.control(maxiter=999))
s <- coef(a)
rm(a)
a <- nlsLM(nppM ~ c0 * exp(-nn/7990)    #Luftdruck
           * pmax(0,  cr0 + radi)^cr1   #Strahlung
           * pmax(0, tanh(ct0*pmax(0, ct1 + t)) - pmax(0, ct2 * t)^ct3) #Temp
           * pmax(0, (tanh(0.003 * (co2 - 50)) * uw)^cp0) #Trockenheit
           * (1 - exp(cn0 * n))^cn1 #N - Mitscherlich
           * (1 - exp(cpo0 * po))^cpo1 #P
           * pmax(0, 1 - cs0^2*s^cs1) #Salz
           * exp(log(pmax(0, cph0 + ph)) + cph1*log(pmax(0, cph0 + ph))^2) #pH
         , data = tt
         , start=s
         , trace=T, control = nls.lm.control(maxiter=999))

tt <- c(lc = lc, er = er, coef(a))
cat(names(tt), "\n", tt, file = paste0("coefLc", lc, "er", er, ".txt"))

plot(tt$radi, pmax(0,  coef(a)["cr0"] + tt$radi)^coef(a)["cr1"], pch=".")
plot(tt$t, pmax(0, tanh(coef(a)["ct0"]*pmax(0, coef(a)["ct1"] + tt$t)) - pmax(0, coef(a)["ct2"] * tt$t)^coef(a)["ct3"]), pch=".")
plot(tt$uw, pmax(0, (tanh(0.003 * (tt$co2 - 50)) * tt$uw)^coef(a)["cp0"]), pch=".")
plot(tt$n, (1 - exp(coef(a)["cn0"] * tt$n))^coef(a)["cn1"], pch=".", log="x")
plot(tt$po, (1 - exp(coef(a)["cpo0"] * tt$po))^coef(a)["cpo1"], pch=".")
plot(tt$s, 1 - coef(a)["cs0"]^2*tt$s^coef(a)["cs1"], pch=".")
plot(tt$ph, exp(log(pmax(0, coef(a)["cph0"] + tt$ph)) + coef(a)["cph1"]*log(pmax(0, coef(a)["cph0"] + tt$ph))^2), pch=".")









x <- -10:30
plot(pmax(0, exp(-log(x)^2) - 0.1*x^2)^0.5)
plot(pmax(0, x - 0.3*x^2)^0.5)
plot(pmax(0, exp(-log(.03*pmax(0, 1.7+x))^2) - 0.0168^2*pmax(0,x)^2)^68)
plot(x, pmax(0, tanh(0.012*pmax(0, 1.1 + x)) - pmax(0, 0.0286 * x)^4.3))

a <- nlsLM(nppM ~ c0 * exp(-nn/7990)    #Luftdruck
           * pmax(0,  cr0 + radi)^cr1   #Strahlung
           * pmax(0, tanh(ct0*pmax(0, ct1 + t)) - pmax(0, ct2 * t)^ct3) #Temp
           #* pmax(0, (tanh(0.003 * (co2 - 50)) * uw)^cp0) #Trockenheit
           #* (1 - exp(cn0 * n))^cn1 #N - Mitscherlich
           #* (1 - exp(cpo0 * po))^cpo1 #P
           #* pmax(0, 1 - cs0^2*s^cs1) #Salz
           * exp(log(pmax(0, cph0 + ph)) + cph1*log(pmax(0, cph0 + ph))^2) #pH
         , data = tt[i,]
         , start=list(c0=0.01, cr0=-45, cr1=0.7, ct0=0.003, ct1=0.9, ct2=0.02, ct3=4.4
                    #, cp0=0.3, cn0=-0.09, cn1=0.4, cpo0=-0.35, cpo1=8, cs0=0.01, cs1=3.3
                    , cph0=-3, cph1=-0.1)
         , trace=T, control = nls.lm.control(maxiter=999))

a <- nlsLM(nppM ~ c0 * exp(-nn/7990)    #Luftdruck
           * pmax(0,  cr0 + radi)^cr1   #Strahlung
           #* pmax(0, ct0 + t)^ct1       #Temperatur
           #* pmax(0, ct0 + t - ct1*pmax(0,t)^2)       #Temperatur
           #* pmax(0, exp(-log(pmax(0, (ct0+t)*ct1))^2) - ct2^2*pmax(0,t)^2)^ct3
           * pmax(0, tanh(ct0*pmax(0, ct1 + t)) - pmax(0, ct2 * t)^ct3)
           #* pmax(0, tanh(0.003 * (co2 - 50)) - cp0*exp(17.62*t/(243.12 + t)) / tanh(cp1*(uw)))^cp2 #Wasser (Niederschlag, Bodenwasser) + CO2
           * pmax(0, (tanh(0.003 * (co2 - 50)) * uw)^cp0)
           * (1 - exp(cn0 * n))^cn1 #N - Mitscherlich
           * (1 - exp(cpo0 * po))^cpo1 #P
           #* exp(cs0 * s)^cs1 #Salz
           * pmax(0, 1 - cs0^2*s^cs1)
           * exp(log(pmax(0, cph0 + ph)) + cph1*log(pmax(0, cph0 + ph))^2)
         , data = tt[i,]
         , start=list(c0=0.0038, cr0=-31, cr1=0.6, ct0=0.03, ct1=2, ct2=0.04, ct3=2, cp0=0.3, cn0=-0.2, cn1=1.5, cpo0=-0.1, cpo1=1, cs0=0.03, cs1=2, cph0=-3, cph1=-0.1)
         , trace=T, control = nls.lm.control(maxiter=999))


exp(log(pmax(0, cph0 + ph)) + cph1*log(pmax(0, cph0 + ph))^2)

a <- nlsLM(nppM ~ c0 * exp(-nn/7990)    #Luftdruck
           * pmax(0,  cr0 + radi)^cr1   #Strahlung
           #* pmax(0, ct0 + t)^ct1       #Temperatur
           #* pmax(0, ct0 + t - ct1*pmax(0,t)^2)       #Temperatur
           * pmax(0, exp(-log(pmax(0, (ct0+t)*ct1))^2) - ct2^2*pmax(0,t)^2)^ct3
           #* pmax(0, tanh(0.003 * (co2 - 50)) - cp0*exp(17.62*t/(243.12 + t)) / tanh(cp1*(uw)))^cp2 #Wasser (Niederschlag, Bodenwasser) + CO2
           * pmax(0, (tanh(0.003 * (co2 - 50)) * uw)^cp0)
           * (1 - exp(cn0 * n))^cn1 #N - Mitscherlich
           * (1 - exp(cpo0 * po))^cpo1 #P
           #* exp(cs0 * s)^cs1 #Salz
           * pmax(0, 1 - cs0^2*s^cs1)
           * exp(log(pmax(0, cph0 + ph)) + cph1*log(pmax(0, cph0 + ph))^2)
         , data = tt[i,]
         , start=list(c0=0.0038, cr0=-31, cr1=0.6, ct0=1.7, ct1=0.03, ct2=0.01, ct3=0.75, cp0=0.3, cn0=-0.2, cn1=1.5, cpo0=-0.1, cpo1=1, cs0=0.03, cs1=2, cph0=-3, cph1=-0.1)
         , trace=T, control = nls.lm.control(maxiter=999))



a <- nlsLM(nppM ~ c0 * exp(-nn/7990)    #Luftdruck
           * pmax(0,  cr0 + radi)^cr1   #Strahlung
           #* pmax(0, ct0 + t)^ct1       #Temperatur
           * pmax(0, ct0 + t - ct1*pmax(0,t)^2)       #Temperatur
           #* pmax(0, tanh(0.003 * (co2 - 50)) - cp0*exp(17.62*t/(243.12 + t)) / tanh(cp1*(uw)))^cp2 #Wasser (Niederschlag, Bodenwasser) + CO2
           * pmax(0, (tanh(0.003 * (co2 - 50)) * uw)^cp0)
           * (1 - exp(cn0 * n))^cn1 #N - Mitscherlich
           * (1 - exp(cpo0 * po))^cpo1 #P
           #* exp(cs0 * s)^cs1 #Salz
           * pmax(0, 1 - cs0^2*s^cs1)
           * exp(log(pmax(0, cph0 + ph)) + cph1*log(pmax(0, cph0 + ph))^2)
         , data = tt[i,]
         , start=list(c0=0.0038, cr0=-31, cr1=0.6, ct0=1.7, ct1=0.03, cp0=0.3, cn0=-0.2, cn1=1.5, cpo0=-0.1, cpo1=1, cs0=0.03, cs1=2, cph0=-3, cph1=-0.1)
         , trace=T, control = nls.lm.control(maxiter=999))
s <- coef(a)
rm(a)
a <- nlsLM(nppM ~ c0 * exp(-nn/7990)    #Luftdruck
           * pmax(0,  cr0 + radi)^cr1   #Strahlung
           #* pmax(0, ct0 + t)^ct1       #Temperatur
           * pmax(0, ct0 + t^1.5 - ct1*pmax(0,t)^2)       #Temperatur
           #* pmax(0, tanh(0.003 * (co2 - 50)) - cp0*exp(17.62*t/(243.12 + t)) / tanh(cp1*(uw)))^cp2 #Wasser (Niederschlag, Bodenwasser) + CO2
           * pmax(0, (tanh(0.003 * (co2 - 50)) * uw)^cp0)
           * (1 - exp(cn0 * n))^cn1 #N - Mitscherlich
           * (1 - exp(cpo0 * po))^cpo1 #P
           #* exp(cs0 * s)^cs1 #Salz
           * pmax(0, 1 - cs0^2*s^cs1)
           * exp(log(pmax(0, cph0 + ph)) + cph1*log(pmax(0, cph0 + ph))^2)
         , data = tt
         , start= s
         , trace=T, control = nls.lm.control(maxiter=999))

tt <- c(lc = lc, er = er, coef(a))
cat(names(tt), "\n", tt, file = paste0("coefLc", lc, "er", er, ".txt"))

plot(tt$radi, pmax(0,  coef(a)["cr0"] + tt$radi)^coef(a)["cr1"], pch=".")
plot(tt$t, pmax(0, coef(a)["ct0"] + tt$t - coef(a)["ct1"]*pmax(0,tt$t)^2), pch=".")
plot(tt$t, pmax(0, exp(-log(pmax(0, (coef(a)["ct0"]+tt$t)*coef(a)["ct1"]))^2) - coef(a)["ct2"]^2*pmax(0,tt$t)^2)^coef(a)["ct3"], pch=".")
          

plot(tt$t, pmax(0,  coef(a)["ct0"] + tt$t)^coef(a)["ct1"], pch=".")
plot(tt$t, pmax(0, 1 - coef(a)["cp0"]*exp(17.62*tt$t/(243.12 + tt$t)) / tanh(coef(a)["cp1"]*(tt$uw)))^coef(a)["cp2"], pch=".")
plot(tt$t, pmax(0,  coef(a)["ct0"] + tt$t)^coef(a)["ct1"] * pmax(0, 1 - coef(a)["cp0"]*exp(17.62*tt$t/(243.12 + tt$t)) / tanh(coef(a)["cp1"]*(tt$uw)))^coef(a)["cp2"], pch=".")
plot(tt$n, (1 - exp(coef(a)["cn0"] * tt$n))^coef(a)["cn1"], pch=".", log="x")
plot(tt$po, (1 - exp(coef(a)["cpo0"] * tt$po))^coef(a)["cpo1"], pch=".")
plot(tt$s, exp(coef(a)["cs0"] * tt$s)^coef(a)["cs1"], pch=".")
plot(tt$ph, exp(log(pmax(0, coef(a)["cph0"] + tt$ph)) + coef(a)["cph1"]*log(pmax(0, coef(a)["cph0"] + tt$ph))^2), pch=".")
