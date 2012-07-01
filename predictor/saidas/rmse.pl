#!/usr/bin/perl

# A simple implementation of the RMSE calculation used for the Netflix Prize
use warnings;
use strict;

my $inFileName;
my $numFields;
my $numValues = 0;
my @sumSquaredValues;
my @predictions;
my $i;
my $first = 1;
my $line;

# parametros
$inFileName = $ARGV[0];  # arquivo de entrada com as predicoes

open FIN, "<".$inFileName or die "\nCan't open file $inFileName.\n";
while ($line=<FIN>) {
    chomp($line);
    my ($rating,@predictions) = split(/ /, $line);
    $numValues++;

    for($i=0; $i<=$#predictions; $i++) {
        if($first) {
            $sumSquaredValues[$i] = 0;
        }
        my $delta = $rating - $predictions[$i];
        $sumSquaredValues[$i] += $delta*$delta;
    }
    $first = 0;
}

# Let perl do the truncation to 0.0001
for($i=0; $i<=$#sumSquaredValues; $i++) {
    printf "[%d] %d pairs RMSE: %.5f\n", $i, $numValues, sqrt($sumSquaredValues[$i]/$numValues);
}
close(FIN);

# Some example data rating & prediction data
# NOTE: This is NOT in the proper prize format
__DATA__
2 3.2 4.1
3 3.1 2.3
4 5.0 1.4

