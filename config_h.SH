case "$CONFIG_SH" in
'') CONFIG_SH=config.sh;;
esac
case "$CONFIG_H" in
'') CONFIG_H=config.h;;
esac
case $CONFIG in
'')
	if test -f $CONFIG_SH; then TOP=.;
	elif test -f ../$CONFIG_SH; then TOP=..;
	elif test -f ../../$CONFIG_SH; then TOP=../..;
	elif test -f ../../../$CONFIG_SH; then TOP=../../..;
	elif test -f ../../../../$CONFIG_SH; then TOP=../../../..;
	else
		echo "Can't find $CONFIG_SH."; exit 1
	fi
	. $TOP/$CONFIG_SH
	;;
esac
case "$0" in
*/*) cd `expr X$0 : 'X\(.*\)/'` ;;
esac
echo "Extracting $CONFIG_H (with variable substitutions)"
sed <<!GROK!THIS! >$CONFIG_H -e 's!^#undef\(.*/\)\*!/\*#define\1 \*!' -e 's!^#un-def!#undef!'
/*
 * This file was produced by running the config_h.SH script, which
 * gets its values from $CONFIG_SH, which is generally produced by
 * running Configure.
 *
 * Feel free to modify any of this as the need arises.  Note, however,
 * that running config_h.SH again will wipe out any changes you've made.
 * For a more permanent change edit $CONFIG_SH and rerun config_h.SH.
 *
 * \$Id: Config_h.U 167 2013-05-08 17:58:00Z rmanfredi $
 */

/*
 * Package name      : $package
 * Source directory  : $src
 * Configuration time: $cf_time
 * Configured by     : $cf_by
 * Target system     : $myuname
 */

#ifndef _config_h_
#define _config_h_

/* EUNICE_SYSTEM:
 *	This symbol, if defined, indicates that the program is being compiled
 *	under the EUNICE package under VMS.  The program will need to handle
 *	things like files that don't go away the first time you unlink them,
 *	due to version numbering.  It will also need to compensate for lack
 *	of a respectable link() command.
 */
#$d_eunice EUNICE_SYSTEM		/**/

/* LOC_SED:
 *	This symbol holds the complete pathname to the sed program.
 */
#define LOC_SED 	"$full_sed"	/**/

/* CPPSTDIN:
 *	This symbol contains the first part of the string which will invoke
 *	the C preprocessor on the standard input and produce to standard
 *	output.	 Typical value of "cc -E" or "/lib/cpp", but it can also
 *	call a wrapper. See CPPRUN.
 */
/* CPPMINUS:
 *	This symbol contains the second part of the string which will invoke
 *	the C preprocessor on the standard input and produce to standard
 *	output.  This symbol will have the value "-" if CPPSTDIN needs a minus
 *	to specify standard input, otherwise the value is "".
 */
#define CPPSTDIN "$cppstdin"
#define CPPMINUS "$cppminus"

/* HAS_BCOPY:
 *	This symbol is defined if the bcopy() routine is available to
 *	copy blocks of memory.
 */
#$d_bcopy HAS_BCOPY	/**/

/* HAS_BZERO:
 *	This symbol is defined if the bzero() routine is available to
 *	set a memory block to 0.
 */
#$d_bzero HAS_BZERO	/**/

/* DEBUGGING:
 *	This symbol, when defined, activates extra debugging.
 */
#$d_debugging DEBUGGING

/* HAS_GETGROUPS:
 *	This symbol, if defined, indicates that the getgroups() routine is
 *	available to get the list of process groups.  If unavailable, multiple
 *	groups are probably not supported.
 */
#$d_getgrps HAS_GETGROUPS		/**/

/* HAS_GNULIBC:
 *	This symbol, if defined, indicates to the C program that 
 *	the GNU C library is being used.  A better check is to use
 *	the __GLIBC__ and __GLIBC_MINOR__ symbols supplied with glibc.
 */
#$d_gnulibc HAS_GNULIBC  	/**/
#if defined(HAS_GNULIBC) && !defined(_GNU_SOURCE)
#define _GNU_SOURCE
#endif

/* HAS_KILLPG:
 *	This symbol, if defined, indicates that the killpg routine is available
 *	to kill process groups.  If unavailable, you probably should use kill
 *	with a negative process number.
 */
#$d_killpg HAS_KILLPG	/**/

/* HAS_RENAME:
 *	This symbol, if defined, indicates that the rename routine is available
 *	to rename files.  Otherwise you should do the unlink(), link(), unlink()
 *	trick.
 */
#$d_rename HAS_RENAME	/**/

/* HAS_SETREGID:
 *	This symbol, if defined, indicates that the setregid routine is
 *	available to change the real and effective gid of the current
 *	process.
 */
#$d_setregid HAS_SETREGID		/**/

/* HAS_SETREUID:
 *	This symbol, if defined, indicates that the setreuid routine is
 *	available to change the real and effective uid of the current
 *	process.
 */
#$d_setreuid HAS_SETREUID		/**/

/* HAS_SETRGID:
 *	This symbol, if defined, indicates that the setrgid routine is available
 *	to change the real gid of the current program.
 */
#$d_setrgid HAS_SETRGID		/**/

/* HAS_SETRUID:
 *	This symbol, if defined, indicates that the setruid routine is available
 *	to change the real uid of the current program.
 */
#$d_setruid HAS_SETRUID		/**/

/* HAS_STAT:
 *	This symbol, if defined, indicates that the stat routine is
 *	available to get file status.
 */
#$d_stat HAS_STAT	/**/

/* USE_STDIO_PTR:
 *	This symbol is defined if the _ptr and _cnt fields (or similar)
 *	of the stdio FILE structure can be used to access the stdio buffer
 *	for a file handle.  If this is defined, then the FILE_ptr(fp)
 *	and FILE_cnt(fp) macros will also be defined and should be used
 *	to access these fields.
 */
#$d_stdstdio USE_STDIO_PTR 	/**/
#ifdef USE_STDIO_PTR
#if defined(STDIO_PTR_LVALUE) && defined(STDIO_CNT_LVALUE)
#endif
#endif

/* HAS_INDEX:
 *	This symbol is defined to indicate that the index()/rindex()
 *	functions are available for string searching.
 */
#$d_index HAS_INDEX	/**/

/* USE_STRUCT_COPY:
 *	This symbol, if defined, indicates that this C compiler knows how
 *	to copy structures.  If undefined, you'll need to use a block copy
 *	routine of some sort instead.
 */
#$d_strctcpy	USE_STRUCT_COPY	/**/

/* HAS_SYMLINK:
 *	This symbol, if defined, indicates that the symlink routine is available
 *	to create symbolic links.
 */
#$d_symlink HAS_SYMLINK	/**/

/* HAS_SYSTEM:
 *	This symbol, if defined, indicates that the system routine is
 *	available to issue a shell command.
 */
#$d_system HAS_SYSTEM	/**/

/* HAS_TIMES:
 *	This symbol, if defined, indicates that the times() routine exists.
 *	Note that this became obsolete on some systems (SUNOS), which now
 * use getrusage(). It may be necessary to include <sys/times.h>.
 */
#$d_times HAS_TIMES		/**/

/* TM_IN_SYS:
 *	This symbol is defined if this system declares "struct tm" in
 *	in <sys/time.h> rather than <time.h>.  We can't just say
 *	-I/usr/include/sys because some systems have both time files, and
 *	the -I trick gets the wrong one.
 */
#$d_tminsys TM_IN_SYS 	/**/

/* HAS_VFORK:
 *	This symbol, if defined, indicates that vfork() exists.
 */
#$d_vfork HAS_VFORK	/**/

/* VOIDSIG:
 *	This symbol is defined if this system declares "void (*signal(...))()" in
 *	signal.h.  The old way was to declare it as "int (*signal(...))()".  It
 *	is up to the package author to declare things correctly based on the
 *	symbol.
 */
#$d_voidsig VOIDSIG 	/**/

/* Gid_t:
 *	This symbol holds the return type of getgid() and the type of
 *	argument to setrgid() and related functions.  Typically,
 *	it is the type of group ids in the kernel. It can be int, ushort,
 *	uid_t, etc... It may be necessary to include <sys/types.h> to get
 *	any typedef'ed information.
 */
#define Gid_t $gidtype		/* Type for getgid(), etc... */

#if defined(HAS_GETGROUPS) || defined(HAS_SETGROUPS)
#define Groups_t $groupstype	/* Type for 2nd arg to [sg]etgroups() */
#endif

/* I_UNISTD:
 *	This symbol, if defined, indicates to the C program that it should
 *	include <unistd.h>.
 */
#$i_unistd I_UNISTD		/**/

/* CAN_PROTOTYPE:
 *	If defined, this macro indicates that the C compiler can handle
 *	function prototypes.
 */
/* _:
 *	This macro is used to declare function parameters for folks who want
 *	to make declarations with prototypes using a different style than
 *	the above macros.  Use double parentheses.  For example:
 *
 *		int main _((int argc, char *argv[]));
 */
#$prototype	CAN_PROTOTYPE	/**/
#ifdef CAN_PROTOTYPE
#define	_(args) args
#else
#define	_(args) ()
#endif

/* STDCHAR:
 *	This symbol is defined to be the type of char used in stdio.h.
 *	It has the values "unsigned char" or "char".
 */
#define STDCHAR $stdchar	/**/

/* Uid_t:
 *	This symbol holds the type used to declare user ids in the kernel.
 *	It can be int, ushort, uid_t, etc... It may be necessary to include
 *	<sys/types.h> to get any typedef'ed information.
 */
#define Uid_t $uidtype		/* UID type */

/* VOIDFLAGS:
 *	This symbol indicates how much support of the void type is given by this
 *	compiler.  What various bits mean:
 *
 *	    1 = supports declaration of void
 *	    2 = supports arrays of pointers to functions returning void
 *	    4 = supports comparisons between pointers to void functions and
 *		    addresses of void functions
 *	    8 = suports declaration of generic void pointers
 *
 *	The package designer should define VOIDUSED to indicate the requirements
 *	of the package.  This can be done either by #defining VOIDUSED before
 *	including config.h, or by defining defvoidused in Myinit.U.  If the
 *	latter approach is taken, only those flags will be tested.  If the
 *	level of void support necessary is not present, defines void to int.
 */
#ifndef VOIDUSED
#define VOIDUSED $defvoidused
#endif
#define VOIDFLAGS $voidflags
#if (VOIDFLAGS & VOIDUSED) != VOIDUSED
#define void int		/* is void to be avoided? */
#define M_VOID			/* Xenix strikes again */
#endif

/* HAS_CRYPT:
 *	This symbol, if defined, indicates that the crypt routine is available
 *	to encrypt passwords and the like.
 */
#$d_crypt HAS_CRYPT		/**/

/* I_CRYPT:
 *	This symbol, if defined, indicates that <crypt.h> exists and
 *	should be included.
 */
#$i_crypt	I_CRYPT		/**/

#endif
!GROK!THIS!
