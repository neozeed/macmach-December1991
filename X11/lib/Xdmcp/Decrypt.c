/*
 * $XConsortium: Decrypt.c,v 1.1 89/12/13 14:32:07 keith Exp $
 *
 * Copyright 1989 Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.  M.I.T. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * M.I.T. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL M.I.T.
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author:  Keith Packard, MIT X Consortium
 */

#include    "Xos.h"
#include    "X.h"
#include    "Xmd.h"
#include    "Xdmcp.h"

#ifdef HASDES

#ifdef NOTDEF
/*
 * The following function exists only to demonstrate the
 * desired functional interface for this routine.  You will
 * need to add the appropriate algorithm if you wish to
 * use XDM-AUTHENTICATION-1/XDM-AUTHORIZATION-1.
 *
 * The interface for this routine is quite simple.  Both
 * arguments are arrays of 8 unsigned characters, the first
 * is 64 bits of encrypted data, the second is 56 bits of useful
 * data packed 7 bits per byte in the low 7 bits of the word.
 *
 * The 64 bits of data are to be modified in place with the
 * decrypted result.
 *
 * Examine the XDMCP specification for the correct algorithm
 */

static void
decryptBits (data, key)
    unsigned char   *data;
    unsigned char   *key;
{
}
#endif

void
XdmcpDecrypt (crypto, key, plain, bytes)
    unsigned char	*crypto, *plain;
    XdmAuthKeyPtr	key;
    int			bytes;
{
    int		    i, j, k;
    int		    len;
    unsigned char   tmp[8];
    unsigned char   blocks[2][8];

    k = 0;
    for (j = 0; j < bytes; j += 8)
    {
	if (bytes - j < 8)
	    return; /* bad crypto length */
	for (i = 0; i < 8; i++)
	{
	    tmp[i] = crypto[j + i];
	    blocks[k][i] = crypto[j + i];
	}
	decryptBits (tmp, key->data);
	/* block chaining */
	k = (k == 0) ? 1 : 0;
	for (i = 0; i < 8; i++)
	{
	    if (j == 0)
		plain[j + i] = tmp[i];
	    else
		plain[j + i] = tmp[i] ^ blocks[k][i];
	}
    }
}

#endif /* HASDES */
