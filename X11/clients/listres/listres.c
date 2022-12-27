/*
 * $XConsortium: listres.c,v 1.22 89/12/10 15:55:50 rws Exp $
 *
 * Copyright 1989 Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.  M.I.T. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * M.I.T. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL M.I.T.
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author:  Jim Fulton, MIT X Consortium
 */

#include <stdio.h>
#include <X11/StringDefs.h>
#include <X11/IntrinsicP.h>
#include <X11/Xaw/Cardinals.h>
#include <X11/Core.h>
#include "defs.h"

extern WidgetNode widget_list[];
extern int nwidgets;


static XrmOptionDescRec Options[] = {
  { "-top", "*topObject", XrmoptionSepArg, (caddr_t) NULL },
  { "-format", "*resourceFormat", XrmoptionSepArg, (caddr_t) NULL },
  { "-tree", "*showTree", XrmoptionNoArg, (caddr_t) "on" },
  { "-nosuper", "*showSuper", XrmoptionNoArg, (caddr_t) "off" },
  { "-variable", "*showVariable", XrmoptionNoArg, (caddr_t) "on" },
};

static struct _appresources {
    Boolean show_tree;
    Boolean show_all;
    Boolean show_variable;
    Boolean show_superclass;
    char *top_object;
    char *format;
} Appresources;


static XtResource Resources[] = {
#define offset(field) XtOffset(struct _appresources *, field)
  { "showTree", "ShowTree", XtRBoolean, sizeof(Boolean),
      offset(show_tree), XtRImmediate, (caddr_t) FALSE },
  { "showSuper", "ShowSuper", XtRBoolean, sizeof(Boolean),
      offset(show_superclass), XtRImmediate, (caddr_t) TRUE },
  { "showVariable", "ShowVariable", XtRBoolean, sizeof(Boolean),
      offset(show_variable), XtRImmediate, (caddr_t) FALSE },
  { "topObject", "TopObject", XtRString, sizeof(char *),
      offset(top_object), XtRString, (caddr_t) "core" },
  { "resourceFormat", "ResourceFormat", XtRString, sizeof(char *),
      offset(format), XtRString, (caddr_t) " %-16s %20s  %-20s  %s" },
#undef offset
};



char *ProgramName;

usage ()
{
    fprintf(stderr, "usage:  %s [-options...]\n", ProgramName);
    fprintf(stderr, "\nwhere options include:\n");
    fprintf(stderr,
	    "    -all             list all known widget and object classes\n");
    fprintf(stderr,
	    "    -tree            list all widgets and objects in a tree\n");
    fprintf(stderr,
	    "    -nosuper         do not print superclass resources\n");
    fprintf(stderr,
	    "    -variable        show variable name instead of class name\n");
    fprintf(stderr,
	    "    -top name        object to be top of tree\n");
    fprintf(stderr,
	    "    -format string   printf format for instance, class, type\n");
    fprintf(stderr, "\n");
    exit (1);
}

static void print_tree_level (wn, level)
    register WidgetNode *wn;
    register int level;
{
    register int i;

    if (!wn) return;

    for (i = 0; i < level; i++) {
	putchar (' '); putchar (' '); 
    }
    printf ("%d:  %s/%s\n", level, wn->label, WnClassname(wn));
    print_tree_level (wn->children, level + 1);
    print_tree_level (wn->siblings, level);
}

static void tree_known_widgets ()
{
    register int i;
    register WidgetNode *wn;

    for (i = 0, wn = widget_list; i < nwidgets; i++, wn++) {
	if (!wn->superclass) {		/* list all rooted objects */
	    print_tree_level (wn, 0);
	}
    }
}

static void list_known_widgets ()
{
    int i;
    WidgetNode *wn;
    int width = 0;
    char format[20];

    for (i = 0, wn = widget_list; i < nwidgets; i++, wn++) {
	int l = strlen (wn->label);
	if (l > width) width = l;
    }
    sprintf (format, "%%-%ds  ", width);
    for (i = 0, wn = widget_list; i < nwidgets; i++, wn++) {
	printf (format, wn->label);
	print_classname (wn, NULL, 0, False);
	putchar ('\n');
    }
}


main (argc, argv)
    int argc;
    char **argv;
{
    int i;
    WidgetNode *topnode;
    Widget toplevel, container;

    ProgramName = argv[0];

    toplevel = XtInitialize (NULL, "Listres", Options, XtNumber (Options),
			     &argc, argv);
    container = XtCreateWidget ("dummy", widgetClass, toplevel, NULL, ZERO);

    XtGetApplicationResources (toplevel, (caddr_t) &Appresources,
			       Resources, XtNumber(Resources), NULL, ZERO);
    initialize_nodes (widget_list, nwidgets);
    if (argc == 1) {
	if (Appresources.show_tree) {
	    tree_known_widgets();
	} else {
	    list_known_widgets();
	}
	exit (0);
    }

    topnode = name_to_node (widget_list, nwidgets, Appresources.top_object);
    argc--, argv++;			/* skip command */

    if (argc > 0 && argv[0][0] == '-') {
	int len = strlen (argv[0]);
	if (len >= 2 && strncmp(argv[0], "-all", len) == 0) {
	    WidgetNode *wn;
	    for (i = 0, wn = widget_list; i < nwidgets; i++, wn++) {
		list_resources (wn, Appresources.format, topnode, container,
				(Bool) Appresources.show_superclass,
				(Bool) Appresources.show_variable);
	    }
	} else
	  usage();
    } else {
	for (; argc > 0; argc--, argv++) {
	    WidgetNode *node;

	    if (argv[0][0] == '-') usage ();
	    node = name_to_node (widget_list, nwidgets, *argv);
	    if (!node) {
		fprintf (stderr, "%s:  unable to find widget \"%s\"\n",
			 ProgramName, *argv);
		continue;
	    }
	    list_resources (node, Appresources.format, topnode, container,
			    (Bool) Appresources.show_superclass,
			    (Bool) Appresources.show_variable);
	}
    }
    exit (0);
}
