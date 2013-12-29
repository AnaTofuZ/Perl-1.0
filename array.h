/* $Header: array.h,v 1.0.1.1 88/03/10 16:26:38 root Exp $
 *
 * $Log:	array.h,v $
 * Revision 1.0.1.1  88/03/10  16:26:38  root
 * patch29: added aclear()
 * 
 * Revision 1.0  87/12/18  13:04:46  root
 * Initial revision
 * 
 */

struct atbl {
    STR	**ary_array;
    int	ary_max;
    int	ary_fill;
};

STR *afetch();
bool astore();
bool adelete();
STR *apop();
STR *ashift();
void afree();
void aclear();
bool apush();
long alen();
ARRAY *anew();
