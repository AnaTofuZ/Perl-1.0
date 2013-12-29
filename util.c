/* $Header: util.c,v 1.0.1.6 88/03/10 17:13:40 root Exp $
 *
 * $Log:	util.c,v $
 * Revision 1.0.1.6  88/03/10  17:13:40  root
 * patch29: made s/\\/\\\\/ work right
 * patch29: enhanced die operator
 * patch29: added unlnk() for Eunice
 * 
 * Revision 1.0.1.5  88/03/02  12:58:14  root
 * patch24: upgraded runtime error messages
 * 
 * Revision 1.0.1.4  88/02/06  00:28:14  root
 * patch21: added trap in saferealloc() for null pointer on input.
 * 
 * Revision 1.0.1.3  88/02/04  11:17:05  root
 * patch18: regularized includes.
 * 
 * Revision 1.0.1.2  88/02/04  00:15:15  root
 * patch16: safefree wasn't safe from null pointers.
 * 
 * Revision 1.0.1.1  88/01/28  11:06:35  root
 * patch8: changed fatal() to support eval operator with exiting.
 * 
 * Revision 1.0  87/12/18  13:06:30  root
 * Initial revision
 * 
 */

#include "EXTERN.h"
#include "perl.h"

#define FLUSH
#define MEM_SIZE unsigned int

static char nomem[] = "Out of memory!\n";

/* paranoid version of malloc */

static int an = 0;

char *
safemalloc(size)
MEM_SIZE size;
{
    char *ptr;
    char *malloc();

    ptr = malloc(size?size:1);	/* malloc(0) is NASTY on our system */
#ifdef DEBUGGING
    if (debug & 128)
	fprintf(stderr,"0x%x: (%05d) malloc %d bytes\n",ptr,an++,size);
#endif
    if (ptr != Nullch)
	return ptr;
    else {
	fputs(nomem,stdout) FLUSH;
	exit(1);
    }
    /*NOTREACHED*/
}

/* paranoid version of realloc */

char *
saferealloc(where,size)
char *where;
MEM_SIZE size;
{
    char *ptr;
    char *realloc();

    if (!where)
	fatal("Null realloc");
    ptr = realloc(where,size?size:1);	/* realloc(0) is NASTY on our system */
#ifdef DEBUGGING
    if (debug & 128) {
	fprintf(stderr,"0x%x: (%05d) rfree\n",where,an++);
	fprintf(stderr,"0x%x: (%05d) realloc %d bytes\n",ptr,an++,size);
    }
#endif
    if (ptr != Nullch)
	return ptr;
    else {
	fputs(nomem,stdout) FLUSH;
	exit(1);
    }
    /*NOTREACHED*/
}

/* safe version of free */

safefree(where)
char *where;
{
#ifdef DEBUGGING
    if (debug & 128)
	fprintf(stderr,"0x%x: (%05d) free\n",where,an++);
#endif
    if (where) {
	free(where);
    }
}

/* safe version of string copy */

char *
safecpy(to,from,len)
char *to;
register char *from;
register int len;
{
    register char *dest = to;

    if (from != Nullch) 
	for (len--; len && (*dest++ = *from++); len--) ;
    *dest = '\0';
    return to;
}

#ifdef undef
/* safe version of string concatenate, with \n deletion and space padding */

char *
safecat(to,from,len)
char *to;
register char *from;
register int len;
{
    register char *dest = to;

    len--;				/* leave room for null */
    if (*dest) {
	while (len && *dest++) len--;
	if (len) {
	    len--;
	    *(dest-1) = ' ';
	}
    }
    if (from != Nullch)
	while (len && (*dest++ = *from++)) len--;
    if (len)
	dest--;
    if (*(dest-1) == '\n')
	dest--;
    *dest = '\0';
    return to;
}
#endif

/* copy a string up to some (non-backslashed) delimiter, if any */

char *
cpytill(to,from,delim)
register char *to, *from;
register int delim;
{
    for (; *from; from++,to++) {
	if (*from == '\\') {
	    if (from[1] == delim)
		from++;
	    else if (from[1] == '\\')
		*to++ = *from++;
	}
	else if (*from == delim)
	    break;
	*to = *from;
    }
    *to = '\0';
    return from;
}

/* return ptr to little string in big string, NULL if not found */

char *
instr(big, little)
char *big, *little;

{
    register char *t, *s, *x;

    for (t = big; *t; t++) {
	for (x=t,s=little; *s; x++,s++) {
	    if (!*x)
		return Nullch;
	    if (*s != *x)
		break;
	}
	if (!*s)
	    return t;
    }
    return Nullch;
}

/* copy a string to a safe spot */

char *
savestr(str)
char *str;
{
    register char *newaddr = safemalloc((MEM_SIZE)(strlen(str)+1));

    (void)strcpy(newaddr,str);
    return newaddr;
}

/* grow a static string to at least a certain length */

void
growstr(strptr,curlen,newlen)
char **strptr;
int *curlen;
int newlen;
{
    if (newlen > *curlen) {		/* need more room? */
	if (*curlen)
	    *strptr = saferealloc(*strptr,(MEM_SIZE)newlen);
	else
	    *strptr = safemalloc((MEM_SIZE)newlen);
	*curlen = newlen;
    }
}

extern int errno;

/*VARARGS1*/
fatal(pat,a1,a2,a3,a4)
char *pat;
{
    extern FILE *e_fp;
    extern char *e_tmpname;
    char *s;

    s = tokenbuf;
    sprintf(s,pat,a1,a2,a3,a4);
    s += strlen(s);
    if (s[-1] != '\n') {
	if (line) {
	    sprintf(s," at %s line %d", in_eval?filename:origfilename, line);
	    s += strlen(s);
	}
	if (last_in_stab &&
	    last_in_stab->stab_io &&
	    last_in_stab->stab_io->lines ) {
	    sprintf(s,", <%s> line %d",
	      last_in_stab == argvstab ? "" : last_in_stab->stab_name,
	      last_in_stab->stab_io->lines);
	    s += strlen(s);
	}
	strcpy(s,".\n");
    }
    if (in_eval) {
	str_set(stabent("@",TRUE)->stab_val,tokenbuf);
	longjmp(eval_env,1);
    }
    fputs(tokenbuf,stderr);
    if (e_fp)
	UNLINK(e_tmpname);
    statusvalue >>= 8;
    exit(errno?errno:(statusvalue?statusvalue:255));
}

static bool firstsetenv = TRUE;
extern char **environ;

void
setenv(nam,val)
char *nam, *val;
{
    register int i=envix(nam);		/* where does it go? */

    if (!environ[i]) {			/* does not exist yet */
	if (firstsetenv) {		/* need we copy environment? */
	    int j;
#ifndef lint
	    char **tmpenv = (char**)	/* point our wand at memory */
		safemalloc((i+2) * sizeof(char*));
#else
	    char **tmpenv = Null(char **);
#endif /* lint */
    
	    firstsetenv = FALSE;
	    for (j=0; j<i; j++)		/* copy environment */
		tmpenv[j] = environ[j];
	    environ = tmpenv;		/* tell exec where it is now */
	}
#ifndef lint
	else
	    environ = (char**) saferealloc((char*) environ,
		(i+2) * sizeof(char*));
					/* just expand it a bit */
#endif /* lint */
	environ[i+1] = Nullch;	/* make sure it's null terminated */
    }
    environ[i] = safemalloc(strlen(nam) + strlen(val) + 2);
					/* this may or may not be in */
					/* the old environ structure */
    sprintf(environ[i],"%s=%s",nam,val);/* all that work just for this */
}

int
envix(nam)
char *nam;
{
    register int i, len = strlen(nam);

    for (i = 0; environ[i]; i++) {
	if (strnEQ(environ[i],nam,len) && environ[i][len] == '=')
	    break;			/* strnEQ must come first to avoid */
    }					/* potential SEGV's */
    return i;
}

#ifdef EUNICE_SYSTEM
unlnk(f)	/* unlink all versions of a file */
char *f;
{
    int i;

    for (i = 0; unlink(f) >= 0; i++) ;
    return i ? 0 : -1;
}
#endif
