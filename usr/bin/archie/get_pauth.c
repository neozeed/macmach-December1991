/*
 * Copyright (c) 1989, 1990 by the University of Washington
 *
 * For copying and distribution information, please see the file
 * <uw-copyright.h>.
 */

#include <uw-copyright.h>
#include <stdio.h>
#include <strings.h>
#include <pwd.h>

#include <pcompat.h>
#include <pauthent.h>

PAUTH
get_pauth(type)
    int		type;
    {
	static PAUTH_ST   no_auth_st;
	static PAUTH		  no_auth = NULL;

	struct passwd *whoiampw;

	if(no_auth == NULL) {
	    no_auth = &no_auth_st;
	    strcpy(no_auth->auth_type,"UNAUTHENTICATED");

	    /* find out who we are */
	    DISABLE_PFS(whoiampw = getpwuid(getuid()));
	    if (whoiampw == 0) strcpy(no_auth->authenticator,"nobody");
	    else strcpy(no_auth->authenticator, whoiampw->pw_name);
	}
	return(no_auth);
    }
