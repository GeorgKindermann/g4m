#!!!!nrows vorher mit >wc calibArea.csv< herraussuchen
x <- read.csv("calibArea.csv", nrows = 153245132, comment.char="", colClasses=c("integer","integer","integer","integer"))

#gctorture(on = TRUE)

names(x) <- c("country", "gridarea", "landarea", "forShare")
gc()

country <- x$country
gridarea <- x$gridarea
landarea <- x$landarea
forShare <- x$forShare

rm(x)
gc()

#Flaechen auf Hektar bringen
gridarea <- gridarea / 10000.
gc()
landarea <- (1. - landarea/16.) * gridarea
gc()

#forshare von 0-100 auf 0-1 bringen
forShare <- forShare / 100.
gc()

#sum(forShare * gridarea)
#3662814025


#ALAND ISLANDS to Finland
country[country==5] <- 73; gc()
#Caspian sea ISO = CA-
#Cocos Iland to costa rica
country[country==42] <- 54; gc()
#CHRISTMAS ISLAND to Australia
country[country==56] <- 16; gc()
#Hong kong to china
country[country==97] <- 45; gc()
#Kosovo to Serbia
country[country==117] <- 206; gc()
#Macao to China
country[country==137] <- 45; gc()
#Taiwan to China
country[country==228] <- 45; gc()
#UNITED STATES MINOR OUTLYING ISLANDS to usa
country[country==232] <- 234; gc()

t2 <- sort(country, index.return = T)$ix
country <- country[t2]
gridarea <- gridarea[t2]
landarea <- landarea[t2]
forShare <- forShare[t2]
rm(t2)
gc()

tmp <- forShare * gridarea
gc()
t1 <- tapply(tmp, country, sum)
gc()
t1 <- data.frame(country=names(t1), forArea=t1)



countryGadm <- read.csv("/data/vectordata/global/adminstrativeBoundaries/gadm/v1/countrynames.csv")


fra2010 <- read.csv("T01FO000.csv")
fra2010$Value <- as.numeric(gsub(",",".",as.character(fra2010$Value)))

sum(t1$forArea)
sum(fra2010$Value)

me <- merge(fra2010[,c("Country", "Value")], countryGadm[,c("ISO", "NAME_FAO")], by.y="NAME_FAO", by.x="Country", all.x=T)
#write.csv(me, "/tmp/csv.csv", na="")
me$ISO[me$Country=="Anguilla"] <- "AIA"
me$ISO[me$Country=="Azerbaijan"] <- "AZE"
me$ISO[me$Country=="Bolivia (plurinational state of)"] <- "BOL"
me$ISO[me$Country=="Congo"] <- "COG"
#me$ISO[me$Country=="Côte d'Ivoire"] <- "CIV"
me$ISO[me$Country=="C\xf4te d'Ivoire"] <- "CIV"
me$ISO[me$Country=="Democratic People's Republic of Korea"] <- "PRK"
me$ISO[me$Country=="Democratic Republic of the Congo"] <- "COD"
me$ISO[me$Country=="Faeroe Islands"] <- "FRO"
me$ISO[me$Country=="Falkland Islands"] <- "FLK"
me$ISO[me$Country=="Fiji"] <- "FJI"
me$ISO[me$Country=="Gibraltar"] <- "GIB"
me$ISO[me$Country=="Greenland"] <- "GRL"
me$ISO[me$Country=="Guernsey"] <- "GGY"
me$ISO[me$Country=="Holy See"] <- "VAT"
me$ISO[me$Country=="Iran (Islamic Republic of)"] <- "IRN"
me$ISO[me$Country=="Jersey"] <- "JEY"
me$ISO[me$Country=="Lao People's Democratic Republic"] <- "LAO"
me$ISO[me$Country=="Micronesia (Federated States of)"] <- "FSM"
me$ISO[me$Country=="Monaco"] <- "MCO"
me$ISO[me$Country=="Montenegro"] <- "MNE"
me$ISO[me$Country=="Nauru"] <- "NRU"
me$ISO[me$Country=="Norfolk Island"] <- "NFK"
me$ISO[me$Country=="Northern Mariana Islands"] <- "MNP"
me$ISO[me$Country=="Occupied Palestinian Territory"] <- "PSE"
me$ISO[me$Country=="Pitcairn"] <- "PCN"
me$ISO[me$Country=="Republic of Korea"] <- "KOR"
me$ISO[me$Country=="Republic of Moldova"] <- "MDA"
#me$ISO[me$Country=="Réunion"] <- "REU"
me$ISO[me$Country=="R\xe9union"] <- "REU"
me$ISO[me$Country=="Saint Pierre and Miquelon"] <- "SPM"
me$ISO[me$Country=="Saint Vincent and the Grenadines"] <- "VCT"
me$ISO[me$Country=="Serbia"] <- "SRB"
me$ISO[me$Country=="Svalbard and Jan Mayen Islands"] <- "SJM"
me$ISO[me$Country=="The former Yugoslav Republic of Macedonia"] <- "MKD"
me$ISO[me$Country=="Tokelau"] <- "TKL"
me$ISO[me$Country=="Turks and Caicos Islands"] <- "TCA"
me$ISO[me$Country=="United Republic of Tanzania"] <- "TZA"
me$ISO[me$Country=="United States Virgin Islands"] <- "VIR"
me$ISO[me$Country=="Venezuela (Bolivarian Republic of)"] <- "VEN"
me$ISO[me$Country=="Wallis and Futuna Islands"] <- "WLF"

#me$ISO[me$Country=="Saint-Barthélemy"] <- ""
#me$ISO[me$Country=="Saint Martin (French part)"] <- ""

me <- me[!is.na(me$ISO),]

t2 <- merge(t1, countryGadm[,c("ISO", "NAME_ISO", "GADMID")], by.x="country", by.y="GADMID")
me2 <- merge(t2, me, by="ISO", all.x=T)
me2 <- me2[!is.na(me2$Country),]


with(me2, plot(forArea, Value*1000, pch="."))
with(me2, text(forArea, Value*1000, ISO))
abline(0,1)

with(me2[me2$Value>0,], summary(forArea/Value/1000))
sort(with(me2, forArea/Value/1000))

t2 <- c(1,(2:NROW(country))[country[1:(NROW(country)-1)] != country[2:NROW(country)]])
t2 <- data.frame(l=t2, h=c(t2[2:NROW(t2)]-1, NROW(country)))
t2$country <- country[t2$l]

me2$coef <- NA
me2$calibArea <- NA
#me2 <- read.csv("calibFactors.csv", na="")
f <- function(x) {abs(sum((t1$forShare * t1$gridarea / t1$landarea)^x * t1$landarea, na.rm=T) - fo)}
#ccountry <- 17
#countrys <- 13
for(countrys in 1:NROW(me2)) {
#for(countrys in 107:NROW(me2)) {
  ccountry <- me2$country[countrys]
  t1 <- data.frame(forShare[t2$l[t2$country==ccountry]:t2$h[t2$country==ccountry]], gridarea[t2$l[t2$country==ccountry]:t2$h[t2$country==ccountry]], landarea[t2$l[t2$country==ccountry]:t2$h[t2$country==ccountry]])
#  t1 <- data.frame(forShare[country==ccountry], gridarea[country==ccountry], landarea[country==ccountry])
  fo <- me2$Value[me2$country==ccountry]*1000
  coef <- optimize(f, interval = c(0.2,3), tol=1e-9)$minimum
  me2$coef[countrys] <- coef
  me2$calibArea[countrys] <- sum((t1$forShare * t1$gridarea / t1$landarea)^coef * t1$landarea, na.rm=T)
  print(me2[countrys,])
  rm(t1)
  gc()
}

me2$multi <- 1
me2$multi[me2$Value<=0] <- 0
me2$coef[me2$Value<=0] <- 1

write.csv(me2, "/tmp/calibFactors.csv", na="")
write.csv(me2[,c("country", "coef")], "/tmp/calibFactorsS.csv", na="",row.names =F)

