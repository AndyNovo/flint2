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
#include "flint.h"
#include "nmod_mat.h"
#include "nmod_poly.h"
#include "nmod_poly_mat.h"

int
main(void)
{
    flint_rand_t state;
    long i;

    printf("sub....");
    fflush(stdout);

    flint_randinit(state);

    /* Check evaluation homomorphism */
    for (i = 0; i < 1000; i++)
    {
        nmod_poly_mat_t A, B, C;
        nmod_mat_t a, b, c, d;
        mp_limb_t mod, x;
        long m, n, deg;

        m = n_randint(state, 20);
        n = n_randint(state, 20);
        deg = 1 + n_randint(state, 10);
        mod = n_randtest_prime(state, 0);

        nmod_poly_mat_init(A, m, n, mod);
        nmod_poly_mat_init(B, m, n, mod);
        nmod_poly_mat_init(C, m, n, mod);

        nmod_mat_init(a, m, n, mod);
        nmod_mat_init(b, m, n, mod);
        nmod_mat_init(c, m, n, mod);
        nmod_mat_init(d, m, n, mod);

        nmod_poly_mat_randtest(A, state, deg);
        nmod_poly_mat_randtest(B, state, deg);
        nmod_poly_mat_sub(C, A, B);

        x = n_randint(state, mod);

        nmod_poly_mat_evaluate_nmod(a, A, x);
        nmod_poly_mat_evaluate_nmod(b, B, x);
        nmod_poly_mat_evaluate_nmod(d, C, x);
        nmod_mat_sub(c, a, b);

        if (!nmod_mat_equal(c, d))
        {
            printf("FAIL:\n");
            printf("A:\n");
            nmod_poly_mat_print(A, "x");
            printf("B:\n");
            nmod_poly_mat_print(B, "x");
            printf("C:\n");
            nmod_poly_mat_print(C, "x");
            printf("\n");
            abort();
        }

        nmod_poly_mat_clear(A);
        nmod_poly_mat_clear(B);
        nmod_poly_mat_clear(C);

        nmod_mat_clear(a);
        nmod_mat_clear(b);
        nmod_mat_clear(c);
        nmod_mat_clear(d);
    }

    /* Check aliasing C and A */
    for (i = 0; i < 1000; i++)
    {
        nmod_poly_mat_t A, B, C;
        long m, n, deg;
        mp_limb_t mod;

        mod = n_randtest_prime(state, 0);
        m = n_randint(state, 20);
        n = n_randint(state, 20);
        deg = 1 + n_randint(state, 10);

        nmod_poly_mat_init(A, m, n, mod);
        nmod_poly_mat_init(B, m, n, mod);
        nmod_poly_mat_init(C, m, n, mod);

        nmod_poly_mat_randtest(A, state, deg);
        nmod_poly_mat_randtest(B, state, deg);

        nmod_poly_mat_sub(C, A, B);
        nmod_poly_mat_sub(A, A, B);

        if (!nmod_poly_mat_equal(C, A))
        {
            printf("FAIL:\n");
            printf("A:\n");
            nmod_poly_mat_print(A, "x");
            printf("B:\n");
            nmod_poly_mat_print(B, "x");
            printf("C:\n");
            nmod_poly_mat_print(C, "x");
            printf("\n");
            abort();
        }

        nmod_poly_mat_clear(A);
        nmod_poly_mat_clear(B);
        nmod_poly_mat_clear(C);
    }

    /* Check aliasing C and B */
    for (i = 0; i < 1000; i++)
    {
        nmod_poly_mat_t A, B, C;
        long m, n, deg;
        mp_limb_t mod;

        mod = n_randtest_prime(state, 0);
        m = n_randint(state, 20);
        n = n_randint(state, 20);
        deg = 1 + n_randint(state, 10);

        nmod_poly_mat_init(A, m, n, mod);
        nmod_poly_mat_init(B, m, n, mod);
        nmod_poly_mat_init(C, m, n, mod);

        nmod_poly_mat_randtest(A, state, deg);
        nmod_poly_mat_randtest(B, state, deg);

        nmod_poly_mat_sub(C, A, B);
        nmod_poly_mat_sub(B, A, B);

        if (!nmod_poly_mat_equal(C, B))
        {
            printf("FAIL:\n");
            printf("A:\n");
            nmod_poly_mat_print(A, "x");
            printf("B:\n");
            nmod_poly_mat_print(B, "x");
            printf("C:\n");
            nmod_poly_mat_print(C, "x");
            printf("\n");
            abort();
        }

        nmod_poly_mat_clear(A);
        nmod_poly_mat_clear(B);
        nmod_poly_mat_clear(C);
    }

    flint_randclear(state);
    printf("PASS\n");
    return 0;
}
