#include <X11/IntrinsicP.h>
#include "defs.h"

extern WidgetClass applicationShellWidgetClass;
extern WidgetClass asciiSinkObjectClass;
extern WidgetClass asciiSrcObjectClass;
extern WidgetClass asciiTextWidgetClass;
extern WidgetClass boxWidgetClass;
extern WidgetClass clockWidgetClass;
extern WidgetClass commandWidgetClass;
extern WidgetClass compositeWidgetClass;
extern WidgetClass constraintWidgetClass;
extern WidgetClass coreWidgetClass;
extern WidgetClass dialogWidgetClass;
extern WidgetClass formWidgetClass;
extern WidgetClass gripWidgetClass;
extern WidgetClass labelWidgetClass;
extern WidgetClass listWidgetClass;
extern WidgetClass logoWidgetClass;
extern WidgetClass mailboxWidgetClass;
extern WidgetClass menuButtonWidgetClass;
extern WidgetClass objectClass;
extern WidgetClass overrideShellWidgetClass;
extern WidgetClass panedWidgetClass;
extern WidgetClass rectObjClass;
extern WidgetClass scrollbarWidgetClass;
extern WidgetClass shellWidgetClass;
extern WidgetClass simpleMenuWidgetClass;
extern WidgetClass simpleWidgetClass;
extern WidgetClass smeBSBObjectClass;
extern WidgetClass smeLineObjectClass;
extern WidgetClass smeObjectClass;
extern WidgetClass stripChartWidgetClass;
extern WidgetClass textSinkObjectClass;
extern WidgetClass textSrcObjectClass;
extern WidgetClass textWidgetClass;
extern WidgetClass toggleWidgetClass;
extern WidgetClass topLevelShellWidgetClass;
extern WidgetClass transientShellWidgetClass;
extern WidgetClass vPanedWidgetClass;
extern WidgetClass vendorShellWidgetClass;
extern WidgetClass viewportWidgetClass;
extern WidgetClass wmShellWidgetClass;

WidgetNode widget_list[] = {
{ "applicationShell", &applicationShellWidgetClass },
{ "asciiSink", &asciiSinkObjectClass },
{ "asciiSrc", &asciiSrcObjectClass },
{ "asciiText", &asciiTextWidgetClass },
{ "box", &boxWidgetClass },
{ "clock", &clockWidgetClass },
{ "command", &commandWidgetClass },
{ "composite", &compositeWidgetClass },
{ "constraint", &constraintWidgetClass },
{ "core", &coreWidgetClass },
{ "dialog", &dialogWidgetClass },
{ "form", &formWidgetClass },
{ "grip", &gripWidgetClass },
{ "label", &labelWidgetClass },
{ "list", &listWidgetClass },
{ "logo", &logoWidgetClass },
{ "mailbox", &mailboxWidgetClass },
{ "menuButton", &menuButtonWidgetClass },
{ "object", &objectClass },
{ "overrideShell", &overrideShellWidgetClass },
{ "paned", &panedWidgetClass },
{ "rect", &rectObjClass },
{ "scrollbar", &scrollbarWidgetClass },
{ "shell", &shellWidgetClass },
{ "simpleMenu", &simpleMenuWidgetClass },
{ "simple", &simpleWidgetClass },
{ "smeBSB", &smeBSBObjectClass },
{ "smeLine", &smeLineObjectClass },
{ "sme", &smeObjectClass },
{ "stripChart", &stripChartWidgetClass },
{ "textSink", &textSinkObjectClass },
{ "textSrc", &textSrcObjectClass },
{ "text", &textWidgetClass },
{ "toggle", &toggleWidgetClass },
{ "topLevelShell", &topLevelShellWidgetClass },
{ "transientShell", &transientShellWidgetClass },
{ "vPaned", &vPanedWidgetClass },
{ "vendorShell", &vendorShellWidgetClass },
{ "viewport", &viewportWidgetClass },
{ "wmShell", &wmShellWidgetClass },
};

int nwidgets = XtNumber(widget_list);

