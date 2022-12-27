...\"	index formatting macros
...\"
...\"
...\"	Iy  --  called between letters of the alphabet
.de Iy
.if !\\$1 \{\
.	fl
.	sp 5v
.	ne .75i
.	in \\n(I#u						\" do indent
\s30\&\\$1\s0
.	sp 1
.	rm I! I@
'	br \}
..
...\"
...\"
...\"	Ix  --  called for an individual index item
.de Ix
'in \\n(I#u						\" do indent
.if !\\$1\\*(I! \{\
.br
.sp .5v                           	\" make sure we're at the start of a line
.ti 0							\" major is always at left margin
\&\\$1\c\"						\" and - output "Major"
.ds I! \\$1\"					\" save major name
.	rm I@
'	br \}
.ie \\$2\\*(I@ \{\
.	ie \\$4 , \\$3\c\"	\" same minor - just output page #
.	el , \fB\\$3\fP\c\"			\"     highlight version
'	br \}
.el \{\
.	br							\" make sure we're on a new line
.	fl
.	ie \\$4 \&\\$2, \&\\$3\c\"	and output "Minor, page#"
.	el \&\\$2, \fB\\$3\fP\c\"	\"     highlight version
.	ds I@ \\$2\"				\" save minor name
'	br \}
..
...\"
...\"	the string I! is used to hold the last major index name
.ds I!
...\"
...\"	the string I@ is used to hold the last minor index name
.ds I@
...\"
...\"	the register I# is used to hold the current indent
.nr I# .5i
...\"
...\"	to get around formatter and sorting limitations:
'fl
...\"
...\"
...\"
...\"	do everything in two column mode:
.2C
.rs
.nh			\" NO hyphenation
.na			\" no justification
.nr Hu 1
.HU "Index"
.tr ~-^\&\"	\" tilde becomes a dash, hat goes to null
.Ix ".mwmrc" "changing" "3~4" ""
.Iy "A"
.Ix "accelerator" "menu" "2~20" ""
.Ix "active window" "" "1~5" ""
.Ix "adding menu selections" "" "3~4" ""
.Ix "arrow buttons" "" "2~4" ""
.Ix "available" "colors" "A~1" ""
.Ix "available" "fonts" "B~1" ""
.Iy "B"
.Ix "behavior" "default" "viii" ""
.Ix "behavior" "default" "xi" ""
.Ix "behavior" "switching" "xi" ""
.Ix "buttons" "mouse" "x" ""
.Iy "C"
.Ix "cascade menu" "" "2~12" ""
.Ix "changing" "fonts" "B~1" ""
.Ix "check boxes" "" "2~14" ""
.Ix "choosing from a menu" "" "1~14" ""
.Ix "click" "" "1~4" ""
.Ix "clients" "for window management" "C~1" ""
.Ix "colors" "available" "A~1" ""
.Ix "colors" "changing" "3~2" ""
.Ix "colors" "changing" "A~1" ""
.Ix "colors" "default" "A~1" ""
.Ix "COLUMNS environment variable" "" "C~1" ""
.Ix "commands" "quitting Motif" "1~18" ""
.Ix "configuration files" "" "3~1" ""
.Ix "controls" "common" "2~3" ""
.Ix "conventions" "" "x" ""
.Ix "custom button" "" "x" ""
.Ix "customizing" "colors" "3~2" ""
.Ix "customizing" "colors" "A~1" ""
.Ix "customizing" "fonts" "3~2" ""
.Ix "customizing" "windows" "B~1" ""
.Iy "D"
.Ix "deleting menu selections" "" "3~4" ""
.Ix "displaying the window menu" "" "1~14" ""
.Ix "documentation" "Motif" "xii" ""
.Ix "double~click" "" "1~4" ""
.Ix "double~clicking" "" "2~7" ""
.Ix "drag" "" "1~4" ""
.Ix "dragging a window" "" "1~7" ""
.Ix "dragging the slider" "" "2~4" ""
.Ix "dragging the slider" "" "2~5" ""
.Iy "E"
.Ix "Edit menu" "" "2~11" ""
.Ix "enlarging a window" "" "1~9" ""
.Ix "entering text" "" "2~8" ""
.Ix "environment" "resetting variables" "C~1" ""
.Ix "error messages" "reacting to" "2~18" ""
.Ix "exiting Motif" "" "1~18" ""
.Iy "F"
.Ix "File menu" "" "2~10" ""
.Ix "File menu" "" "2~16" ""
.Ix "file" "saving" "2~16" ""
.Ix "fonts" "" "B~1" ""
.Ix "fonts" "changing" "3~2" ""
.Ix "fonts" "list of" "B~1" ""
.Iy "G"
.Ix "grabbing the title bar" "" "1~6" ""
.Iy "H"
.Ix "hexadecimal value (color)" "" "A~1" ""
.Ix "how to use this guide" "" "ix" ""
.Iy "I"
.Ix "icons" "" "1~10" ""
.Ix "icons" "making" "1~10" ""
.Ix "icons" "moving" "1~11" ""
.Ix "icons" "normalizing" "1~12" ""
.Ix "input" "" "2~8" ""
.Iy "L"
.Ix "leaving Motif" "" "1~18" ""
.Ix "LINES environment variable" "" "C~1" ""
.Ix "list box" "" "2~3" ""
.Ix "list" "fonts" "B~1" ""
.Ix "list" "scrolling" "2~4" ""
.Ix "list" "selecting and item" "2~5" ""
.Ix "list" "using" "2~4" ""
.Ix "login" "" "viii" ""
.Iy "M"
.Ix "managing" "window manager" "3~4" ""
.Ix "managing" "windows" "C~1" ""
.Ix "menu bar" "" "2~3" ""
.Ix "menu button" "" "x" ""
.Ix "menu functions" "choosing" "1~14" ""
.Ix "menu functions" "undoing" "1~15" ""
.Ix "menu" "accelerator" "2~20" ""
.Ix "menu" "cascade" "2~12" ""
.Ix "menu" "mnemonic" "2~20" ""
.Ix "menu" "navigating with cursor keys" "2~21" ""
.Ix "menu" "short cuts" "2~20" ""
.Ix "menus" "adding selections" "3~4" ""
.Ix "menus" "deleting selections" "3~4" ""
.Ix "menus" "window" "1~14" ""
.Ix "messages" "reacting to" "2~18" ""
.Ix "mnemonic" "menu" "2~20" ""
.Ix "Motif documentation" "" "xii" ""
.Ix "Motif Window Manager" "" "1~2" ""
.Ix "Motif Window Manager" "" "viii" ""
.Ix "Motif" "" "vii" ""
.Ix "Motif" "quitting" "1~18" ""
.Ix "mouse button locations:" "" "x" ""
.Ix "mouse" "pointer" "1~4" ""
.Ix "mouse" "using" "1~4" ""
.Ix "moving a window" "" "1~6" ""
.Ix "moving a window" "" "1~7" ""
.Ix "moving an icon" "" "1~11" ""
.Ix "mre" "" "2~1" ""
.Ix "mre" "" "viii" ""
.Ix "mre" "controls" "2~3" ""
.Ix "mre" "starting" "2~2" ""
.Iy "N"
.Ix "normalizing" "" "1~12" ""
.Iy "O"
.Ix "OSF/Motif" "" "vii" ""
.Iy "P"
.Ix "palette button" "" "2~6" ""
.Ix "password" "" "viii" ""
.Ix "pointer shapes" "" "1~4" ""
.Ix "pointer" "" "1~6" ""
.Ix "programs" "starting" "3~6" ""
.Ix "pull~down menus" "" "2~10" ""
.Ix "push buttons" "" "2~3" ""
.Ix "push buttons" "" "2~6" ""
.Iy "Q"
.Ix "quitting Motif" "" "1~18" ""
.Iy "R"
.Ix "radio buttons" "" "2~14" ""
.Ix "redrawing the screen" "" "C~3" ""
.Ix "releasing a window" "" "1~7" ""
.Ix "repainting the screen" "" "C~3" ""
.Ix "resetting" "" "xi" ""
.Ix "resize" "" "C~1" ""
.Ix "resize" "when to use" "C~2" ""
.Ix "resizing windows" "" "1~8" ""
.Iy "S"
.Ix "saving a file" "" "2~16" ""
.Ix "screen" "repainting" "C~3" ""
.Ix "scroll bar" "" "2~3" ""
.Ix "scroll bar" "" "2~4" ""
.Ix "scrolling" "" "2~4" ""
.Ix "select button" "" "x" ""
.Ix "selecting from a menu" "" "1~14" ""
.Ix "short cuts" "menus" "2~20" ""
.Ix "short cuts" "menus" "2~22" ""
.Ix "shrinking a window" "" "1~9" ""
.Ix "slider" "dragging" "2~4" ""
.Ix "slider" "dragging" "2~5" ""
.Ix "starting clients" "" "3~6" ""
.Ix "stopping a window move" "" "1~7" ""
.Ix "stopping Motif" "" "1~18" ""
.Ix "sys.Xdefaults file" "" "2~2" ""
.Ix "system.mwmrc" "" "3~4" ""
.Iy "T"
.Ix "TERM environment variable" "" "C~1" ""
.Ix "terminal emulator" "" "1~6" ""
.Ix "text editor" "" "viii" ""
.Ix "text" "entering" "2~8" ""
.Ix "title bar" "" "1~6" ""
.Ix "toggle buttons" "" "2~14" ""
.Ix "type style" "" "B~1" ""
.Ix "typing text" "" "2~8" ""
.Ix "typographical conventions" "" "x" ""
.Iy "U"
.Ix "undoing a menu selection" "" "1~15" ""
.Iy "W"
.Ix "window management clients" "" "C~1" ""
.Ix "window manager" "modifying" "3~4" ""
.Ix "window menu" "" "1~14" ""
.Ix "window" "active" "1~5" ""
.Ix "window" "root" "1~4" ""
.Ix "windows" "changing sizes" "1~8" ""
.Ix "windows" "changing to icons" "1~10" ""
.Ix "windows" "customizing" "B~1" ""
.Ix "windows" "moving" "1~6" ""
.Ix "windows" "moving" "1~7" ""
.Ix "windows" "normalizing from icons" "1~12" ""
.Ix "windows" "operations control" "1~14" ""
.Iy "X"
.Ix "X Window System" "" "viii" ""
.Ix "xrefresh" "" "C~3" ""
...\"	index formatting macros
...\"
...\"     this simply restores one column text
.1C
