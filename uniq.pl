#!/usr/bin/perl

while (<>)
{
    ($a, $b, $c, $d, $e, $f, $g) = split(" ");
    $id = "$g";
    if (!$seen{$id}) 
    {
        $seen{$id}=1;
	print $_;
    }

}
