#!/usr/bin/perl -w

#./3aggregate.pl </tmp/datG4m4Vulvio2018.txt

$line = <STDIN>;
chop($line);
@head = split(/ /,$line);

while($line = <STDIN>) {
  if(substr($line, 0, 1) ne '#') {
    @atom = split(/ /,$line);
    $area{$atom[2]} += $atom[3];
    for($i=3; $i<@head; ++$i) {
      $sum[$i]{$atom[2]} += $atom[$i];
    }
  }
}

for($i=2; $i<@head; ++$i) {print "$head[$i] ";}
print "\n";
while(my($k, $v) = each %area) {
  print("$k");
  printf " %.0f", $v;
  for($i=3; $i<@head; ++$i) {
    printf " %.2f ", $sum[$i]{$k}/$v;
  }
  print "\n";
}
