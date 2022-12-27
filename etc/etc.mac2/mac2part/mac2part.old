/* mac2part.c -- MacMach utility to partition and label Macintosh disks */

/* define OLD_FORMAT to allow the old Apple partition format */
#undef OLD_FORMAT

#include <stdio.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/disklabel.h>

int minimum = 1536; /* the size of an 800K floppy disk */

/* Apple driver descriptor map info - see Inside Mac V-577 */
struct dm {
  short sbSig;
  short sbBlockSize;
  long sbBlkCount;
};

/* Apple partition map info - see Inside Mac V-579 */
struct pm {
  short pmSig;
  short pmSigPad;
  long pmMapBlkCnt;
  long pmPyPartStart;
  long pmPartBlkCnt;
  char pmPartName[32];
  char pmPartType[32];
};

/* old style Apple device partition map - see Inside Mac IV-292 */
struct pd {
  short pdSig;
  long pdStart;
  long pdSize;
  long pdFSID;
};

#define MACH_PARTITION_TYPE "Mach_UNIX_BSD4.3"
#define FREE_PARTITION_TYPE "Apple_Free"
#define MAX_A_PART 64 /* max # Apple partitions to examine */

#define BLK_SIZE 512

/* check_disk() return codes */
#define ERROR    -1
#define HAS_FREE 0
#define HAS_MACH 1
int partition_size;

main(argc, argv)
int argc;
char *argv[];
{
  int i, fd_x, fd_c;
  char dname[20];
  int nfree, nmach;
  struct { int size, scsiid; } has_free[8], has_mach[8];
  char number[80];

  if ((argc < 2) || (argc > 3)) {
    fprintf(stderr, "Usage: %s SCSIID [ MINDISK ]\n", argv[0]);
    fprintf(stderr, "Usage: %s SCSIID -display\n", argv[0]);
    exit(1);
  }
  if (argc > 2 && !strcmp(argv[2], "-display")) display(atoi(argv[1]));
  if (argc > 2) minimum = atoi(argv[2]);
  if (strcmp(argv[1], "search")) {
    i = (argv[1][0] >= '0' && argv[1][0] <= '6') ? atoi(argv[1]) : -1;
  }
  else {
    for (i = 0; i < 7; i++) { /* id 7 is the Mac */
      sprintf(dname, "/dev/rsd%dx", i);
      if ((fd_x = open(dname, 0)) < 0) continue;
      switch (check_disk(dname, i, fd_x, 0, 0)) {
        case HAS_FREE:
          has_free[nfree].size = partition_size;
          has_free[nfree++].scsiid = i;
          break;
        case HAS_MACH:
          has_mach[nmach].size = partition_size;
          has_mach[nmach++].scsiid = i;
          break;
      }
      close(fd_x);
    }
    if (nfree + nmach) {
      if ((nfree == 1) && !nmach) i = has_free[0].scsiid;
      else if ((nmach == 1) && !nfree) i = has_mach[0].scsiid;
      else {
        fprintf(stderr, "Found %d usable disks:\n", nfree + nmach);
        for(i = 0; i < nfree; i++)
          fprintf(stderr, "  %dMB, SCSI disk %d [Free Partition]\n",
            has_free[i].size, has_free[i].scsiid);
        for(i = 0; i < nmach; i++)
          fprintf(stderr, "  %dMB, SCSI disk %d [Existing Mach Partition]\n",
            has_mach[i].size, has_mach[i].scsiid);
        fprintf(stderr, "Enter the number of the disk to use: ");
        gets(number);
        i = (number[0] >= '0' && number[0] <= '6') ? atoi(number) : -1;
      }
    }
    else goto no_disk;
  }
  if (i < 0 || i > 6) {
    fprintf(stderr, "ERROR: SCSIID is a number between 0 and 6.\n");
    exit(1);
  }
  sprintf(dname, "/dev/rsd%dx", i);
  fd_x = open(dname, 2);
  if (fd_x < 0) {
    fprintf(stderr, "ERROR: SCSI disk %d, can not write %s\n", i, dname);
    exit(1);
  }
  sprintf(dname, "/dev/rsd%dc", i);
  fd_c = open(dname, 2);
  if (fd_c < 0) {
    fprintf(stderr, "ERROR: SCSI disk %d, can not write %s\n", i, dname);
    exit(1);
  }
  /* make Mach partition and write Unix label on specified disk */
  switch (check_disk(dname, i, fd_x, fd_c, 1)) {
    case HAS_FREE:
    case HAS_MACH:
      printf("%d\n", i);
      exit(0);
  }
no_disk:
  fprintf(stderr, "ERROR: Could not find a usable disk.\n");
  exit(1);
}

/* return non-zero if blocks is less than minimum */
int check_minimum(int blocks, int scsiid, int error)
{
  if (blocks < minimum) {
    fprintf(stderr, "%sSCSI disk %d partition has %d blocks, minimum is %d\n",
      error ? "ERROR: " : "WARNING: ", scsiid, blocks, minimum);
    return 1;
  }
  return 0;
}

/* return zero if ok to use existing partiton */
int use_existing_partition(int scsiid, int partition_size)
{
  char yes[80];

  fprintf(stderr, "SCSI disk %d has existing %dMB Mach partition.\n",
    scsiid, partition_size);
  fprintf(stderr, "Do you want use this partition? [YES] ");
  gets(yes);
  return yes[0] && yes[0] != 'Y' && yes[0] != 'y';
}

check_disk(disk, scsiid, fd_x, fd_c, doit)
char *disk;
int scsiid;
int fd_x, fd_c;
int doit;
{
  int i, j, asize;
  char blk_buf[BLK_SIZE];
  struct pm *pm = (struct pm *)blk_buf;
  struct pd *pd = (struct pd *)blk_buf;
  struct dm *dm = (struct dm *)blk_buf;

  lseek(fd_x, 0, 0);
  if ((j = read(fd_x, blk_buf, BLK_SIZE)) != BLK_SIZE) {
    if (doit) fprintf(stderr, "ERROR: Can't read block 0 on %s\n", disk);
    return ERROR;
  }
  if (dm->sbSig == 0x4552) asize = dm->sbBlkCount; /* see Inside MAC V-577 */
  else {
    fprintf(stderr, "WARNING: SCSI disk %d not initialized.\n", scsiid);
    return ERROR;
  }
  /* read Apple partition maps looking for the Mach partition */
  for(i = 1; i < MAX_A_PART; i++) {
    if ((j = read(fd_x, blk_buf, BLK_SIZE)) != BLK_SIZE) {
      if (doit) fprintf(stderr, "ERROR: Can't read block %d on %s\n", i, disk);
      return ERROR;
    }
    /* check old style Apple partitioning - see Inside MAC IV-292 */
    if (i == 1 && pd->pdSig == 0x5453) {
#ifdef OLD_FORMAT
      return old_check_disk(disk, scsiid, fd_x, fd_c, doit, asize);
#else
      fprintf(stderr, "WARNING: SCSI disk %d has obsolete partition format\n",
        scsiid);
      return ERROR;
#endif
    }
    /* is it a partition map - see Inside MAC V-579 */
    if (pm->pmSig != 0x504D) break;
    partition_size = (pm->pmPartBlkCnt * 512) / 1000000;
    if (strcmp(MACH_PARTITION_TYPE, pm->pmPartType) == 0) {
      if (check_minimum(pm->pmPartBlkCnt, scsiid, 0)) continue;
      if (doit) {
        if (use_existing_partition(scsiid, partition_size)) return ERROR;
        label_disk(scsiid, fd_x, fd_c, pm->pmPartBlkCnt, pm->pmPyPartStart);
      }
      return HAS_MACH;
    }
    if (strcmp(FREE_PARTITION_TYPE, pm->pmPartType) == 0) {
      if (check_minimum(pm->pmPartBlkCnt, scsiid, 0)) continue;
      if (!doit) return HAS_FREE;
      fprintf(stderr,
        "SCSI DISK %d has free %dMB partition.\n", scsiid, partition_size);
      strcpy(pm->pmPartType, MACH_PARTITION_TYPE);
      lseek(fd_x, i*BLK_SIZE, 0);
      if ((j = write(fd_x, blk_buf, BLK_SIZE)) != BLK_SIZE) {
        fprintf(stderr, "ERROR: Failed to update Apple partition.\n");
        return ERROR;
      }
      fprintf(stderr, "Creating new Mach partition #%d, %d blocks at %d\n",
        i, pm->pmPartBlkCnt, pm->pmPyPartStart);
      label_disk(scsiid, fd_x, fd_c, pm->pmPartBlkCnt, pm->pmPyPartStart);
      return HAS_FREE;
    }
  }
  if (doit)
    fprintf(stderr, "ERROR: SCSI disk %d has no free partition.\n", scsiid);
  return ERROR;
}

/* version of check_disk() for old Apple partition format */
char old_mach_string[] = "MACH";
#define OLD_MACH (*(int *)old_mach_string)
old_check_disk(disk, scsiid, fd_x, fd_c, doit, disksize)
char *disk;
int scsiid;
int fd_x, fd_c;
int doit;
int disksize;
{
  int i, j, next_block = 0;
  char blk_buf[BLK_SIZE];
  struct pd *pd = (struct pd *)blk_buf;

  /* examine old Apple partition info looking for Mach partition */
  for (i = 1; i < MAX_A_PART; i++, pd++) {
    if (pd->pdSig == 0x5453) {  /* see Inside MAC IV-292 */
      next_block = pd->pdStart + pd->pdSize;
      if (pd->pdFSID == OLD_MACH) {
        partition_size = (pd->pdSize * 512) / 1000000;
        if (check_minimum(pd->pdSize, scsiid, 0)) return ERROR;
        if (doit) {
          if (use_existing_partition(scsiid, partition_size)) return ERROR;
          label_disk(scsiid, fd_x, fd_c, pd->pdSize, pd->pdStart);
        }
        return HAS_MACH;
      }
    }
    if (pd->pdSig == 0) {
      if (check_minimum(disksize - next_block, scsiid, 0)) return ERROR;
      partition_size = ((disksize-next_block) * 512) / 1000000;
      if (!doit) return HAS_FREE;
      fprintf(stderr,
        "SCSI disk %d has free %dMB partiton.\n", scsiid, partition_size);
      pd->pdSig = 0x5453; /* see Inside MAC IV-292 */
      pd->pdStart = next_block;
      pd->pdSize = disksize - next_block;
      pd->pdFSID = OLD_MACH;
      pd++;
      pd->pdSig = 0;
      pd->pdStart = 0;
      pd->pdSize = 0;
      pd->pdFSID = 0;
      pd--;
      lseek(fd_x, BLK_SIZE, 0);
      if ((j = write(fd_x, blk_buf, BLK_SIZE)) != BLK_SIZE) {
        fprintf(stderr, "ERROR: Failed to update Apple partition.\n");
        return ERROR;
      }
      fprintf(stderr, "Created new Mach partition #%d, %d blocks at %d\n",
        i, pd->pdSize, pd->pdStart);
      label_disk(scsiid, fd_x, fd_c, pd->pdSize, pd->pdStart);
      return HAS_FREE;
    }
  }
  if (doit)
    fprintf(stderr, "ERROR: SCSI disk %d has no free partition.\n", scsiid);
  return ERROR;
}

/* write Unix label to Mach partition */
label_disk(int scsiid, int fd_x, int fd_c, int blkcnt, int start_blk)
{
  register int i;
  char label_block[BLK_SIZE];
  struct disklabel *dlbp = (struct disklabel *)(label_block + LABELOFFSET);

  if (check_minimum(blkcnt, scsiid, 1)) exit(1);
  bzero(dlbp, sizeof(*dlbp));
  dlbp->d_checksum = 0;
  dlbp->d_magic = DISKMAGIC;
  dlbp->d_magic2 = DISKMAGIC;
  strcpy(dlbp->d_typename,  "Mach partition");
  dlbp->d_type = DTYPE_SCSI;
  dlbp->d_rpm = 3600;
  dlbp->d_secsize = BLK_SIZE;
  dlbp->d_interleave = 1;
  dlbp->d_nsectors = 32;
  dlbp->d_ntracks = 6;
  dlbp->d_ncylinders = 823;
  dlbp->d_secpercyl = 192;
  dlbp->d_secperunit = blkcnt;
  dlbp->d_bbsize = 8192;
  dlbp->d_sbsize = 8192;
  dlbp->d_npartitions = 8;
  dlbp->d_partitions[0].p_size = 19584; /* 'a' section */
  dlbp->d_partitions[0].p_fsize = 1024;
  dlbp->d_partitions[0].p_frag = 8;
  dlbp->d_partitions[0].p_fstype = FS_BSDFFS;
  dlbp->d_partitions[2].p_size = blkcnt; /* 'c' section */
  dlbp->d_partitions[2].p_fsize = 1024;
  dlbp->d_partitions[2].p_frag = 8;
  dlbp->d_partitions[2].p_fstype = FS_BSDFFS;
  dlbp->d_partitions[6].p_size = blkcnt - 19584; /* 'g' section */
  dlbp->d_partitions[6].p_offset = 19584;
  dlbp->d_partitions[6].p_fsize = 1024;
  dlbp->d_partitions[6].p_frag = 8;
  dlbp->d_partitions[6].p_fstype = FS_BSDFFS;
  if(ioctl(fd_c, DIOCWDINFO, dlbp) < 0) {
    perror("labeling failed: ioctl DIOCWDINFO");
    exit(1);
  }
  fprintf(stderr, "Allocated %.1f Mb in %d blocks for Mach\n",
    ((float)blkcnt * BLK_SIZE) / 1048576, blkcnt);
}

/* display Apple partition informatin on disk */
display(int scsiid)
{

#define MAXDD 61

struct {
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
} ddm;

struct {
  unsigned short pdSig;
  unsigned long pdStart;
  unsigned long pdSize;
  unsigned long pdFSID;
} dpm;

struct {
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
} pme;

  unsigned short magic;
  int i, j;
  char fsid[5];
  char partname[33];
  char parttype[33];
  char processor[17];
  FILE *disk;
  char device[20];

  sprintf(device, "/dev/rsd%dx", scsiid);
  if (!(disk = fopen(device, "r"))) { perror(device); exit(1); }
  printf("Apple partition on SCSI disk %d.", scsiid);
  printf("See Inside Mac, V-577, V-579, IV-292 for partition documentation.\n");
  if (fread(&ddm, sizeof(ddm), 1, disk) != 1) {
    printf("can not read ddm\n");
    exit(1);
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
    exit(1);
  }
  if (fread(&magic, sizeof(magic), 1, disk) != 1) {
    printf("can not read magic\n");
    exit(1);
  }
  if (magic == 0x5453) {
    printf("\nold partition map format\n");
    if (fseek(disk, 512, 0)) {
      printf("can not seek to position 512\n");
      exit(1);
    }
    while(1) {
      if (fread(&dpm, sizeof(dpm), 1, disk) != 1) {
        printf("can not read dpm\n");
        exit(1);
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
    if (fseek(disk, 512, 0)) {
      printf("can not seek to position 512\n");
      exit(1);
    }
    i = j = 0;
    while (1) {
      if (fread(&pme, sizeof(pme), 1, disk) != 1) {
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
      else if (!strcmp(parttype, "Apple_Driver")) printf("  Partition contains a device driver\n");
      else if (!strcmp(parttype, "Apple_PRODOS")) printf("  Partition designated for Apple IIGS\n");
      else if (!strcmp(parttype, "Apple_Free")) printf("  Partition unused and available for assignment\n");
      else if (!strcmp(parttype, "Apple_Scratch")) printf("  Partition empty and free for use\n");
      else if (!strcmp(parttype, "Mach_UNIX_BSD4.3")) printf("  Partition for MACH BSD UNIX\n");
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
   exit(1);
  }
  exit(0);
}
