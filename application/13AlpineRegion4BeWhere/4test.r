x <- read.csv("/tmp/g4mAlpsScen.csv")

plot(x$mai_conif_tc_ha_year, x$conif_maxInc_increment, pch=".")
abline(0,1)
plot(x$mai_nonconif_tc_ha_year, x$nonconif_maxInc_increment, pch=".")
abline(0,1)


plot(x$conif_cur_increment, x$conif_maxInc_increment, pch=".")
abline(0,1, col=2)

plot(x$conif_maxBm_stock, x$conif_cur_stock, pch=".")
abline(0,1, col=2)

plot(x$conif_maxBm_rotationtime, x$conif_cur_rotationtime, pch=".")
abline(0,1, col=2)


plot(x$nonconif_cur_increment, x$nonconif_maxInc_increment, pch=".")
abline(0,1, col=2)

plot(x$nonconif_maxBm_stock, x$nonconif_cur_stock, pch=".")
abline(0,1, col=2)

plot(x$nonconif_maxBm_rotationtime, x$nonconif_cur_rotationtime, pch=".")
abline(0,1, col=2)




x <- read.table("Maximum_biomass.txt", header=T)
with(x, plot(long, lat, main="Maximum biomass"))

x <- read.table("Maximum_increment.txt", header=T)
with(x, plot(long, lat, main="Maximum increment"))

x <- read.table("current_increment.txt", header=T)
with(x, plot(long, lat, main="Current increment"))


