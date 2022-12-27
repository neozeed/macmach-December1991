/*
 * $XConsortium: Encrypt.c,v 1.1 89/12/13 14:32:54 keith Exp $
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
 * is 64 bits of useful data, the second is 56 bits of useful
 * data packed 7 bits per byte in the low 7 bits of the word
 *
 * The 64 bits of data are to be modified in place with the
 * encrypted result
 *
 * Examine the XDMCP specification for the correct algorithm
 */

static void
encryptBits (data, key)
    unsigned char   *data;
    unsigned char   *key;
{
}
#endif

void
XdmcpEncrypt (plain, key, crypto, bytes)
    unsigned char	*plain, *crypto;
    XdmAuthKeyPtr	key;
    int			bytes;
{
    int		    i, j;
    int		    len;
    unsigned char   tmp[8];

    for (j = 0; j < bytes; j += 8)
    {
	len = 8;
	if (bytes - j < len)
	    len = bytes - j;
	/* block chaining */
	for (i = 0; i < len; i++)
	{
	    if (j == 0)
		tmp[i] = plain[i];
	    else
		tmp[i] = plain[j + i] ^ crypto[j - 8 + i];
	}
	for (; i < 8; i++)
	{
	    if (j == 0)
		tmp[i] = 0;
	    else
		tmp[i] = 0 ^ crypto[j - 8 + i];
	}
	encryptBits (tmp, key->data);
	for (i = 0; i < 8; i++)
	    crypto[j + i] = tmp[i];
    }
}
#endif
