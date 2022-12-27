#! /bin/sh

# This shell script starts up the Macintosh Emulator.

MACPATCHES=`wh -L macpatches`
MACSERVER=`wh -L macserver`

# Set console break so that console is reset when emulator terminates.
# This should be done inside the macserver.
[ `tty` = '/dev/console' ] && stty break

$MACSERVER $MACPATCHES
