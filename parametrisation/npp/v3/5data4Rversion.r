x <- read.table("baseData30.txt")

colnames(x) <- c("lon", "lat", "nn", "P", "whc", "N", "pH", "S", paste0("p", seq(12)), paste0("t", seq(12)), paste0("r", seq(12)))

x$nn <- round(x$nn)
x$P <- round(x$P)
x$whc <- round(x$whc)
x$N <- round(x$N)
x$pH <- round(x$pH,2)
x$S <- round(x$S)

i <- grep("^p([1-9]|1[0-2])$", names(x))
x[i] <- round(x[i])

i <- grep("^t([1-9]|1[0-2])$", names(x))
x[i] <- round(x[i],1)

i <- grep("^r([1-9]|1[0-2])$", names(x))
x[i] <- round(x[i])

write.csv(x, "/tmp/g4mSiteData.csv", quote = FALSE, row.names = FALSE)



source("g4m.r")
#source("https://raw.githubusercontent.com/GeorgKindermann/g4mR/main/g4m.r")

x <- read.csv("/tmp/g4mSiteData.csv.xz")
#lon ..... Longitude - not needed in g4m but needed to display
#lat ..... Latitude  - not needed in g4m but needed to display
#nn ...... Altitude
#P ....... Phosphorous 
#whc ..... Water holding capacity
#N ....... Nitrogen
#pH ...... pH-Value
#S ....... Salinity
#p1-p12 .. Precipitation
#t1-t12 .. Temperature
#r1-r12 .. Radiation

#Integer Position on map
x$x <- as.integer((x$lon+180.25)*2)
x$y <- as.integer((x$lat+90.25)*2)
x$y <- x$y - min(x$y) + 1

#Calculate NPP
npp <- apply(x, 1, function(.) {
  g4mMai(t=.[paste0("t", 1:12)], p=.[paste0("p", 1:12)], r=.[paste0("r", 1:12)], whc=.["whc"], nn=.["nn"], co2=380, N=.["N"], P=.["P"], S=.["S"], pH=.["pH"])
})

#Display map
. <- matrix(NA, max(x$x), max(x$y))
.[cbind(x$x, x$y)] <- apply(npp, 2, max)
#image(.)

n <- ceiling(max(c(.), na.rm=TRUE))
COL <- hcl.colors(n+1, "YlOrRd", rev = TRUE)
image(seq_along(.[,1]), seq_along(.[1,]), z=., col = COL, axes=FALSE, asp=1, xlab="", ylab="")
legend("bottomleft", legend=0:n, fill = COL, xpd = NA, bty="n", title="mai\n[tC/ha/year]")
