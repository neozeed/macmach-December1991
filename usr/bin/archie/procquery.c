/*
 * procquery.c : Routines for processing results from Archie
 *
 * Originally part of the Prospero Archie client by Cliff Neuman (bcn@isi.edu).
 * Modified by Brendan Kehoe (brendan@cs.widener.edu).
 * Re-modified by George Ferguson (ferguson@cs.rochester.edu).
 *
 * Copyright (c) 1991 by the University of Washington
 *
 * For copying and distribution information, please see the file
 * <uw-copyright.h>.
 *
 * v1.1.1 - bpk 08/20/91 - took out archie_query from error msg
 */
#include <uw-copyright.h>
#include <stdio.h>
#include <pfs.h>
#include <perrno.h>
#include <pmachine.h>
#include <archie.h>
#ifdef AUX
# include <time.h>
#else
# include <sys/time.h>
#endif
extern int client_dirsrv_timeout,client_dirsrv_retry;	/* dirsend.c */
extern char *progname;


/*
 * Functions defined here
 */
void display_link(), procquery();

/*
 * Data defined here
 */
int perrno, pfs_debug;
static struct tm *presenttime;

/*	-	-	-	-	-	-	-	-	*/
/*
 * display_link : Prints the contents of the given virtual link. If
 *	listflag is 0, then this uses static variables to save state
 *	between calls for a less verbose output. If listflag is non-zero
 *	then all information is printed every time.
 */
void
display_link(l,listflag)
VLINK l;
int listflag;
{
    static char	lastpath[MAX_VPATH] = "\001";
    static char	lasthost[MAX_VPATH] = "\001";

    PATTRIB 	ap;
    char	linkpath[MAX_VPATH];
    int		dirflag = 0;
    int		size = 0;
    char	*modes = "";
    char	archie_date[20];
    char	*gt_date = "";
    int		gt_year = 0;
    int		gt_mon = 0;
    int		gt_day = 0;
    int		gt_hour = 0;
    int		gt_min = 0;
    
    /* Initialize local buffers */
    *archie_date = '\0';

    /* Remember if we're looking at a directory */
    if (sindex(l->type,"DIRECTORY"))
	dirflag = 1;
    else
	dirflag = 0;
    
    /* Extract the linkpath from the filename */
    strcpy(linkpath,l->filename);
    *(linkpath + (strlen(linkpath) - strlen(l->name) - 1)) = '\0';
    
    /* Is this a new host? */
    if (strcmp(l->host,lasthost) != 0) {
	if (!listflag)
	    printf("\nHost %s\n\n",l->host);
	strcpy(lasthost,l->host);
	*lastpath = '\001';
    }
    
    /* Is this a new linkpath (location)? */
    if(strcmp(linkpath,lastpath) != 0) {
	if (!listflag)
	    printf("    Location: %s\n",(*linkpath ? linkpath : "/"));
	strcpy(lastpath,linkpath);
    }
    
    /* Parse the attibutes of this link */
    for (ap = l->lattrib; ap; ap = ap->next) {
	if (strcmp(ap->aname,"SIZE") == 0) {
	    sscanf(ap->value.ascii,"%d",&size);
	} else if(strcmp(ap->aname,"UNIX-MODES") == 0) {
	    modes = ap->value.ascii;
	} else if(strcmp(ap->aname,"LAST-MODIFIED") == 0) {
	    gt_date = ap->value.ascii;
	    sscanf(gt_date,"%4d%2d%2d%2d%2d",&gt_year,
		   &gt_mon, &gt_day, &gt_hour, &gt_min);
	    if ((12 * (presenttime->tm_year + 1900 - gt_year) + 
					presenttime->tm_mon - gt_mon) > 6) 
		sprintf(archie_date,"%s %2d %4d",month_sname(gt_mon),
			gt_day, gt_year);
	    else
		sprintf(archie_date,"%s %2d %02d:%02d",month_sname(gt_mon),
			 gt_day, gt_hour, gt_min);
	}
    }
    
    /* Print this link's information */
    if (listflag)
	printf("%s %6d %s %s%s\n",gt_date,size,l->host,l->filename,
							(dirflag ? "/" : ""));
    else
	printf("      %9s %s %10d  %s  %s\n",(dirflag ? "DIRECTORY" : "FILE"),
					modes,size,archie_date,l->name);

    /* Free the attibutes */
    atlfree(l->lattrib);
    l->lattrib = NULL;
}

/*	-	-	-	-	-	-	-	-	*/
/*
 * procquery : Process the given query and display the results. If
 *	sortflag is non-zero, then the results are sorted by increasing
 *	date, else by host/filename. If listflag is non-zero then each
 *	entry is printed on a separate, complete line. Note that listflag
 *	is ignored by xarchie.
 */
void
procquery(host,str,max_hits,offset,query_type,sortflag,listflag)
char *host,*str;
int max_hits,offset;
char query_type;
int sortflag,listflag;
{
    VLINK l;
    long now;
    extern int rdgram_priority;

    /* initialize data structures for this query */
    (void)time(&now);
    presenttime = localtime(&now);

    /* Do the query */
    if (sortflag == 1)
	l = archie_query(host,str,max_hits,offset,query_type,AQ_INVDATECMP,0);
    else
	l = archie_query(host,str,max_hits,offset,query_type,NULL,0);

    /* Error? */
    if (perrno != PSUCCESS) {
	if (*p_err_string)
	  fprintf(stderr, "%s: failed: %s - %s\n", progname,
		  p_err_text[perrno], p_err_string);
	else
	  fprintf(stderr, "%s failed: %s\n", progname, p_err_text[perrno]);
    }

    /* Display the results */
    while (l != NULL) {
	display_link(l,listflag);
	l = l->next;
    }
}
