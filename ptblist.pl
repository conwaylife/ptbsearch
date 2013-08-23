#!/usr/bin/perl

open LIST1, $ARGV[0];
while (<LIST1>) 
{
    @flds = split(" ");
    $pat1 = $flds[0];
    if (scalar(@flds)<2)
    {
	$gen0 = 18;
    }
    else
    {
        $gen0 = $flds[1];
    }

    $pat1 =~ s/!/.\n/g;
    open OUTPAT, "> pat1";
    print OUTPAT $pat1;
    close OUTPAT;

    system("../ptb2 pat1 $ARGV[1] $ARGV[2] 0 $gen0 1");

}
close LIST1;
