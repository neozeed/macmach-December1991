/*
 * Copyright (c) 1989, 1990, 1991 by the University of Washington
 *
 * For copying and distribution information, please see the file
 * <uw-copyright.h>.
 */

/*
 * Miscellaneous routines pulled from ~beta/lib/pfs and ~beta/lib/filters
 */

#include <uw-copyright.h>
#include <stdio.h>
#include <strings.h>
#include <errno.h>
#include <netdb.h>

#ifdef AUX
#include <sys/types.h>
#endif
#include <sys/file.h>
#include <sys/param.h>

#include <pfs.h>
#include <pprot.h>
#include <perrno.h>
#include <pcompat.h>
#include <pauthent.h>
#include <pmachine.h>

int	pfs_enable = PMAP_ATSIGN;
char	p_err_string[P_ERR_STRING_SZ];

#ifndef FALSE
#define TRUE 	1
#define FALSE   0
#endif

#ifdef NOREGEX
#include REGEX_FILE
#endif

/* 
 * wcmatch - Match string s against template containing widlcards
 *
 *	     WCMATCH takes a string and a template, and returns
 *	     true if the string matches the template, and 
 *	     FALSE otherwise.
 *
 *    ARGS:  s        - string to be tested
 *           template - Template containing optional wildcards
 *
 * RETURNS:  TRUE (non-zero) on match.  FALSE (0) otherwise.
 *
 *    NOTE:  If template is NULL, will return TRUE.
 *
 */
wcmatch(s,template)
    char	*s;
    char	*template;
    {
	char	temp[200];
	char	*p = temp;

	if(!template) return(TRUE);
	*p++ = '^';

	while(*template) {
	    if(*template == '*') {*(p++)='.'; *(p++) = *(template++);}
	    else if(*template == '?') {*(p++)='.';template++;}
	    else if(*template == '.') {*(p++)='\\';*(p++)='.';template++;}
	    else if(*template == '[') {*(p++)='\\';*(p++)='[';template++;}
	    else if(*template == '$') {*(p++)='\\';*(p++)='$';template++;}
	    else if(*template == '^') {*(p++)='\\';*(p++)='^';template++;}
	    else if(*template == '\\') {*(p++)='\\';*(p++)='\\';template++;}
	    else *(p++) = *(template++);
	}
	    
	*p++ = '$';
	*p++ = '\0';

	if(re_comp(temp)) return(FALSE);

	return(re_exec(s));
    }

/*
 * ul_insert - Insert a union link at the right location
 *
 *             UL_INSERT takes a directory and a union link to be added
 *             to a the list of union links in the directory.  It then
 *             inserts the union link in the right spot in the linked
 *             list of union links associated with that directory.
 *
 *	       If an identical link already exists, then the link which
 *             would be evaluated earlier (closer to the front of the list)
 *             wins and the other one is freed.  If this happens, an error
 *             will also be returned.
 *        
 *    ARGS:    ul    - link to be inserted
 *	       vd    - directory to get link
 *             p     - vl that this link will apper after
 *                     NULL - This vl will go at end of list
 *                     vd   - This vl will go at head of list
 *
 * RETURNS:    Success, or UL_INSERT_ALREADY_THERE or UL_INSERT_SUPERSEDING
 */
ul_insert(ul,vd,p)
    VLINK	ul;		/* Link to be inserted                   */
    VDIR	vd;		/* Directory to receive link             */
    VLINK	p;		/* Union link to appear prior to new one */
    {
	VLINK	current;

	/* This is the first ul in the directory */
	if(vd->ulinks == NULL) {
	    vd->ulinks = ul;
	    ul->previous = NULL;
	    ul->next = NULL;
	    return(PSUCCESS);
	}

	/* This ul will go at the head of the list */
	if(p == (VLINK) vd) {
	    ul->next = vd->ulinks;
	    ul->next->previous = ul;
	    vd->ulinks = ul;
	    ul->previous = NULL;
	}
	/* Otherwise, decide if it must be inserted at all  */
	/* If an identical link appears before the position */
	/* at which the new one is to be inserted, we can   */
	/* return without inserting it 			    */
	else {
	    current = vd->ulinks;

	    while(current) {
		/* p == NULL means we insert after last link */
		if(!p && (current->next == NULL))
		    p = current;

		if(vl_comp(current,ul) == 0) {
		    vlfree(ul);
		    return(UL_INSERT_ALREADY_THERE);
		}

		if(current == p) break;
		current = current->next;
	    }

	    /* If current is null, p was not found */
	    if(current == NULL)
		return(UL_INSERT_POS_NOTFOUND);

	    /* Insert ul */
	    ul->next = p->next;
	    p->next = ul;
	    ul->previous = p;
	    if(ul->next) ul->next->previous = ul;
	}

	/* Check for identical links after ul */
	current = ul->next;

	while(current) {
	    if(vl_comp(current,ul) == 0) {
		current->previous->next = current->next;
		if(current->next)
		    current->next->previous = current->previous;
		vlfree(current);
		return(UL_INSERT_SUPERSEDING);
	    }
	    current = current->next;
	}
	
	return(PSUCCESS);
    }

/*
 * vl_insert - Insert a directory link at the right location
 *
 *             VL_INSERT takes a directory and a link to be added to a 
 *             directory and inserts it in the linked list of links for
 *             that directory.  
 *
 *             If a link already exists with the same name, and if the
 *             information associated with the new link matches that in
 *             the existing link, an error is returned.  If the information
 *             associated with the new link is different, but the magic numbers
 *             match, then the new link will be added as a replica of the
 *             existing link.  If the magic numbers do not match, the new
 *             link will only be added to the list of "replicas" if the
 *             allow_conflict flag has been set.
 * 
 *             If the link is not added, an error is returned and the link
 *             is freed.  Ordering for the list of links is by the link name.  
 *        
 *             If vl is a union link, then VL_INSERT calls ul_insert with an
 *	       added argument indicating the link is to be included at the
 *             end of the union link list.
 * 
 *    ARGS:    vl - Link to be inserted, vd - directory to get link
 *             allow_conflict - insert links with conflicting names
 *
 * RETURNS:    Success, or VL_INSERT_ALREADY_THERE
 */
vl_insert(vl,vd,allow_conflict)
    VLINK	vl;		/* Link to be inserted               */
    VDIR	vd;		/* Directory to receive link         */
    int		allow_conflict;	/* Allow duplicate names             */
    {
	VLINK	current;	/* To step through list		     */
	VLINK	crep;		/* To step through list of replicas  */
	int	retval;		/* Temp for checking returned values */

	/* This can also be used to insert union links at end of list */
	if(vl->linktype == 'U') return(ul_insert(vl,vd,NULL));

	/* If this is the first link in the directory */
	if(vd->links == NULL) {
	    vd->links = vl;
	    vl->previous = NULL;
	    vl->next = NULL;
	    vd->lastlink = vl;
	    return(PSUCCESS);
	}

	/* If no sorting is to be done, just insert at end of list */
	if(allow_conflict == VLI_NOSORT) {
	    vd->lastlink->next = vl;
	    vl->previous = vd->lastlink;
	    vl->next = NULL;
	    vd->lastlink = vl;
	    return(PSUCCESS);
	}

	/* If it is to be inserted at start of list */
	if(vl_comp(vl,vd->links) < 0) {
	    vl->next = vd->links;
	    vl->previous = NULL;
	    vl->next->previous = vl;
	    vd->links = vl;
	    return(PSUCCESS);
	}

	current = vd->links;

	/* Otherwise, we must find the right spot to insert it */
	while((retval = vl_comp(vl,current)) > 0) {
	    if(!current->next) {
		/* insert at end */
		vl->previous = current;
		vl->next = NULL;
		current->next = vl;
		vd->lastlink = vl;
		return(PSUCCESS);
	    }
	    current = current->next;
	}

	/* If we found an equivilant entry already in list */
	if(!retval) {
	    if(vl_equal(vl,current)) {
		vlfree(vl);
		return(VL_INSERT_ALREADY_THERE);
	    }
	    if((allow_conflict == VLI_NOCONFLICT) &&
	       ((vl->f_magic_no != current->f_magic_no) ||
		(vl->f_magic_no==0)))
		return(VL_INSERT_CONFLICT);
	    /* Insert the link into the list of "replicas" */
	    /* If magic is 0, then create a pseudo magic number */
	    if(vl->f_magic_no == 0) vl->f_magic_no = -1;
	    crep = current->replicas;
	    if(!crep) {
		current->replicas = vl;
		vl->next = NULL;
		vl->previous = NULL;
	    }
	    else {
		while(crep->next) {
		    /* If magic was 0, then we need a unique magic number */
		    if((crep->f_magic_no < 0) && (vl->f_magic_no < 1))
			(vl->f_magic_no)--;
		    crep = crep->next;
		}
		/* If magic was 0, then we need a unique magic number */
		if((crep->f_magic_no < 0) && (vl->f_magic_no < 1))
		    (vl->f_magic_no)--;
		crep->next = vl;
		vl->previous = crep;
		vl->next = NULL;
	    }
	    return(PSUCCESS);
	}

	/* We found the spot where vl is to be inserted */
	vl->next = current;
	vl->previous = current->previous;
	current->previous = vl;
	vl->previous->next = vl;
	return(PSUCCESS);
    }

/*
 * nlsindex - Find first instance of string 2 in string 1 following newline
 *
 *	      NLSINDEX scans string 1 for the first instance of string
 *	      2 that immediately follows a newline.  If found, NLSINDEX
 *	      returns a pointer to the first character of that instance.
 *	      If no instance is found, NLSINDEX returns NULL (0).
 *
 *    NOTE:   This function is only useful for searching strings that
 *            consist of multiple lines.  s1 is assumed to be preceeded
 * 	      by a newline.  Thus, if s2 is at the start of s1, it will
 *	      be found.
 *    ARGS:   s1 - string to be searched
 *            s2 - string to be found
 * RETURNS:   First instance of s2 in s1, or NULL (0) if not found
 */
char *
nlsindex(s1,s2)
    char	*s1;		/* String to be searched */
    char	*s2;		/* String to be found    */
    {
	register int s2len = strlen(s2);
	char	*curline = s1;	/* Pointer to start of current line */

	/* In case s2 appears at start of s1 */
	if(strncmp(curline,s2,s2len) == 0)
	    return(curline);

	/* Check remaining lines of s1 */
	while((curline = index(curline,'\n')) != NULL) {
	    curline++;
	    if(strncmp(curline,s2,s2len) == 0)
		return(curline);
	}

	/* We didn't find it */
	return(NULL);
    }

/*
 * month_sname - Return a month name from it's number
 *
 *               MONTH_SNAME takes a number in the range 0
 *               to 12 and returns a pointer to a string
 *               representing the three letter abbreviation
 *	         for that month.  If the argument is out of 
 *		 range, MONTH_SNAME returns a pointer to "Unk".
 *
 *       ARGS:   n - Number of the month
 *    RETURNS:   Abbreviation for selected month
 */
char *month_sname(n)
    int n;		/* Month number */
{
    static char *name[] = { "Unk",
        "Jan","Feb","Mar","Apr","May","Jun",
        "Jul","Aug","Sep","Oct","Nov","Dec"
    };
    return((n < 1 || n > 12) ? name[0] : name[n]);
}

/*
 * sindex - Find first instance of string 2 in string 1 
 *
 *	      SINDEX scans string 1 for the first instance of string
 *	      2.  If found, SINDEX returns a pointer to the first
 *	      character of that instance.  If no instance is found, 
 *	      SINDEX returns NULL (0).
 *
 *    ARGS:   s1 - string to be searched
 *            s2 - string to be found
 * RETURNS:   First instance of s2 in s1, or NULL (0) if not found
 */
char *
sindex(s1,s2)
    char	*s1;		/* String to be searched   */
    char	*s2;		/* String to be found      */
    {
	register int s2len = strlen(s2);
	char	*s = s1;	/* Temp pointer to string  */

	/* Check for first character of s2 */
	while((s = index(s,*s2)) != NULL) {
	    if(strncmp(s,s2,s2len) == 0)
		return(s);
	    s++;
	}

	/* We didn't find it */
	return(NULL);
    }

scan_error(erst)
    char	*erst;
    {
      *p_err_string = '\0';

	if(strncmp(erst,"NOT-A-DIRECTORY",15) == 0) 
	    return(DIRSRV_NOT_DIRECTORY);

	/* The rest start with "FAILURE" */

	if(strncmp(erst,"FAILURE",7) != 0) 
	    return(DIRSRV_BAD_FORMAT);

	if(strncmp(erst,"FAILURE ",8) != 0) 
	    return(PFAILURE);

	erst += 8;

        sscanf(erst, "%*s %[^\n]", p_err_string);

	/* Still to add               */
	/* DIRSRV_AUTHENT_REQ     242 */
	/* DIRSRV_BAD_VERS        245 */

	if(strncmp(erst,"NOT-FOUND",9) == 0) 
	    return(DIRSRV_NOT_FOUND);

	if(strncmp(erst,"NOT-AUTHORIZED",13) == 0) 
	    return(DIRSRV_NOT_AUTHORIZED);

	if(strncmp(erst,"ALREADY-EXISTS",14) == 0) 
	    return(DIRSRV_ALREADY_EXISTS);

	if(strncmp(erst,"NAME-CONFLICT",13) == 0) 
	    return(DIRSRV_NAME_CONFLICT);

	if(strncmp(erst,"SERVER-FAILED",13) == 0) 
	    return(DIRSRV_SERVER_FAILED);

 
	/* Use it whether it starts with FAILURE or not */
	if(strncmp(erst,"NOT-A-DIRECTORY",15) == 0) 
	    return(DIRSRV_NOT_DIRECTORY);

	return(PFAILURE);
    }

PATTRIB 
parse_attribute(line)
    char	*line;
    {
	char	l_precedence[MAX_DIR_LINESIZE];
	char	l_name[MAX_DIR_LINESIZE];
	char	l_type[MAX_DIR_LINESIZE];
	char	l_value[MAX_DIR_LINESIZE];
	PATTRIB	at;
	int	tmp;

	tmp = sscanf(line,"OBJECT-INFO %s %s %[^\n]", l_name, l_type, l_value);
	
	if(tmp < 3) {
	    tmp = sscanf(line,"LINK-INFO %s %s %s %[^\n]", l_precedence,
			 l_name, l_type, l_value);
	    if(tmp < 4) {
		perrno = DIRSRV_BAD_FORMAT;
		return(NULL);
	    }
	}

	at = atalloc();

	if(tmp == 4) {
	    if(strcmp(l_precedence,"CACHED") == 0) 
		at->precedence = ATR_PREC_CACHED;
	    else if(strcmp(l_precedence,"LINK") == 0) 
		at->precedence = ATR_PREC_LINK;
	    else if(strcmp(l_precedence,"REPLACEMENT") == 0) 
		at->precedence = ATR_PREC_REPLACE;
	    else if(strcmp(l_precedence,"ADDITIONAL") == 0) 
		at->precedence = ATR_PREC_ADD;
	}

	at->aname = stcopy(l_name);
	at->avtype = stcopy(l_type);
	if(strcmp(l_type,"ASCII") == 0) 
	    at->value.ascii = stcopy(l_value);
	else if(strcmp(l_type,"LINK") == 0) {
	    char		ftype[MAX_DIR_LINESIZE];
	    char		lname[MAX_DIR_LINESIZE];
	    char		htype[MAX_DIR_LINESIZE];
	    char		host[MAX_DIR_LINESIZE];
	    char		ntype[MAX_DIR_LINESIZE];
	    char		fname[MAX_DIR_LINESIZE];
	    VLINK		al;

	    al = vlalloc();
	    at->value.link = al;

	    tmp = sscanf(l_value,"%c %s %s %s %s %s %s %d %d",
			 &(al->linktype),
			 ftype,lname,htype,host,ntype,fname,
			 &(al->version),
			 &(al->f_magic_no));
	    if(tmp == 9) {
		al->type = stcopyr(ftype,al->type);
		al->name = stcopyr(unquote(lname),al->name);
		al->hosttype = stcopyr(htype,al->hosttype);
		al->host = stcopyr(host,al->host);
		al->nametype = stcopyr(ntype,al->nametype);
		al->filename = stcopyr(fname,al->filename);
	    }
	    else {
		perrno = DIRSRV_BAD_FORMAT;
		return(NULL);
	    }
	    
	}

	return(at);
    }

/*
 * nxtline - Find the next line in the string
 *
 *	      NXTLINE takes a string and returns a pointer to
 *	      the character immediately following the next newline.
 *
 *    ARGS:   s - string to be searched
 *
 * RETURNS:   Next line or NULL (0) on failure
 */
char *
nxtline(s)
    char	*s;		/* String to be searched */
 {
	s = index(s,'\n');
	if(s) return(++s);
	else return(NULL);
    }


/*
 * unquote - unquote string if necessary
 *
 *	      UNQUOTE takes a string and unquotes it if it has been quoted.
 *
 *    ARGS:   s - string to be unquoted
 *            
 * RETURNS:   The original string.  If the string has been quoted, then the
 *            result appears in static storage, and must be copied if 
 *            it is to last beyond the next call to quote.
 *
 */
char *
unquote(s)
    char	*s;		/* String to be quoted */
    {
	static char	unquoted[200];
	char		*c = unquoted;

	if(*s != '\'') return(s);

	s++;

	/* This should really treat a quote followed by other */
	/* than a quote or a null as an error                 */
	while(*s) {
	    if(*s == '\'') s++;
	    if(*s) *c++ = *s++;
	}

	*c++ = '\0';

	return(unquoted);
    }

#if defined(DEBUG) && defined(STRSPN)
/* needed for -D option parsing */
/*
 * strspn - Count initial characters from chrs in s
 *
 *	      STRSPN counts the occurances of chacters from chrs
 *            in the string s preceeding the first occurance of
 *            a character not in s.
 *
 *    ARGS:   s    - string to be checked
 *            chrs - string of characters we are looking for
 *
 * RETURNS:   Count of initial characters from chrs in s
 */
strspn(s,chrs)
    char	*s;    /* String to search                         */
    char	*chrs; /* String of characters we are looking for  */
    {
	char	*cp;   /* Pointer to the current character in chrs */
	int	count; /* Count of characters seen so far          */
	
	count = 0;

	while(*s) {
	    for(cp = chrs;*cp;cp++)
		if(*cp == *s) {
		    s++;
		    count++;
		    goto done;
		}
	    return(count);
	done:
	    ;
	}
	return(count);
    }
#endif
