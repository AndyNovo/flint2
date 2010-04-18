/*============================================================================

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

===============================================================================*/
/****************************************************************************

   Copyright (C) 2009 William Hart

*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <mpir.h>
#include "flint.h"
#include "ulong_extras.h"
#include "fmpz.h"

void fmpz_randm(fmpz_t f, fmpz_t m)
{
   mp_bitcnt_t bits = fmpz_bits(m);
   int sgn = fmpz_sgn(m);

   if (bits <= FLINT_BITS - 2)
   {
      _fmpz_demote(f);
      if (sgn >= 0)
		 *f = n_randint(*m);
	  else
         *f = -n_randint(-(*m));
   } else
   {
      __mpz_struct * mpz_ptr = _fmpz_promote(f);
      mpz_urandomm(mpz_ptr, fmpz_randstate, COEFF_TO_PTR(*m));
      if (sgn < 0) mpz_neg(mpz_ptr, mpz_ptr);
	  _fmpz_demote_val(f);
   }
}