#NROWS NEED TO BE CORRECT wc /tmp/testData.txt
x <- read.table("/tmp/testData.txt", header=F, colClasses=c("integer","numeric","integer","integer","integer"),sep="\t",quote="", stringsAsFactors=FALSE,comment.char="",nrows=152982349)
names(x) <- c("country","gridarea","waterLand","forest","irri")
gc()

t1 <- tapply(x$forest * x$gridarea / 1000000, x$country, sum)
gc()
t2 <- tapply(x$irri * x$gridarea / 1000000, x$country, sum)

me <- merge(t1, t2, by=0)
names(me) <- c("country", "cforest", "cirrir")

me2 <- merge(read.csv("calibFactors.csv", na=""), me)

write.csv(me2, "/tmp/testRes.csv", na="", row.names = F)
