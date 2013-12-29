#!./perl

# $Header: base.lex,v 1.0.1.2 88/02/03 16:27:16 root Exp $

print "1..7\n";

$ # this is the register <space>
= 'x';

print "#1	:$ : eq :x:\n";
if ($  eq 'x') {print "ok 1\n";} else {print "not ok 1\n";}

$x = $#;	# this is the register $#

if ($x eq '') {print "ok 2\n";} else {print "not ok 2\n";}

$x = $#x;

if ($x eq '-1') {print "ok 3\n";} else {print "not ok 3\n";}

$x = '\\'; # ';

if (length($x) == 1) {print "ok 4\n";} else {print "not ok 4\n";}

eval 'while (0) {
    print "foo\n";
}
/^/ && (print "ok 5\n");
';

eval '$foo{1} / 1;';
if (!$@) {print "ok 6\n";} else {print "not ok 6\n";}

eval '$foo = 123+123.4+123e4+123.4E5+123.4e+5+.12;';

if ($foo eq 25910246.52) {print "ok 7\n";} else {print "not ok 7\n";}
