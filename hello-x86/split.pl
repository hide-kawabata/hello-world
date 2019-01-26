#!/usr/bin/perl

# Usage:
# $ echo '(+ 1 2)' | perl split.pl
# (
# +
# 1
# 2
# )

while (<>) {
    s/^ *//g; # remove preceding spaces
    s/ *$//g; # remove following spaces

    s/\( +/(/g;
    s/ +\)/)/g;
    s/ +/\n/g;
    s/\(/(\n/g;
    s/\)/\n)/g;
    print;
}
