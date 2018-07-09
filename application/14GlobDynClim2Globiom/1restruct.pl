#!/usr/bin/perl -w

#./1restruct.pl ./orig/tas_hadgem2-es_rcp8p5_monmed_2.csv ./orig/pr_hadgem2-es_rcp8p5_monmed_abs_2.csv >/tmp/dat_hadgem2-es_rcp8p5.txt

system "cut -d , -f '1-7 18' $ARGV[0] | tail -n+2 | sed 's/,/\t/g' | sed 's/\t/,/;s/\t/,/;' | sort -k 1b,1 >/tmp/s1.txt";
system "cut -d , -f '1-7 18' $ARGV[1] | tail -n+2 | sed 's/,/\t/g' | sed 's/\t/,/;s/\t/,/;' | sort -k 1b,1 >/tmp/s2.txt";
system "join -a 1 -a 2 -e NA -o auto /tmp/s1.txt /tmp/s2.txt | sort -t , -k1g,1 -k2g,2 >/tmp/jo.txt";

$id = "";
@month = ("jan","feb","mar","apr","may","jun","jul","aug","sep","oct","nov","dec");
foreach(@month) {$dat{$_} = "NA NA NA NA NA NA NA NA NA NA";}

open(IN, "/tmp/jo.txt");
while($line = <IN>) {
  chop($line);
  @atom = split / /,$line;
  @atom2 = split /,/,$atom[0];
  if($id eq "") {$id = "$atom2[0] $atom2[1]";}
  if($id ne "$atom2[0] $atom2[1]") {
    print "$id";
    foreach(@month) {print " $dat{$_}";}
    print "\n";
    foreach(@month) {$dat{$_} = "NA NA NA NA NA NA NA NA";}
    $id = "$atom2[0] $atom2[1]";
  }
  if($atom[5] ne "NA" and $atom[1] ne "NA") {$atom[5] = $atom[1] + $atom[5];}
  if($atom[10] ne "NA" and $atom[6] ne "NA") {$atom[10] = $atom[6] + $atom[10];}
  $dat{$atom2[2]} = "@atom[1..10]";
}
close(IN);
