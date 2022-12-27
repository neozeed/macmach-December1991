.\" $XConsortium: xfontsel.man,v 1.6 89/12/21 11:09:45 rws Exp $
.de EX		\"Begin example
.ne 5
.if n .sp 1
.if t .sp .5
.nf
.in +.5i
..
.de EE
.fi
.in -.5i
.if n .sp 1
.if t .sp .5
..
.TH xfontsel 1 "Release 4" "X Version 11"
.SH NAME
.PP
xfontsel - point & click interface for selecting X11 font names
.PP
.SH SYNTAX
.PP
\fBxfontsel\fP [-\fItoolkitoption\fP ...]
[\fB-pattern \fIfontname\fP]
[\fB-print\fP]
[\fB-sample \fItext\fP]
.PP
.SH DESCRIPTION
.PP
The \fIxfontsel\fP application provides a simple way to display
the fonts known to your X server, examine samples of each, and
retrieve the X Logical Font Description ("XLFD") full name for a font.
.PP
If \fB-pattern\fP is not specified, all fonts with XLFD 14-part
names will be selectable.  To work with only a subset of the
fonts, specify \fB-pattern\fP followed by a partially or fully
qualified font name; e.g., ``-pattern *medium*'' will
select that subset of fonts which contain the string ``medium''
somewhere in their font name.  Be careful about escaping
wildcard characters in your shell.
.PP
If \fB-print\fP is specified on the command line the selected
font specifier will be written to standard output when the \fIquit\fP
button is activated.  Regardless of whether or not \fB-print\fP was
specified, the font specifier may be made the PRIMARY (text)
selection by activating the \fIselect\fP button.
.PP
The \fB-sample\fP option specifies the sample text to be used to
display the selected font, overriding the default.
.PP
.SH INTERACTIONS
.PP
Clicking any pointer button in one of the XLFD field names will pop up
a menu of the currently-known possibilities for that field.  If
previous choices of other fields were made, only values
for fonts which matched the previously selected fields will be
selectable; to make other values selectable, you must deselect
some other field(s) by choosing the ``*'' entry in that field.
Unselectable values may be omitted from the menu entirely as
a configuration option; see the \fBShowUnselectable\fP resource, below.
Whenever any change is made to a field value, \fIxfontsel\fP will
assert ownership of the PRIMARY_FONT selection.  Other applications
(see, e.g., \fIxterm\fP) may then retrieve the selected font specification.
.PP
Clicking the left pointer button in the \fIselect\fP widget will
cause the currently selected font name to become the PRIMARY text
selection as well as the PRIMARY_FONT selection.
This then allows you to paste the string into other
applications.  The \fBselect\fP button remains
highlighted to remind you of this fact, and de-highlights when
some other application takes the PRIMARY selection away.  The
\fIselect\fP widget is a toggle; pressing it when it is highlighted
will cause \fIxfontsel\fP to release the selection ownership and
de-highlight the widget.  Activating the \fIselect\fP widget twice
is the only way to cause \fIxfontsel\fP to release the
PRIMARY_FONT selection.
.PP
.SH RESOURCES
.PP
The application class is \fBXFontSel\fP.  Most of the user-interface
is configured in the app-defaults file; if this file is missing
a warning message will be printed to standard output and the
resulting window will be nearly incomprehensible.
.PP
Most of the significant parts of the widget hierarchy are documented
in the app-defaults file (normally /usr/lib/X11/app-defaults/XFontSel).
.PP
Application specific resources:
.PP
.TP 8
.B "cursor (\fPclass\fB Cursor)"
Specifies the cursor for the application window.
.TP 8
.B "pattern (\fPclass\fB Pattern)"
Specifies the font name pattern for selecting a subset of
available fonts.  Equivalent to the \fB-pattern\fP option.
Most useful patterns will contain at least one field
delimiter; e.g. ``*-m-*'' for monospaced fonts.
.TP 8
.B "printOnQuit (\fPclass\fP PrintOnQuit)"
If \fITrue\fP the currently selected font name is printed
to standard output when the quit button is activated.
Equivalent to the \fB-print\fP option.
.PP
Widget specific resources:
.PP
.TP 8
.B "showUnselectable (\fPclass\fB ShowUnselectable)"
Specifies, for each field menu, whether or not to show values that
are not currently selectable, based upon previous field selections.
If shown, the unselectable values are clearly identified as such
and do not highlight when the pointer is moved down the menu.
The full name of this resource is \fBfieldN.menu.options.showUnselectable\fP,
class \fBMenuButton.SimpleMenu.Options.ShowUnselectable\fP;
where N is replaced with the field
number (starting with the left-most field numbered 0).
The default is True for all but field 11 (average width of characters
in font) and False for field 11.  If you never want to see
unselectable entries, '*menu.options.showUnselectable:False' is
a reasonable thing to specify in a resource file.
.PP
.SH FILES
.PP
 $XFILESEARCHPATH/XFontSel
.PP
.SH SEE ALSO
xrdb(1)
.PP
.SH BUGS
.PP
Sufficiently ambiguous patterns can be misinterpreted and lead to an
initial selection string which may not correspond to what the user intended
and which may cause the initial sample text output to fail to match
the proffered string.  Selecting any new field value will correct the
sample output, though possibly resulting in no matching font.
.PP
Should be able to return a FONT for the PRIMARY selection, not
just a STRING.
.PP
Any change in a field value will cause \fIxfontsel\fP to assert
ownership of the PRIMARY_FONT selection.  Perhaps this should
be parameterized.
.PP
When running on a slow machine, it is possible for the user to
request a field menu before the font names have been completely
parsed.  An error message indicating a missing menu is printed
to stderr but otherwise nothing bad (or good) happens.
.PP
.SH COPYRIGHT
Copyright 1989 by the Massachusetts Institute of Technology
.br
See \fIX(1)\fP for a full statement of rights and permissions.
.SH AUTHOR
.PP
Ralph R. Swick, Digital Equipment Corporation/MIT Project Athena


