
/*
 * $XConsortium: Xrm.c,v 1.32 89/12/11 19:10:50 rws Exp $
 */

/***********************************************************
Copyright 1987, 1988 by Digital Equipment Corporation, Maynard, Massachusetts,
and the Massachusetts Institute of Technology, Cambridge, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the names of Digital or MIT not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

#include	<X11/Xlib.h>
#include	"Xlibint.h"
#include	<X11/Xresource.h>
#include	<stdio.h>
#include	<ctype.h>
#ifdef SYSV
#include	<X11/Xos.h>
#endif /* SYSV */


/*

These Xrm routines allow very fast lookup of resources in the resource
database.  Several usage patterns are exploited:

(1) Widgets get a lot of resources at one time.  Rather than look up each from
scratch, we can precompute the prioritized list of database levels once, then
search for each resource starting at the beginning of the list.

(2) Many database levels don't contain any leaf resource nodes.  There is no
point in looking for resources on a level that doesn't contain any.  This
information is kept on a per-level basis...if even just a single hash bucket
contains a value, that level is searched.  I considered having 4 arrays coming
off each database level, split by tight/loose and intermediate/leaf, but this
looked like it would burn a lot of memory for negligible performance gains.

(3) Sometimes the widget instance tree is structured such that you get the same
class name repeated on the fully qualified widget name.  This can result in the
same database level occuring multiple times on the search list.  The code below
only checks to see if you get two identical search lists in a row, rather than
look back through all database levels, but in practice this removes all
duplicates I've ever observed.

Joel McCormack

*/



extern void bzero();

XrmQuark    XrmQString;

typedef	Bool (*DBEnumProc)();

#define HASHSIZE	64
#define HASHMASK	63
#define HashIndex(quark)	(quark & HASHMASK)

/*
typedef struct _XrmHashBucketRec	*XrmHashBucket;
*/
typedef struct _XrmHashBucketRec {
    XrmHashBucket	next;       /* Next entry in this hash chain	    */
    XrmQuark		quark;      /* Quark for string			    */
    XrmRepresentation   type;       /* Representation of value (if any)     */
    XrmValue		value;      /* Value of this node (if any)	    */
    XrmHashTable	tables[2];  /* Hash table pointers for tight, loose */
    char		hasValues[2]; /* Do any buckets in table have value */
} XrmHashBucketRec;

/*
typedef XrmHashBucket	*XrmHashTable;
*/

/*
typedef XrmHashTable XrmSearchList[];
*/

/*
   resourceQuarks keeps track of what quarks are associated with actual
   resources in the database.  If a quark isn't used as a resource name,
   we don't need to bother looking for it.
*/

static char    *resourceQuarks = NULL;
static int     maxResourceQuark = -1;

#define IsResourceQuark(q)  ((q) <= maxResourceQuark && resourceQuarks[q])


#if NeedFunctionPrototypes
void XrmStringToQuarkList(
    register const char  *name,
    register XrmQuarkList quarks)   /* RETURN */
#else
void XrmStringToQuarkList(name, quarks)
    register char 	 *name;
    register XrmQuarkList quarks;   /* RETURN */
#endif
{
    register int    i;
    register char   ch;
    char	    oneName[1000];

    if (name != NULL) {
	for (i = 0 ; ((ch = *name) != '\0') ; name++) {
	    if (ch == '.') {
		if (i != 0) {
		    oneName[i] = 0;
		    *quarks = XrmStringToQuark(oneName);
		    quarks++;
		    i = 0;
		}
	    } else {
		oneName[i] = ch;
	    	i++;
	    }
	}
	if (i != 0) {
	    oneName[i] = 0;
	    *quarks = XrmStringToQuark(oneName);
	    quarks++;
	}
    }
    *quarks = NULLQUARK;
}

#if NeedFunctionPrototypes
void XrmStringToBindingQuarkList(
    register const char	   *name,
    register XrmBindingList bindings,   /* RETURN */
    register XrmQuarkList   quarks)     /* RETURN */
#else
void XrmStringToBindingQuarkList(name, bindings, quarks)
    register char	    *name;
    register XrmBindingList bindings;   /* RETURN */
    register XrmQuarkList   quarks;     /* RETURN */
#endif
{
    register int	i;
    register XrmBinding binding;
    register char       ch;
    char		oneName[1000];

    if (name != NULL) {
	binding = XrmBindTightly;
	for (i = 0 ; ((ch = *name) != '\0') ; name++) {
	    if (ch == '.' || ch == '*') {
		if (i != 0) {
		    /* Found a complete name */
		    *bindings = binding;
		    bindings++;
		    oneName[i] = '\0';
		    *quarks = XrmStringToQuark(oneName);
		    quarks++;
		    i = 0;
		    binding = XrmBindTightly;
		}
		if (ch == '*') binding = XrmBindLoosely;
	    } else {
		oneName[i] = ch;
	    	i++;
	    }
	}
	/* Do last name */
	if (i != 0) {
	    oneName[i] = '\0';
	    *bindings = binding;
	    *quarks = XrmStringToQuark(oneName);
	    quarks++;
	}
    }
    *quarks = NULLQUARK;
} /* XrmStringToBindingQuarkList */


static void PutEntry(bucket, bindings, quarks, type, value)
    register XrmHashBucket	bucket;
	     XrmBindingList	bindings;
    register XrmQuarkList	quarks;
	     XrmRepresentation  type;
    	     XrmValuePtr	value;
{
    register XrmHashBucket	*pBucket;
    register int		binding;
    register int		quark;
    register XrmHashTable       table;
	     XrmHashBucket      parent;

    parent = NULL;
    for (; (quark = *quarks) != NULLQUARK; quarks++, bindings++) {
	binding = (int) *bindings;

	/* Remember parent for marking later */
	parent = bucket;

	/* Allocate new hash table if needed */
	table = bucket->tables[binding];
	if (table == NULL) {
	    if ((table = bucket->tables[binding] =
		 (XrmHashTable) Xmalloc(sizeof(XrmHashBucket) * HASHSIZE))
		 == NULL)
		return;
	    bzero((char *) table, sizeof(XrmHashBucket) * HASHSIZE);
	}

	/* Find bucket containing quark if possible */
	pBucket = &(table[HashIndex(quark)]);
	bucket = *pBucket;
	while ((bucket != NULL) && (bucket->quark != quark)) {
	    bucket = bucket->next;
	}

	/* Create new bucket if needed */
	if (bucket == NULL) {
	    if ((bucket = (XrmHashBucket) Xmalloc(sizeof(XrmHashBucketRec)))
		 == NULL)
		return;
	    bzero((char *) bucket, sizeof(XrmHashBucketRec));
	    bucket->next = *pBucket;
	    *pBucket = bucket;
	    bucket->quark = quark;
	}
    } /* for */

    /* Mark parent database as having a child with a value, and 
       update resourceQuarks */
    if (parent != NULL) {
	parent->hasValues[(int) binding] = True;
	quark = quarks[-1];
	if (quark > maxResourceQuark) {
	    if (resourceQuarks) 
		resourceQuarks = Xrealloc(resourceQuarks, quark+1);
	    else resourceQuarks = Xmalloc(quark+1);
	    if (!resourceQuarks)
		return;
	    if (quark-maxResourceQuark-1)
		bzero(&resourceQuarks[maxResourceQuark+1],
		      quark-maxResourceQuark-1);
	    maxResourceQuark = quark;
	}
	resourceQuarks[quark] = True;
    }
    /* Set value passed in */
    if (bucket->value.addr != NULL) {
	Xfree((char *) bucket->value.addr);
    }
    bucket->type = type;
    bucket->value.size = value->size;
    if ((bucket->value.addr = (caddr_t) Xmalloc(value->size)) == NULL)
	return;
    bcopy((char *) value->addr, (char *) bucket->value.addr, (int) value->size);
} /* PutEntry */


static Bool GetEntry(tight, loose, names, classes, type, value)
	     XrmHashTable	tight;
    register XrmHashTable	loose;
    register XrmNameList	names;
    register XrmClassList	classes;
    	     XrmRepresentation  *type;  /* RETURN */
    	     XrmValuePtr	value;  /* RETURN */
{
    register XrmHashBucket	bucket;
    register XrmName		name;
    register XrmClass		class;
	     XrmHashTable       nTight, nLoose;

    /* (tight != NULL || loose != NULL) && names[0] != NULLQUARK */

#define GetEntryLookup(table, q) 					    \
{									    \
    bucket = table[HashIndex(q)];					    \
    while (bucket != NULL) {						    \
	if (bucket->quark == q) {					    \
	    if (names[1] == NULLQUARK) {				    \
		/* Must be leaf node with data, else doesn't match */       \
		if (bucket->value.addr == NULL) {			    \
		    return False;					    \
		} else {						    \
		    *type = bucket->type;				    \
		    *value = bucket->value;				    \
		    return True;					    \
		}							    \
	    } else {							    \
		nTight = bucket->tables[(int) XrmBindTightly];		    \
		nLoose = bucket->tables[(int) XrmBindLoosely];		    \
		if ((nTight != NULL || nLoose != NULL)			    \
		    && GetEntry(nTight, nLoose, names+1, classes+1,	    \
			type, value)) {					    \
		return True;						    \
		}							    \
	    break;							    \
	    }								    \
	}								    \
	bucket = bucket->next;						    \
    }									    \
} /* GetEntryLookup */

    /* Check very first name & class in both tight and loose tables */
    name = *names;
    if (tight != NULL) GetEntryLookup(tight, name);
    if (loose != NULL) GetEntryLookup(loose, name);
    class = *classes;
    if (tight != NULL) GetEntryLookup(tight, class);
    if (loose != NULL) GetEntryLookup(loose, class);

    /* Now check any remaining names and class, but just in loose table */
    if (loose != NULL) {
	names++;
	classes++;
	for (;(name = *names) != NULLQUARK; names++, classes++) {
	    GetEntryLookup(loose, name);
	    class = *classes;
	    GetEntryLookup(loose, class);
	}
    }

    /* Didn't find any of the names or classes in either hash table */
    return False;
} /* GetEntry */

static int numTables;
static int lenTables;

static void GetTables(tight, loose, names, classes, tables)
             XrmHashTable   tight;
    register XrmHashTable   loose;
    register XrmNameList    names;
    register XrmClassList   classes;
	     XrmSearchList  tables;     /* RETURN */
{
    register XrmName	    name;
    register XrmClass       class;
    register XrmHashBucket  bucket;
             XrmHashTable   nTight, nLoose;
	
#define GetTablesLookup(table, q)					    \
{									    \
    bucket = table[HashIndex(q)];					    \
    while (bucket != NULL) {						    \
	if (bucket->quark == q) {					    \
	    nTight = bucket->tables[(int) XrmBindTightly];		    \
	    nLoose = bucket->tables[(int) XrmBindLoosely];		    \
	    if (nTight != NULL || nLoose != NULL) {			    \
		if (names[1] != NULLQUARK) {				    \
		    GetTables(nTight, nLoose, names+1, classes+1, tables);  \
		} else if (nTight != NULL				    \
			&& bucket->hasValues[(int) XrmBindTightly]	    \
			/* Quicky test for recurring names/classes */       \
			&& (numTables == 0 || tables[numTables-1] != nTight)) {\
		    if (numTables == lenTables) return;			    \
		    tables[numTables++] = nTight;			    \
		}							    \
		if (nLoose != NULL					    \
			&& bucket->hasValues[(int) XrmBindLoosely]	    \
			/* Quicky test for recurring names/classes */	    \
			&& (numTables == 0 || tables[numTables-1] != nLoose)) {\
		    if (numTables == lenTables) return;			    \
		    tables[numTables++] = nLoose;			    \
		}							    \
	    }								    \
	    break;							    \
	}								    \
	bucket = bucket->next;						    \
    }									    \
} /* GetTablesLookup */

    /* Check first name and class in both tight and loose tables */
    name = *names;
    if (tight != NULL) GetTablesLookup(tight, name);
    if (loose != NULL) GetTablesLookup(loose, name);
    class = *classes;
    if (tight != NULL) GetTablesLookup(tight, class);
    if (loose != NULL) GetTablesLookup(loose, class);

    /* Now check any remaining names and class, but just in loose table */
    if (loose != NULL) {
	names++;
	classes++;
	for (; (name = *names) != NULLQUARK; names++, classes++) {
	    GetTablesLookup(loose, name);
	    class = *classes;
	    GetTablesLookup(loose, class);
	}
    }
} /* GetTables */

static XrmDatabase NewDatabase()
{
    register XrmHashBucket   bucket;

    if ((bucket = (XrmHashBucket) Xmalloc(sizeof(XrmHashBucketRec))) == NULL)
	return NULL;
    bucket->next = NULL;
    bucket->quark = NULLQUARK;
    bucket->type = NULLQUARK;
    bucket->value.addr = NULL;
    bucket->value.size = 0;
    bucket->tables[(int) XrmBindTightly] = NULL;
    bucket->tables[(int) XrmBindLoosely] = NULL;
    bucket->hasValues[(int) XrmBindTightly] = False;
    bucket->hasValues[(int) XrmBindLoosely] = False;
    return(bucket);
} /* NewDatabase */

static char *getstring(buf, nchars, dp)
	char *buf;
	register int nchars;
	caddr_t dp;		/* is char** source */
{
	register char *src = *(char**)dp;
	register char *dst = buf;
	register char c;

	if (src == NULL) return NULL;
	if (*src == '\0') return NULL;
	while (--nchars > 0) {
		*dst++ = c = *src++;
		if (c == '\n') {
			*(char**)dp = src;
			return (buf);
		}
		if (c == '\0') {
			*(char**)dp = src-1;
			return (buf);
		}
	}
	*dst = '\0';
	*(char**)dp = src;
	return buf;
}

static Bool Enum(db, bindings, quarks, count, proc, closure)
    XrmHashBucket   db;
    XrmBindingList  bindings;
    XrmQuarkList    quarks;
    unsigned	    count;
    DBEnumProc      proc;
    caddr_t	    closure;
{
    register int	    i;
    register XrmHashBucket  bucket;
    register XrmHashTable   table;

#define EnumTable(binding)						    \
{									    \
    table = db->tables[(int) binding];					    \
    if (table != NULL) {						    \
	bindings[count] = binding;					    \
	quarks[count+1] = NULLQUARK;					    \
	for (i=0; i < HASHSIZE; i++) {					    \
	    bucket = table[i];						    \
	    while (bucket != NULL) {					    \
		quarks[count] = bucket->quark;				    \
		if (Enum(bucket, bindings, quarks, count+1, proc, closure)) \
		   return True;						    \
		bucket = bucket->next;					    \
	    }								    \
	}								    \
    }									    \
} /* EnumTable */

    if (db == NULL) return False;
    EnumTable(XrmBindTightly);
    EnumTable(XrmBindLoosely);

    quarks[count] = NULLQUARK;
    if (db->value.addr != NULL) {
	if ((*proc)(bindings, quarks, db->type, &(db->value), closure))
	    return True;
    }
    return False;
}

static Bool EnumerateDatabase(db, proc, closure)
    XrmHashBucket   db;
    caddr_t     closure;
    DBEnumProc      proc;
{
    XrmBinding  bindings[100];
    XrmQuark	quarks[100];
   
    return Enum(db, bindings, quarks, (unsigned)0, proc, closure);
}

static void PrintBindingQuarkList(bindings, quarks, stream)
    XrmBindingList      bindings;
    XrmQuarkList	quarks;
    FILE		*stream;
{
    Bool	firstNameSeen;

    for (firstNameSeen = False; (*quarks) != NULLQUARK; bindings++, quarks++) {
	if (*bindings == XrmBindLoosely) {
	    (void) fprintf(stream, "*");
	} else if (firstNameSeen) {
	    (void) fprintf(stream, ".");
	}
	firstNameSeen = True;
	(void) fputs(XrmQuarkToString(*quarks), stream);
    }
}

static Bool DumpEntry(bindings, quarks, type, value, stream)
    XrmBindingList      bindings;
    XrmQuarkList	quarks;
    XrmRepresentation   type;
    XrmValuePtr		value;
    FILE		*stream;
{

    register unsigned int	i;

    PrintBindingQuarkList(bindings, quarks, stream);
    if (type == XrmQString) {
	if (index(value->addr, '\n') == NULL)
	    (void) fprintf(stream, ":\t%s\n", value->addr);
	else {
	    register char *s1, *s2;
	    (void) fprintf(stream, ":\t\\\n");
	    s1 = value->addr;
	    while ((s2 = index(s1, '\n')) != NULL) {
		*s2 = '\0';
		if (s2[1] == '\0') {
		    (void) fprintf(stream, "%s\\n\n", s1);
		} else {
		    (void) fprintf(stream, "%s\\n\\\n", s1);
		}
		*s2 = '\n';
		s1 = s2 + 1;
	    }		
	    (void) fprintf(stream, "%s\\n\n", s1);
	}
    } else {
	(void) fprintf(stream, "!%s:\t", XrmRepresentationToString(type));
	for (i = 0; i < value->size; i++)
	    (void) fprintf(stream, "%02x", (int) value->addr[i]);
        if (index(value->addr, '\n')) {
           (void) fprintf(stream, ":\t\\\n");
           for (i = 0; value->addr[i]; i++) {
               if (value->addr[i] == '\n') {
                   (void) fprintf(stream, "\\n");
                   if (value->addr[i+1]) (void) fprintf(stream, "\\");
                   (void) fprintf(stream, "\n");
               } else {
                   (void) putc(value->addr[i], stream);
               }
           }
        } else {
           (void) fprintf(stream, ":\t%s\n", value->addr);
        }
     }
    return False;
}

static void Merge(new, old)
    XrmHashBucket   new, old;
{
    register XrmHashTable   newTable, oldTable;
    XrmHashBucket	    oldBucket;
    register XrmHashBucket  newBucket, nextNewBucket, oldSearchBucket;
    int			    binding;
    register int	    i;

    /* Merge data in new into old, and destroy new in the process */
    /* new # NULL && old # NULL */
    
    /* Merge new value into old value */
    if (new->value.addr != NULL) {
	if (old->value.addr != NULL) {
	    Xfree(old->value.addr);
	}
	old->type = new->type;
	old->value = new->value;
    } 

    /* Merge new hash tables into old hash tables */
    for (binding = (int) XrmBindTightly;
         binding <= (int) XrmBindLoosely;
	 binding++) {

	/* Merge hasValues information */
	if (new->hasValues[binding]) {
	    old->hasValues[binding] = True;
	}

	oldTable = old->tables[binding];
	newTable = new->tables[binding];
	if (oldTable == NULL) {
	    old->tables[(int) binding] = newTable;
	} else if (newTable != NULL) {
	    /* Copy each bucket over individually */
	    for (i = 0; i < HASHSIZE; i++) {
		oldBucket = oldTable[i];
		newBucket = newTable[i];
		/* Find each item in newBucket list in the oldBucket list */
		while (newBucket != NULL) {
		    nextNewBucket = newBucket->next;
		    oldSearchBucket = oldBucket;
		    while (   oldSearchBucket != NULL
			   && oldSearchBucket->quark != newBucket->quark) {
			oldSearchBucket = oldSearchBucket->next;
		    }
		    if (oldSearchBucket == NULL) {
			/* Just stick newBucket at head of old bucket list */
			newBucket->next = oldTable[i];
			oldTable[i] = newBucket;
		    } else {
			/* Merge the two */
			Merge(newBucket, oldSearchBucket);
		    }
		    newBucket = nextNewBucket;
		} /* while newBucket != NULL */
	    } /* for i */
	    Xfree(newTable);
	} /* if */
    } /* for binding */
    Xfree(new);
} /* Merge */


#ifdef DEBUG

static void PrintQuarkList(quarks, stream)
    XrmQuarkList    quarks;
    FILE	    *stream;
{
    Bool	    firstNameSeen;

    for (firstNameSeen = False; (*quarks) != NULLQUARK; quarks++) {
	if (firstNameSeen) {
	    (void) fprintf(stream, ".");
	}
	firstNameSeen = True;
	(void) fputs(XrmQuarkToString(*quarks), stream);
    }
} /* PrintQuarkList */


static void DumpSearchList(names, classes, searchList)
    XrmNameList	    names;
    XrmClassList    classes;
    XrmSearchList   searchList;
{
    register XrmHashTable table;
    register int    i, j, k;
    register XrmDatabase  bucket;
	     char   *str;

    fprintf(stdout,
	"\n\n\n===============================\n Name list : ");
    PrintQuarkList(names, stdout);
    fprintf(stdout, "\n Class list: ");
    PrintQuarkList(classes, stdout);
    fprintf(stdout, "\n");

    for (i=0 ; (table = searchList[i]) != NULL; i++) {
	fprintf(stdout, "\n--------------%d---------------\n", i);
	for (j=0; j < HASHSIZE; j++) {
	    bucket = table[j];
	    while (bucket != NULL) {
		fprintf(stdout, "%s: ", XrmQuarkToString(bucket->quark));
		if (bucket->value.addr == NULL) {
		    fprintf(stdout, "<<<NOTHING>>>\n");
		} else if (bucket->type == XrmQString) {
		    str = bucket->value.addr;
		    for (k = 0; str[k] != '\0' && str[k] != '\n'; k++) {};
		    fprintf(stdout, "'");
		    fwrite(str, 1, k, stdout);
		    fprintf(stdout, "'");
		    if (str[k] == '\n' && str[k] != '\0') {
			fprintf(stdout, "...");
		    }
		    fprintf(stdout, "\n");
		} else {
		    fprintf(stdout, "<<<Binary data>>>\n");
		}
	    bucket = bucket->next;
	    }
	}
    }
} /* DumpSearchList */;
#endif /* DEBUG */


Bool XrmQGetSearchList(db, names, classes, searchList, listLength)
    XrmHashBucket   db;
    XrmNameList	    names;
    XrmClassList    classes;
    XrmSearchList   searchList;	/* RETURN */
    int		    listLength;
{
    XrmHashTable    nTight, nLoose;

    numTables = 0;
    lenTables = listLength;
    if (db != NULL) {
	nTight = db->tables[(int) XrmBindTightly];
	nLoose = db->tables[(int) XrmBindLoosely];
	if (nTight != NULL || nLoose != NULL) {
	    if (*names != NULLQUARK) {
		GetTables(nTight, nLoose, names, classes, searchList);
	    } else if (nTight != NULL && db->hasValues[(int) XrmBindTightly]) {
		if (numTables == lenTables) return False;
		searchList[numTables++] = nTight;
	    }
	    if (db->hasValues[(int) XrmBindLoosely]) {
		if (numTables == lenTables) return False;
		searchList[numTables++] = nLoose;
	    }
	}
    }
    if (numTables == lenTables) return False;
    searchList[numTables] = NULL;
    return True;
/*    DumpSearchList(names, classes, searchList); */
} /* XrmGetSearchList */

Bool XrmQGetSearchResource(searchList, name, class, pType, pVal)
	     XrmSearchList	searchList;
    register XrmName		name;
    register XrmClass		class;
    	     XrmRepresentation	*pType; /* RETURN */
    	     XrmValue		*pVal;  /* RETURN */
{
    register XrmHashBucket	bucket;

#define SearchTable(q, hash)						    \
{									    \
    bucket = (*searchList)[hash];					    \
    while (bucket != NULL) {						    \
	if (bucket->quark == q) {					    \
	    if (bucket->value.addr != NULL) {				    \
		/* Leaf node, it really matches */			    \
		(*pType) = bucket->type;				    \
		(*pVal) = bucket->value;				    \
		return True;						    \
	    }								    \
	    break;							    \
	}								    \
	bucket = bucket->next;						    \
    }									    \
} /* SearchTable */

    /* My numbers show an average of 0.3 searches per call.  Compare this to
       the maximal bound of 2 searches per call.  Further, the searchList
       tends to be short--often 1 or 2 tables, generally not more than 4.
       So this routine optimizes for the common cases.  Code is minimized
       if there is nothing to do, and only three registers are used to
       avoid saving/restoring. */
       
    if (!IsResourceQuark(name) && !IsResourceQuark(class)) {
	/* Most common case */
	(*pType) = NULLQUARK;
	(*pVal).addr = NULL;
	(*pVal).size = 0;
	return False;
    } else if IsResourceQuark(name) {
	int nameHash  = HashIndex(name);
	if IsResourceQuark(class) {
	    /* Must search for either one */
	    int classHash = HashIndex(class);
	    for (; (*searchList) != NULL; searchList++) {
		SearchTable(name, nameHash);
		SearchTable(class, classHash);
	    }
	} else {
	    /* Just search for name */
	    for (; (*searchList) != NULL; searchList++) {
		SearchTable(name, nameHash);
	    }
	}
    } else {
	/* Just search for class */
	int classHash = HashIndex(class);
	for (; (*searchList) != NULL; searchList++) {
	    SearchTable(class, classHash);
	}
    }
    (*pType) = NULLQUARK;
    (*pVal).addr = NULL;
    (*pVal).size = 0;
    return False;
} /* XrmQGetSearchResource */


void XrmQPutResource(pdb, bindings, quarks, type, value)
    XrmDatabase		*pdb;
    XrmBindingList      bindings;
    XrmQuarkList	quarks;
    XrmRepresentation	type;
    XrmValuePtr		value;
{
    if (*pdb == NULL) *pdb = NewDatabase();
    PutEntry(*pdb, bindings, quarks, type, value);
} /* XrmQPutResource */

#if NeedFunctionPrototypes
void XrmPutResource(
    XrmDatabase     *pdb,
    const char	    *specifier,
    const char	    *type,
    XrmValuePtr	    value)
#else
void XrmPutResource(pdb, specifier, type, value)
    XrmDatabase     *pdb;
    char	    *specifier;
    char	    *type;
    XrmValuePtr	    value;
#endif
{
    XrmBinding	    bindings[100];
    XrmQuark	    quarks[100];

    if (*pdb == NULL) *pdb = NewDatabase();
    XrmStringToBindingQuarkList(specifier, bindings, quarks);
    PutEntry(*pdb, bindings, quarks, XrmStringToQuark(type), value);
} /* XrmPutResource */

#if NeedFunctionPrototypes
void XrmQPutStringResource(
    XrmDatabase     *pdb,
    XrmBindingList  bindings,
    XrmQuarkList    quarks,
    const char	    *str)
#else
void XrmQPutStringResource(pdb, bindings, quarks, str)
    XrmDatabase     *pdb;
    XrmBindingList  bindings;
    XrmQuarkList    quarks;
    char	    *str;
#endif
{
    XrmValue	value;

    if (*pdb == NULL) *pdb = NewDatabase();
    value.addr = (caddr_t) str;
    value.size = strlen(str)+1;
    PutEntry(*pdb, bindings, quarks, XrmQString, &value);
} /* XrmQPutStringResource */


static void PutLineResources(pdb, get_line, closure)
    XrmDatabase *pdb;
    char	*((*get_line)(/* buf, count, closure */));
    caddr_t     closure;
{
    register char   *s, *ts, ch;
    char	buf[5000];
    char        *pbuf = buf;
    int		pbuf_size = sizeof(buf);
    Bool	have_entire_value;
    register char   *nameStr, *valStr;
    XrmBinding	    bindings[100];
    XrmQuark	    quarks[100];
    XrmValue	    value;

#define CheckForFullBuffer()						   \
    if (s - pbuf == pbuf_size - 1) { /* if line filled buffer */	   \
	char *obuf = pbuf;						   \
	if (pbuf == buf) {						   \
	    int osize = pbuf_size;					   \
	    pbuf_size *= 2;						   \
	    if ((pbuf = Xmalloc(pbuf_size)) == NULL)			   \
		return;							   \
	    bcopy(buf, pbuf, osize);					   \
	}								   \
	else {								   \
	    pbuf_size *= 2;						   \
	    if ((pbuf = Xrealloc(pbuf, pbuf_size)) == NULL) 		   \
		return;							   \
	}								   \
	s = pbuf + (s - obuf);						   \
	ts = pbuf + (ts - obuf);					   \
	nameStr = pbuf + (nameStr - obuf);				   \
	valStr = pbuf + (valStr - obuf);				   \
	if ((char *)(*get_line)(s, pbuf_size - (s-pbuf), closure) != NULL) \
	    have_entire_value = False;					   \
    }

    if (*pdb == NULL) *pdb = NewDatabase();
    for (;;) {
	s = (char *)(*get_line)(pbuf, pbuf_size, closure);
	if (s == NULL) break;

	/*
	 * Scan to start of resource name/class specification
	 *
	 * Comments are indicated by an exclamation mark in column 0 only.  The
	 * check for sharp sign after optional white space is in violation of 
	 * the spec, but is there for compatibility (for now).
	 */
	if (*s == '!') continue;
	for (; ((ch = *s) != '\n') && isspace(ch); s++) {};
	if ((ch == '\0') || (ch == '\n') || (ch == '#')) continue;
    
	nameStr = valStr = s;
	ts = s - 1;
	do {
	    have_entire_value = True;
	    /* Scan to end of resource name/class specification */
	    for (; ; s++) {
		if ((ch = *s) == '\0' || ch == '\n')
		    break;
		if (ch == ':') {
		    s++;
		    break;
		}
		if (! isspace(ch))
		    ts = s;
	    }
	    CheckForFullBuffer();
	} while (! have_entire_value);
    
	/* Remove trailing white space from name/class */
	ts[1] = '\0';
	
	/* Scan to start of resource value */
	for (; *s != '\n' && isspace(*s); s++) {};
    
	valStr = ts = s;
	do {
	    have_entire_value = True;
	    /* Scan to end of resource value */
	    for (; ((ch = *s) != '\0' && ch != '\n');s++) {
		if (ch == '\\') {
		    if (*++s == 'n') {	    /* \n becomes LF */
			*ts = '\n';
			ts++;
		    } else if (*s == '\n') {  /* \LF means continue next line */
			if ((char *)(*get_line)(s, pbuf_size - (s-pbuf), closure) == NULL)
			    break;
			s--;
		    } else if (isascii(s[0]) && isdigit(s[0]) &&
			       isascii(s[1]) && isdigit(s[1]) &&
			       isascii(s[2]) && isdigit(s[2])) {
			register unsigned char *uts = (unsigned char *) ts;
			*uts = (unsigned char) ((s[0] - '0') * 0100 + 
						(s[1] - '0') * 010 +
						(s[2] - '0'));
			ts++;
			s += 2;
		    } else if (*s != '\0') {
			*ts = *s;
			ts++;
		    } else break;
		} else {
		    *ts = ch;
		    ts++;
		}
	    };
	    CheckForFullBuffer();
	} while (! have_entire_value);
	*ts = '\0';
    
	/* Store it in database */
	value.size = s - valStr + 1;
	value.addr = (caddr_t) valStr;
	
	XrmStringToBindingQuarkList(nameStr, bindings, quarks);
	XrmQPutResource(pdb, bindings, quarks, XrmQString, &value);
    } /* for lines left to process */
    if (pbuf != buf) Xfree(pbuf);
#undef CheckForFullBuffer
} /* PutLineResources */


#if NeedFunctionPrototypes
void XrmPutStringResource(
    XrmDatabase *pdb,
    const char	*specifier,
    const char	*str)
#else
void XrmPutStringResource(pdb, specifier, str)
    XrmDatabase *pdb;
    char	*specifier;
    char	*str;
#endif
{
    XrmValue	value;
    XrmBinding	bindings[100];
    XrmQuark	quarks[100];

    if (*pdb == NULL) *pdb = NewDatabase();
    XrmStringToBindingQuarkList(specifier, bindings, quarks);
    value.addr = (caddr_t) str;
    value.size = strlen(str)+1;
    PutEntry(*pdb, bindings, quarks, XrmQString, &value);
} /* XrmPutStringResource */


#if NeedFunctionPrototypes
void XrmPutLineResource(
    XrmDatabase *pdb,
    const char	*line)
#else
void XrmPutLineResource(pdb, line)
    XrmDatabase *pdb;
    char	*line;
#endif
{
    PutLineResources(pdb, getstring, (caddr_t) &line);
} 

#if NeedFunctionPrototypes
XrmDatabase XrmGetStringDatabase(
    const char	    *data)
#else
XrmDatabase XrmGetStringDatabase(data)
    char	    *data;
#endif
{
    XrmDatabase     db;

    db = NULL;
    PutLineResources(&db, getstring, (caddr_t) &data);
    return db;
}

#if NeedFunctionPrototypes
XrmDatabase XrmGetFileDatabase(
    const char 	    *fileName)
#else
XrmDatabase XrmGetFileDatabase(fileName)
    char 	    *fileName;
#endif
{
    register FILE   *file;
    XrmDatabase     db;

    if (fileName == NULL)
    	return NULL;

    file = fopen(fileName, "r");
    if (file == NULL) {
	return NULL;
    }

    db = NULL;
    PutLineResources(&db, fgets, (caddr_t) file);
    fclose(file);
    return db;
}

#if NeedFunctionPrototypes
void XrmPutFileDatabase(
    XrmDatabase db,
    const char 	*fileName)
#else
void XrmPutFileDatabase(db, fileName)
    XrmDatabase db;
    char 	*fileName;
#endif
{
    FILE	*file;
    
    if ((file = fopen(fileName, "w")) == NULL) return;
    (void)EnumerateDatabase(db, DumpEntry, (caddr_t) file);
    fclose(file);
}


void XrmMergeDatabases(new, into)
    XrmDatabase	new, *into;
{
    if (*into == NULL) {
	*into = new;
    } else if (new != NULL) {
	Merge(new, *into);
    }
}

Bool XrmQGetResource(db, names, classes, pType, pValue)
    XrmHashBucket       db;
    XrmNameList		names;
    XrmClassList 	classes;
    XrmRepresentation	*pType;  /* RETURN */
    XrmValuePtr		pValue;  /* RETURN */
{
    XrmHashTable	tight, loose;

    if (db != NULL) {
	tight = db->tables[(int) XrmBindTightly];
	loose = db->tables[(int) XrmBindLoosely];
	if ((tight != NULL || loose != NULL) && (*names != NULL)
		&& GetEntry(tight, loose, names, classes, pType, pValue)) {
	    /* Found it */
	    return True;
	}
    }
    (*pType) = NULLQUARK;
    (*pValue).addr = NULL;
    (*pValue).size = 0;
    return False;
}

#if NeedFunctionPrototypes
Bool XrmGetResource(db, name_str, class_str, pType_str, pValue)
    XrmHashBucket       db;
    const char		*name_str;
    const char		*class_str;
    XrmString		*pType_str;  /* RETURN */
    XrmValuePtr		pValue;      /* RETURN */
#else
Bool XrmGetResource(db, name_str, class_str, pType_str, pValue)
    XrmHashBucket       db;
    XrmString		name_str;
    XrmString		class_str;
    XrmString		*pType_str;  /* RETURN */
    XrmValuePtr		pValue;      /* RETURN */
#endif
{
    XrmName		names[100];
    XrmClass		classes[100];
    XrmRepresentation   fromType;
    Bool		result;

    XrmStringToNameList(name_str, names);
    XrmStringToClassList(class_str, classes);
    result = XrmQGetResource(db, names, classes, &fromType, pValue);
    (*pType_str) = XrmQuarkToString(fromType);
    return result;
} /* XrmGetResource */

void XrmInitialize()
{
    XrmQString = XrmStringToQuark("String");
}

void XrmDestroyDatabase(rdb)
    XrmDatabase   rdb;
{
    register XrmHashTable   table;
    int			    binding;
    register int	    i;

    if (rdb == NULL) return;
  
    if (rdb->value.addr != NULL) {
	Xfree(rdb->value.addr);
    }

    XrmDestroyDatabase(rdb->next);
    for (binding = (int) XrmBindTightly;
         binding <= (int) XrmBindLoosely;
	 binding++) {
	table = rdb->tables[binding];
	if (table != NULL) {
	    /* Free each bucket */
	    for (i = 0; i < HASHSIZE; i++) {
		XrmDestroyDatabase( table[i] );
	    }
	    Xfree(table);
	}
    }
    Xfree(rdb);
} /* XrmDestroyDatabase */

typedef struct _GRNData {
    char *name;
    XrmRepresentation type;
    XrmValuePtr value;
} GRNData;

/*ARGSUSED*/
static Bool SameValue(bindings, quarks, type, value, data)
    XrmBindingList      bindings;
    XrmQuarkList	quarks;
    XrmRepresentation   type;
    XrmValuePtr		value;
    GRNData		*data;
{
    if ((type == data->type) && (value->size == data->value->size) &&
	!strncmp((char *)value->addr, (char *)data->value->addr, value->size))
    {
	data->name = XrmQuarkToString(*quarks); /* XXX */
	return True;
    }
    return False;
} /* SameValue */

/* Gross internal hack */
char *_XrmGetResourceName(rdb, type_str, pValue)
    XrmDatabase rdb;
    XrmString type_str;
    XrmValuePtr pValue;
{
    GRNData data;
    data.name = (char *)NULL;
    data.type = XrmStringToQuark(type_str);
    data.value = pValue;

    (void)EnumerateDatabase(rdb, SameValue, (caddr_t)&data);
    return data.name;
} /* _XrmGetResourceName */
