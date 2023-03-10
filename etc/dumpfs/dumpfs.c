/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */
/* 3-Feb-89  Zon Williamson (zon) at Carnegie Mellon University
 *	Added AFS compatability for mac2.
 */

#ifndef lint
char copyright[] =
"@(#) Copyright (c) 1983 Regents of the University of California.\n\
 All rights reserved.\n";
#endif not lint

#ifndef lint
static char sccsid[] = "@(#)dumpfs.c	5.6 (Berkeley) 5/1/88";
#endif not lint

#include <sys/param.h>
#include <sys/inode.h>
#ifdef mac2
#include <mach_tahoe_fs.h>
#else
#include <sys/fs.h>
#endif

#include <stdio.h>
#include <fstab.h>

/*
 * dumpfs
 */

union {
	struct fs fs;
	char pad[MAXBSIZE];
} fsun;
#define	afs	fsun.fs

union {
	struct cg cg;
	char pad[MAXBSIZE];
} cgun;
#define	acg	cgun.cg

long	dev_bsize = 1;

main(argc, argv)
	char **argv;
{
	register struct fstab *fs;

	argc--, argv++;
	if (argc < 1) {
		fprintf(stderr, "usage: dumpfs fs ...\n");
		exit(1);
	}
	for (; argc > 0; argv++, argc--) {
		fs = getfsfile(*argv);
		if (fs == 0)
			dumpfs(*argv);
		else
			dumpfs(fs->fs_spec);
	}
}

dumpfs(name)
	char *name;
{
	int c, i, j, k, size;

	close(0);
	if (open(name, 0) != 0) {
		perror(name);
		return;
	}
	lseek(0, SBOFF, 0);
	if (read(0, &afs, SBSIZE) != SBSIZE) {
		perror(name);
		return;
	}
#ifndef mac2
	if (afs.fs_postblformat == FS_42POSTBLFMT)
		afs.fs_nrpos = 8;
#endif
	dev_bsize = afs.fs_fsize / fsbtodb(&afs, 1);
	printf("magic\t%x\tformat\t%s\ttime\t%s", afs.fs_magic,
#ifdef mac2
	"static",
#else
	    afs.fs_postblformat == FS_42POSTBLFMT ? "static" : "dynamic",
#endif
	    ctime(&afs.fs_time));
	printf("nbfree\t%d\tndir\t%d\tnifree\t%d\tnffree\t%d\n",
	    afs.fs_cstotal.cs_nbfree, afs.fs_cstotal.cs_ndir,
	    afs.fs_cstotal.cs_nifree, afs.fs_cstotal.cs_nffree);
	printf("ncg\t%d\tncyl\t%d\tsize\t%d\tblocks\t%d\n",
	    afs.fs_ncg, afs.fs_ncyl, afs.fs_size, afs.fs_dsize);
	printf("bsize\t%d\tshift\t%d\tmask\t0x%08x\n",
	    afs.fs_bsize, afs.fs_bshift, afs.fs_bmask);
	printf("fsize\t%d\tshift\t%d\tmask\t0x%08x\n",
	    afs.fs_fsize, afs.fs_fshift, afs.fs_fmask);
	printf("frag\t%d\tshift\t%d\tfsbtodb\t%d\n",
	    afs.fs_frag, afs.fs_fragshift, afs.fs_fsbtodb);
	printf("cpg\t%d\tbpg\t%d\tfpg\t%d\tipg\t%d\n",
	    afs.fs_cpg, afs.fs_fpg / afs.fs_frag, afs.fs_fpg, afs.fs_ipg);
	printf("minfree\t%d%%\toptim\t%s\tmaxcontig %d\tmaxbpg\t%d\n",
	    afs.fs_minfree, afs.fs_optim == FS_OPTSPACE ? "space" : "time",
	    afs.fs_maxcontig, afs.fs_maxbpg);
#ifdef mac2
	printf("rotdelay %dms\trps\t%d\n",
	    afs.fs_rotdelay, afs.fs_rps);
#else
	printf("rotdelay %dms\theadswitch %dus\ttrackseek %dus\trps\t%d\n",
	    afs.fs_rotdelay, afs.fs_headswitch, afs.fs_trkseek, afs.fs_rps);
#endif
#ifdef mac2
	printf("ntrak\t%d\tnsect\t%d\tspc\t%d\n",
	    afs.fs_ntrak, afs.fs_nsect, afs.fs_spc);
#else
	printf("ntrak\t%d\tnsect\t%d\tnpsect\t%d\tspc\t%d\n",
	    afs.fs_ntrak, afs.fs_nsect, afs.fs_npsect, afs.fs_spc);
#endif
#ifndef mac2
	printf("trackskew %d\tinterleave %d\n",
	    afs.fs_trackskew, afs.fs_interleave);
#endif
	printf("nindir\t%d\tinopb\t%d\tnspf\t%d\n",
	    afs.fs_nindir, afs.fs_inopb, afs.fs_nspf);
	printf("sblkno\t%d\tcblkno\t%d\tiblkno\t%d\tdblkno\t%d\n",
	    afs.fs_sblkno, afs.fs_cblkno, afs.fs_iblkno, afs.fs_dblkno);
	printf("sbsize\t%d\tcgsize\t%d\tcgoffset %d\tcgmask\t0x%08x\n",
	    afs.fs_sbsize, afs.fs_cgsize, afs.fs_cgoffset, afs.fs_cgmask);
	printf("csaddr\t%d\tcssize\t%d\tshift\t%d\tmask\t0x%08x\n",
	    afs.fs_csaddr, afs.fs_cssize, afs.fs_csshift, afs.fs_csmask);
	printf("cgrotor\t%d\tfmod\t%d\tronly\t%d\n",
	    afs.fs_cgrotor, afs.fs_fmod, afs.fs_ronly);
	if (afs.fs_cpc != 0)
		printf("blocks available in each of %d rotational positions",
#ifdef mac2
		     NRPOS);
#else
		     afs.fs_nrpos);
#endif
	else
		printf("insufficient space to maintain rotational tables\n");
	for (c = 0; c < afs.fs_cpc; c++) {
		printf("\ncylinder number %d:", c);
#ifdef mac2
		for (i = 0; i < NRPOS; i++) {
#else
		for (i = 0; i < afs.fs_nrpos; i++) {
#endif
#ifdef mac2
			if (fs_npostbl(&afs, c)[i] == -1)
#else
			if (fs_postbl(&afs, c)[i] == -1)
#endif
				continue;
			printf("\n   position %d:\t", i);
#ifdef mac2
			for (j = fs_npostbl(&afs, c)[i], k = 1; ;
			     j += fs_nrotbl(&afs)[j], k++) {
#else
			for (j = fs_postbl(&afs, c)[i], k = 1; ;
			     j += fs_rotbl(&afs)[j], k++) {
#endif
				printf("%5d", j);
				if (k % 12 == 0)
					printf("\n\t\t");
#ifdef mac2
				if (fs_nrotbl(&afs)[j] == 0)
#else
				if (fs_rotbl(&afs)[j] == 0)
#endif
					break;
			}
		}
	}
	printf("\ncs[].cs_(nbfree,ndir,nifree,nffree):\n\t");
	for (i = 0, j = 0; i < afs.fs_cssize; i += afs.fs_bsize, j++) {
		size = afs.fs_cssize - i < afs.fs_bsize ?
		    afs.fs_cssize - i : afs.fs_bsize;
		afs.fs_csp[j] = (struct csum *)calloc(1, size);
		lseek(0, fsbtodb(&afs, (afs.fs_csaddr + j * afs.fs_frag)) *
		    dev_bsize, 0);
		if (read(0, afs.fs_csp[j], size) != size) {
			perror(name);
			return;
		}
	}
	for (i = 0; i < afs.fs_ncg; i++) {
		struct csum *cs = &afs.fs_cs(&afs, i);
		if (i && i % 4 == 0)
			printf("\n\t");
		printf("(%d,%d,%d,%d) ",
		    cs->cs_nbfree, cs->cs_ndir, cs->cs_nifree, cs->cs_nffree);
	}
	printf("\n");
	if (afs.fs_ncyl % afs.fs_cpg) {
		printf("cylinders in last group %d\n",
		    i = afs.fs_ncyl % afs.fs_cpg);
		printf("blocks in last group %d\n",
		    i * afs.fs_spc / NSPB(&afs));
	}
	printf("\n");
	for (i = 0; i < afs.fs_ncg; i++)
		dumpcg(name, i);
	close(0);
};

dumpcg(name, c)
	char *name;
	int c;
{
	int i,j;

	printf("\ncg %d:\n", c);
	lseek(0, fsbtodb(&afs, cgtod(&afs, c)) * dev_bsize, 0);
	i = lseek(0, 0, 1);
	if (read(0, (char *)&acg, afs.fs_bsize) != afs.fs_bsize) {
		printf("dumpfs: %s: error reading cg\n", name);
		return;
	}
	printf("magic\t%x\ttell\t%x\ttime\t%s",
#ifdef mac2
	    ((struct ocg *)&acg)->cg_magic,
#else
	    afs.fs_postblformat == FS_42POSTBLFMT ?
	    ((struct ocg *)&acg)->cg_magic : acg.cg_magic,
#endif
	    i, ctime(&acg.cg_time));
	printf("cgx\t%d\tncyl\t%d\tniblk\t%d\tndblk\t%d\n",
	    acg.cg_cgx, acg.cg_ncyl, acg.cg_niblk, acg.cg_ndblk);
	printf("nbfree\t%d\tndir\t%d\tnifree\t%d\tnffree\t%d\n",
	    acg.cg_cs.cs_nbfree, acg.cg_cs.cs_ndir,
	    acg.cg_cs.cs_nifree, acg.cg_cs.cs_nffree);
	printf("rotor\t%d\tirotor\t%d\tfrotor\t%d\nfrsum",
	    acg.cg_rotor, acg.cg_irotor, acg.cg_frotor);
	for (i = 1, j = 0; i < afs.fs_frag; i++) {
		printf("\t%d", acg.cg_frsum[i]);
		j += i * acg.cg_frsum[i];
	}
	printf("\nsum of frsum: %d\niused:\t", j);
	pbits(cg_inosused(&acg), afs.fs_ipg);
	printf("free:\t");
	pbits(cg_blksfree(&acg), afs.fs_fpg);
	printf("b:\n");
	for (i = 0; i < afs.fs_cpg; i++) {
		if (cg_blktot(&acg)[i] == 0)
			continue;
		printf("   c%d:\t(%d)\t", i, cg_blktot(&acg)[i]);
#ifdef mac2
		for (j = 0; j < NRPOS; j++) {
#else
		for (j = 0; j < afs.fs_nrpos; j++) {
#endif
			if (afs.fs_cpc > 0 &&
#ifdef mac2
			    fs_npostbl(&afs, i % afs.fs_cpc)[j] == -1)
#else
			    fs_postbl(&afs, i % afs.fs_cpc)[j] == -1)
#endif
				continue;
			printf(" %d", cg_blks(&afs, &acg, i)[j]);
		}
		printf("\n");
	}
};

pbits(cp, max)
	register char *cp;
	int max;
{
	register int i;
	int count = 0, j;

	for (i = 0; i < max; i++)
		if (isset(cp, i)) {
			if (count)
				printf(",%s", count % 6 ? " " : "\n\t");
			count++;
			printf("%d", i);
			j = i;
			while ((i+1)<max && isset(cp, i+1))
				i++;
			if (i != j)
				printf("-%d", i);
		}
	printf("\n");
}
