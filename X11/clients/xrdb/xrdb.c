#ifndef lint
static char rcs_id[] = "$XConsortium: xrdb.c,v 11.35 89/12/13 13:54:15 rws Exp $";
#endif

/*
 *			  COPYRIGHT 1987
 *		   DIGITAL EQUIPMENT CORPORATION
 *		       MAYNARD, MASSACHUSETTS
 *			ALL RIGHTS RESERVED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT NOTICE AND
 * SHOULD NOT BE CONSTRUED AS A COMMITMENT BY DIGITAL EQUIPMENT CORPORATION.
 * DIGITAL MAKES NO REPRESENTATIONS ABOUT THE SUITABILITY OF THIS SOFTWARE FOR
 * ANY PURPOSE.  IT IS SUPPLIED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
 *
 * IF THE SOFTWARE IS MODIFIED IN A MANNER CREATING DERIVATIVE COPYRIGHT RIGHTS,
 * APPROPRIATE LEGENDS MAY BE PLACED ON THE DERIVATIVE WORK IN ADDITION TO THAT
 * SET FORTH ABOVE.
 *
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Digital Equipment Corporation not be
 * used in advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.
 */

/*
 * this program is used to load, or dump the resource manager database
 * in the server.
 *
 * Author: Jim Gettys, August 28, 1987
 * Extensively Modified: Phil Karlton, January 5, 1987
 */

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xos.h>
#include <ctype.h>

#ifndef CPP
#define CPP "/usr/lib/cpp"
#endif /* CPP */

char *ProgramName;
static Bool quiet = False;

#define RESOURCE_PROPERTY_NAME "RESOURCE_MANAGER"
#define BACKUP_SUFFIX ".bak"		/* for editting */


extern FILE *popen();

typedef struct _Entry {
    char *tag, *value;
    int lineno;
    Bool usable;
} Entry;
typedef struct _Buffer {
    char *buff;
    int  room, used;
} Buffer;
typedef struct _Entries {
    Entry *entry;
    int   room, used;
} Entries;

#define INIT_BUFFER_SIZE 10000
#define INIT_ENTRY_SIZE 500

#if defined(USG) && !defined(CRAY)
int rename (from, to)
    char *from, *to;
{
    (void) unlink (to);
    if (link (from, to) == 0) {
        unlink (from);
        return 0;
    } else {
        return -1;
    }
}
#endif

void InitBuffer(b)
    Buffer *b;
{
    b->room = INIT_BUFFER_SIZE;
    b->used = 0;
    b->buff = (char *)malloc(INIT_BUFFER_SIZE*sizeof(char));
}

void AppendToBuffer(b, str, len)
    register Buffer *b;
    char *str;
    register int len;
{
    while (b->used + len > b->room) {
	b->buff = (char *)realloc(b->buff, 2*b->room*(sizeof(char)));
	b->room *= 2;
    }
    strncpy(b->buff + b->used, str, len);
    b->used += len;
}

void InitEntries(e)
    Entries *e;
{
    e->room = INIT_ENTRY_SIZE;
    e->used = 0;
    e->entry = (Entry *)malloc(INIT_ENTRY_SIZE*sizeof(Entry));
}

void AddEntry(e, entry)
    register Entries *e;
    Entry entry;
{
    register int n;

    for (n = 0; n < e->used; n++) {
	if (strcmp(e->entry[n].tag, entry.tag) == 0) { /* overwrite old entry */
	    if (!quiet) {
		fprintf (stderr, 
			 "%s:  \"%s\" on line %d overrides entry on line %d\n",
			 ProgramName, entry.tag, entry.lineno, 
			 e->entry[n].lineno);
	    }
	    e->entry[n] = entry;
	    return ;  /* ok to leave, now there's only one of each tag in db */
	}
    }

    if (e->used == e->room) {
	e->entry = (Entry *)realloc(e->entry, 2*e->room*(sizeof(Entry)));
	e->room *= 2;
    }
    entry.usable = True;
    e->entry[e->used++] = entry;
}


int CompareEntries(e1, e2)
    Entry *e1, *e2;
{
    return strcmp(e1->tag, e2->tag);
}

void AppendEntryToBuffer(buffer, entry)
    register Buffer *buffer;
    Entry entry;
{
    AppendToBuffer(buffer, entry.tag, strlen(entry.tag));
    AppendToBuffer(buffer, ":\t", 2);
    AppendToBuffer(buffer, entry.value, strlen(entry.value));
    AppendToBuffer(buffer, "\n", 1);
}

char *FindFirst(string, dest)
    register char *string;
    register char dest;
{
    for (;;) {
	if (*string == '\0')
	    return NULL;
	if (*string == '\\') {
	    if (*++string == '\0')
		return NULL;
	}
	else if (*string == dest)
	    return string;
	string++;
    }
}

void GetEntries(entries, buff, dosort)
    register Entries *entries;
    Buffer *buff;
    int dosort;
{
    register char *line, *colon, *temp, *str;
    Entry entry;
    register int length;
    int lineno = 0;

    str = buff->buff;
    if (!str) return;
    for (; str < buff->buff + buff->used; str = line + 1) {
	line = FindFirst(str, '\n');
	lineno++;
	if (line == NULL)
	    break; 
	if (str[0] == '!')
	    continue;
	if (str[0] == '\n')
	    continue;
	if (str[0] == '#') {
	    int dummy;
	    if (sscanf (str, "# %d", &dummy) == 1) lineno = dummy - 1;
	    continue;
	}
	for (temp = str; 
	     *temp && *temp != '\n' && isascii(*temp) && isspace(*temp); 
	     temp++) ;
	if (!*temp || *temp == '\n') continue;

	colon = FindFirst(str, ':');
	if (colon == NULL)
	    break;
	if (colon > line) {
	    line[0] = '\0';
	    fprintf (stderr, 
		     "%s:  colon missing on line %d, ignoring entry \"%s\"\n",
		     ProgramName, lineno, str);
	    continue;
	}

	temp = (char *)malloc((length = colon - str) + 1);
	strncpy(temp, str, length);
	temp[length] = '\0';
	while (temp[0] == ' ' || temp[0] == '\t') {
	    temp++; length--;
	}
	while (temp[length-1] == ' ' || temp[length-1] == '\t')
	    temp[--length] = '\0';
	entry.tag = temp;

	temp = (char *)malloc((length = line - colon - 1) + 1);
	strncpy(temp, colon + 1, length);
	temp[length] = '\0';
	while (temp[0] == ' ' || temp[0] == '\t') {
	    temp++; length--;
	}
	entry.value = temp;
	entry.lineno = lineno;

	AddEntry(entries, entry);
    }
    if (dosort && (entries->used > 0))
      qsort(entries->entry, entries->used, sizeof(Entry), CompareEntries);
}

int MergeEntries(buffer, new, old)
    Entries new, old;
    Buffer *buffer;
{
    int n, o, cmp;

    buffer->used = 0;
    n = o = 0;
    while ((n < new.used) && (o < old.used)) {
	cmp = strcmp(new.entry[n].tag, old.entry[o].tag);
	if (cmp > 0)
	    AppendEntryToBuffer(buffer, old.entry[o++]);
	else {
	    AppendEntryToBuffer(buffer, new.entry[n++]);
	    if (cmp == 0)
		o++;
	}
    }
    while (n < new.used)
	AppendEntryToBuffer(buffer, new.entry[n++]);
    while (o < old.used)
	AppendEntryToBuffer(buffer, old.entry[o++]);
    AppendToBuffer(buffer, "\0", 1);
}

void ReadFile(buffer, input)
	register Buffer *buffer;
	FILE *input;
{
	     char	buf[BUFSIZ];
    register int	bytes;

    buffer->used = 0;
    while ((bytes = fread(buf, 1, BUFSIZ, input)) > 0) {
	AppendToBuffer(buffer, buf, bytes);
    }
    AppendToBuffer(buffer, "\0", 1);
}

AddDef(buff, title, value)
    char *buff, *title, *value;
{
    strcat(buff, " -D");
    strcat(buff, title);
    if (value && (value[0] != '\0')) {
	strcat(buff, "=");
	strcat(buff, value);
    }
}

AddDefQ(buff, title, value)
    char *buff, *title, *value;
{
    strcat(buff, " -D");
    strcat(buff, title);
    if (value && (value[0] != '\0')) {
	strcat(buff, "=\"");
	strcat(buff, value);
	strcat(buff, "\"");
    }
}

AddNum(buff, title, value)
    char *buff, *title;
    int value;
{
    char num[20];
    sprintf(num, "%d", value);
    AddDef(buff, title, num);
}

AddSimpleDef(buff, title)
    char *buff, *title;
{
    strcat(buff, " -D");
    strcat(buff, title);
}

int Resolution(pixels, mm)
    int pixels, mm;
{
    return ((pixels * 100000 / mm) + 50) / 100;
}


/*
 * It would probably be best to enumerate all of the screens/visuals
 * rather than just using the defaults. However, most of the current
 * servers only have one screen/visual.
 */
void
DoDefines(display, defs, prog, host)
    Display *display;
    register char *defs;
    char *prog, *host;
{
#define MAXHOSTNAME 255
    Screen *screen;
    Visual *visual;
    char client[MAXHOSTNAME], server[MAXHOSTNAME], *colon;
    
    XmuGetHostname(client, MAXHOSTNAME);
    strcpy(server, XDisplayName(host));
    colon = index(server, ':');
    if (colon != NULL)
	*colon = '\0';
    if (server[0] == '\0')	/* must be connected to :0 */
	strcpy(server, client);
    AddDef(defs, "HOST", server); /* R3 compatibility */
    AddDef(defs, "SERVERHOST", server);
    AddDef(defs, "CLIENTHOST", client);
    AddNum(defs, "VERSION", ProtocolVersion(display));
    AddNum(defs, "REVISION", ProtocolRevision(display));
    AddDefQ(defs, "VENDOR", ServerVendor(display));
    AddNum(defs, "RELEASE", VendorRelease(display));
    screen = DefaultScreenOfDisplay(display);
    visual = DefaultVisualOfScreen(screen);
    AddNum(defs, "WIDTH", screen->width);
    AddNum(defs, "HEIGHT", screen->height);
    AddNum(defs, "X_RESOLUTION", Resolution(screen->width,screen->mwidth));
    AddNum(defs, "Y_RESOLUTION", Resolution(screen->height,screen->mheight));
    AddNum(defs, "PLANES", DisplayPlanes(display, DefaultScreen(display)));
    AddNum(defs, "BITS_PER_RGB", visual->bits_per_rgb);
    switch(visual->class) {
	case StaticGray:
	    AddDef(defs, "CLASS", "StaticGray");
	    break;
	case GrayScale:
	    AddDef(defs, "CLASS", "GrayScale");
	    break;
	case StaticColor:
	    AddDef(defs, "CLASS", "StaticColor");
	    AddSimpleDef(defs, "COLOR");
	    break;
	case PseudoColor:
	    AddDef(defs, "CLASS", "PseudoColor");
	    AddSimpleDef(defs, "COLOR");
	    break;
	case TrueColor:
	    AddDef(defs, "CLASS", "TrueColor");
	    AddSimpleDef(defs, "COLOR");
	    break;
	case DirectColor:
	    AddDef(defs, "CLASS", "DirectColor");
	    AddSimpleDef(defs, "COLOR");
	    break;
	default:
	    fatal("%s: unexpected visual class=%d\n", prog, visual->class);
    }
}

Entry *FindEntry(db, b)
    register Entries *db;
    Buffer *b;
{
    int i;
    register Entry *e;
    Entries phoney;
    Entry entry;

    entry.usable = False;
    entry.tag = NULL;
    entry.value = NULL;
    phoney.used = 0;
    phoney.room = 1;
    phoney.entry = &entry;
    GetEntries(&phoney, b, 0);
    if (phoney.used < 1)
	return (NULL);
    for (i = 0; i < db->used; i++) {
	e = &db->entry[i];
	if (!e->usable)
	    continue;
	if (strcmp(e->tag, entry.tag))
	    continue;
	e->usable = False;
	if (strcmp(e->value, entry.value))
	    return e;
	return NULL;
    }
    return NULL;
}

void EditFile(new, in, out)
    register Entries *new;
    FILE *in, *out;
{
    Buffer b;
    char buff[BUFSIZ];
    register Entry *e;
    register char *c;
    int i;

    InitBuffer(&b);
    for (;;) {
	b.used = 0;
	while (1) {
	    buff[0] ='\0';
	    if (fgets(buff, BUFSIZ, in) == NULL)
		goto cleanup;
	    AppendToBuffer(&b, buff, strlen(buff));
	    c = &b.buff[b.used - 1];
	    if ((*(c--) == '\n') && (b.used == 1 || *c != '\\'))
		break;
	}
	if (e = FindEntry(new, &b))
	    fprintf(out, "%s:\t%s\n", e->tag, e->value);
	else
	    fwrite(b.buff, 1, b.used, out);
    }
cleanup:
    for (i = 0; i < new->used; i++) {
	e = &new->entry[i];
	if (e->usable)
	    fprintf(out, "%s:\t%s\n", e->tag, e->value);
    }
}

void Syntax ()
{
    fprintf (stderr, 
	     "usage:  %s [-options ...] [filename]\n\n",
	     ProgramName);
    fprintf (stderr, 
	     "where options include:\n");
    fprintf (stderr, 
	     "    -display host:dpy            display to use\n");
    fprintf (stderr,
	     "    -n                           show but don't do changes\n");
    fprintf (stderr, 
	     "    -cpp filename                preprocessor to use [%s]\n",
	     CPP);
    fprintf (stderr, 
	     "    -nocpp                       do not use a preprocessor\n");
    fprintf (stderr, 
	     "    -query                       query %s\n",
	     RESOURCE_PROPERTY_NAME);
    fprintf (stderr,
	     "    -load                        load file into %s [default]\n",
	     RESOURCE_PROPERTY_NAME);
    fprintf (stderr, 
	     "    -merge                       merge file into %s\n",
	     RESOURCE_PROPERTY_NAME);
    fprintf (stderr, 
	     "    -edit filename               edit %s into file\n",
	     RESOURCE_PROPERTY_NAME);
    fprintf (stderr, 
	     "    -backup string               backup suffix for -edit [%s]\n",
	     BACKUP_SUFFIX);
    fprintf (stderr, 
	     "    -symbols                     show preprocessor symbols\n");
    fprintf (stderr, 
	     "    -remove                      remove %s from its window\n",
	     RESOURCE_PROPERTY_NAME);
    fprintf (stderr, 
	     "    -retain                      avoid server reset if first client\n");
    fprintf (stderr,
	     "    -quiet                       don't warn about duplicates\n");
    fprintf (stderr, 
	     "    -Dname[=value], -Uname, -Idirectory    %s\n",
	     "passed to preprocessor");
    fprintf (stderr, 
	     "\n");
    fprintf (stderr,
	     "A - or no input filename represents stdin.\n");  
    exit (1);
}

/*
 * The following is a hack until XrmParseCommand is ready.  It determines
 * whether or not the given string is an abbreviation of the arg.
 */

static Bool isabbreviation (arg, s, minslen)
    char *arg;
    char *s;
    int minslen;
{
    int arglen;
    int slen;

    /* exact match */
    if (strcmp (arg, s) == 0) return (True);

    arglen = strlen (arg);
    slen = strlen (s);

    /* too long or too short */
    if (slen >= arglen || slen < minslen) return (False);

    /* abbreviation */
    if (strncmp (arg, s, slen) == 0) return (True);

    /* bad */
    return (False);
}


main (argc, argv)
    int argc;
    char **argv;
{
    Display *dpy;
    char *xdefs;
    int i;
    char *displayname = NULL;
    char *filename = NULL;
    FILE *fp, *input, *output;
    Buffer buffer;
    Entries newDB, oldDB;
    char cmd[BUFSIZ];
    char defines[BUFSIZ];
    int printit = 0;
    int showDefines = 0;
    int removeProp = 0;
    int retainProp = 0;
    int merge = 0;
    char *editFile = NULL;
    char *cpp_program = CPP;
    char *backup_suffix = BACKUP_SUFFIX;
    Bool dont_execute = False;

    ProgramName = argv[0];

    InitBuffer(&buffer);
    InitEntries(&newDB);
    InitEntries(&oldDB);
    defines[0] = '\0';

    /* needs to be replaced with XrmParseCommand */

    for (i = 1; i < argc; i++) {
	char *arg = argv[i];

	if (arg[0] == '-') {
	    if (arg[1] == '\0') {
		filename = NULL;
		continue;
	    } else if (isabbreviation ("-help", arg, 2)) {
		Syntax ();
		/* doesn't return */
	    } else if (isabbreviation ("-display", arg, 2)) {
		if (++i >= argc) Syntax ();
		displayname = argv[i];
		continue;
	    } else if (isabbreviation ("-geometry", arg, 2)) {
		if (++i >= argc) Syntax ();
		/* ignore geometry */
		continue;
	    } else if (isabbreviation ("-cpp", arg, 2)) {
		if (++i >= argc) Syntax ();
		cpp_program = argv[i];
		continue;
	    } else if (strcmp ("-n", arg) == 0) {
		dont_execute = True;
		continue;
	    } else if (isabbreviation ("-nocpp", arg, 3)) {
		cpp_program = NULL;
		continue;
	    } else if (isabbreviation ("-query", arg, 2)) {
		printit = 1;
		continue;
	    } else if (isabbreviation ("-load", arg, 2)) {
		merge = 0;
		continue;
	    } else if (isabbreviation ("-merge", arg, 2)) {
		merge = 1;
		continue;
	    } else if (isabbreviation ("-edit", arg, 2)) {
		if (++i >= argc) Syntax ();
		editFile = argv[i];
		continue;
	    } else if (isabbreviation ("-backup", arg, 2)) {
		if (++i >= argc) Syntax ();
		backup_suffix = argv[i];
		continue;
	    } else if (isabbreviation ("-symbols", arg, 2)) {
		showDefines = 1;
		continue;
	    } else if (isabbreviation ("-remove", arg, 3)) {
		removeProp = 1;
		continue;
	    } else if (isabbreviation ("-retain", arg, 3)) {
		retainProp = 1;
		continue;
	    } else if (isabbreviation ("-quiet", arg, 2)) {
		quiet = True;
		continue;
	    } else if (arg[1] == 'I' || arg[1] == 'U' || arg[1] == 'D') {
		strcat(defines, " \"");
		strcat(defines, arg);
		strcat(defines, "\"");
		continue;
	    }
	    Syntax ();
	} else if (arg[0] == '=') 
	    continue;
	else
	    filename = arg;
    }							/* end for */

    /* Open display  */
    if (!(dpy = XOpenDisplay (displayname)))
	fatal("%s: Can't open display '%s'\n", ProgramName,
		 XDisplayName (displayname));

    xdefs = XResourceManagerString (dpy);

    DoDefines(dpy, defines, ProgramName, displayname);
    if (showDefines) {
	printf ("%s\n", defines);
    }
    if (printit == 1) {
	/* user wants to print contents */
	if (xdefs)
	    printf ("%s", xdefs);	/* fputs broken in SunOS 4.0 */
    }
    if (showDefines || printit) {
	XCloseDisplay (dpy);
	exit (0);
    }

    /* modify property */

    if (removeProp) {
	if (XResourceManagerString (dpy))
	    XDeleteProperty(dpy, RootWindow(dpy, 0), XA_RESOURCE_MANAGER);
    } else if (editFile) {
	char template[100], old[100];

	if (editFile == NULL)
	    fatal("%s: must specify file name to be edited.\n", ProgramName);
	input = fopen(editFile, "r");
	if (input == NULL) {
	    input = fopen (editFile, "w");
	    if (!input) {
		fatal ("%s:  unable to create file '%s' for editing\n",
		       ProgramName, editFile);
		/* doesn't return */
	    }
	    (void) fclose (input);
	    input = fopen (editFile, "r");
	    if (!input) {
		fatal ("%s:  unable to open file '%s' for editing\n",
		       ProgramName, editFile);
		/* doesn't return */
	    }
	}
	strcpy(template, editFile);
	strcat(template, "XXXXXX");
	output = fopen(mktemp(template), "w");
	if (output == NULL)
	    fatal("%s: can't open temporary file '%s'\n", ProgramName, template);
	buffer.used = (xdefs ? strlen(xdefs) : 0);
	buffer.buff = xdefs;		/* drop it on the floor */
	buffer.room = buffer.used;
	GetEntries(&newDB, &buffer, 0);
	EditFile(&newDB, input, output);
	fclose(input);
	fclose(output);
	strcpy(old, editFile);
	strcat(old, backup_suffix);
	if (dont_execute) {		/* then write to standard out */
	    char buf[BUFSIZ];
	    int n;

	    output = fopen (template, "r");
	    if (output) {
		while ((n = fread (buf, 1, sizeof buf, output)) > 0) {
		    fwrite (buf, 1, n, stdout);
		}
		fclose (output);
	    }
	    unlink (template);
	} else {
	    rename (editFile, old);
	    rename (template, editFile);
	}
    } else {
	if (filename != NULL) {
		fp = freopen (filename, "r", stdin);
		if (fp == NULL)
		    fatal("%s: can't open file '%s'\n", ProgramName, filename);
	}
	if (cpp_program) {
	    sprintf(cmd, "%s %s", cpp_program, defines);
	    if ((input = popen(cmd, "r")) == NULL)
		fatal("%s: cannot run '%s'\n", ProgramName, cmd);
	} else {
	    input = stdin;
	}
	ReadFile(&buffer, input);
	GetEntries(&newDB, &buffer, merge);
	if (merge && xdefs) {
	    char *saveBuff = buffer.buff;
	    buffer.used = strlen(xdefs);
	    buffer.buff = xdefs;
	    GetEntries(&oldDB, &buffer, 1);
	    buffer.buff = saveBuff;
	} else
	    oldDB.used = 0;
	buffer.used = 0;
	MergeEntries(&buffer, newDB, oldDB);
	if (dont_execute) {
	    if (buffer.used > 0) {
		fwrite (buffer.buff, 1, buffer.used, stdout);
		if (buffer.buff[buffer.used - 1] != '\n') putchar ('\n');
	    }
	} else {
	    XChangeProperty (dpy, RootWindow(dpy, 0), XA_RESOURCE_MANAGER,
			     XA_STRING, 8, PropModeReplace,
			     (unsigned char *)buffer.buff, buffer.used);
	    if (retainProp)
	      XSetCloseDownMode(dpy, RetainPermanent);
	}
    }

    XCloseDisplay(dpy);
    exit (0);
}

fatal(msg, prog, x1, x2, x3, x4, x5)
    char *msg, *prog;
    int x1, x2, x3, x4, x5;
{
    extern int errno;

    if (errno)
	perror(prog);
    (void) fprintf(stderr, msg, prog, x1, x2, x3, x4, x5);
    exit(1);
}
