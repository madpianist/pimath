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

#ifndef _PIMATH_LINEALGO__H_
#define _PIMATH_LINEALGO__H_

#include <boost/python.hpp>
#include <ImathLineAlgo.h>
#include <ImathLine.h>
#include "util.h"

/*
 * closestPoints, intersect: now return a tuple f the output values rather than
 * altering args.
 */

namespace pimath
{
	namespace bp = boost::python;


	template<typename T>
	struct LineAlgoBind
	{
		typedef Imath::Line3<T> 	line_type;
		typedef Imath::Vec3<T> 		vec3_type;
		LineAlgoBind()
		{
			vec3_type (*closestVertex)(
					const vec3_type &, const vec3_type &, const vec3_type &,
					const line_type & ) = &Imath::closestVertex;

			bp::def("closestPoints", closestPoints);
			bp::def("intersect", intersect);
			bp::def("closestVertex", closestVertex);
			bp::def("rotatePoint", &Imath::rotatePoint<T> );
		}

		static bp::object
		closestPoints( const line_type & line1, const line_type & line2 )
		{
			vec3_type p1, p2;
			return Imath::closestPoints( line1, line2, p1, p2 ) ?
						bp::make_tuple( p1, p2 ) : bp::object();
		}

		static bp::object
		intersect( const line_type & line, const vec3_type & v0,
		           const vec3_type & v1, const vec3_type & v2 )
		{
			vec3_type pt, bary;
			bool front;
			return Imath::intersect( line, v0, v1, v2, pt, bary, front) ?
						bp::make_tuple( pt, bary, front ) : bp::object();
		}

		static vec3_type
		rotatePoint( const vec3_type p, line_type l, T angle )
		{
			return Imath::rotatePoint( p, l, angle );
		}
	};
}

#endif

