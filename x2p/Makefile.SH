case $CONFIG in
'')
    if test ! -f config.sh; then
	ln ../config.sh . || \
	ln ../../config.sh . || \
	ln ../../../config.sh . || \
	(echo "Can't find config.sh."; exit 1)
    fi
    . ./config.sh
    ;;
esac
case "$0" in
*/*) cd `expr X$0 : 'X\(.*\)/'` ;;
esac
case "$mallocsrc" in
'') ;;
*) mallocsrc="../$mallocsrc";;
esac
echo "Extracting x2p/Makefile (with variable substitutions)"
cat >Makefile <<!GROK!THIS!
# $Header: Makefile.SH,v 1.0.1.4 88/03/02 12:58:56 root Exp $
#
# $Log:	Makefile.SH,v $
# Revision 1.0.1.4  88/03/02  12:58:56  root
# patch24: a2p.o didn't depend on config.h
# patch24: mv of old version was noisy
# 
# Revision 1.0.1.3  88/02/12  10:52:32  root
# patch22: support for systems without . in path
# 
# Revision 1.0.1.2  88/02/04  10:27:04  root
# patch17: changed chmod 755 to +x.
# 
# Revision 1.0.1.1  88/01/26  14:15:24  root
# Added mallocsrc stuff.
# 
# Revision 1.0  87/12/18  17:50:17  root
# Initial revision
# 
# 

CC = $cc
bin = $bin
lib = $lib
mansrc = $mansrc
manext = $manext
CFLAGS = $ccflags -O
LDFLAGS = $ldflags
SMALL = $small
LARGE = $large $split
mallocsrc = $mallocsrc
mallocobj = $mallocobj

libs = $libnm -lm
!GROK!THIS!

cat >>Makefile <<'!NO!SUBS!'

public = a2p s2p

private = 

manpages = a2p.man s2p.man

util =

sh = Makefile.SH makedepend.SH

h = EXTERN.h INTERN.h config.h handy.h hash.h a2p.h str.h util.h

c = hash.c $(mallocsrc) str.c util.c walk.c

obj = hash.o $(mallocobj) str.o util.o walk.o

lintflags = -phbvxac

addedbyconf = Makefile.old bsd eunice filexp loc pdp11 usg v7

# grrr
SHELL = /bin/sh

.c.o:
	$(CC) -c $(CFLAGS) $(LARGE) $*.c

all: $(public) $(private) $(util)
	touch all

a2p: $(obj) a2p.o
	$(CC) $(LDFLAGS) $(LARGE) $(obj) a2p.o $(libs) -o a2p

a2p.c: a2p.y
	@ echo Expect 95 shift/reduce errors...
	yacc a2p.y
	mv y.tab.c a2p.c

a2p.o: a2p.c a2py.c a2p.h EXTERN.h util.h INTERN.h handy.h ../config.h
	$(CC) -c $(CFLAGS) $(LARGE) a2p.c

# if a .h file depends on another .h file...
$(h):
	touch $@
install: a2p s2p
# won't work with csh
	export PATH || exit 1
	- mv $(bin)/a2p $(bin)/a2p.old 2>/dev/null
	- mv $(bin)/s2p $(bin)/s2p.old
	- if test `pwd` != $(bin); then cp $(public) $(bin); fi
	cd $(bin); \
for pub in $(public); do \
chmod +x `basename $$pub`; \
done
#	chmod +x makedir
#	- makedir `filexp $(lib)`
#	- \
#if test `pwd` != `filexp $(lib)`; then \
#cp $(private) `filexp $(lib)`; \
#fi
#	cd `filexp $(lib)`; \
#for priv in $(private); do \
#chmod +x `basename $$priv`; \
#done
	- if test `pwd` != $(mansrc); then \
for page in $(manpages); do \
cp $$page $(mansrc)/`basename $$page .man`.$(manext); \
done; \
fi

clean:
	rm -f *.o

realclean:
	rm -f a2p *.orig */*.orig *.o core $(addedbyconf)

# The following lint has practically everything turned on.  Unfortunately,
# you have to wade through a lot of mumbo jumbo that can't be suppressed.
# If the source file has a /*NOSTRICT*/ somewhere, ignore the lint message
# for that spot.

lint:
	lint $(lintflags) $(defs) $(c) > a2p.fuzz

depend: ../makedepend
	../makedepend

clist:
	echo $(c) | tr ' ' '\012' >.clist

hlist:
	echo $(h) | tr ' ' '\012' >.hlist

shlist:
	echo $(sh) | tr ' ' '\012' >.shlist

# AUTOMATICALLY GENERATED MAKE DEPENDENCIES--PUT NOTHING BELOW THIS LINE
$(obj):
	@ echo "You haven't done a "'"make depend" yet!'; exit 1
makedepend: makedepend.SH
	/bin/sh makedepend.SH
!NO!SUBS!
$eunicefix Makefile
case `pwd` in
*SH)
    $rm -f ../Makefile
    ln Makefile ../Makefile
    ;;
esac
