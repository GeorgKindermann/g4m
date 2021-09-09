x <- data.frame(name=dir(pattern = "^psnNetMOD17A2HGF006_.{10}Sin.tif"))

#psnNetMOD17A2HGF006_2016.04.22Sin.tif has different size
#tt <- sapply(x$name, function(x) system(paste("gdalinfo", x, "| grep \"^Size is \""), TRUE))
#tt <- read.table(text=sub("^Size is ", "", tt), sep=",")
#table(tt)
#i <- which(tt$V1 == 76800)
#system(paste("gdalwarp -te -20015109.354 -6671703.118 20015109.354 10007554.677", x$name[i], "tmp.tif"))
#system(paste("mv tmp.tif", x$name[i]))

x$b <- as.Date(substr(x$name, 21, 30), "%Y.%m.%d")
x$e <- x$b+7
y <- data.frame(b=as.Date(unique(format(x$b,"%Y-%m-01"))))
y$e <- as.Date(sapply(y$b, function(i) seq(i, length=2, by="1 months")[2]-1),origin="1970-01-01")

cat("", file = "3perMonth.sh")
for(i in seq_len(nrow(y))) {
    j <- pmax(pmin(x$e, y$e[i]) - pmax(x$b, y$b[i]) + 1,0)
    t1 <- paste0("where(", LETTERS[seq(sum(j>0))], ">32761, nan, " , LETTERS[seq(sum(j>0))], "*", j[j>0], ")", collapse = ", ")
    t2 <- paste0("where(", LETTERS[seq(sum(j>0))], ">32761, nan, " , j[j>0], ")", collapse = ", ")
    cat(paste0("gdal_calc.py --co=\"BIGTIFF=YES\" --co=\"COMPRESS=ZSTD\" --co=\"TILED=YES\" --co=\"PREDICTOR=2\" --calc=\"where(nansum([", t2, "], 0) > 0.5, nansum([", t1, "], 0)/maximum(1, nansum([", t2, "], 0)), 32767)\" --outfile=psnNetMOD17A2HGF006_",format(y$b[i],"%Y-%m"),".tif ", paste0("-", LETTERS[seq(sum(j>0))], " ", x$name[j>0], collapse = " ")), file = "3perMonth.sh", append=TRUE, sep="\n")
}

#And then run 3perMonth.sh
#nohup nice -19 bash < 3perMonth.sh &
