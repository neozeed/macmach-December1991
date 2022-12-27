/*
 * Determine colormap of rgb tuple and remap for xloadimage
 *
 * Portions of this were borrowed from the PBMPLUS software package
 * written by Jef Poskanzer (see Copyright below)
 */
/*
** Copyright (C) 1989 by Jef Poskanzer.
**
** Permission to use, copy, modify, and distribute this software and its
** documentation for any purpose and without fee is hereby granted, provided
** that the above copyright notice appear in all copies and that both that
** copyright notice and this permission notice appear in supporting
** documentation.  This software is provided "as is" without express or
** implied warranty.
*/

#define PM_SCALE(a, b, c) (long)((a) * (c))/(b)
#define HASHLEN 6553
#define HashRGB(r, g, b)  ((r)*33023 + (g)*30013 + (b)*27011) % HASHLEN

typedef struct hash_elem *hash_ent;

struct hash_elem
{
	struct pix
	{ unsigned int r,g,b;} pix;
	unsigned long mapnum;
	struct hash_elem *next;
};

typedef hash_ent *hash_list;

/* Get memory set up for hashing */
hash_list init_hash();

/* Add a colour to the hashing list */
void add_hash( /* hash_list hl; unsigned int r,g,b */ );

/* Count the colours in the map */
int count_hash( /* hash_list hl */);

/* Fill a colour map from hash_list */
int fill_map( /* hash_list hl; RGBMap *rgbm; unsigned int maxval */ );

/* Find map value from rgb triplet */
unsigned int find_hash( /* hash_list hl; unsigned int r,g,b */ );

/* Free hash list */
void free_hash( /* hash_list hl */ );

/* Free hash list element */
void free_helem( /* hash_ent he */ );
