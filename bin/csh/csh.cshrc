# standard MacMach .cshrc
set path = ( /etc /bin /usr/ucb /usr/bin /usr/old /usr/bin/X11 )
if ("$USER" != 'root') then
  set path = ( $path /usr/local/bin /usr/games $HOME/bin ./ )
endif
stty dec
umask 22
set history = 100
set filec
set prompt = "${USER}% "
