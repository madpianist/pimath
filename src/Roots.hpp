/*******************************************************************************
Copyright (c) 2011, Dr. D. Studios
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list
of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright notice, this
list of conditions and the following disclaimer in the documentation and/or other
materials provided with the distribution.
Neither the name of the Dr. D. Studios nor the names of its contributors may be
used to endorse or promote products derived from this software without specific
prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

#ifndef _PIMATH_ROOTS__H_
#define _PIMATH_ROOTS__H_

#include <boost/python.hpp>
#include <ImathMath.h>
#include <ImathRoots.h>
#include "util.h"

/**
 * These functions have been changed to return tuples rather than the root count.
 * If there are no roots, an empty tuple will be returned (rather than None).
 * Return arguments have been removed.
 */

namespace pimath
{
	namespace bp = boost::python;


	template<typename T>
	struct RootsBind
	{
		RootsBind()
		{
			bp::def("solveLinear", solveLinear);
			bp::def("solveQuadratic", solveQuadratic);
			bp::def("solveNormalizedCubic", solveNormalizedCubic );
			bp::def("solveCubic", solveCubic );
		}

		static bp::object
		solveLinear( T a, T b )
		{
			T rv;
			return Imath::solveLinear( a, b, rv ) == 1 ?
					bp::make_tuple( rv ) : bp::make_tuple();
		}

		static bp::object
		solveQuadratic( T a, T b, T c )
		{
			T x[2];
			int count = Imath::solveQuadratic( a, b, c, x );
			switch( count )
			{
				case 1:
					return bp::make_tuple(x[0]);
				case 2:
					return bp::make_tuple(x[0], x[1]);
			}
			return bp::make_tuple();
		}

		static bp::object
		solveNormalizedCubic( T r, T s, T t )
		{
			T x[3];
			int count = Imath::solveNormalizedCubic( r, s, t, x );
			switch( count )
			{
				case 1:
					return bp::make_tuple(x[0]);
				case 2:
					return bp::make_tuple(x[0],x[1]);
				case 3:
					return bp::make_tuple(x[0],x[1],x[2]);
			}
			return bp::make_tuple();
		}

		static bp::object
		solveCubic( T a, T b, T c, T d )
		{
			T x[3];
			int count = Imath::solveCubic( a, b, c, d, x );
			switch( count )
			{
				case 1:
					return bp::make_tuple(x[0]);
				case 2:
					return bp::make_tuple(x[0],x[1]);
				case 3:
					return bp::make_tuple(x[0],x[1],x[2]);
			}
			return bp::make_tuple();
		}
	};
}

#endif

