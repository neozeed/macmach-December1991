/* mac2part.c -- MacMach utility to create "Mach" Apple partition */

#include <stdio.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/fs.h>
#include <sys/disklabel.h>
#include <fcntl.h>

#define MACH_PARTITION_TYPE "Mach_UNIX_BSD4.3"
#define MACH_PARTITION_NAME "Mach Partition"
#define FREE_PARTITION_TYPE "Apple_Free"
#define FREE_PARTITION_NAME "Free Partition"
#define MAXDD 61
#define BLK_SIZE 512

int verbose = 0; /* if non-zero, output lots of noise */

struct ddm {
  unsigned short sbSig;
  unsigned short sbBlockSize;
  unsigned long sbBlkCount;
  unsigned short sbDevType;
  unsigned short sbDevID;
  unsigned long sbData;
  unsigned short sbDrvrCount;
  struct {
    unsigned long ddBlock;
    unsigned short ddSize;
    unsigned short ddType;
  } dd[MAXDD];
  char unused[6];
};

struct dpm {
  unsigned short pdSig;
  unsigned long pdStart;
  unsigned long pdSize;
  unsigned long pdFSID;
};

struct pme {
  unsigned short pmSig;
  unsigned short pmSigPad;
  unsigned long pmMapBlkCnt;
  unsigned long pmPyPartStart;
  unsigned long pmPartBlkCnt;
  unsigned char pmPartName[32];
  unsigned char pmPartType[32];
  unsigned long pmLgDataStart;
  unsigned long pmDataCnt;
  unsigned long pmPartStatus;
  unsigned long pmLgBootStart;
  unsigned long pmBootSize;
  unsigned long pmBootLoad;
  unsigned long pmBootLoad2;
  unsigned long pmBootEntry;
  unsigned long pmBootEntry2;
  unsigned long pmBootCksum;
  unsigned char pmProcessor[16];
  unsigned char bootargs[128];
  char unused[248];
};

#define IS_MACH 1
#define IS_FREE 2
struct status {
  int type; /* IS_MACH or IS_FREE or zero */
  int size; /* in MB */
};

/* guess these parameters for Apple SCSI disks */
#define RPM      (3600)
#define NSECTORS (BBSIZE / BLK_SIZE)
#define NTRACKS  (NRPOS)
#define FSIZE    (1024)
#define FRAG     (8)

/* determine number of blocks in n mb */
#define MB(n) ((n * 1024 * 1024) / BLK_SIZE)

/* determine number of kb in n blocks (like /bin/du) */
#define KB(n) (howmany(dbtob(n), 1024))

/* sectors per cylinder */
#define SECPERCYL (NSECTORS * NTRACKS)

/* adjust size n to be a multiple of SECPERCYL */
#define SIZE(n) (SECPERCYL * (n / SECPERCYL))

/* mac2 disk partitioning
 * "x" is the entire Apple disk, use this to get to the partition map
 * "a" is standard 10MB root partition for big and small disks
 * "b" is 90MB /usr partition for big disks
 * "c" is the entire Mach partition -- largest possible Unix partition
 * "d" is 50MB /usr/tmp partition for big disks
 * "e" is 165MB /usr/src partition for big disks
 * "f" is "everything else" /usr/users partition for big disks
 * "g" is "everything else" /usr partition for small disks
 */

#define A_SIZE SIZE(MB(10))
#define B_SIZE SIZE(MB(90))
#define D_SIZE SIZE(MB(50))
#define E_SIZE SIZE(MB(165))

void display_disklabel(int scsiid)
{
  int f, i;
  char name[35];
  struct disklabel dl;

  sprintf(name, "/dev/rsd%da", scsiid);
  if ((f = open(name, O_RDWR)) == -1) { perror(name); return; }
  if(ioctl(f, DIOCGDINFO, &dl) < 0) { perror(name); close(f); return; }
  close(f);
  for (i = 0; i < dl.d_npartitions; i++)
    if (dl.d_partitions[i].p_size)
      fprintf(stderr, "  %d kbytes on /dev/sd%d%c\n",
        KB(dl.d_partitions[i].p_size), scsiid, i + 'a');
}

/* write unix disklabel on newly created Mach partition */
int create_disklabel(int scsiid, int size)
{
  int f, used;
  char name[35];
  struct disklabel dl;

  sprintf(name, "/dev/rsd%da", scsiid);

  if (verbose) fprintf(stderr, "mac2part: creating disklabel on %s\n", name);

  /* adjust size to be multiple of secpercyl */
  if (verbose) fprintf(stderr, "mac2part: size = %d\n", size);
  size = SIZE(size);
  if (verbose) fprintf(stderr, "mac2part: adjusted size = %d\n", size);

  bzero(&dl, sizeof(dl));

  dl.d_checksum = 0;
  dl.d_magic = DISKMAGIC;
  dl.d_magic2 = DISKMAGIC;
  strcpy(dl.d_typename,  "mac2");
  dl.d_type = DTYPE_SCSI;
  dl.d_rpm = RPM;
  dl.d_secsize = BLK_SIZE;
  dl.d_bbsize = BBSIZE;
  dl.d_sbsize = SBSIZE;
  dl.d_nsectors = NSECTORS;
  dl.d_ntracks = NTRACKS;
  dl.d_npartitions = 7; /* a, b, c, d, e, f, g */
  dl.d_interleave = 1;
  dl.d_secpercyl = SECPERCYL;
  dl.d_ncylinders = size / SECPERCYL;
  dl.d_secperunit = size;

  /* 'a' section, standard root */
  used = dl.d_partitions[0].p_size = A_SIZE;
  dl.d_partitions[0].p_offset = 0;
  dl.d_partitions[0].p_fsize = FSIZE;
  dl.d_partitions[0].p_frag = FRAG;
  dl.d_partitions[0].p_fstype = FS_BSDFFS;

  /* 'b' section, /usr on big disk */
  if ((size - used) > B_SIZE) {
    dl.d_partitions[1].p_size = B_SIZE;
    dl.d_partitions[1].p_offset = used;
    used += dl.d_partitions[1].p_size;
    dl.d_partitions[1].p_fsize = FSIZE;
    dl.d_partitions[1].p_frag = FRAG;
    dl.d_partitions[1].p_fstype = FS_BSDFFS;
  }

  /* 'c' section, entire disk (Mach partition) */
  dl.d_partitions[2].p_size = size;
  dl.d_partitions[2].p_offset = 0;
  dl.d_partitions[2].p_fsize = FSIZE;
  dl.d_partitions[2].p_frag = FRAG;
  dl.d_partitions[2].p_fstype = FS_BSDFFS;

  /*'d' section, /usr/tmp on big disk */
  if ((size - used) > D_SIZE) {
    dl.d_partitions[3].p_size = D_SIZE;
    dl.d_partitions[3].p_offset = used;
    used += dl.d_partitions[3].p_size;
    dl.d_partitions[3].p_fsize = FSIZE;
    dl.d_partitions[3].p_frag = FRAG;
    dl.d_partitions[3].p_fstype = FS_BSDFFS;
  }

  /* 'e' section, /usr/src on big disk */
  if ((size - used) > E_SIZE) {
    dl.d_partitions[4].p_size = E_SIZE;
    dl.d_partitions[4].p_offset = used;
    used += dl.d_partitions[4].p_size;
    dl.d_partitions[4].p_fsize = FSIZE;
    dl.d_partitions[4].p_frag = FRAG;
    dl.d_partitions[4].p_fstype = FS_BSDFFS;
  }

  /* 'f' section, /usr/users on big disk */
  if ((size - used) > 0) {
    dl.d_partitions[5].p_size = size - used;
    dl.d_partitions[5].p_offset = used;
    used += dl.d_partitions[5].p_size;
    dl.d_partitions[5].p_fsize = FSIZE;
    dl.d_partitions[5].p_frag = FRAG;
    dl.d_partitions[5].p_fstype = FS_BSDFFS;
  }

  /* 'g' section, /usr on small disk */
  dl.d_partitions[6].p_size = size - dl.d_partitions[0].p_size;
  dl.d_partitions[6].p_offset = dl.d_partitions[0].p_size;
  dl.d_partitions[6].p_fsize = FSIZE;
  dl.d_partitions[6].p_frag = FRAG;
  dl.d_partitions[6].p_fstype = FS_BSDFFS;

  /* set checksum */
  dl.d_checksum = dkcksum(&dl);

  /* write disklabel to disk */
  if (verbose) fprintf(stderr, "mac2part: writing disklabel on %s\n", name);
  if ((f = open(name, O_RDWR)) == -1) { perror(name); return 1; }
  if(ioctl(f, DIOCWDINFO, &dl) < 0) { perror(name); close(f); return 1; }
  close(f);

  /* all done */
  return 0;

} /* create_disklabel() */

#define CHECK_ONLY 0
#define CREATE_MACH 1
#define CREATE_FREE 2

/* check disk, set status values, maybe create Mach partition from Free */
void check(struct status *status, int scsiid, int maxdisk, int create)
{
  int f;
  char name[35];
  struct ddm ddm;
  struct pme pme;
  unsigned short magic;
  char parttype[33];
  long position;
  int i, n;

  /* open "apple" unix disk device */
  sprintf(name, "/dev/rsd%dx", scsiid);
  if (!(f = open(name, create ? O_RDWR : O_RDONLY))) return;

  if (verbose) fprintf(stderr, "mac2part: checking %s\n", name);

  /* read device descriptor map, verify magic number */
  if (read(f, &ddm, sizeof(ddm)) != sizeof(ddm)) { close(f); return; }
  if (ddm.sbSig != 0x4552) { close(f); return; }

  /* loop through partition map, checking for Free or Mach partitions */
  for (i = -1, n = 0; i; i--, n++) {
    position = lseek(f, 0, 1);
    if (read(f, &pme, sizeof(pme)) != sizeof(pme)) {
      fprintf(stderr, "mac2part: can not read pme on %s\n", name);
      break;
    }
    if (pme.pmSig != 0x504D) {
      fprintf(stderr, "mac2part: pme.pmSig 0x%X != 0x%X on %s\n",
        pme.pmSig, 0x504D, name);
      if (pme.pmSig == 0x5453)
        fprintf(stderr, "mac2part: obsolete partition map format on %s\n",
          name);
      break;
    }
    if (i == -1) i = pme.pmMapBlkCnt;
    if (status) status->size = pme.pmPartBlkCnt;
    if (status && (maxdisk > 0) && (status->size < maxdisk)) continue;
    strncpy(parttype, pme.pmPartType, 32);
    parttype[32] = 0;

    if (verbose) fprintf(stderr, "mac2part: found \"%s\" partition on %s\n",
      parttype, name);

    /* done if Mach partition found */
    if (!strcmp(parttype, MACH_PARTITION_TYPE)) {
      if (create == CREATE_FREE) {
        if (verbose) fprintf(stderr, "Creating Free partition on %s\n", name);
        pme.pmPartStatus = 0x33; /* valid, allocated, read, write */
	bzero(&pme.pmPartName, sizeof(pme.pmPartName));
	strcpy(pme.pmPartName, FREE_PARTITION_NAME);
	bzero(&pme.pmPartType, sizeof(pme.pmPartType));
        strcpy(pme.pmPartType, FREE_PARTITION_TYPE);
        if (lseek(f, position, 0) != position) {
          fprintf(stderr, "mac2part: can not seek to %d on %s\n",
            position, name);
          break;
        }
        if (write(f, &pme, sizeof(pme)) != sizeof(pme)) {
          fprintf(stderr, "mac2part: can not write pme on %s\n", name);
          break;
        }
        if (status) status->type = IS_FREE;
      }
      else if (status) status->type = IS_MACH;
      break;
    }
    /* done if Free partition found */
    if (!strcmp(parttype, FREE_PARTITION_TYPE)) {
      if (create == CREATE_MACH) {
        if (verbose) fprintf(stderr, "Creating Mach partition on %s\n", name);
        pme.pmPartStatus = 0x33; /* valid, allocated, read, write */
	bzero(&pme.pmPartName, sizeof(pme.pmPartName));
        strcpy(pme.pmPartName, MACH_PARTITION_NAME);
	bzero(&pme.pmPartType, sizeof(pme.pmPartType));
        strcpy(pme.pmPartType, MACH_PARTITION_TYPE);
        if (lseek(f, position, 0) != position) {
          fprintf(stderr, "mac2part: can not seek to %d on %s\n",
            position, name);
          break;
        }
        if (write(f, &pme, sizeof(pme)) != sizeof(pme)) {
          fprintf(stderr, "mac2part: can not write pme on %s\n", name);
          break;
        }
        if (status) status->type = IS_MACH;
      }
      else if (status) status->type = IS_FREE;
      break;
    }
  }

  /* all done */
  close(f);

} /* check() */

#ifndef SMALL

/* display partition map for specified disk, return non-zero if error */
int display_partition_map(int scsiid)
{

  struct ddm ddm;
  struct dpm dpm;
  struct pme pme;
  unsigned short magic;
  int i, j;
  char fsid[5];
  char partname[33];
  char parttype[33];
  char processor[17];
  FILE *f;
  char name[35];

  sprintf(name, "/dev/rsd%dx", scsiid);

  if (!(f = fopen(name, "r"))) { perror(name); exit(1); }

  printf("Apple partition on SCSI disk %d.", scsiid);
  printf("See Inside Mac, V-577, V-579, IV-292 for partition documentation.\n");

  if (fread(&ddm, sizeof(ddm), 1, f) != 1) {
    printf("can not read ddm\n");
    return 1;
  }
  if (ddm.sbSig == 0x4552) {
    printf("\ndriver descriptor map\n\n");
    printf("sbSig = 0x%X, sbBlockSize = %d (0x%X), sbBlkCount = %d (0x%X) (%dMB)\n",
      ddm.sbSig, ddm.sbBlockSize, ddm.sbBlockSize, ddm.sbBlkCount, ddm.sbBlkCount,
      (ddm.sbBlkCount * ddm.sbBlockSize) / 1000000);
    printf("sbDevType = %d (0x%X), sbDevID = %d (0x%X), sbData = %d (0x%X)\n",
      ddm.sbDevType, ddm.sbDevType, ddm.sbDevID, ddm.sbDevID, ddm.sbData, ddm.sbData);
    printf("sbDrvrCount = %d\n", ddm.sbDrvrCount);
    for (i = 0; i < ddm.sbDrvrCount; i++) {
      printf("\n%2d ddBlock = %d (0x%X), ddSize = %d, (0x%X), ddType = %d (0x%X)\n",
        i, ddm.dd[i].ddBlock, ddm.dd[i].ddBlock, ddm.dd[i].ddSize, ddm.dd[i].ddSize,
        ddm.dd[i].ddType, ddm.dd[i].ddType);
    }
  }
  else {
    printf("there is no driver descriptor map (magic = 0x%X)\n", ddm.sbSig);
    return 1;
  }

  if (fread(&magic, sizeof(magic), 1, f) != 1) {
    printf("can not read magic\n");
    return 1;
  }

  if (magic == 0x5453) {
    printf("\nold partition map format\n");
    if (fseek(f, 512, 0)) {
      printf("can not seek to 512\n");
      return 1;
    }
    while(1) {
      if (fread(&dpm, sizeof(dpm), 1, f) != 1) {
        printf("can not read dpm\n");
        return 1;
      }
      if (!dpm.pdSig && !dpm.pdStart && !dpm.pdSize && !dpm.pdFSID) break;
      if (dpm.pdSig != 0x5453) printf("\n*** pdSig is not 0x5453\n");
      strncpy(fsid, &dpm.pdFSID, 4);
      fsid[4] = 0;
      printf("\npdSig = 0x%X, pdStart = %d (0x%X), pdSize = %d (0x%X) (%dMB), pdFSID = \"%s\"\n",
        dpm.pdSig, dpm.pdStart, dpm.pdStart, dpm.pdSize, dpm.pdSize,
        (dpm.pdSize * 512) / 1000000, fsid);
      if (!strcmp(fsid, "TFS1")) printf("Partition for Macintosh Plus\n");
      else if (!strcmp(fsid, "MACH")) printf("Partition for MACH BSD UNIX\n");
      else printf("Unknown partition pdFSID\n");
    }
  }

  else if (magic = 0x504D) {

    printf("\nnew partition map format\n");
    if (fseek(f, 512, 0)) {
      printf("can not seek to position 512\n");
      return 1;
    }

    i = j = 0;
    while (1) {

      if (fread(&pme, sizeof(pme), 1, f) != 1) {
        printf("can not read pme\n");
        exit(1);
      }

      strncpy(partname, pme.pmPartName, 32); partname[32] = 0;
      strncpy(parttype, pme.pmPartType, 32); parttype[32] = 0;
      strncpy(processor, pme.pmProcessor, 16); processor[16] = 0;
      if (pme.pmSig != 0x504D) printf("\n*** pmSig is not 0x504D\n");
      if (!i) i = pme.pmMapBlkCnt;
      printf("\npartiton map entry %d\n\n", j++);
      printf("pmSig = 0x%X, pmSigPad = 0x%X, pmMapBlkCnt = %d (0x%X)\n",
        pme.pmSig, pme.pmSigPad, pme.pmMapBlkCnt, pme.pmMapBlkCnt);
      printf("pmPyPartStart = %d (0x%X), pmPartBlkCnt = %d (0x%X) (%dMB)\n",
        pme.pmPyPartStart, pme.pmPyPartStart, pme.pmPartBlkCnt,
        pme.pmPartBlkCnt, (pme.pmPartBlkCnt * 512) / 1000000);
      printf("pmPartName = \"%s\", pmPartType =\"%s\"\n", partname, parttype);
      if (!strcmp(partname, "Apple_MFS")) printf("  Flat file system (64K ROM)\n");
      else if (!strcmp(parttype, "Apple_HFS")) printf("  Hierarchical file system (128K ROM and later)\n");
      else if (!strcmp(parttype, "Apple_Unix_SVR2")) printf("  Partition for UNIX\n");
      else if (!strcmp(parttype, "Apple_partition_map")) printf("  Partition containing partition map\n");
      else if (!strcmp(parttype, "Apple_Driver")) printf("  Partition contains a name driver\n");
      else if (!strcmp(parttype, "Apple_PRODOS")) printf("  Partition designated for Apple IIGS\n");
      else if (!strcmp(parttype, FREE_PARTITION_TYPE)) printf("  Partition unused and available for assignment\n");
      else if (!strcmp(parttype, "Apple_Scratch")) printf("  Partition empty and free for use\n");
      else if (!strcmp(parttype, MACH_PARTITION_TYPE)) printf("  Partition for MACH BSD UNIX\n");
      else printf("  Unknown pmPartType\n");
      printf("pmLgDataStart = %d (0x%X), pmDataCnt = %d (0x%X)\n",
        pme.pmLgDataStart, pme.pmLgDataStart, pme.pmDataCnt, pme.pmDataCnt);
      printf("pmPartStatus = 0x%X\n", pme.pmPartStatus);
      printf("  %s\n", pme.pmPartStatus & 0x1 ?
        "valid partition map entry" : "not a valid partition map entry");
      printf("  partition %s\n", pme.pmPartStatus & 0x2 ?
        "allocated" : "available");
      printf("  partition %s in use\n", pme.pmPartStatus & 0x4 ?
        "is" : "is not");
      printf("  partition %s boot information\n", pme.pmPartStatus & 0x8 ?
        "contains valid" : "does not contain");
      printf("  partition %s reading\n", pme.pmPartStatus & 0x10 ?
        "allows" : "does not allow");
      printf("  partition %s writing\n", pme.pmPartStatus & 0x20 ?
        "allows" : "does not allow");
      printf("  boot code %s position independent\n", pme.pmPartStatus & 0x40 ?
        "is" : "is not");
      printf("  %s\n", pme.pmPartStatus & 0x80 ?
        "user bit set" : "user bit clear");
      printf("pmLgBootStart = %d (0x%X), pmBootSize = %d (0x%X)\n",
        pme.pmLgBootStart, pme.pmLgBootStart, pme.pmBootSize, pme.pmBootSize);
      printf("pmBootLoad = %d (0x%X), pmBootLoad2 = %d (0x%X)\n",
        pme.pmBootLoad, pme.pmBootLoad, pme.pmBootLoad2, pme.pmBootLoad2);
      printf("pmBootEntry = %d (0x%X), pmBootEntry2 = %d (0x%X)\n",
        pme.pmBootEntry, pme.pmBootEntry, pme.pmBootEntry2, pme.pmBootEntry2);
      printf("pmBootCksum = 0x%X, pmProcessor = \"%s\"\n",
        pme.pmBootCksum, processor);

      i--;
      if (!i) break;

    }

  }
  else {
    printf("there is no partition map (magic = 0x%X)\n", magic);
    return 1;
  }

  return 0;

} /* display_partition_map() */

#endif /* SMALL */

/* find usable mach partition, return non-zero if error */
int find_mach_partition(int scsiid, int maxdisk)
{
  struct status status[7];
  int i, n_mach, n_free, mach_scsiid, free_scsiid;
  char number[1024];

  bzero(status, sizeof(status));

  /* if "any", check all disks, otherwise check specified disk */
  if (scsiid == -1)
    for (i = 0; i < 7; i++) check(&status[i], i, maxdisk, CHECK_ONLY);
  else if ((scsiid < 0) || (scsiid > 6)) {
    fprintf(stderr, "mac2part: scsiid out of range\n");
    return 1;
  }
  else check(&status[scsiid], scsiid, maxdisk, CHECK_ONLY);

  /* count number of disks with Mach or Free partitions */
  for (n_mach = n_free = i = 0; i < 7; i++) {
    if (status[i].type == IS_MACH) {
      n_mach++;
      mach_scsiid = i;
    }
    else if (status[i].type == IS_FREE) {
      n_free++;
      free_scsiid = i;
    }
  }

  /* error if no usable partitions found */
  if (!n_mach && !n_free) {
    fprintf(stderr, "Can not find any usable partitions on any disks.\n");
    return 1;
  }

  /* if only one usable partition, use it, otherwise ask user which to use */
  if ((n_mach == 1) && !n_free) scsiid = mach_scsiid;
  else if (!n_mach && (n_free == 1)) scsiid = free_scsiid;
  else do {
    fprintf(stderr, "Found %d usable disks:\n", n_mach + n_free);
    for (i = 0; i < 7; i++) {
      if (status[i].type == IS_MACH)
        fprintf(stderr, "  %d block %s partition on disk %d\n",
          status[i].size, MACH_PARTITION_TYPE, i);
      if (status[i].type == IS_FREE)
        fprintf(stderr, "  %d block %s partition on disk %d\n",
          status[i].size, FREE_PARTITION_TYPE, i);
    }
    fprintf(stderr, "Enter the number of the disk to use: ");
    gets(number);
    scsiid = atoi(number);
  } while (!status[scsiid].type);

  /* if choice is a Free partition, make a Mach partition out of it */
  if (status[scsiid].type == IS_FREE) {
    check(&status[scsiid], scsiid, maxdisk, CREATE_MACH);
    if (status[scsiid].type != IS_MACH) return 1;
    if (create_disklabel(scsiid, status[scsiid].size)) return 1;
  }

  /* all done, announce results, write scsiid to stdout */
  fprintf(stderr, "Using %d block %s partition on disk %d.\n",
    status[scsiid].size, MACH_PARTITION_TYPE, scsiid);
  display_disklabel(scsiid);
  printf("%d", scsiid);

  return 0;

} /* find_mach_partition() */

main(int argc, char **argv)
{
  int scsiid, maxdisk;

  argc--; argv++;
  if (argc && !strcmp(*argv, "-v")) {
    verbose++;
    argc--; argv++;
  }
  if (!argc) goto usage;
#ifndef SMALL
  else if (!strcmp(*argv, "display")) {
    argc--; argv++;
    if (argc == 1) exit(display_partition_map(atoi(*argv)));
    else goto usage;
  }
#endif /* SMALL */
  else if (!strcmp(*argv, "free")) {
    argc--; argv++;
    if (!argc) goto usage;
    scsiid = atoi(*argv);
    argc--; argv++;
    if (!argc) {
      check(0, scsiid, 0, CREATE_FREE);
      exit(0);
    }
    else goto usage;
  }
  else {
    scsiid = !strcmp(*argv, "search") ? -1 : atoi(*argv);
    argc--; argv++;
    if (argc > 1) goto usage;
    maxdisk = !argc ? -1 : atoi(*argv);
    exit(find_mach_partition(scsiid, maxdisk));
  }
usage:
  fprintf(stderr, "usage: mac2part [-v] <scsiid> [ <mindisk> ]\n");
  fprintf(stderr, "usage: mac2part [-v] search [ <mindisk> ]\n");
#ifndef SMALL
  fprintf(stderr, "usage: mac2part [-v] display <scsiid>\n");
#endif /* SMALL */
  fprintf(stderr, "usage: mac2part [-v] free <scsiid>\n");
  exit(1);
}
