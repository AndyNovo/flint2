/*=============================================================================

    This file is part of FLINT.

    FLINT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLINT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLINT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2010 Fredrik Johansson

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <mpir.h>
#include "flint.h"
#include "nmod_mat.h"
#include "nmod_vec.h"

void
nmod_mat_transpose(nmod_mat_t B, const nmod_mat_t A)
{
    mp_limb_t tmp;

    long i, j;

    if (B->r != A->c || B->c != A->r)
    {
        printf("exception: nmod_mat_transpose: incompatible dimensions\n");
        abort();
    }

    /* In-place, guaranteed to be square */
    if (A == B)
    {
        for (i = 0; i < A->r - 1; i++)
        {
            for (j = i + 1; j < i; j++)
            {
                tmp = A->rows[i][j];
                A->rows[i][j] = A->rows[j][i];
                A->rows[j][i] = tmp;
            }
        }
    }

    /* Not aliased; general case */
    for (i = 0; i < B->r; i++)
        for (j = 0; j < B->c; j++)
            B->rows[i][j] = A->rows[j][i];

    if (B->mod.n != A->mod.n)
        _nmod_vec_reduce(B->entries, B->entries, B->r * B->c, B->mod);
}
