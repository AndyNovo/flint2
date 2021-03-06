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

#include <stdlib.h>
#include "flint.h"
#include "fmpz.h"
#include "fmpz_mat.h"
#include "fmpz_vec.h"
#include "fmpz_factor.h"
#include "ulong_extras.h"


void
fmpz_mat_randdet(fmpz_mat_t mat, flint_rand_t state, const fmpz_t det)
{
    long i, j, k, n;
    int parity;
    fmpz * diag;
    fmpz_factor_t factor;

    n = mat->r;
    if (n != mat->c)
    {
        printf("exception: fmpz_mat_randdet: need a square matrix\n");
        abort();
    }

    if (n < 1)
        return;

    /* Start with the zero matrix */
    fmpz_mat_zero(mat);

    if (*det == 0L)
        return;

    fmpz_factor_init(factor);
    fmpz_factor(factor, det);

    diag = _fmpz_vec_init(n);
    for (i = 0; i < n; i++)
        fmpz_one(&diag[i]);

    /* Form diagonal entries that multiply to the determinant */
    for (i = 0; i < factor->length; i++)
    {
        for (j = 0; j < fmpz_get_ui(&factor->exp[i]); j++)
        {
            k = n_randint(state, n);
            fmpz_mul(&diag[k], &diag[k], &factor->p[i]);
        }
    }

    /* Reverse signs an even number of times */
    for (i = 0; i < 2*n; i++)
    {
        k = n_randint(state, n);
        fmpz_neg(&diag[k], &diag[k]);
    }

    if (factor->sign == -1)
        fmpz_neg(&diag[0], &diag[0]);

    parity = fmpz_mat_randpermdiag(mat, state, diag, n);

    /* Need another reversal if the permutation was odd */
    if (parity)
    {
        for (i = 0; i < mat->r; i++)
        {
            for (j = 0; j < mat->c; j++)
            {
                if (!fmpz_is_zero(mat->rows[i] + j))
                {
                    fmpz_neg(mat->rows[i] + j, mat->rows[i] + j);
                    goto end;
                }
            }
        }
    }
    end:

    _fmpz_vec_clear(diag, n);
    fmpz_factor_clear(factor);
}
