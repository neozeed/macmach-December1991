/* cmuwmraster.h
 *
 * this describes the header for ITC (CMU WM) raster files. It is
 * essentially a byte reversed Sun raster, 1 plane, no encoding.
 */

#include "copyright.h"

struct cmuwm_header
{
    char magic[4];
    char width[4];
    char height[4];
    char depth[2];
};

#define CMUWM_MAGIC 0xf10040bb
