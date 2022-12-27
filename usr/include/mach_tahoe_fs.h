/* /usr/include/mach_tahoe_fs.h
 *
 * TAHOE compatability macros for MACH
 *
 * include this file instead of sys/fs.h
 *
 * Note change: rotbl => nrotbl and postbl => npostbl
 */

#include <sys/fs.h>

#define FS_42POSTBLFMT /*not used*/
#define FS_DYNAMICPOSTBLFMT /*not used*/
#define ocg cg
#define fs_npsect /*not used*/
#define fs_interleave /*not used*/
#define fs_trackskew /*not used*/
#define fs_headswitch /*not used*/
#define fs_trkseek /*not used*/
#define fs_qbmask /*not used*/
#define fs_qfmask /*not used*/
#define fs_postblformat /*not used*/
#define fs_nrpos /*not used*/
#define fs_postbloff /*not used*/
#define fs_rotbloff /*not used*/
#define fs_npostbl(fs, cylno) ((fs)->fs_postbl[cylno])
#define fs_nrotbl(fs) ((fs)->fs_rotbl)
#define	cg_blktot(cgp) ((cgp)->cg_btot)
#define cg_blks(fs, cgp, cylno) ((cgp)->cg_b[cylno])
#define cg_blksfree(cgp) ((cgp)->cg_free)
#define cg_chkmagic(cgp) ((cgp)->cg_magic == CG_MAGIC)
#define cg_inosused(cgp) ((cgp)->cg_iused)

