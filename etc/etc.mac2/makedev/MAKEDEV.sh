#! /bin/sh

# make devices for MacMach

SAVEDDIR=`pwd`

if [ "$1" ]; then
  cd $1
else
  cd /dev
fi

while read DEVICE TYPE MAJOR MINOR OWNER GROUP MODE; do
  rm -f $DEVICE
  mknod $DEVICE $TYPE $MAJOR $MINOR
  chown ${OWNER}.${GROUP} $DEVICE
  chmod $MODE $DEVICE
  ls -l $DEVICE
done <<@EOF@
console c 0 0 root bin 666
tty c 2 0 root bin 666
kmem c 3 1 root kmem 640
null c 3 2 root bin 666
map c 3 3 root kmem 646
keyboard c 10 2 root bin 444
mouse c 10 3 root bin 444
scn0 c 5 0 root bin 666
scn1 c 5 1 root bin 666
scn2 c 5 2 root bin 666
scn3 c 5 3 root bin 666
sony0 c 9 1 root bin 666
sony1 c 9 2 root bin 666
modem c 11 0 root bin 600
printer c 11 1 root bin 600
rsd0x c 7 0 root operator 640
rsd1x c 7 32 root operator 640
rsd2x c 7 64 root operator 640
rsd3x c 7 96 root operator 640
rsd4x c 7 128 root operator 640
rsd5x c 7 160 root operator 640
rsd6x c 7 192 root operator 640
sd0a b 0 0 root operator 640
sd0b b 0 1 root operator 640
sd0c b 0 2 root operator 640
sd0d b 0 3 root operator 640
sd0e b 0 4 root operator 640
sd0f b 0 5 root operator 640
sd0g b 0 6 root operator 640
rsd0a c 6 0 root operator 640
rsd0b c 6 1 root operator 640
rsd0c c 6 2 root operator 640
rsd0d c 6 3 root operator 640
rsd0e c 6 4 root operator 640
rsd0f c 6 5 root operator 640
rsd0g c 6 6 root operator 640
sd1a b 0 32 root operator 640
sd1b b 0 33 root operator 640
sd1c b 0 34 root operator 640
sd1d b 0 35 root operator 640
sd1e b 0 36 root operator 640
sd1f b 0 37 root operator 640
sd1g b 0 38 root operator 640
rsd1a c 6 32 root operator 640
rsd1b c 6 33 root operator 640
rsd1c c 6 34 root operator 640
rsd1d c 6 35 root operator 640
rsd1e c 6 36 root operator 640
rsd1f c 6 37 root operator 640
rsd1g c 6 38 root operator 640
sd2a b 0 64 root operator 640
sd2b b 0 65 root operator 640
sd2c b 0 66 root operator 640
sd2d b 0 67 root operator 640
sd2e b 0 68 root operator 640
sd2f b 0 69 root operator 640
sd2g b 0 70 root operator 640
rsd2a c 6 64 root operator 640
rsd2b c 6 65 root operator 640
rsd2c c 6 66 root operator 640
rsd2d c 6 67 root operator 640
rsd2e c 6 68 root operator 640
rsd2f c 6 69 root operator 640
rsd2g c 6 70 root operator 640
sd3a b 0 96 root operator 640
sd3b b 0 97 root operator 640
sd3c b 0 98 root operator 640
sd3d b 0 99 root operator 640
sd3e b 0 100 root operator 640
sd3f b 0 101 root operator 640
sd3g b 0 102 root operator 640
rsd3a c 6 96 root operator 640
rsd3b c 6 97 root operator 640
rsd3c c 6 98 root operator 640
rsd3d c 6 99 root operator 640
rsd3e c 6 100 root operator 640
rsd3f c 6 101 root operator 640
rsd3g c 6 102 root operator 640
sd4a b 0 128 root operator 640
sd4b b 0 129 root operator 640
sd4c b 0 130 root operator 640
sd4d b 0 131 root operator 640
sd4e b 0 132 root operator 640
sd4f b 0 133 root operator 640
sd4g b 0 134 root operator 640
rsd4a c 6 128 root operator 640
rsd4b c 6 129 root operator 640
rsd4c c 6 130 root operator 640
rsd4d c 6 131 root operator 640
rsd4e c 6 132 root operator 640
rsd4f c 6 133 root operator 640
rsd4g c 6 134 root operator 640
sd5a b 0 160 root operator 640
sd5b b 0 161 root operator 640
sd5c b 0 162 root operator 640
sd5d b 0 163 root operator 640
sd5e b 0 164 root operator 640
sd5f b 0 165 root operator 640
sd5g b 0 166 root operator 640
rsd5a c 6 160 root operator 640
rsd5b c 6 161 root operator 640
rsd5c c 6 162 root operator 640
rsd5d c 6 163 root operator 640
rsd5e c 6 164 root operator 640
rsd5f c 6 165 root operator 640
rsd5g c 6 166 root operator 640
sd6a b 0 192 root operator 640
sd6b b 0 193 root operator 640
sd6c b 0 194 root operator 640
sd6d b 0 195 root operator 640
sd6e b 0 196 root operator 640
sd6f b 0 197 root operator 640
sd6g b 0 198 root operator 640
rsd6a c 6 192 root operator 640
rsd6b c 6 193 root operator 640
rsd6c c 6 194 root operator 640
rsd6d c 6 195 root operator 640
rsd6e c 6 196 root operator 640
rsd6f c 6 197 root operator 640
rsd6g c 6 198 root operator 640
ptyp0 c 21 0 root tty 666
ptyp1 c 21 1 root tty 666
ptyp2 c 21 2 root tty 666
ptyp3 c 21 3 root tty 666
ptyp4 c 21 4 root tty 666
ptyp5 c 21 5 root tty 666
ptyp6 c 21 6 root tty 666
ptyp7 c 21 7 root tty 666
ptyp8 c 21 8 root tty 666
ptyp9 c 21 9 root tty 666
ptypa c 21 10 root tty 666
ptypb c 21 11 root tty 666
ptypc c 21 12 root tty 666
ptypd c 21 13 root tty 666
ptype c 21 14 root tty 666
ptypf c 21 15 root tty 666
ttyp0 c 20 0 root tty 666
ttyp1 c 20 1 root tty 666
ttyp2 c 20 2 root tty 666
ttyp3 c 20 3 root tty 666
ttyp4 c 20 4 root tty 666
ttyp5 c 20 5 root tty 666
ttyp6 c 20 6 root tty 666
ttyp7 c 20 7 root tty 666
ttyp8 c 20 8 root tty 666
ttyp9 c 20 9 root tty 666
ttypa c 20 10 root tty 666
ttypb c 20 11 root tty 666
ttypc c 20 12 root tty 666
ttypd c 20 13 root tty 666
ttype c 20 14 root tty 666
ttypf c 20 15 root tty 666
@EOF@

# default tar device is the floppy disk
rm -f rmt8
ln -s sony0 rmt8

cd $SAVEDDIR
