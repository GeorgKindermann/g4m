#!/usr/bin/perl -w


$key = {};
open(IN, "/data/tmp/baseData.txt");
while($line = <IN>) {
  @atom = split(/\t/,$line);
  #$atom[2]; #Landcover
  #$atom[16]; #EcoRegion
  $key{"$atom[2]_$atom[16]"} = 1;
}
close(IN);

my @filehandles;
$i=0;
for (keys %key) {
  local *FILE;
  open(FILE, ">/data/tmp/lcEr$_.txt") || die;
  push(@filehandles, *FILE);
  $key{$_} = $i;
  print "$_\n";
  ++$i;
}

open(IN, "/data/tmp/baseData.txt");

for my $fh ( @filehandles ) {
  print $fh "x\ty\tlandcover\tnpp\tnnp01\tnpp02\tnpp03\tnpp04\tnpp05\tnpp06\tnpp07\tnpp08\tnpp09\tnpp10\tnpp11\tnpp12\tecoRegionFao\tsoilIiasaFao90\tsoilIiasaSwr\tsoilIiasaAwc\televation\tprec01\tprec02\tprec03\tprec04\tprec05\tprec06\tprec07\tprec08\tprec09\tprec10\tprec11\tprec12\ttmean01\ttmean02\ttmean03\ttmean04\ttmean05\ttmean06\ttmean07\ttmean08\ttmean09\ttmean10\ttmean11\ttmean12\tradi01\tradi02\tradi03\tradi04\tradi05\tradi06\tradi07\tradi08\tradi09\tradi10\tradi11\tradi12\n";
}

while($line = <IN>) {
  @atom = split(/\t/,$line);
  $fh = $filehandles[$key{"$atom[2]_$atom[16]"}];
  print $fh $line;
}
close(IN);
