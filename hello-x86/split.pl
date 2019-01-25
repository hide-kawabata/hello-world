#!/usr/bin/perl

# Usage:
# $ echo '(+ 1 2)' | perl split.pl
# (
# +
# 1
# 2
# )

while (<>) {
    s/\(/(\n/g;
    s/\)/\n)/g;
    s/\)\(/\)\n\(/g;
    s/ +/\n/g;
    print;
}
