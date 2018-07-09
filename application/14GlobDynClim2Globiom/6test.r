x <- read.csv("/data/tmp/g4m_w_hadgem2.csv")
#x <- read.csv("/data/tmp/g4m_w_hadgem2_Test.csv")

table(x$col)
table(x$row)
plot(x$col, -1*x$row, pch=".")

hist(x$altitudeclass.m., breaks = (-1:29)*300-150)
plot(x$col, -1*x$row, pch=".", col=1+x$altitudeclass.m./1000)

sum(x$landarea.m2.)
summary(x$landarea.m2.)

t1 <- with(x, tapply(landarea.m2., paste(col,row), sum))
t1 <- data.frame(col=as.numeric(unlist(strsplit(names(t1), " "))[c(TRUE,FALSE)]), row=as.numeric(unlist(strsplit(names(t1), " "))[c(FALSE,TRUE)]), val=t1)
plot(t1$col, -1*t1$row, pch=".", col=t1$val/1e9)

summary(x$forestshare.1.)
sum(x$forestshare.1. * x$landarea.m2.)

summary(x$primaryForestShare.1.)
sum(x$forestshare.1. * x$primaryForestShare.1. * x$landarea.m2.)
hist(x$primaryForestShare.1.)
plot(x$col, -1*x$row, pch=".", col=1+x$primaryForestShare*10)

hist(x$slope0.10deg.1.)
plot(x$col, -1*x$row, pch=".", col=1+x$slope0.10deg.1*10)

hist(x$slope10.15deg.1.)
plot(x$col, -1*x$row, pch=".", col=1+x$slope10.15deg.1*10)

hist(x$slope15.30deg.1.)
plot(x$col, -1*x$row, pch=".", col=1+x$slope15.30deg.1*10)

hist(x$slope30.50deg.1.)
plot(x$col, -1*x$row, pch=".", col=1+x$slope30.50deg.1*10)

hist(x$slope.50deg.1.)
plot(x$col, -1*x$row, pch=".", col=1+x$slope.50deg.1*10)

summary(rowSums(x[,c("slope0.10deg.1.","slope10.15deg.1.","slope15.30deg.1.","slope30.50deg.1.","slope.50deg.1.")]))

sum(x$X0potForestArea.ha.)
sum(x$X1potForestArea.ha.)
sum(x$X2potForestArea.ha.)
sum(x$X3potForestArea.ha.)
sum(x$X4potForestArea.ha.)
sum(x$X5potForestArea.ha.)
sum(x$X6potForestArea.ha.)
sum(x$X7potForestArea.ha.)
sum(x$X8potForestArea.ha.)
sum(x$X9potForestArea.ha.)
plot(x$col, -1*x$row, pch=".", col=x$X0potForestArea.ha./1e5)
plot(x$col, -1*x$row, pch=".", col=x$X9potForestArea.ha./1e5)

summary(x$X0bmUnmanaged.tc.ha.)
summary(x$X1bmUnmanaged.tc.ha.)
summary(x$X2bmUnmanaged.tc.ha.)
summary(x$X3bmUnmanaged.tc.ha.)
summary(x$X4bmUnmanaged.tc.ha.)
summary(x$X5bmUnmanaged.tc.ha.)
summary(x$X6bmUnmanaged.tc.ha.)
summary(x$X7bmUnmanaged.tc.ha.)
summary(x$X8bmUnmanaged.tc.ha.)
summary(x$X9bmUnmanaged.tc.ha.)
plot(x$col, -1*x$row, pch=".", col=x$X0bmUnmanaged.tc.ha./20)
plot(x$col, -1*x$row, pch=".", col=x$X9bmUnmanaged.tc.ha./20)


summary(x$X0shortCurHarvestSw.tC.)
sum(x$X0shortCurHarvestSw.tC * x$forestshare.1. * (1-x$primaryForestShare.1.) * x$landarea.m2.) / sum(x$forestshare.1. * (1-x$primaryForestShare.1.) * x$landarea.m2.)

summary(x$X0shortCurHarvestRw.tC.)

summary(x$X0shortCurTotalCut.tC.)
sum(x$X0shortCurTotalCut.tC. * x$forestshare.1. * (1-x$primaryForestShare.1.) * x$landarea.m2.) / sum(x$forestshare.1. * (1-x$primaryForestShare.1.) * x$landarea.m2.)

summary(x$X0shortCurCost.1.)
sum(x$X0shortCurCost.1. * x$forestshare.1. * (1-x$primaryForestShare.1.) * x$landarea.m2.) / sum(x$forestshare.1. * (1-x$primaryForestShare.1.) * x$landarea.m2.)

summary(x$X0shortCurStock.tC.)
sum(x$X0shortCurStock.tC. * x$forestshare.1. * (1-x$primaryForestShare.1.) * x$landarea.m2.) / sum(x$forestshare.1. * (1-x$primaryForestShare.1.) * x$landarea.m2.)










plot(x$X0potForestArea.ha., x$X0typicalCurArea.ha., pch=".")
abline(0,100/16)

sum(x$X0potForestArea.ha.)
sum(x$X1potForestArea.ha.)

sum(x$X0shortCurArea.ha.)
sum(x$X0typicalCurArea.ha.)
sum(x$X0longCurArea.ha.)

sum(x$X9typicalCurArea.ha.)


0shortCurHarvestSw

0shortCurHarvestRw

0shortCurTotalCut

0shortCurCost

0shortCurStock

sum(x$X0typicalAfor0jStock.tC.)
sum(x$X0typicalAfor30jStock.tC.)
sum(x$X0typicalAfor60jStock.Tc.)

colMeans(x[,grep("X?typicalAfor0jStock.tC.", names(x))])



colSums(x[,grep("X?typicalCurStock.tC.", names(x))])/colSums(x[,grep("X?typicalCurArea.ha.", names(x))])

colSums(x[,grep("X?typicalCurTotalCut.tC.", names(x))])/colSums(x[,grep("X?typicalCurArea.ha.", names(x))])

summary(x[,grep("X?typicalCurTotalCut.tC.", names(x))])
