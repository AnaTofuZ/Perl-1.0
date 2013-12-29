/* $Header: hash.c,v 1.0.1.2 88/03/10 16:27:20 root Exp $
 *
 * $Log:	hash.c,v $
 * Revision 1.0.1.2  88/03/10  16:27:20  root
 * patch29: added hclear() for reset operator
 * 
 * Revision 1.0.1.1  88/02/04  11:16:20  root
 * patch18: regularized includes.
 * 
 * Revision 1.0  87/12/18  13:05:17  root
 * Initial revision
 * 
 */

#include "EXTERN.h"
#include "perl.h"

STR *
hfetch(tb,key)
register HASH *tb;
char *key;
{
    register char *s;
    register int i;
    register int hash;
    register HENT *entry;

    if (!tb)
	return Nullstr;
    for (s=key,		i=0,	hash = 0;
      /* while */ *s;
	 s++,		i++,	hash *= 5) {
	hash += *s * coeff[i];
    }
    entry = tb->tbl_array[hash & tb->tbl_max];
    for (; entry; entry = entry->hent_next) {
	if (entry->hent_hash != hash)		/* strings can't be equal */
	    continue;
	if (strNE(entry->hent_key,key))	/* is this it? */
	    continue;
	return entry->hent_val;
    }
    return Nullstr;
}

bool
hstore(tb,key,val)
register HASH *tb;
char *key;
STR *val;
{
    register char *s;
    register int i;
    register int hash;
    register HENT *entry;
    register HENT **oentry;

    if (!tb)
	return FALSE;
    for (s=key,		i=0,	hash = 0;
      /* while */ *s;
	 s++,		i++,	hash *= 5) {
	hash += *s * coeff[i];
    }

    oentry = &(tb->tbl_array[hash & tb->tbl_max]);
    i = 1;

    for (entry = *oentry; entry; i=0, entry = entry->hent_next) {
	if (entry->hent_hash != hash)		/* strings can't be equal */
	    continue;
	if (strNE(entry->hent_key,key))	/* is this it? */
	    continue;
	safefree((char*)entry->hent_val);
	entry->hent_val = val;
	return TRUE;
    }
    entry = (HENT*) safemalloc(sizeof(HENT));

    entry->hent_key = savestr(key);
    entry->hent_val = val;
    entry->hent_hash = hash;
    entry->hent_next = *oentry;
    *oentry = entry;

    if (i) {				/* initial entry? */
	tb->tbl_fill++;
	if ((tb->tbl_fill * 100 / (tb->tbl_max + 1)) > FILLPCT)
	    hsplit(tb);
    }

    return FALSE;
}

bool
hdelete(tb,key)
register HASH *tb;
char *key;
{
    register char *s;
    register int i;
    register int hash;
    register HENT *entry;
    register HENT **oentry;

    if (!tb)
	return FALSE;
    for (s=key,		i=0,	hash = 0;
      /* while */ *s;
	 s++,		i++,	hash *= 5) {
	hash += *s * coeff[i];
    }

    oentry = &(tb->tbl_array[hash & tb->tbl_max]);
    entry = *oentry;
    i = 1;
    for (; entry; i=0, oentry = &entry->hent_next, entry = entry->hent_next) {
	if (entry->hent_hash != hash)		/* strings can't be equal */
	    continue;
	if (strNE(entry->hent_key,key))	/* is this it? */
	    continue;
	*oentry = entry->hent_next;
	hentfree(entry);
	if (i)
	    tb->tbl_fill--;
	return TRUE;
    }
    return FALSE;
}

hsplit(tb)
HASH *tb;
{
    int oldsize = tb->tbl_max + 1;
    register int newsize = oldsize * 2;
    register int i;
    register HENT **a;
    register HENT **b;
    register HENT *entry;
    register HENT **oentry;

    a = (HENT**) saferealloc((char*)tb->tbl_array, newsize * sizeof(HENT*));
    bzero((char*)&a[oldsize], oldsize * sizeof(HENT*)); /* zero second half */
    tb->tbl_max = --newsize;
    tb->tbl_array = a;

    for (i=0; i<oldsize; i++,a++) {
	if (!*a)				/* non-existent */
	    continue;
	b = a+oldsize;
	for (oentry = a, entry = *a; entry; entry = *oentry) {
	    if ((entry->hent_hash & newsize) != i) {
		*oentry = entry->hent_next;
		entry->hent_next = *b;
		if (!*b)
		    tb->tbl_fill++;
		*b = entry;
		continue;
	    }
	    else
		oentry = &entry->hent_next;
	}
	if (!*a)				/* everything moved */
	    tb->tbl_fill--;
    }
}

HASH *
hnew()
{
    register HASH *tb = (HASH*)safemalloc(sizeof(HASH));

    tb->tbl_array = (HENT**) safemalloc(8 * sizeof(HENT*));
    tb->tbl_fill = 0;
    tb->tbl_max = 7;
    hiterinit(tb);	/* so each() will start off right */
    bzero((char*)tb->tbl_array, 8 * sizeof(HENT*));
    return tb;
}

void
hentfree(hent)
register HENT *hent;
{
    if (!hent)
	return;
    str_free((char*)hent->hent_val);
    safefree(hent->hent_key);
    safefree((char*)hent);
}

void
hclear(tb)
register HASH *tb;
{
    register HENT *hent;
    register HENT *ohent = Null(HENT*);

    if (!tb)
	return;
    hiterinit(tb);
    while (hent = hiternext(tb)) {	/* concise but not very efficient */
	hentfree(ohent);
	ohent = hent;
    }
    hentfree(ohent);
    tb->tbl_fill = 0;
    bzero((char*)tb->tbl_array, (tb->tbl_max + 1) * sizeof(HENT*));
}

#ifdef NOTUSED
void
hfree(tb)
HASH *tb;
{
    if (!tb)
	return
    hiterinit(tb);
    while (hent = hiternext(tb)) {
	hentfree(ohent);
	ohent = hent;
    }
    hentfree(ohent);
    safefree((char*)tb->tbl_array);
    safefree((char*)tb);
}
#endif

#ifdef NOTUSED
hshow(tb)
register HASH *tb;
{
    fprintf(stderr,"%5d %4d (%2d%%)\n",
	tb->tbl_max+1,
	tb->tbl_fill,
	tb->tbl_fill * 100 / (tb->tbl_max+1));
}
#endif

hiterinit(tb)
register HASH *tb;
{
    tb->tbl_riter = -1;
    tb->tbl_eiter = Null(HENT*);
    return tb->tbl_fill;
}

HENT *
hiternext(tb)
register HASH *tb;
{
    register HENT *entry;

    entry = tb->tbl_eiter;
    do {
	if (entry)
	    entry = entry->hent_next;
	if (!entry) {
	    tb->tbl_riter++;
	    if (tb->tbl_riter > tb->tbl_max) {
		tb->tbl_riter = -1;
		break;
	    }
	    entry = tb->tbl_array[tb->tbl_riter];
	}
    } while (!entry);

    tb->tbl_eiter = entry;
    return entry;
}

char *
hiterkey(entry)
register HENT *entry;
{
    return entry->hent_key;
}

STR *
hiterval(entry)
register HENT *entry;
{
    return entry->hent_val;
}
