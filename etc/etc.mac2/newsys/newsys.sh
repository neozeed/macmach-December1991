#! /bin/sh

# /etc/newsys for MacMach BSD 4.3, version 01

echo "MacMach New System Installation / Update"

# This script installs systems.  The basic operation is to prepare a local
# disk and then install files on it from some other source.  This script is
# usually run from a small ramdisk-based boot system.
# See /usr/src/etc/etc.mac2/newconfig/rc.ramdisk for more information.

# The command line arguments are:
METHOD="$1"
SCRIPT="$2"
ADDR="$3"
GATEWAY="$4"
SCSIID="$5"
DESTDIR="$6"
SRCDIR="$7"
MODE="$8"
MINDISK="$9"

# METHOD:  the installation method: DISK or FTP
# SCRIPT:  path to update script -- selects what type of system to make
# ADDR:    network address of new system
# GATEWAY: network gateway address
# SCSIID:  disk address (* = search for disk/partition)
# DESTDIR: where new disk should be mounted (usually /mnt)
# SRCDIR:  where source file system is mounted
# MODE:    install or update
# MINDISK: minimum number of blocks in disk *optional*
# SOURCE:  system source directory, if sources are to be installed *optional*

# If arguments are not given, the user will be prompted for any needed
# information.

# When using FTP, three shell environment variables may be set up:
# FTP_SERVER: the network address of the server
# FTP_USER:   the user to login as
# FTP_PASSWD: the password for $FTP_USER
# If they are not set up, the user will be prompted.

# These update scripts reside in /etc/newconfig.
# disk.macmach -- do a standard installation from local disk
# ftp.macmach  -- do a standard installation (or update) via FTP

# choose update or install
while [ ! "$MODE" ]; do
  echo ""
  echo "A full installation will erase and rebuild all file systems while an"
  echo "update will only install new versions of changed files in existing"
  echo "file systems."
  echo -n "Do you wish to update or install? "
  read MODE
  case "$MODE" in
    update|UPDATE|Update) MODE="UPDATE";;
    install|INSTALL|Install) MODE="INSTALL";;
    *) MODE="";;
  esac
done
case "$MODE" in
  update|UPDATE|Update) MODE="update"; UPDATE="YES"; INSTALL="";;
  install|INSTALL|Install) MODE="installation"; UPDATE=""; INSTALL="YES";;
  *) echo "ERROR: invalid mode: \"$MODE\""
    echo "Valid modes are: INSTALL or UPDATE"
    EXIT=1; exit $EXIT;;
esac

# prompt for installation/update method
DEFAULT="DISK"
while [ ! "$METHOD" ]; do
  echo ""
  echo "Possible $MODE methods are: DISK FTP"
  echo -n "Enter the $MODE method: [$DEFAULT] "
  read METHOD; [ "$METHOD" ] || METHOD="$DEFAULT"
  case "$METHOD" in
    disk|DISK) ;;
    ftp|FTP) ;;
    *) METHOD="";;
  esac
done

# normalize METHOD, set flags
NETWORK=""
FTP=""
case "$METHOD" in
  disk|DISK) METHOD="DISK"; NETWORK=""; FTP="";;
  ftp|FTP) METHOD="FTP"; NETWORK="YES"; FTP="YES";;
  *) echo "ERROR: invalid method: \"$METHOD\""
    echo "Valid methods are: DISK or FTP"
    EXIT=1; exit $EXIT;;
esac
	
# prompt for network address
while [ ! "$ADDR" ]; do
  echo ""
  echo "Network addresses MUST be assigned by the network administrator."
  echo "The network address for the target system will be used by the"
  echo "installing system if its network is not already enabled."
  echo "The /etc/hosts and /.hostname files on the target machine will"
  echo "have to be updated if the address is not already in the host table."
  echo -n "Enter the network address of this system: "
  read ADDR
done
case "$ADDR" in
  none|NONE|None) ADDR="NONE";;
esac

# get gateway address
[ ! "$GATEWAY" -a "$ADDR" = "NONE" ] && GATEWAY="NONE"
while [ ! "$GATEWAY" ]; do
  echo ""
  echo "A gateway is used to contact systems outside of the local network."
  echo -n "Enter the GATEWAY network address: [NONE] "
  read GATEWAY; [ "$GATEWAY" ] || GATEWAY="NONE"
done
case "$GATEWAY" in
  none|NONE|None) GATEWAY="NONE";;
esac

# prompt for extra FTP information
[ "$METHOD" = "FTP" ] && {
  while [ ! "$FTP_SERVER" ]; do
    echo ""
    echo -n "Enter FTP server address: "; read FTP_SERVER
  done
  while [ ! "$FTP_USER" ]; do
    echo ""
    echo -n "Enter FTP user name: "; read FTP_USER
  done
  while [ ! "$FTP_PASSWD" ]; do
    echo ""
    FTP_PASSWD=`getpass "Enter password for FTP user \"$FTP_USER\": "`
  done
}

# prompt for source directory
case "$METHOD" in
  DISK) DEFAULT="/";;
  FTP) DEFAULT="/";;
esac
ASKUSRSRC=""
while [ ! "$SRCDIR" ]; do
  echo ""
  echo "The system files exist in a source directory on the file server"
  echo "or local disk."
  echo -n "Enter the source directory: [$DEFAULT] "
  read SRCDIR; [ "$SRCDIR" ] || SRCDIR="$DEFAULT"
  ASKUSRSRC="YES"
done
# note that USRSRC may already exist as a non-null environment variable
if [ "$ASKUSRSRC" -a ! "$USRSRC" ]; then
  echo ""
  echo "Normally, system sources are not included in an installation."
  echo "They take up a lot of space and are protected under various"
  echo "copyrights.  If you have the space, and are allowed to, you"
  echo "can have /usr/src installed/updated at this time."
  while true; do
    echo -n "Ok to include /usr/src? [NO] "
    read USRSRC; [ "$USRSRC" ] || USRSRC="NO"
    case "$USRSRC" in
      Y|y|YES|yes) USRSRC="YES"; break;;
      N|n|NO|no) USRSRC=""; break;;
      *) echo "Please enter YES or NO.";;
    esac
  done
fi
[ "$USRSRC" ] && export USRSRC

# prompt for update script path
case "$METHOD" in
  DISK) DEFAULT="$SRCDIR/etc/newconfig/disk.macmach";;
  FTP) DEFAULT="$SRCDIR/etc/newconfig/ftp.macmach";;
esac
while [ ! "$SCRIPT" ]; do
  echo ""
  echo "An update script will do the actual $METHOD file copy."
  echo -n "Enter the path to the $METHOD update script: [$DEFAULT] "
  read SCRIPT; [ "$SCRIPT" ] || SCRIPT="$DEFAULT"
done

# prompt for SCSI disk ID
DEFAULT="SEARCH"
while [ ! "$SCSIID" ]; do
  echo ""
  echo "The target disk must have an Apple partition of one of these types:"
  echo "  Apple_Free"
  echo "  Mach_UNIX_BSD4.3"
  echo "An old Apple partition of type "MACH" is also accepted."
  echo "The installation software can search all disks for usable partitions."
  echo -n "Enter the SCSI ID # of the target disk drive: [$DEFAULT] "
  read SCSIID; [ "$SCSIID" ] || SCSIID="$DEFAULT"
done
case "$SCSIID" in
  SEARCH|Search) SCSIID="search";;
esac

# prompt for destination directory
DEFAULT="/mnt"
while [ ! "$DESTDIR" ]; do
  echo ""
  echo "The target disk will be mounted as a local directory during the"
  echo "$MODE process.  This will be the destination directory."
  echo -n "Enter the root of the new file system: [$DEFAULT] "
  read DESTDIR; [ "$DESTDIR" ] || DESTDIR="$DEFAULT"
done
[ -d $DESTDIR ] || {
  echo "ERROR: destination directory does not exist: \"$DESTDIR\""
  EXIT=1; exit $EXIT
}

# confirm that all parameters are correct
while true; do
  echo ""
  echo "*** $METHOD $MODE parameters ***"
  [ "$FTP" ] && {
    echo "FTP server address:   $FTP_SERVER"
    echo "FTP user:             $FTP_USER"
    echo "FTP passwd:"
  }
  echo "Update script:        $SCRIPT"
  echo "Network address:      $ADDR"
  echo "Gateway address:      $GATEWAY"
  echo "SCSI Disk address:    $SCSIID"
  [ "$MINDISK" ] && echo "Minimum disk blocks:  $MINDISK"
  echo "New file system root: $DESTDIR"
  echo "File system source:   $SRCDIR"
  [ "$USRSRC" ] && echo "System sources included."
  echo ""
  [ "$INSTALL" ] &&
    echo "WARNING: Installation will erase any existing Unix file system."
  echo -n "Ok to proceed with $MODE? "
  read YES
  case "$YES" in
    Y|y|YES|yes) break;;
    N|n|NO|no) EXIT=1; exit $EXIT;;
    *) echo "Please enter YES or NO.";;
  esac
done

# prepare disk partition
echo ""
SCSIID="`mac2part $SCSIID $MINDISK`"
[ "$SCSIID" ] || {
  echo "ERROR: mac2part failed"
  echo "Can not determine SCSI disk ID."
  EXIT=1; exit $EXIT
}

# make and mount new file system
umount /dev/sd$SCSIID"g" >/dev/null 2>&1
umount /dev/sd$SCSIID"a" >/dev/null 2>&1
if [ "$INSTALL" ]; then
  echo ""
  echo "Making new root file system on disk $SCSIID."
  newfs /dev/rsd$SCSIID"a"
  echo ""
  echo "Making new usr file system on disk $SCSIID."
  newfs /dev/rsd$SCSIID"g"
else
  echo ""
  echo "fsck'ing existing root file system on disk $SCSIID."
  fsck /dev/sd$SCSIID"a" || {
    echo "ERROR: fsck failed"
    EXIT=1; exit $EXIT
  }
  echo ""
  echo "fsck'ing existing usr file system on disk $SCSIID."
  fsck /dev/sd$SCSIID"g" || {
    echo "ERROR: fsck failed"
    EXIT=1; exit $EXIT
  }
fi
echo ""
echo "Mounting file systems."
mount /dev/sd$SCSIID"a" $DESTDIR || {
  echo "ERROR: can not mount $DESTDIR file system"
  EXIT=1; exit $EXIT
}
[ -d $DESTDIR/lost+found ] || { cd $DESTDIR; mklost+found; }
[ -d $DESTDIR/usr ] || mkdir $DESTDIR/usr
mount /dev/sd$SCSIID"g" $DESTDIR/usr || {
  echo "ERROR: can not mount $DESTDIR/usr file system"
  EXIT=1; exit $EXIT
}
[ -d $DESTDIR/usr/lost+found ] || { cd $DESTDIR/usr; mklost+found; }

# Note that /tmp and /usr on a ramdisk are setup as symbolic
# links to $DESTDIR/tmp and $DESTDIR/usr, make these directories now
[ -d $DESTDIR/tmp ] || mkdir $DESTDIR/tmp; chmod 777 $DESTDIR/tmp
[ -d $DESTDIR/usr/tmp ] || mkdir $DESTDIR/usr/tmp; chmod 777 $DESTDIR/usr/tmp

# start up network if needed
[ "$NETWORK" ] && {
  # if network not up, bring up using specified address
  expr "`ifconfig en0`" : ".*UP" >/dev/null 2>&1 || {
    echo ""
    echo "Starting network access for $ADDR."
    ifconfig en0 $ADDR up arp -trailers || {
      echo "ERROR: ifconfig failed for address \"$ADDR\""
      echo "This should not have happened!"
      EXIT=1; exit $EXIT
    }
    [ "$GATEWAY" != "NONE" ] && {
      route -f -n add 0 "$GATEWAY" 1 || {
        echo "ERROR: route failed"
        EXIT=1; exit $EXIT
      }
    }
  }
  [ "$ADDR" ] || {
    echo "ERROR: No network address."
    EXIT=1; exit $EXIT
  }
}

# get update script from FTP server if needed
[ "$FTP" ] && {
  echo ""
  echo "Loading installation script from ftp server."
  ( echo "user $FTP_USER $FTP_PASSWD"
    echo "get $SCRIPT /usr/tmp/ftp.script"
    echo "bye"
  ) | ftp -n -i $FTP_SERVER
  SCRIPT="/usr/tmp/ftp.script"
  export FTP_SERVER FTP_USER FTP_PASSWD
}

# continue execution in update script
# note that the following are passwd as environment
# variables: FTP_SERVER, FTP_USER, FTP_PASSWD and USRSRC
[ -r "$SCRIPT" ] && \
  exec sh $SCRIPT $ADDR $GATEWAY $SCSIID $DESTDIR $SRCDIR
echo "ERROR: can not read update script: \"$SCRIPT\""
EXIT=1; exit $EXIT
