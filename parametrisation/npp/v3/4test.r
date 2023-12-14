source("g4m.r")
#source("https://raw.githubusercontent.com/GeorgKindermann/g4mR/main/g4m.r")

x <- read.table("baseData30.txt")

colnames(x) <- c("lon", "lat", "nn", "P", "whc", "N", "pH", "S", paste0("p", seq(12)), paste0("t", seq(12)), paste0("r", seq(12)))

x$x <- as.integer((x$lon+180.25)*2)
x$y <- as.integer((x$lat+90.25)*2)
x$y <- x$y - min(x$y) + 1

cT <- grep("^t\\d{1,2}$", names(x))
cP <- grep("^p\\d{1,2}$", names(x))
cR <- grep("^r\\d{1,2}$", names(x))

npp <- apply(x, 1, function(.) {
  g4mMai(t=.[cT], p=.[cP], r=.[cR], whc=.["whc"], nn=.["nn"], co2=380, N=.["N"], P=.["P"], S=.["S"], pH=.["pH"])
})

. <- matrix(NA, max(x$x), max(x$y))

.[cbind(x$x, x$y)] <- pmin(50, apply(npp, 2, max))
image(.)

for(i in 1:nrow(npp)) {
  .[cbind(x$x, x$y)] <- pmin(50, npp[i,])
  image(., main=i, zlim=c(0,8))
  print(i)
  invisible(readline())
}

.[cbind(x$x, x$y)] <- apply(npp, 2, \(x) {
    i <- order(x, decreasing=TRUE)[1]
    if(x[i] > 0) i
    else 0
})
image(seq_len(dim(.)[1]), seq_len(dim(.)[2]), .)
repeat(print(.[matrix(unlist(locator(1)),1)]))

.[cbind(x$x, x$y)] <- apply(npp, 2, \(x) {
    diff(x[order(x, decreasing=TRUE)[1:2]])
})



g4mMai(t=rep(20,12), p=rep(80,12), r=rep(100,12), whc=300, nn=300, co2=380, N=30, P=30, S=0, pH=4)


# 1..Nadel-Evergreen-Tropical, 2..Nadel-Evergreen-Subtropical, 3..Nadel-Evergreen-Temperate, 4..Nadel-Evergreen-Boreal, 5..Laub-Evergreen, .., 9..Nadel-Deciduous, .., 13..Laub-Deciduous, .., 17..woody savannas-Tropical, .., 20..woody savannas-Boreal
