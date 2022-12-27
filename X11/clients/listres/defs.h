extern char *ProgramName;

/*
 * This is usually initialized by setting the first two fields and letting
 * rest be implicitly nulled (by genlist.sh, for example)
 */
typedef struct _WidgetNode {
    char *label;			/* mixed case name */
    WidgetClass *widget_class_ptr;	/* addr of widget class */
    struct _WidgetNode *superclass;	/* superclass of widget_class */
    struct _WidgetNode *children, *siblings;	/* subclass links */
    char *lowered_label;		/* lowercase version of label */
    char *lowered_classname;		/* lowercase version of class_name */
    Bool have_resources;		/* resources have been fetched */
    XtResourceList resources;		/* extracted resource database */
    struct _WidgetNode **resourcewn;	/* where resources come from */
    Cardinal nresources;		/* number of resources */
    char *data;				/* hook for traversing */
} WidgetNode;

#define WnClass(wn) ((wn)->widget_class_ptr[0])
#define WnClassname(wn) (WnClass(wn)->core_class.class_name)
#define WnSuperclass(wn) ((WnClass(wn))->core_class.superclass)

void initialize_nodes();
WidgetNode *name_to_node();
