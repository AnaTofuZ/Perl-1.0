# Case-insensitive filesystems don't get along with Makefile and
# makefile in the same place.  Since Darwin uses GNU make, this dodges
# the problem.

firstmakefile=GNUmakefile;
