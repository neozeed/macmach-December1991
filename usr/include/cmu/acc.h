/*
 *  see getacent(3)
 *
 **********************************************************************
 * HISTORY
 * $Log:	acc.h,v $
 * Revision 1.2  88/12/14  23:29:04  mja
 * 	Added declarations for __STDC__.
 * 	[88/01/06            jjk]
 * 
 * 11-Oct-85  Glenn Marcy (gm0w) at Carnegie-Mellon University
 *	Add external declarations for account routines.  Change
 *	ac_passwd to ac_attrs for new accounting file information.
 *
 **********************************************************************
 */

struct	account
{
	char	*ac_name;
	char	**ac_attrs;
	int	 ac_uid;
	int	 ac_aid;
	char	*ac_created;
	char	*ac_expires;
	char	*ac_sponsor;
	time_t	 ac_ctime;
	time_t	 ac_xtime;
};

#if defined(__STDC__)
extern struct account *getacent(void);
extern struct account *getacauid(int, int);
extern struct account *getacanam(const char *, int);
extern void setacent(void);
extern void endacent(void);
#else
struct account *getacent(), *getacauid(), *getacanam();
#endif
