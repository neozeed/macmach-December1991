/*
 * Copyright (c) 1991 by the University of Washington
 *
 * For copying and distribution information, please see the file
 * <uw-copyright.h>.
 */

#include <uw-copyright.h>

/*
 * Archie client using the Prospero protocol
 *
 * Suggestions and improvements to Clifford Neuman (bcn@isi.edu)
 */

#include <stdio.h>

#include <pfs.h>
#include <rdgram.h>
#include <archie.h>

int		listflag = 0;
int		sortflag = 0;   /* 1 = by date                    */
char		*progname;
#ifdef DEBUG
int			pfs_debug = 0;
#endif
extern int	rdgram_priority;

main(argc,argv)
    int		argc;
    char	*argv[];
    {
	char		*cur_arg;
	char		qtype = '=';    /* Default to exact string match  */
	char		etype = '=';	/* Type if only -e is specified   */
	int		eflag = 0;	/* Exact flag specified		  */
	int		max_hits = MAX_HITS;
	int		offset = 0;
	int		versionflag = 0;/* Display release identifier     */
	int		tmp;
	char		*host = ARCHIE_HOST;

	int		sort_ins_link(), display_link();

	progname = *argv;
	argc--;argv++;

	while (argc > 0 && **argv == '-') {
	    cur_arg = argv[0]+1;

	    /* If a - by itself, or --, then no more arguments */
	    if(!*cur_arg || ((*cur_arg == '-') && (!*(cur_arg+1)))) {
	        argc--, argv++;
		goto scandone;
	    }

	    while (*cur_arg) {
		switch (*cur_arg++) {
#ifdef DEBUG		
		case 'D':  /* Debug level */
		    pfs_debug = 1; /* Default debug level */
		    if(*cur_arg && index("0123456789",*cur_arg)) {
			sscanf(cur_arg,"%d",&pfs_debug);
			cur_arg += strspn(cur_arg,"0123456789");
		    }
		    else if(argc > 2) {
		        tmp = sscanf(argv[1],"%d",&pfs_debug);
			if (tmp == 1) {argc--;argv++;}
		    }
		    break;
#endif
		case 'N':  /* Priority (nice) */
		    rdgram_priority = RDGRAM_MAX_PRI; /* Use this if no # */
		    if(*cur_arg && index("-0123456789",*cur_arg)) {
			sscanf(cur_arg,"%d",&rdgram_priority);
			cur_arg += strspn(cur_arg,"-0123456789");
		    }
		    else if(argc > 2) {
		        tmp = sscanf(argv[1],"%d",&rdgram_priority);
			if (tmp == 1) {argc--;argv++;}
		    }
		    if(rdgram_priority > RDGRAM_MAX_SPRI) 
			rdgram_priority = RDGRAM_MAX_PRI;
		    if(rdgram_priority < RDGRAM_MIN_PRI) 
			rdgram_priority = RDGRAM_MIN_PRI;
  		    break;

		case 'c':  /* substring (case sensitive) */
		    qtype = 'C';
		    etype = 'c';
		    break;

		case 'e':  /* Exact match */
		    /* If -e specified by itself, then we use the  */
		    /* default value of etype which must be '='    */
		    eflag++;
		    break;

		case 'h':  /* Host */
		    host = argv[1];
		    argc--;argv++;
		    break;

		case 'l':  /* List one match per line */
		    listflag++;
		    break;

		case 'm':  /* Max hits */
		    max_hits = -1;  
		    if(*cur_arg && index("0123456789",*cur_arg)) {
			sscanf(cur_arg,"%d",&max_hits);
			cur_arg += strspn(cur_arg,"0123456789");
		    }
		    else if(argc > 1) {
		        tmp = sscanf(argv[1],"%d",&max_hits);
			if (tmp == 1) {argc--;argv++;}
		    }
		    if (max_hits < 1) {
			fprintf(stderr, "%s: -m option requires a value for max hits (>= 1)\n",
				progname);
			exit(1);
		    }
		    break;

		case 'o':  /* Offset */
		    if(argc > 1) {
		      tmp = sscanf(argv[1],"%d",&offset);
		      if (tmp != 1)
			argc = -1;
		      else {
			argc--;argv++;
		      }
		    }
		    break;

		case 'r':  /* Regular expression search */
		    qtype = 'R';
		    etype = 'r';
		    break;

		case 's':  /* substring (case insensitive) */
		    qtype = 'S';
		    etype = 's';
		    break;

		case 't':  /* Sort inverted by date */
		    sortflag = 1;
		    break;

		case 'v':  /* Display version */
		    fprintf(stderr,
			"Client version %s based upon Prospero version %s\n",
			    CLIENT_VERSION, PFS_RELEASE);
		    versionflag++;
		    break;

		default:
		    fprintf(stderr,"Usage: %s [-[cers][l][t][m #][h host][N#]] string\n", progname);
		    exit(1);
		}
	    }
	    argc--, argv++;
	}

      scandone:

	if (eflag) qtype = etype;

	if ((argc != 1) && versionflag) exit(0);

	if (argc != 1) {
	    fprintf(stderr, "Usage: %s [-[cers][l][t][m #][h host][N#]] string\n", progname);
	    fprintf(stderr,"       -c : case sensitive substring search\n");
	    fprintf(stderr,"       -e : exact string match (default)\n");
	    fprintf(stderr,"       -r : regular expression search\n");
	    fprintf(stderr,"       -s : case insensitive substring search\n");
	    fprintf(stderr,"       -l : list one match per line\n");
	    fprintf(stderr,"       -t : sort inverted by date\n");
	    fprintf(stderr,"     -m # : specifies maximum number of hits to return (default %d)\n", max_hits);
	    fprintf(stderr,"  -h host : specifies server host\n");
	    fprintf(stderr,"      -N# : specifies query niceness level (0-35765)\n");
	    exit(1);
	}

	procquery(host, argv[0], max_hits, offset, qtype, sortflag, listflag);

	exit(0);
    }
