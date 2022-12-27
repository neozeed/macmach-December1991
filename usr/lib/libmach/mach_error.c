/* 
 * Mach Operating System
 * Copyright (c) 1991,1990,1989 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS 
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 * 
 * Carnegie Mellon requests users of this software to return to
 * 
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 * 
 * any improvements or extensions that they make and grant Carnegie the
 * rights to redistribute these changes.
 */
/*
 * HISTORY
 * $Log:	mach_error.c,v $
 * Revision 2.3  91/08/29  15:51:50  rpd
 * 	Changed IPC_MIG_MOD to MACH_IPC_MIG_MOD, to get the new error strings.
 * 	[91/08/22            rpd]
 * 
 * Revision 2.2  91/03/27  16:06:29  mrt
 * 	Changed include of "errorlib.h" to <servers/errorlib.h>
 * 	Added new copyright
 * 	[91/03/20            mrt]
 * 
 */
/*
 * 	File:	mach_error.c
 *	Author:	Douglas Orr, Carnegie Mellon University
 *	Date:	Mar 1988
 *
 *      interprets structured mach error codes and prints
 *      or returns a descriptive string.
 */

#include <stdio.h>
#include <mach/error.h>
#include <mach_error.h>
#include <servers/errorlib.h>

static char * mach_error_string_int();

static
do_compat( org_err )
	mach_error_t		* org_err;
{
	mach_error_t		err = *org_err;

	/* 
	 * map old error numbers to 
	 * to new error sys & subsystem 
	 */

	if ((-200 < err) && (err <= -100))
		err = -(err + 100) | IPC_SEND_MOD;
	else if ((-300 < err) && (err <= -200))
		err = -(err + 200) | IPC_RCV_MOD;
	else if ((-400 < err) && (err <= -300))
		err = -(err + 300) | MACH_IPC_MIG_MOD;
	else if ((1000 <= err) && (err < 1100))
		err = (err - 1000) | SERV_NETNAME_MOD;
	else if ((1600 <= err) && (err < 1700))
		err = (err - 1600) | SERV_ENV_MOD;
	else if ((27600 <= err) && (err < 27700))
		err = (err - 27600) | SERV_EXECD_MOD;

	*org_err = err;
}

typedef enum { false=0, true=1 } boolean;
#if	DEBUG
boolean mach_error_full_diag = true;
#else
boolean mach_error_full_diag = false;
#endif	DEBUG


void
mach_error( str, err )	
	char	*str;
	mach_error_t		err;
{
	char * err_str;
	char buf[1024];
	int diag;

	do_compat( &err );

	err_str=mach_error_string_int(err, &diag);

	if ( diag ) {
		sprintf( buf, "%s %s (%x)", mach_error_type(err), err_str, err );
		err_str = buf;
	}

	fprintf(stderr, "%s %s\n", str, err_str);
}


char *
mach_error_type( err )
	mach_error_t		err;
{
	int sub, system;

	do_compat( &err );

	sub = err_get_sub(err);
	system = err_get_system(err);

	if (system > err_max_system
	||  sub >= errors[system].max_sub ) return( "(?/?)" );
	return( errors[system].subsystem[sub].subsys_name );
}

static char *
mach_error_string_int( err, diag )
	mach_error_t		err;
	boolean			* diag;
{
	int sub, system, code;
	char * err_str;

	do_compat( &err );

	sub = err_get_sub(err);
	system = err_get_system(err);
	code = err_get_code(err);

	*diag = true;

	if (system > err_max_system) return( "(?/?) unknown error system" );
	if (sub >= errors[system].max_sub) return( errors[system].bad_sub );
	if (code >= errors[system].subsystem[sub].max_code) return ( NO_SUCH_ERROR );

	*diag = mach_error_full_diag;
	return( errors[system].subsystem[sub].codes[code] );
}


char *
mach_error_string( err )
	mach_error_t		err;
{
	boolean diag;

	return mach_error_string_int( err, &diag );

}

