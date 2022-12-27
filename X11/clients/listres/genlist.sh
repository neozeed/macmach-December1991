#!/bin/sh

tmp=/tmp/gl.$$
nm $* | egrep '(Widget|Obj|Object)Class' | \
awk '{ if ($2 == "D" || $2 == "G") print $3; }' | sed -e 's/^_//' | \
(cat - ; echo "objectClass") | sort -u | egrep -v 'ClassRec$' | \
awk '
{
    printf "extern WidgetClass %s;\n", $1;
    printf "{ \"%s\", &%s },\n", $1, $1;
}' | sed -e 's/WidgetClass"/"/' -e 's/ObjClass"/"/' \
	 -e 's/ObjectClass"/"/' -e 's/objectClass"/object"/' \
	 -e 's/widgetClass"/widget"/' >$tmp

cat <<EOF
#include <X11/IntrinsicP.h>
#include "defs.h"

EOF
egrep '^extern WidgetClass' $tmp
echo ""
cat <<EOF
WidgetNode widget_list[] = {
EOF
egrep '^{' $tmp
cat <<EOF
};

int nwidgets = XtNumber(widget_list);

EOF

rm $tmp
