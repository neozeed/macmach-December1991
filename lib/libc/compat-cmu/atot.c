/*
 *  atot - convert ascii string to time
 *
 **********************************************************************
 * HISTORY
 * 09-Oct-85  Glenn Marcy (gm0w) at Carnegie-Mellon University
 *	Fixed previous edit.  Need to zero time fields of tm struct
 *	after parsedate() because it sets them to -1, which is
 *	considered bad form by gtime().
 *
 * 30-Apr-85  Steven Shafer (sas) at Carnegie-Mellon University
 *	Adapted for 4.2 BSD UNIX.  Changed to new timeval struct, use
 *	gettimeofday(), and new parsedate().
 *
 * 03-Jan-80  Mike Accetta (mja) at Carnegie-Mellon University
 *	Created.
 *
 **********************************************************************
 *
 *  Usage:  t = atot (date);
 *	time_t t;
 *	char *date;
 *
 *     Converts a date string to its corresponding UNIX date/time
 *  format using pdate().  The month, day and year default to
 *  today, hours, minutes and seconds default to 0.
 *
 *     Returns converted time or -1 on error (bad date string).
 */

#include <sys/types.h>
#include <sys/time.h>
#include <cmu/c.h>
#include <cmu/libc.h>

extern time_t gtime();

time_t atot(str)
const char *str;
{

    struct timeval now;
    struct timezone zone;
    register struct tm *t;

    gettimeofday (&now,&zone);
    t = localtime(&now.tv_sec);
    t->tm_sec = t->tm_min = t->tm_hour = 0;
    if (parsedate(str, t, 1, 0, 0) < 0)
	return(CERROR);
    if (t->tm_sec == -1 && t->tm_min == -1 && t->tm_hour == -1)
	t->tm_sec = t->tm_min = t->tm_hour = 0;
    return(gtime(t));

}
