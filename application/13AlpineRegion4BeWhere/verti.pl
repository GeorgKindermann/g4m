#!/usr/bin/perl -w

print "VERTI:\n";
$i=0;
while($line = <STDIN>) {
  ++$i;
  chop($line);
  @atom = split(/\s/,$line);
  if($i<6) {
    if($i==1) {print "B 5 1\n";}
    print "$atom[0] $atom[1]\n"
  } else {
    $i=0;
    print "1 $atom[2]\nC 1 1\n$atom[0] $atom[1]\n1 $atom[2]\n"
  }
}
