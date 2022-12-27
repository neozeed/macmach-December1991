/*
 * $XConsortium: util.c,v 1.16 89/10/30 18:30:39 jim Exp $
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
#include <X11/Xos.h>
#include <X11/IntrinsicP.h>
#include <X11/Xaw/Cardinals.h>
#include "defs.h"

static char *bsearch (key, base, nelems, elemsize, compar)
    char *key;				/* template of object to find */
    char *base;				/* beginning of array */
    int nelems;				/* number of elements in array */
    int elemsize;			/* sizeof an element */
    int (*compar)();			/* qsort-style compare function */
{
    int lower = 0, upper = nelems - 1;

    while (lower <= upper) {
	int middle = (lower + upper) / 2;
	char *p = base + middle * elemsize;
	int res = (*compar) (p, key);

	if (res < 0) {
	    lower = middle + 1;
	} else if (res == 0) {
	    return p;
	} else {
	    upper = middle - 1;
	}
    }

    return NULL;
}

/*
 * print_classname - print out the superclass-to-subclass hierchy of names
 * in the form super\sub\sub....
 */
int print_classname (node, topnode, level, showvar)
    WidgetNode *node, *topnode;
    int level;
    Bool showvar;
{
    int retval;

    if (node && node != topnode) {
	retval = print_classname (node->superclass, topnode, level + 1,
				  showvar);
    } else {
	retval = level - 1;
    }
    if (node)
      printf ("%s%s", showvar ? node->label : WnClassname(node),
	      level ? "\\" : "");

    return retval;
}


/*
 * initialize__node_list - generate a tree of nodes
 */
void initialize_nodes (nodearray, nnodes)
    WidgetNode *nodearray;
    int nnodes;
{
    int i;
    WidgetNode *wn;

    for (i = 0, wn = nodearray; i < nnodes; i++, wn++) {
	WidgetClass superclass = WnSuperclass(wn);
	int j;
	WidgetNode *swn;
	int lablen = strlen (wn->label);
	int namelen = strlen (WnClassname(wn));

	wn->lowered_label = XtMalloc (lablen + namelen + 2);
	if (!wn->lowered_label) {
	    fprintf (stderr,
		     "%s:  unable to allocate %d bytes for widget name\n",
		     ProgramName);
	    exit (1);
	}
	wn->lowered_classname = wn->lowered_label + (lablen + 1);
	XmuCopyISOLatin1Lowered (wn->lowered_label, wn->label);
	XmuCopyISOLatin1Lowered (wn->lowered_classname, WnClassname(wn));
	wn->superclass = NULL;
	wn->have_resources = False;
	wn->resources = NULL;
	wn->resourcewn = NULL;
	wn->nresources = 0;
	wn->data = NULL;

	/*
	 * walk up the superclass chain
	 */
	while (superclass) {
	    for (j = 0, swn = nodearray; j < nnodes; j++, swn++) {
		if (superclass == WnClass(swn)) {
		    wn->superclass = swn;
		    goto done;		/* stupid C language */
	        }
	    }
	    /*
	     * Hmm, we have a hidden superclass (such as in core in R4); just
	     * ignore it and keep on walking
	     */
	    superclass = superclass->core_class.superclass;
	}
      done: 
	if (wn->superclass) {
	    wn->siblings = wn->superclass->children;
	    wn->superclass->children = wn;
	} else {
	    wn->children = wn->siblings = NULL;
	}
    }

    return;
}

static int compare_resource_entries (a, b)
    XtResourceList a, b;
{
    return strcmp (a->resource_name, b->resource_name);
}

static WidgetNode *find_resource (node, name)
    WidgetNode *node;
    char *name;
{
    WidgetNode *sup;
    XtResource res;

    res.resource_name = name;
    for (sup = node->superclass; 
	 sup && (XtResourceList) bsearch ((char *) &res,
					  (char *) sup->resources,
					  sup->nresources, sizeof(XtResource),
					  compare_resource_entries);
	 node = sup, sup = sup->superclass) ;

    return node;
}


static void mark_resource_owner (node)
    WidgetNode *node;
{
    int i;
    XtResourceList childres;

    childres = node->resources;

    for (i = 0; i < node->nresources; i++, childres++) {
	node->resourcewn[i] = find_resource (node, childres->resource_name);
    }
}


static void fetch_resources (node, toplevel, topnode)
    WidgetNode *node;
    Widget toplevel;
    WidgetNode *topnode;
{
    Widget dummy;
    WidgetNode *wn;

    if (node->have_resources) return;

    dummy = XtCreateWidget (node->label, WnClass(node), toplevel, NULL, ZERO);
    if (dummy) XtDestroyWidget (dummy);


    /*
     * walk up tree geting resources; since we've instantiated the widget,
     * we know that all of our superclasses have been initialized
     */
    for (wn = node; wn && !wn->have_resources; wn = wn->superclass) {
	XtGetResourceList (WnClass(wn), &wn->resources, &wn->nresources);
	if (wn->resources) {
	    qsort ((char *) wn->resources, wn->nresources,
		   sizeof(XtResource), compare_resource_entries);
	}
	wn->resourcewn = (WidgetNode **) XtCalloc (wn->nresources,
						  sizeof (WidgetNode *));
	if (!wn->resourcewn) {
	    fprintf (stderr,
		     "%s:  unable to calloc %d %d byte widget node ptrs\n",
		     ProgramName, wn->nresources, sizeof (WidgetNode *));
	    exit (1);
	}
	wn->have_resources = True;
	if (wn == topnode) break;
    }


    /*
     * Walk up tree removing all resources that appear in superclass; we can
     * mash the resource list in place since it was copied out of widget.
     */
    for (wn = node; wn; wn = wn->superclass) {
	mark_resource_owner (wn);
	if (wn == topnode) break;
    }

    return;
}

static void print_resources (node, format, topnode, showsuper, showvar)
    WidgetNode *node;
    char *format;
    WidgetNode *topnode;
    Bool showsuper;
    Bool showvar;
{
    int i;
    XtResourceList res = node->resources;
    WidgetNode **wn = node->resourcewn;

    for (i = 0; i < node->nresources; i++, res++, wn++) {
	if (!showsuper && *wn != node) continue;
	printf (format, showvar ? (*wn)->label : WnClassname(*wn),
		res->resource_name, res->resource_class, res->resource_type);
	putchar ('\n');
    }
    return;
}


/*
 * list_resources - display resources of a widget, identifying class from
 * which they come
 */
list_resources (node, format, topnode, toplevel, showsuper, showvar)
    WidgetNode *node;
    char *format;
    WidgetNode *topnode;
    Widget toplevel;
    Bool showsuper;
    Bool showvar;
{
    static Bool first = True;

    fetch_resources (node, toplevel, topnode);
    if (first) {
	printf (format, showvar ? "Variable" : "WidgetClass",
		"Instance", "Class", "Type");
	putchar ('\n');
	printf (format, showvar ? "--------" : "-----------",
		"--------", "-----", "----");
	putchar ('\n');
	first = False;
    }
    printf ("%s:  ", node->label);
    print_classname (node, topnode, 0, showvar);
    putchar ('\n');
    print_resources (node, format, topnode, showsuper, showvar);
    putchar ('\n');
}



WidgetNode *name_to_node (nodelist, nnodes, name)
    WidgetNode *nodelist;
    int nnodes;
    char *name;
{
    int i;
    WidgetNode *wn;
    char tmp[1024];

    XmuCopyISOLatin1Lowered (tmp, name);
    for (i = 0, wn = nodelist; i < nnodes; i++, wn++) {
	if (strcmp (tmp, wn->lowered_label) == 0 ||
	    strcmp (tmp, wn->lowered_classname) == 0)
	  return wn;
    }
    return NULL;
}
