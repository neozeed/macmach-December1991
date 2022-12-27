#!/bin/sh

/**/# If environment variable TWMMOTIF is non-null, configure TWM to look a bit
/**/# like Motif, otherwise, make it Mac-like.

# configuration files created in the users home directory
STARTX=$HOME/.startx.`hostname`
XINITRC=$HOME/.startx.xinitrc.`hostname`
TWMRC=$HOME/.startx.twmrc.`hostname`
XTERMX=$HOME/.startx.xterm.`hostname`
EMACSX=$HOME/.startx.emacs.`hostname`

[ $# = 0 -a -f $STARTX ] && exec $STARTX

[ $# = 0 ] && {
cat <<'@EOF@'
This is the MacMach X Window startup script.  Give one argument per display.
The first argument is for /dev/scn0, then second for /dev/scn1, etc.
  small -- a small black and white display
  color -- a 13" color display
  grey  -- a 12" grey-scale display
  1page -- a one-page black and white display
  2page -- a 2-page black and white display
@EOF@
exit 1
}

# standard X11 programs
XSETROOT=/usr/bin/X11/xsetroot
XCLOCK=/usr/bin/X11/xclock
XMAN=/usr/bin/X11/xman
XCALC=/usr/bin/X11/xcalc
XINIT=/usr/bin/X11/xinit
XTERM=/usr/bin/X11/xterm
EMACS=/usr/bin/emacs
TWM=/usr/bin/X11/twm
X=/usr/bin/X11/X

/**/###########################################################################
/**/# check arguments, create temporary file of display arguments

DISPLAYARGS=/tmp/DISPLAYARGS$$
rm -f $DISPLAYARGS

SERVERARGS='-s 10 -p 10 -v'
I=0
for ARG in $*; do
  DISPLAY="unix:0.$I"
  DEV="/dev/scn$I"
  case "$ARG" in
    small)
      DEVARG=""
      echo "$DEV $ARG $DISPLAY 6x10 =80x24+0+0" >>$DISPLAYARGS
      ;;
    color)
      DEVARG="-depth 8"
      echo "$DEV $ARG $DISPLAY 6x13 =80x28+0+0" >>$DISPLAYARGS
      ;;
    grey)
      DEVARG="-depth 8 -cc 1"
      echo "$DEV $ARG $DISPLAY 6x13 =80x28+0+0" >>$DISPLAYARGS
      ;;
    1page)
      DEVARG=""
      echo "$DEV $ARG $DISPLAY 6x13 =80x55+0+0" >>$DISPLAYARGS
      ;;
    2page)
      DEVARG=""
      echo "$DEV $ARG $DISPLAY 12x24 =80x30+0+0" >>$DISPLAYARGS
      ;;
    *)
      echo "Incorrect display code \"$ARG\" for $DEV."
      sh $0
      rm -f $DISPLAYARGS
      exit 1
      ;;
  esac
  DISPLAYS="$DISPLAY $DISPLAYS"
  SERVERARGS="-dev $DEV $DEVARG $SERVERARGS"
  I=`expr $I + 1`
done

if [ "$TWMMOTIF" ]; then
  echo "TWM is Motif Style."
else
  echo "TWM is Mac Style."
fi

/**/###########################################################################
/**/# create xterm launch script

echo "Creating $XTERMX"
NAME='-n xterm[$$] -T $USER@`hostname`[$$]'
ARGS="-sb -sl 100 $NAME -vb -wf -bg wheat"
{
  echo '[ "$1" ] && DISPLAY="$1"'
  echo 'case $DISPLAY in'
  cat $DISPLAYARGS | while read DEV NAME DISPLAY FONT GEOMETRY; do
    echo "  $DISPLAY) exec $XTERM -display $DISPLAY -fn $FONT $ARGS -geometry $GEOMETRY;;"
  done
  echo 'esac'
} >$XTERMX
chmod 700 $XTERMX

/**/###########################################################################
/**/# create xterm launch script

echo "Creating $EMACSX"
{
  echo 'case $DISPLAY in'
  cat $DISPLAYARGS | while read DEV NAME DISPLAY FONT GEOMETRY; do
    echo "  $DISPLAY) exec $EMACS -fn $FONT -geometry $GEOMETRY;;"
  done
  echo 'esac'
} >$EMACSX
chmod 700 $EMACSX

/**/###########################################################################
/**/# create xinitrc script

echo "Creating $XINITRC"

{
  echo "#!/bin/sh"
  echo "# xinit script created by startx"
} >$XINITRC

cat $DISPLAYARGS | while read DEV NAME DISPLAY FONT GEOMETRY; do
  echo "# $DEV is $NAME display $DISPLAY"
done >>$XINITRC

/**/# keep small console window on default display forever
LOGFILE='/tmp/console.log'
NAME='-n Console -T "Console: root@`hostname`"'
FN='-fn 6x10'
GEOMETRY='-geometry =80x5+0-0'
COLOR='-bg coral'
ARGS="-lf $LOGFILE -l $FN -sb -sl 100 $NAME -vb -wf $COLOR $GEOMETRY"
{
  echo "rm -f $LOGFILE"
  echo "while true; do "
  echo "  $XTERM -C $ARGS -e su"
  echo "done &"
} >>$XINITRC

/**/# do xsetroot for each display
ARGS="-solid black"
[ "$ARGS" ] && for DISPLAY in $DISPLAYS; do
  echo "$XSETROOT -display $DISPLAY $ARGS"
done >>$XINITRC

/**/# put an xclock on each display
COLORS="-fg white -bg black -hd white -hl white -bd black"
GEOMETRY="-geometry =60x60-0+24"
ARGS="$COLORS $GEOMETRY"
[ "$ARGS" ] && for DISPLAY in $DISPLAYS; do
  echo "$XCLOCK -display $DISPLAY $ARGS &"
done >>$XINITRC

/**/# start users xterm(s)
cat $DISPLAYARGS | while read DEV NAME DISPLAY FONT GEOMETRY; do
  echo "$XTERMX $DISPLAY &"
done >> $XINITRC

/**/# start up the window manager
echo "exec $TWM -f $TWMRC" >>$XINITRC

/**/###########################################################################
/**/# create twmrc

echo "Creating $TWMRC"

{
  echo "# TWM configuration created by startx"
  cat $DISPLAYARGS | while read DEV NAME DISPLAY FONT GEOMETRY; do
    echo "# $DEV is $NAME display $DISPLAY"
  done

} >$TWMRC

if [ "$TWMMOTIF" ]; then

cat >>$TWMRC <<'@EOF@'

MenuFont	"6x13"
TitleFont	"6x13B"
BorderWidth	2
ButtonIndent	0
TitleButtonBorderWidth	1
IconifyByUnmapping
NoDefaults
NoRaiseOnMove
NoRaiseOnResize
NoIconManagers
Cursors
{
	Menu	"top_left_arrow"
	Destroy	"dot"
	Button	"top_left_arrow"
}
Color
{
}

/**/# functions for motion-based actions.
MoveDelta 3
Function "move-or-iconify" { f.move f.deltastop f.iconify }

Button1 =	: root		: f.menu "applications"
Button1 = meta	: root		: f.menu "TwmWindows"
Button1 =	: title		: f.move
Button1 = meta	: title		: f.raise
Button1 = meta	: window	: f.raise
Button1 =	: icon		: f.function "move-or-iconify"

Button2 =	: root		: f.menu "session"
Button2 = meta	: root		: f.menu "TwmWindows"
Button2	= meta	: window	: f.move

Button3 =	: root		: f.menu "window"
Button3 = meta	: root		: f.menu "window"
Button3 = meta	: window	: f.menu "window"

LeftTitleButton		":xlogo" = f.menu "window"
RightTitleButton	":resize" = f.menu "zoom"
RightTitleButton	":xlogo" = f.menu "window"

StartIconified
{
}

NoTitle
{
	"xclock"
	"xload"
	"topBox"
	"oclock"
}

menu "applications"
{
"Applications"	f.title
@EOF@

{
  echo "\"XTerm\"		!\"$XTERMX &\""
  echo "\"Emacs\"		!\"$EMACSX &\""
  echo "\"Manual Pages\"	!\"$XMAN &\""
} >>$TWMRC

cat >>$TWMRC <<'@EOF@'
}

menu "session"
{
"Session Ops"	f.title
"Refresh All"	f.refresh
"Restart"	f.twmrc
"Exit"		f.quit
}

menu "window"
{
"Window Ops"	f.title
"Hide"		f.iconify
"Move"		f.move
"Resize"	f.resize
"Raise"		f.raise
"Lower"		f.lower
"Identify"	f.identify
"Close"		f.destroy
}

menu "zoom"
{
"Zoom Window"	f.title
"Full"		f.fullzoom
"Tall"		f.zoom
"Wide"		f.horizoom
"Left"		f.leftzoom
"Right"		f.rightzoom
"Top"		f.topzoom
"Bottom"	f.bottomzoom
}

@EOF@

else

cat >>$TWMRC <<'@EOF@'

MenuFont	"6x13"
TitleFont	"6x13B"
IconFont	"6x13"
ResizeFont	"6x13"
BorderWidth	2
ButtonIndent	0
TitleButtonBorderWidth	1
Cursors
{
  Menu    "top_left_arrow"
  Destroy "dot"
  Button  "top_left_arrow"
}
Color
{
  BorderColor "slategrey"
  DefaultBackground "maroon"
  DefaultForeground "gray85"
  TitleBackground "maroon"
  TitleForeground "gray85"
  MenuBackground "maroon"
  MenuForeground "gray85"
  MenuTitleBackground "gray70"
  MenuTitleForeground "maroon"
  IconBackground "maroon"
  IconForeground "gray85"
  IconBorderColor "gray85"
  IconManagerBackground "maroon"
  IconManagerForeground "gray85"
}

MoveDelta 3
Function "move-or-raise" { f.move f.deltastop f.raise }
Function "move-or-iconify" { f.move f.deltastop f.iconify }

StartIconified
{
}

NoTitle
{
  "xclock"
  "xload"
  "topBox"
  "oclock"
}

Button1 =      : root    : f.menu "main"
Button1 =  m   : root    : f.menu "main"
Button1 =  m   : window  : f.menu "main"
Button1 =      : title   : f.function "move-or-raise"
Button1 =  m   : title   : f.menu "main"
Button1 =      : icon    : f.function "move-or-iconify"
Button1 =  m   : icon    : f.menu "main"
Button1 =      : iconmgr : f.iconify
Button1 =  m   : iconmgr : f.menu "main"

menu "main"
{
"MacMach (twm)"	f.title
"kill"	 	f.destroy
"refresh"	f.refresh
@EOF@

{
  echo "\"xterm\"		!\"$XTERMX &\""
  echo "\"emacs\"		!\"$EMACSX &\""
  echo "\"calculator\"	!\"$XCALC &\""
  echo "\"man\"		!\"$XMAN &\""
} >>$TWMRC

cat >>$TWMRC <<'@EOF@'
"other"		f.menu "more"
"exit"		f.quit
}

menu "more"
{
"-other stuff-"	f.title
"focus"		f.focus
"unfocus"	f.unfocus
"iconify"	f.iconify
"move"		f.move
"raise"		f.raise
"lower"		f.lower
"resize"	f.resize
"delete"	f.delete
"iconmgr on"	f.showiconmgr
"iconmgf off"	f.hideiconmgr
"restart"	f.twmrc
"exit"		f.quit
}

@EOF@

fi

/**/###########################################################################
/**/# create startx

echo "Creating $STARTX"

{
  echo "#! /bin/sh"
  echo "[ `tty` = /dev/console ] || { echo \"tty is not console\"; exit 1; }"
  echo "for F in $X $TWM $XTERM $EMACS $XINITRC $TWMRC; do"
  echo "  [ -f \$F ] || { echo \"Missing \$F\"; exit 1; }"
  echo "done"
  cat $DISPLAYARGS | while read DEV NAME DISPLAY FONT GEOMETRY; do
    echo "echo \"Screen $DEV is $NAME display $DISPLAY\""
  done
  echo 'DISPLAY="unix:0.0"'
  echo 'export DISPLAY'
  echo "echo \"Starting $X and $TWM\""
  echo "exec $XINIT $XINITRC -- $X $SERVERARGS"
} >$STARTX
chmod 700 $STARTX

/**/###########################################################################
/**/# all done

rm -f $DISPLAYARGS

exec $STARTX
