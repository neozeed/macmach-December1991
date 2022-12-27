/*
 * archie.h : Definitions for the programmatic Prospero interface to Archie
 *
 *     Written by Brendan Kehoe (brendan@cs.widener.edu), 
 *                George Ferguson (ferguson@cs.rochester.edu), and
 *                Clifford Neuman (bcn@isi.edu).
 *
 */

/*
 * Archie server (one of):   archie.mcgill.ca
 *                           archie.funet.fi
 *                           archie.au
 */
#define ARCHIE_HOST "ARCHIE.MCGILL.CA"

/*
 * Default value for max hits.  Note that this is normally different
 * for different client implementations.  Doing so makes it easier to
 * collect statistics on the use of the various clients.
 */
#define	MAX_HITS	95

/*
 * CLIENT_VERSION may be used to identify the version of the client if 
 * distributed separately from the Prospero distribution.  The version
 * command should then identify both the client version and the Prospero
 * version identifiers.   
 */
#define CLIENT_VERSION	"1.1"

/* Procedures from user/aquery.c */

/* archie_query(host,string,max_hits,offset,query_type,cmp_proc,flags) */
extern VLINK archie_query(); 

/* defcmplink(p,q) and invdatecmplink(p,q)                             */
extern int defcmplink();	/* Compare by host then by filename    */
extern int invdatecmplink();	/* Compare links inverted by date      */

/* Definitions for the comparison procedures                           */
#define AQ_DEFCMP	defcmplink
#define AQ_INVDATECMP	invdatecmplink

/* Flags                                                               */
#define AQ_NOSORT	0x01	/* Don't sort                          */
#define AQ_NOTRANS	0x02	/* Don't translate Archie responses    */
