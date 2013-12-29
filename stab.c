/* $Header: stab.c,v 1.0.1.6 88/03/10 16:49:11 root Exp $
 *
 * $Log:	stab.c,v $
 * Revision 1.0.1.6  88/03/10  16:49:11  root
 * patch29: made $! more magic than ever
 * patch29: $< and $> are uid and euid, $( and $) are gid and egid
 * 
 * Revision 1.0.1.5  88/02/04  11:16:57  root
 * patch18: regularized includes.
 * 
 * Revision 1.0.1.4  88/02/04  10:26:31  root
 * patch17: now prints warning if no signal handler defined.
 * 
 * Revision 1.0.1.3  88/02/04  00:13:15  root
 * patch16: stabset didn't copy some strings it should have.
 * 
 * Revision 1.0.1.2  88/02/02  11:25:53  root
 * patch13: moved extern int out of function for a poor Xenix machine.
 * 
 * Revision 1.0.1.1  88/01/28  10:35:17  root
 * patch8: changed some stabents to support eval operator.
 * 
 * Revision 1.0  87/12/18  13:06:14  root
 * Initial revision
 * 
 */

#include "EXTERN.h"
#include "perl.h"

#include <signal.h>

static char *sig_name[] = {
    "",
    "HUP",
    "INT",
    "QUIT",
    "ILL",
    "TRAP",
    "IOT",
    "EMT",
    "FPE",
    "KILL",
    "BUS",
    "SEGV",
    "SYS",
    "PIPE",
    "ALRM",
    "TERM",
    "???"
#ifdef SIGTSTP
    ,"STOP",
    "TSTP",
    "CONT",
    "CHLD",
    "TTIN",
    "TTOU",
    "TINT",
    "XCPU",
    "XFSZ"
#ifdef SIGPROF
    ,"VTALARM",
    "PROF"
#ifdef SIGWINCH
    ,"WINCH"
#ifdef SIGLOST
    ,"LOST"
#ifdef SIGUSR1
    ,"USR1"
#endif
#ifdef SIGUSR2
    ,"USR2"
#endif /* SIGUSR2 */
#endif /* SIGLOST */
#endif /* SIGWINCH */
#endif /* SIGPROF */
#endif /* SIGTSTP */
    ,0
    };

STR *
stab_str(stab)
STAB *stab;
{
    register int paren;
    register char *s;
    register int i;

    switch (*stab->stab_name) {
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9': case '&':
	if (curspat) {
	    paren = atoi(stab->stab_name);
	    if (curspat->spat_compex.subend[paren] &&
	      (s = getparen(&curspat->spat_compex,paren))) {
		curspat->spat_compex.subend[paren] = Nullch;
		str_set(stab->stab_val,s);
	    }
	}
	break;
    case '+':
	if (curspat) {
	    paren = curspat->spat_compex.lastparen;
	    if (curspat->spat_compex.subend[paren] &&
	      (s = getparen(&curspat->spat_compex,paren))) {
		curspat->spat_compex.subend[paren] = Nullch;
		str_set(stab->stab_val,s);
	    }
	}
	break;
    case '.':
	if (last_in_stab) {
	    str_numset(stab->stab_val,(double)last_in_stab->stab_io->lines);
	}
	break;
    case '?':
	str_numset(stab->stab_val,(double)statusvalue);
	break;
    case '^':
	s = curoutstab->stab_io->top_name;
	str_set(stab->stab_val,s);
	break;
    case '~':
	s = curoutstab->stab_io->fmt_name;
	str_set(stab->stab_val,s);
	break;
    case '=':
	str_numset(stab->stab_val,(double)curoutstab->stab_io->lines);
	break;
    case '-':
	str_numset(stab->stab_val,(double)curoutstab->stab_io->lines_left);
	break;
    case '%':
	str_numset(stab->stab_val,(double)curoutstab->stab_io->page);
	break;
    case '/':
	*tokenbuf = record_separator;
	tokenbuf[1] = '\0';
	str_set(stab->stab_val,tokenbuf);
	break;
    case '[':
	str_numset(stab->stab_val,(double)arybase);
	break;
    case '|':
	str_numset(stab->stab_val,
	   (double)((curoutstab->stab_io->flags & IOF_FLUSH) != 0) );
	break;
    case ',':
	str_set(stab->stab_val,ofs);
	break;
    case '\\':
	str_set(stab->stab_val,ors);
	break;
    case '#':
	str_set(stab->stab_val,ofmt);
	break;
    case '!':
	str_numset(stab->stab_val, (double)errno);
	str_set(stab->stab_val,
	  errno < 0 || errno > sys_nerr ? "(unknown)" : sys_errlist[errno]);
	stab->stab_val->str_nok = 1;	/* what a wonderful hack! */
	break;
    case '<':
	str_numset(stab->stab_val,(double)uid);
	break;
    case '>':
	str_numset(stab->stab_val,(double)euid);
	break;
    case '(':
	s = tokenbuf;
	sprintf(s,"%d",(int)getgid());
	goto add_groups;
    case ')':
	s = tokenbuf;
	sprintf(s,"%d",(int)getegid());
      add_groups:
	while (*s) s++;
#ifdef HAS_GETGROUPS
#ifndef NGROUPS
#define NGROUPS 32
#endif
	{
	    Gid_t gary[NGROUPS];

	    i = getgroups(NGROUPS,gary);
	    while (i >= 0) {
		sprintf(s," %d", gary[i--]);
		while (*s) s++;
	    }
	}
#endif
	str_set(stab->stab_val,tokenbuf);
	break;
    }
    return stab->stab_val;
}

stabset(stab,str)
register STAB *stab;
STR *str;
{
    char *s;
    int i;
    int sighandler();

    if (stab->stab_flags & SF_VMAGIC) {
	switch (stab->stab_name[0]) {
	case '^':
	    safefree(curoutstab->stab_io->top_name);
	    curoutstab->stab_io->top_name = s = savestr(str_get(str));
	    curoutstab->stab_io->top_stab = stabent(s,TRUE);
	    break;
	case '~':
	    safefree(curoutstab->stab_io->fmt_name);
	    curoutstab->stab_io->fmt_name = s = savestr(str_get(str));
	    curoutstab->stab_io->fmt_stab = stabent(s,TRUE);
	    break;
	case '=':
	    curoutstab->stab_io->page_len = (long)str_gnum(str);
	    break;
	case '-':
	    curoutstab->stab_io->lines_left = (long)str_gnum(str);
	    break;
	case '%':
	    curoutstab->stab_io->page = (long)str_gnum(str);
	    break;
	case '|':
	    curoutstab->stab_io->flags &= ~IOF_FLUSH;
	    if (str_gnum(str) != 0.0) {
		curoutstab->stab_io->flags |= IOF_FLUSH;
	    }
	    break;
	case '*':
	    multiline = (int)str_gnum(str) != 0;
	    break;
	case '/':
	    record_separator = *str_get(str);
	    break;
	case '\\':
	    if (ors)
		safefree(ors);
	    ors = savestr(str_get(str));
	    break;
	case ',':
	    if (ofs)
		safefree(ofs);
	    ofs = savestr(str_get(str));
	    break;
	case '#':
	    if (ofmt)
		safefree(ofmt);
	    ofmt = savestr(str_get(str));
	    break;
	case '[':
	    arybase = (int)str_gnum(str);
	    break;
	case '?':
	    statusvalue = (unsigned short)str_gnum(str);
	    break;
	case '!':
	    errno = (int)str_gnum(str);		/* will anyone ever use this? */
	    break;
	case '<':
	{
	    int r;
	    uid = (int)str_gnum(str);
#ifdef HAS_SETRUID
            r = setruid(uid);
#else
#ifdef HAS_SETREUID
            r = setreuid(uid, (Uid_t)-1);
#else
	    fatal("setruid() not implemented");
#endif
#endif
	    if (r < 0)
		uid = (int)getuid();
	    break;
	}
	case '>':
	    euid = (int)str_gnum(str);
	    if (seteuid(euid) < 0)
		euid = (int)geteuid();
	    break;
	case '(':
	{
	   int a = (int)str_gnum(str);
#ifdef HAS_SETRGID
            setrgid(uid);
#else
#ifdef HAS_SETREGID
            setregid(uid, (Uid_t)-1);
#else
            fatal("setrgid() not implemented");
#endif
#endif
	    break;
	} 
	case ')':
	    setegid((int)str_gnum(str));
	    break;
	case '.':
	case '+':
	case '&':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	    break;		/* "read-only" registers */
	}
    }
    else if (stab == envstab && envname) {
	xsetenv(envname,str_get(str));
				/* And you'll never guess what the dog had */
	safefree(envname);	/*   in its mouth... */
	envname = Nullch;
    }
    else if (stab == sigstab && signame) {
	s = str_get(str);
	i = whichsig(signame);	/* ...no, a brick */
	if (strEQ(s,"IGNORE"))
	    signal(i,SIG_IGN);
	else if (strEQ(s,"DEFAULT") || !*s)
	    signal(i,SIG_DFL);
	else
	    signal(i,sighandler);
	safefree(signame);
	signame = Nullch;
    }
}

whichsig(signame)
char *signame;
{
    register char **sigv;

    for (sigv = sig_name+1; *sigv; sigv++)
	if (strEQ(signame,*sigv))
	    return sigv - sig_name;
    return 0;
}

sighandler(sig)
int sig;
{
    STAB *stab;
    ARRAY *savearray;
    STR *str;

    stab = stabent(str_get(hfetch(sigstab->stab_hash,sig_name[sig])),TRUE);
    if (stab->stab_sub) {
	savearray = defstab->stab_array;
	defstab->stab_array = anew();
	str = str_new(0);
	str_set(str,sig_name[sig]);
	apush(defstab->stab_array,str);
	str = cmd_exec(stab->stab_sub);
	afree(defstab->stab_array);  /* put back old $_[] */
	defstab->stab_array = savearray;
    }
    else
	fprintf(stderr,"perl warning: SIG%s handler \"%s\" not defined.\n",
	    sig_name[sig], stab->stab_name );
}

char *
reg_get(name)
char *name;
{
    return STAB_GET(stabent(name,TRUE));
}

#ifdef NOTUSED
reg_set(name,value)
char *name;
char *value;
{
    str_set(STAB_STR(stabent(name,TRUE)),value);
}
#endif

STAB *
aadd(stab)
register STAB *stab;
{
    if (!stab->stab_array)
	stab->stab_array = anew();
    return stab;
}

STAB *
hadd(stab)
register STAB *stab;
{
    if (!stab->stab_hash)
	stab->stab_hash = hnew();
    return stab;
}
