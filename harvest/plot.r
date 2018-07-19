x <- read.table("/tmp/out.txt", header = T)

pdf("/tmp/plots.pdf")
for(i in 2:NCOL(x)) {
  plot(x$vs, x[,i], main=names(x)[i], type="l", xlab="Stemvolume [m3]", ylab="Time", lwd=3)
}
dev.off()

