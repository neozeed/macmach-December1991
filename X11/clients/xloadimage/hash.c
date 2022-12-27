/*
 * Determine colormap of rgb tuple and remap for xloadimage
 *
 * Portions of this were borrowed from the PBMPLUS software package
 * written by Jef Poskanzer (see Copyright below)
 *
 * The rest of it was munged up by Ian MacPhedran (macphed@dvinci.usask.ca)
 *
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
#include "hash.h"

/* Swiped from image.h, but it's the only thing we need */
typedef unsigned short Intensity;
typedef struct rgbmap {
  unsigned int  size;  /* size of RGB map */
  unsigned int  used;  /* number of colors used in RGB map */
  unsigned int  compressed; /* image uses colormap fully */
  Intensity    *red;   /* color values in X style */
  Intensity    *green;
  Intensity    *blue;
} RGBMap;

/* Get memory set up for hashing */
hash_list init_hash()
{
	hash_list h; int i;
	h = (hash_list) lmalloc(HASHLEN*sizeof(hash_ent));
	if (h == (hash_list) 0)
	{
		printf("Can't allocate room for mapping function.\n");
		exit(1);
	}

	for (i=0; i<HASHLEN; i++) h[i] = (hash_ent)0;
	return( h );
}

/* Add a colour to the hashing list */
void add_hash(hl,r,g,b)
hash_list hl; unsigned int r,g,b;
{
	int hval;
	hash_ent loc,*prev;
	hval = HashRGB(r,g,b);
	prev = &(hl[hval]);
	loc = hl[hval];
	while (loc != (hash_ent)0)
	{ /* Descend through entries */
		if ((loc->pix.r == r) && (loc->pix.g == g) && (loc->pix.b == b))
			return;
		prev = &(loc->next);
		loc = loc->next;
	} /* Fell through - must be new colour */

	loc = (hash_ent)lmalloc(sizeof(struct hash_elem));
	if (loc == (hash_ent)0)
	{
		printf("Out of memory in color hashing.\n");
		exit(1);
	}
	loc->pix.r = r;
	loc->pix.g = g;
	loc->pix.b = b;
	loc->mapnum = 0; loc->next = (hash_ent)0;
	*prev = loc;
	return;
}

/* Generate a listing for the colour table from the hash list */
int count_hash(hl)
hash_list hl;
{
	int i,j;
	hash_ent loc;
	j = 0;
	for (i=0; i<HASHLEN; i++)
	{
		if ((hash_ent)hl[i] != (hash_ent)0)
		{
			loc = (hash_ent)hl[i];
			while (loc != (hash_ent)0)
			{
			 loc->mapnum = j++;
			 loc = loc->next;
			}
		}
	}
	return(j);
}

/* Generate a color map from the hash list */
int fill_map(hl,rgbm,maxval)
hash_list hl; RGBMap *rgbm; unsigned int maxval;
{
	int i,j;
	hash_ent loc;
	j = 0;
	if (rgbm == (RGBMap *)0)
	{
		printf("Memory map not allocated.\n");
		exit(1);
	}
	for (i=0; i<HASHLEN; i++)
	{
		if ((hash_ent)hl[i] != (hash_ent)0)
		{
			loc = (hash_ent)hl[i];
			while (loc != (hash_ent)0)
			{
			 j = loc->mapnum;
			 rgbm->red[j] = PM_SCALE(loc->pix.r,maxval,0xffff);
			 rgbm->green[j] = PM_SCALE(loc->pix.g,maxval,0xffff);
			 rgbm->blue[j] = PM_SCALE(loc->pix.b,maxval,0xffff);
			 loc = loc->next;
			}
		}
	}
	return(j);
}

/* Find map value from rgb triplet */
unsigned int find_hash(hl,r,g,b)
hash_list hl; unsigned int r,g,b;
{
	int hval;
	hash_ent loc;
	hval = HashRGB(r,g,b);
	loc = hl[hval];
	while (loc != (hash_ent)0)
	{ /* Descend through entries */
		if ((loc->pix.r == r) && (loc->pix.g == g) && (loc->pix.b == b))
			return(loc->mapnum);
		loc = loc->next;
	} /* Fell through - must be new colour - shouldn't happen here */

	printf("Colour not found in hash table.\n");
	exit(1);
}

/* Free hash list */
void free_hash(hl)
hash_list hl;
{
	int i;

	for (i=0; i<HASHLEN; i++)
	{
		if (hl[i] != 0) free_helem(hl[i]);
	}

	lfree(hl);
}

void free_helem(he)
hash_ent he;
{
	if (he->next != (hash_ent)0) free_helem(he->next);
	lfree(he);
}
