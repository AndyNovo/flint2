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
    Copyright (C) 2010 William Hart

******************************************************************************/

#include <mpir.h>
#include "flint.h"
#include "nmod_vec.h"
#include "nmod_poly.h"

void
_nmod_poly_pow_trunc(mp_ptr res, mp_srcptr poly, 
                                       long trunc, nmod_t mod, ulong e)
{
    _nmod_poly_pow_trunc_binexp(res, poly, trunc, mod, e);
}

void
nmod_poly_pow_trunc(nmod_poly_t res, 
                           const nmod_poly_t poly, long trunc, ulong e)
{
    const long len = poly->length;
    mp_ptr p;
    int pcopy = 0;

    if (len < 2 || e < 3UL || trunc == 0)
    {
        if (len == 0 || trunc == 0)
            nmod_poly_zero(res);
        else if (len == 1)
        {
            nmod_poly_fit_length(res, 1);
            res->coeffs[0] = n_powmod2_preinv(poly->coeffs[0], e, poly->mod.n, poly->mod.ninv);
            res->length = 1;
            _nmod_poly_normalise(res);
        }
        else if (e == 0UL)
        {
            nmod_poly_set_coeff_ui(res, 0, 1UL);
            res->length = 1;
            _nmod_poly_normalise(res);
        }
        else if (e == 1UL)
        {
            nmod_poly_set(res, poly);
            nmod_poly_truncate(res, trunc);
        }
        else  /* e == 2UL */
            nmod_poly_mullow_n(res, poly, poly, trunc);

        return;
    }

    if (poly->length < trunc)
    {
        p = nmod_vec_init(trunc);
        mpn_copyi(p, poly->coeffs, poly->length);
        mpn_zero(p + poly->length, trunc - poly->length);
        pcopy = 1;
    } else
        p = poly->coeffs;

    if (res != poly || pcopy)
    {
        nmod_poly_fit_length(res, trunc);
        _nmod_poly_pow_trunc(res->coeffs, p, trunc, poly->mod, e);
    }
    else
    {
        nmod_poly_t t;
        nmod_poly_init2(t, poly->mod.n, trunc);
        _nmod_poly_pow_trunc(t->coeffs, p, trunc, poly->mod, e);
        nmod_poly_swap(res, t);
        nmod_poly_clear(t);
    }

    if (pcopy)
        nmod_vec_free(p);

    res->length = trunc;
    _nmod_poly_normalise(res);
}