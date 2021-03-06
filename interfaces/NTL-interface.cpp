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

    NTL-interface.cpp: Functions for conversion between NTL and FLINT format

    Copyright (C) 2007 William Hart
    Copyright (C) 2011 Sebastian Pancratz

******************************************************************************/

#include <cstdio>

#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include <NTL/mat_ZZ.h>
#include <NTL/lip.h>
#include <NTL/ctools.h>
#include <NTL/g_lip.h>
#include <gmp.h>

#include "flint.h"
#include "fmpz.h"
#include "fmpz_poly.h"
#include "NTL-interface.h"

#define ZZ_SIZE(p) (((long *) (p))[1])
#define ZZ_DATA(p) ((mp_limb_t *) (((long *) (p)) + 2))

static void fmpz_set_limbs(fmpz_t f, mp_srcptr x, mp_size_t limbs)
{
    if (limbs == 0)
    {
        fmpz_zero(f);
    }
    else if (limbs == 1)
    {
        fmpz_set_ui(f, x[0]);
    }
    else
    {
        __mpz_struct *mpz_ptr = _fmpz_promote(f);

        mpz_import(mpz_ptr, limbs, -1, sizeof(mp_limb_t), 0, 0, x);
    }
}

mp_size_t ZZ_limbs(const ZZ& z)
{
    return (z.rep) ? FLINT_ABS(ZZ_SIZE(z.rep)) : 0;
}

void fmpz_set_ZZ(fmpz_t rop, const ZZ& op)
{
    const _ntl_gbigint x = op.rep;

    if (!x) 
    {
        fmpz_zero(rop);
    }
    else
    {
        const mp_size_t lw = ZZ_limbs(op);
        const mp_limb_t *xp = ZZ_DATA(x);

        fmpz_set_limbs(rop, xp, lw);

        if (op < 0L)
            fmpz_neg(rop, rop);
    }
}

void fmpz_poly_get_ZZX(ZZX& rop, const fmpz_poly_t op)
{
    const long len = op->length;

    if (len == 0)
    {
        rop = 0;
    }
    else
    {
        long i;
        ZZ *ap;

        rop.rep.SetLength(len);

        for (i = 0, ap = rop.rep.elts(); i < len; i++, ap++)
        {
            fmpz_get_ZZ(*ap, op->coeffs + i);
        }
    }
}

void fmpz_poly_set_ZZX(fmpz_poly_t rop, const ZZX& op)
{
    const long len = deg(op) + 1;

    if (len == 0)
    {
        fmpz_poly_zero(rop);
    }
    else
    {
        long i;
        const ZZ *ap; 

        fmpz_poly_fit_length(rop, len);
        _fmpz_poly_set_length(rop, len);

        for (i = 0, ap = op.rep.elts(); i < len; i++, ap++)
        {
            fmpz_set_ZZ(rop->coeffs + i, *ap);
        }
    }
}

#undef ZZ_SIZE
#undef ZZ_DATA

