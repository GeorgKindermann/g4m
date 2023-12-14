if(!require(Rcpp)) {print("Trying to install Rcpp")
    install.packages("Rcpp")
    if(!require(Rcpp)) stop("Could not install Rcpp\nPlease install Rcpp")
}

g4m <- new.env()

sourceCpp("mai.cc", env=g4m)
#u2f <- function(url) {
#   tf <- tempfile( fileext = ".cc")
#   download.file(url, tf, quiet=TRUE)
#   tf
#}
#sourceCpp(u2f("https://raw.githubusercontent.com/GeorgKindermann/g4mR/main/mai.cc"), env=g4m)

g4m$maiInit()

g4mMai <- function(t,p,r,whc,nn,co2,N,P,S,pH) {
#Returns the MeanAnnual increment for:
# 1..Nadel-Evergreen-Tropical, 2..Nadel-Evergreen-Subtropical, 3..Nadel-Evergreen-Temperate, 4..Nadel-Evergreen-Boreal, 5..Laub-Evergreen, .., 9..Nadel-Deciduous, .., 13..Laub-Deciduous, .., 17..woody savannas-Tropical, .., 20..woody savannas-Boreal
#Input:
#t .. Temperature for each month of this year [12]
#p .. Precipitation for each month of this year [12]
#r .. Radiation for each month of this year [12]
#whc .. Water holding capacity
#nn .. altitude
#co2 .. Co2 concentration

    if(!missing(t)) g4m$maiSetTemperature(t)
    if(!missing(p)) g4m$maiSetPrecipitation(p)
    if(!missing(r)) g4m$maiSetRadiation(r)
    if(!missing(whc)) g4m$maiSetWhc(whc)
    if(!missing(nn)) g4m$maiSetAltitude(nn)
    if(!missing(co2)) g4m$maiSetCo2(co2)
    if(!missing(N)) g4m$maiSetN(N)
    if(!missing(P)) g4m$maiSetP(P)
    if(!missing(S)) g4m$maiSetS(S)
    if(!missing(pH)) g4m$maiSetpH(pH)
    g4m$maiGet()
}
