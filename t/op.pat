#!./perl

# $Header: op.pat,v 1.0.1.3 88/03/10 16:55:45 root Exp $
print "1..27\n";

$x = "abc\ndef\n";

if ($x =~ /^abc/) {print "ok 1\n";} else {print "not ok 1\n";}
if ($x !~ /^def/) {print "ok 2\n";} else {print "not ok 2\n";}

$* = 1;
if ($x =~ /^def/) {print "ok 3\n";} else {print "not ok 3\n";}
$* = 0;

$_ = '123';
if (/^([0-9][0-9]*)/) {print "ok 4\n";} else {print "not ok 4\n";}

if ($x =~ /^xxx/) {print "not ok 5\n";} else {print "ok 5\n";}
if ($x !~ /^abc/) {print "not ok 6\n";} else {print "ok 6\n";}

if ($x =~ /def/) {print "ok 7\n";} else {print "not ok 7\n";}
if ($x !~ /def/) {print "not ok 8\n";} else {print "ok 8\n";}

if ($x !~ /.def/) {print "ok 9\n";} else {print "not ok 9\n";}
if ($x =~ /.def/) {print "not ok 10\n";} else {print "ok 10\n";}

if ($x =~ /\ndef/) {print "ok 11\n";} else {print "not ok 11\n";}
if ($x !~ /\ndef/) {print "not ok 12\n";} else {print "ok 12\n";}

$_ = 'aaabbbccc';
if (/(a*b*)(c*)/ && $1 eq 'aaabbb' && $2 eq 'ccc') {
	print "ok 13\n";
} else {
	print "not ok 13\n";
}
if (/(a+b+c+)/ && $1 eq 'aaabbbccc') {
	print "ok 14\n";
} else {
	print "not ok 14\n";
}

if (/a+b?c+/) {print "not ok 15\n";} else {print "ok 15\n";}

$_ = 'aaabccc';
if (/a+b?c+/) {print "ok 16\n";} else {print "not ok 16\n";}
if (/a*b+c*/) {print "ok 17\n";} else {print "not ok 17\n";}

$_ = 'aaaccc';
if (/a*b?c*/) {print "ok 18\n";} else {print "not ok 18\n";}
if (/a*b+c*/) {print "not ok 19\n";} else {print "ok 19\n";}

$_ = 'abcdef';
if (/bcd|xyz/) {print "ok 20\n";} else {print "not ok 20\n";}
if (/xyz|bcd/) {print "ok 21\n";} else {print "not ok 21\n";}

if (m|bc/*d|) {print "ok 22\n";} else {print "not ok 22\n";}

if (/^$_$/) {print "ok 23\n";} else {print "not ok 23\n";}

$* = 1;		# test 3 only tested the optimized version--this one is for real
if ("ab\ncd\n" =~ /^cd/) {print "ok 24\n";} else {print "not ok 24\n";}
$* = 0;

$XXX{123} = 123;
$XXX{234} = 234;
$XXX{345} = 345;

@XXX = ('ok 25','not ok 25', 'ok 26','not ok 26','not ok 27');
while ($_ = shift(XXX)) {
    ?(.*)? && (print $1,"\n");
    /not/ && reset;
    /not ok 26/ && reset 'X';
}

while (($key,$val) = each(XXX)) {
    print "not ok 27\n";
    exit;
}

print "ok 27\n";
