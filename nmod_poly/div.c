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

    Copyright (C) 2010 Sebastian Pancratz
    Copyright (C) 2011 William Hart

******************************************************************************/

#include <stdlib.h>
#include <mpir.h>
#include "flint.h"
#include "nmod_vec.h"
#include "nmod_poly.h"

void
_nmod_poly_div(mp_ptr Q, mp_srcptr A, long lenA, 
                                  mp_srcptr B, long lenB, nmod_t mod)
{
    if (lenB < 6000)
        _nmod_poly_div_divconquer(Q, A, lenA, B, lenB, mod);
    else
        _nmod_poly_div_newton(Q, A, lenA, B, lenB, mod);
}

void
nmod_poly_div(nmod_poly_t Q, 
                 const nmod_poly_t A, const nmod_poly_t B)
{
    nmod_poly_t tQ;
    mp_ptr q;
    long A_len, B_len;

    B_len = B->length;
    
    if (B_len == 0)
    {
        printf("Exception: division by zero in nmod_poly_divrem\n");
        abort();
    }

    A_len = A->length;
    
    if (A_len < B_len)
    {
        nmod_poly_zero(Q);
        return;
    }

    if (Q == A || Q == B)
    {
        nmod_poly_init2(tQ, A->mod.n, A_len - B_len + 1);
        q = tQ->coeffs;
    }
    else
    {
        nmod_poly_fit_length(Q, A_len - B_len + 1);
        q = Q->coeffs;
    }

    _nmod_poly_div(q, A->coeffs, A_len,
                            B->coeffs, B_len, A->mod);

    if (Q == A || Q == B)
    {
        nmod_poly_swap(tQ, Q);
        nmod_poly_clear(tQ);
    }
    
    Q->length = A_len - B_len + 1;
}
