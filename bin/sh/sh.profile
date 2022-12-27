# standard MacMach .profile
stty dec
PATH="/etc:/bin:/usr/ucb:/usr/bin:/usr/old:/usr/bin/X11"
[ "$USER" = "root" ] || PATH="$PATH:/usr/local/bin:/usr/games:$HOME/bin:./"
export PATH
TERM=""
eval "`mac2term -sh`"
[ "$TERM" ] || {
  echo -n "term: "
  read TERM
  [ "$TERM" ] || TERM="ansi"
  echo ""
}
PS1="${USER}# "
export PS1 TERM
