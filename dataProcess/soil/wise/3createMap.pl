#!/usr/bin/perl -w

my $col = 0;
if(@ARGV>0) {$col = $ARGV[0];}
my $file = "/tmp/soilData.txt";
open my $in, $file or die "$file: $!";
my $line = <$in>;
while ($line = <$in>) {
  chomp $line;
  @atom = split(' ',$line);
  $dat{$atom[0]} = [ @atom[1 .. $#atom] ];
}
close $in;

$file = "/tmp/out.asc";
open $in, $file or die "$file: $!";
for(my $i=0; $i<6; ++$i) {
  $line = <$in>;
  print $line;
}
while ($line = <$in>) {
  chomp $line;
  @atom = split(' ',$line);
  for(my $i=0; $i<@atom; ++$i) {
    if (exists $dat{$atom[$i]}) {
      print " $dat{$atom[$i]}[$col]";
    } else {
      print " 0";
    }

  }
  print "\n";
}
