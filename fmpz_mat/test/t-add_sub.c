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

    Copyright (C) 2011 Fredrik Johansson

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <mpir.h>
#include "flint.h"
#include "fmpz.h"
#include "fmpz_mat.h"
#include "ulong_extras.h"

int
main(void)
{
    long m, n, rep;
    flint_rand_t state;

    printf("add/sub/neg....");
    fflush(stdout);

    flint_randinit(state);

    for (rep = 0; rep < 10000; rep++)
    {
        fmpz_mat_t A;
        fmpz_mat_t B;
        fmpz_mat_t C;

        m = n_randint(state, 20);
        n = n_randint(state, 20);

        fmpz_mat_init(A, m, n);
        fmpz_mat_init(B, m, n);
        fmpz_mat_init(C, m, n);

        fmpz_mat_randtest(A, state, 100);
        fmpz_mat_randtest(B, state, 100);

        fmpz_mat_neg(C, A);
        fmpz_mat_add(A, A, B);
        fmpz_mat_sub(A, A, B);
        fmpz_mat_neg(A, A);

        if (!fmpz_mat_equal(A, C))
        {
            printf("FAIL: matrices not equal!\n");
            abort();
        }

        fmpz_mat_clear(A);
        fmpz_mat_clear(B);
        fmpz_mat_clear(C);
    }

    flint_randclear(state);
    _fmpz_cleanup();
    printf("PASS\n");
    return 0;
}
