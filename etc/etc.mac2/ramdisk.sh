#! /bin/sh

# /usr/src/ramdisk.sh

# This shell script makes a ramdisk image.

# See /usr/src/etc/etc.mac2/newconfig/README.INSTALL for installation
# documentation

SRC="`pwd`/../.."
DIR="/usr/tmp/ramdisk_dir"
MNT="/usr/tmp/ramdisk_mnt"
IMAGE="/usr/tmp/ramdisk_image"

# This is the size, in 512 byte sectors, of the ramdisk image.
# This MUST agree with "mac2ramdisk" in /etc/disktab.
SIZE=1536
BYTES=`expr $SIZE \* 512`

echo -n "Enter scratch disk SCSIID: "; read SCSIID

DEVICE=/dev/sd"$SCSIID"a
RDEVICE=/dev/rsd"$SCSIID"a

echo "MACMACH RAMDISK BUILDER"
echo ""
echo "SRC =        $SRC"
echo "IMAGE =      $IMAGE"
echo "DIR =        $DIR"
echo "MNT =        $MNT"

if [ "$SCSIID" ]; then
  echo ""
  echo "DEVICE =     $DEVICE"
  df $DEVICE
  echo "WARNING: This operation will trash any file system on disk $SCSIID"
  echo "WARNING: If any file system on disk $SCSIID is currently mounted,"
  echo "         the following operation may crash the system."
  while true; do
    echo -n "Ok to proceed? "
    read YES
    case $YES in
      Y|y|YES|yes) break
        ;;
      N|n|NO|no) exit 0
        ;;
      *) echo "Please enter YES or NO."
        ;;
    esac
  done
fi

if [ ! -d $DIR ]; then
  echo "Creating ramdisk directory: $DIR"
  set -x
  mkdir $DIR
  cd $DIR
  mkdir mnt disk bin etc dev
  chmod 755 mnt disk bin etc dev
  sh $SRC/etc/etc.mac2/makedev/MAKEDEV.sh $DIR/dev
  ( cd $DIR/dev
    rm keyboard mouse
    rm ptyp5 ptyp6 ptyp7 ptyp8 ptyp9 ptypa ptypb ptypc ptypd ptype ptypf
    rm rsd0b rsd0d rsd0e rsd0f rsd1b rsd1d rsd1e rsd1f rsd2b rsd2d rsd2e rsd2f
    rm rsd3b rsd3d rsd3e rsd3f rsd4b rsd4d rsd4e rsd4f rsd5b rsd5d rsd5e rsd5f
    rm rsd6b rsd6d rsd6e rsd6f
    rm sd0b sd0d sd0e sd0f sd1b sd1d sd1e sd1f sd2b sd2d sd2e sd2f
    rm sd3b sd3d sd3e sd3f sd4b sd4d sd4e sd4f sd5b sd5d sd5e sd5f
    rm sd6b sd6d sd6e sd6f
    rm ttyp5 ttyp6 ttyp7 ttyp8 ttyp9 ttypa ttypb ttypc ttypd ttype ttypf
  )
  ln -s /mnt/tmp tmp
  ln -s /mnt/usr usr
  chmod 755 mnt disk bin etc
  cd $SRC/bin/true
  install -c -m 555 -o root -g bin true.sh $DIR/bin/true
  cd $SRC/bin/false
  install -c -m 555 -o root -g bin false.sh $DIR/bin/false
  cd $SRC/bin/cp
  cc -n -O cp.c -o cp
  install -s -m 555 -o root -g bin cp $DIR/bin
  cd $SRC/bin/ls
  cc -n -O ls.c -o ls
  install -s -m 555 -o root -g bin ls $DIR/bin
  cd $SRC/bin/dd
  cc -n -O dd.c -o dd
  install -s -m 555 -o root -g bin dd $DIR/bin
  cd $SRC/bin/pwd
  cc -n -O pwd.c -o pwd
  install -s -m 555 -o root -g bin pwd $DIR/bin
  cd $SRC/bin/cmp
  cc -n -O cmp.c -o cmp
  install -s -m 555 -o root -g bin cmp $DIR/bin
  cd $SRC/bin/ln
  cc -n -O ln.c -o ln
  install -s -m 555 -o root -g bin ln $DIR/bin
  cd $SRC/bin/rm
  cc -n -O rm.c -o rm
  install -s -m 555 -o root -g bin rm $DIR/bin
  cd $SRC/bin/mkdir
  cc -n -O mkdir.c -o mkdir
  install -s -m 555 -o root -g bin mkdir $DIR/bin
  cd $SRC/bin/cat
  cc -n -O cat.c -o cat
  install -s -m 555 -o root -g bin cat $DIR/bin
  cd $SRC/bin/sync
  cc -n -O sync.c -o sync
  install -s -m 555 -o root -g bin sync $DIR/bin
  cd $SRC/bin/stty
  cc -n -O stty.c -o stty
  install -s -m 555 -o root -g bin stty $DIR/bin
  cd $SRC/bin/echo
  cc -n -O echo.c -o echo
  install -s -m 555 -o root -g bin echo $DIR/bin
  cd $SRC/bin/chmod
  cc -n -O chmod.c -o chmod
  install -s -m 555 -o root -g bin chmod $DIR/bin
  cd $SRC/bin/ed
  cc -n -O ed.c -o ed
  install -s -m 555 -o root -g bin ed $DIR/bin
  cd $SRC/bin/df
  cc -n -O df.c -o df
  install -s -m 555 -o root -g bin df $DIR/bin
  cd $SRC/bin/test
  cc -n -O test.c -o test
  install -s -m 555 -o root -g bin test $DIR/bin
  rm -f $DIR/bin/[
  ln $DIR/bin/test $DIR/bin/[
  cd $SRC/bin/expr
  make expr.c
  cc -n -O expr.c -o expr
  install -s -m 555 -o root -g bin expr $DIR/bin
  rm expr.c
  cd $SRC/bin/sh
  make clean; make CFLAGS="-n -O -w" sh
  install -s -m 555 -o root -g bin sh $DIR/bin
  make clean
  cd $SRC/usr/ucb/ftp
  make clean; make CFLAGS="-n -O -w" ftp
  install -s -m 555 -o root -g bin ftp $DIR/bin
  make clean
  cd $SRC/etc/mklost+found
  install -c -m 555 -o root -g bin mklost+found.sh $DIR/etc/mklost+found
  cd $SRC/etc/init
  cc -n -O init.c -o init
  install -s -m 555 -o root -g bin init $DIR/etc
  cd $SRC/etc/mknod
  cc -n -O mknod.c -o mknod
  install -s -m 555 -o root -g bin mknod $DIR/etc
  cd $SRC/etc/mount
  cc -n -O mount.c -o mount
  install -s -m 555 -o root -g bin mount $DIR/etc
  cc -n -O umount.c -o umount
  install -s -m 555 -o root -g bin umount $DIR/etc
  cd $SRC/etc/ifconfig
  cc -n -O ifconfig.c -o ifconfig
  install -s -m 555 -o root -g bin ifconfig $DIR/etc
  install -c -m 444 -o root -g bin services $DIR/etc
  cd $SRC/etc/route
  cc -n -O route.c -o route
  install -s -m 555 -o root -g bin route $DIR/etc
  cd $SRC/etc/reboot
  cc -n -O reboot.c -o reboot
  install -s -m 555 -o root -g bin reboot $DIR/etc
  cd $SRC/etc/fsck
  make clean; make CFLAGS="-n -O" fsck
  install -s -m 555 -o root -g bin fsck $DIR/etc
  make clean
  cd $SRC/etc/newfs
  make clean; make CFLAGS="-n -O" newfs
  install -s -m 555 -o root -g bin newfs $DIR/etc
  make clean
  cd $SRC/etc/etc.mac2/sony
  cc -n -O -DSMALL sony.c -o sony
  install -s -m 555 -o root -g bin sony $DIR/etc
  cd $SRC/etc/etc.mac2/newsys
  install -c -m 555 -o root -g bin newsys.sh $DIR/etc/newsys
  cd $SRC/etc/etc.mac2/mac2part
  cc -n -O -DSMALL mac2part.c -o mac2part
  install -s -m 555 -o root -g bin mac2part $DIR/etc
  cd $SRC/etc/getpass
  cc -n -O getpass.c -o getpass
  install -s -m 555 -o root -g bin getpass $DIR/etc
  set +x

cat >$DIR/etc/rc <<'@EOF@'
#! /bin/sh

RAMDISK_VERSION="Ramdisk Image version 1.8"

# system varieties
MACH2=/users/macmach/mach2
MACH3=/users/macmach/mach3
# 74MB is an 80MB disk with a minimum 2MB MacOS partition
# only a small (no motif or doc) system will fit on an 80MB disk
BINMIN="74"
SRCMIN="290"
METHOD="FTP"
ADDRESS=""
MODE=""
SCSIID="search"
DESTDIR="/mnt"

HOME="/"; export HOME
PATH="/bin:/etc"; export PATH
[ -f /dev/console ] || reboot -h
exec </dev/console >/dev/console 2>&1
stty -tabs crt kill '^u' intr '^c'

echo ""
echo "$RAMDISK_VERSION"
echo ""
echo "For the following questions, press return to use the default in []."
echo ""
echo "MacMach systems can be installed or updated over the network via FTP."
echo "MacMach requires at least ${BINMIN}MB of disk space, ${SRCMIN}MB if the system"
echo "sources are to be installed.  Use a standard Macintosh disk utility"
echo "to set aside the required space as a free partition before trying to"
echo "install a system.  Mach 3.0 systems also require an \"Apple_Scratch\""
echo "partition to page to.  This should be at least 20MB."

# SH is an undocumented way to get to a single-user shell for maintenance.

echo ""
while [ ! "$RELEASE" ]; do
  echo -n "Which release do you want, MACH2, or MACH3? "
  read RELEASE;
  case "$RELEASE" in
    mach2|MACH2|Mach2) RELEASE="$MACH2";;
    mach3|MACH3|Mach3) RELEASE="$MACH3";;
    other|OTHER|Other) RELEASE="OTHER";;
    sh|SH) echo ""; echo "Entering single user shell."; echo ""; exec /bin/sh;;
    *) RELEASE="";;
  esac
done

# "OTHER" is an undocumented way to specify a non-standard source path.
# Later on, newsys will prompt for installation script and source directory.

if [ "$RELEASE" = "OTHER" ]; then
  SCRIPT=""
  SRCDIR=""
  MINDISK=""
else
  SRCDIR="$RELEASE"
  SCRIPT="$RELEASE/etc/newconfig/ftp.macmach"
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
  export USRSRC
  if [ "$USRSRC" ]; then
    MINDISK=`expr \( $SRCMIN \* 1024 \* 1024 \) / 512`
  else
    MINDISK=`expr \( $BINMIN \* 1024 \* 1024 \) / 512`
  fi
fi

# SH is an undocumented way to get to a single-user shell for maintenance.

echo ""
newsys "$METHOD" "$SCRIPT" "$ADDRESS" "$GATEWAY" "$SCSIID" "$DESTDIR" "$SRCDIR" "$MODE" "$MINDISK" || {
  echo ""
  echo "Installation/update failed.  Type return to reboot."
  read SH
  case "$SH" in
    sh|SH) echo ""; echo "Entering single user shell."; echo ""; exec /bin/sh;;
  esac
}

echo ""
echo "Rebooting..."
reboot
@EOF@

chown root.bin $DIR/etc/rc
chmod 555 $DIR/etc/rc
cat >$DIR/.profile <<'@EOF@'
# ramdisk /.profile, simulates autoboot if booted single-user
PATH=/bin:/etc
export PATH
stty -tabs crt kill '^u' intr '^c'
exec /bin/sh /etc/rc
@EOF@
chown root.bin $DIR/.profile
chmod 444 $DIR/.profile

fi

if [ "$SCSIID" ]; then

echo "Creating ramdisk file system."
disklabel -w $RDEVICE mac2ramdisk
newfs $DEVICE

echo "Mounting ramdisk file system."
[ -d $MNT ] || mkdir $MNT
mount $DEVICE $MNT || {
  echo "mount failed"
  exit 1
}

echo "Copying ramdisk files."
rm -rf $MNT/*
(cd $DIR; find . -print | cpio -pduvm $MNT) || {
  echo "cpio failed"
  umount $DEVICE
  rm -rf $MNT
  exit 1
}

echo "Creating: $IMAGE"
df $DEVICE
umount $DEVICE
rm -rf $MNT
dd if=$RDEVICE of=$IMAGE.tmp bs=$BYTES count=1 || {
  echo "dd failed"
  rm -f $IMAGE.tmp
  exit 1
}

mv $IMAGE.tmp $IMAGE
echo "Operation complete."

fi
