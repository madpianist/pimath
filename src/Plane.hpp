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

#ifndef _PIMATH_PLANE__H_
#define _PIMATH_PLANE__H_

#include <ImathPlane.h>
#include "util.h"

/**
 * intersect and intersectT now return the point of intersection rather than
 * whether the intersection occurred (they return None if no intersection)
 */

namespace pimath
{
	namespace bp = boost::python;

	template<typename T>
	struct PlaneBind
	{
		typedef Imath::Plane3<T> 			plane_type;
		typedef Imath::Vec3<T> 				vec_type;
		typedef Imath::Line3<T> 			line_type;
		typedef bp::class_<plane_type>		bp_class;

		PlaneBind(const char* name)
		{
			void (plane_type::*pointSet)(
					const vec_type &, const vec_type &) = &plane_type::set;
			void (plane_type::*pointDistSet)(
					const vec_type &, T ) = &plane_type::set;
			void (plane_type::*triangleSet)(
					const vec_type &, const vec_type &, const vec_type &) = &plane_type::set;

			bp_class cl(name);
			cl
			.def("__init__", bp::make_constructor(defaultInit))
			.def(bp::init<vec_type, vec_type>())
			.def(bp::init<vec_type, T>())
			.def(bp::init<vec_type, vec_type, vec_type>())
			.def("__init__", bp::make_constructor(sequenceInit))
			.def("set", pointSet )
			.def("set", pointDistSet )
			.def("set", triangleSet )
			.def("intersect", intersect )
			.def("intersectT", intersectT )
			.def("distanceTo", &plane_type::distanceTo )
			.def("reflectPoint", &plane_type::reflectPoint )
			.def("reflectVector", &plane_type::reflectVector )
			.add_property("value", getValue, setValue)
			.def("__str__", toString);
		}

		static plane_type*
		defaultInit( )
		{
			return new plane_type(vec_type(0,1,0),0);
		}

		static plane_type*
		sequenceInit( const bp::object & x )
		{
			plane_type * f = new plane_type();
			setValue( *f, x );
			return f;
		}

		static bp::object
		intersect( plane_type & self, const line_type & line )
		{
			vec_type intersection;
			return self.intersect( line, intersection ) ?
					bp::object( intersection ) : bp::object();
		}

		static bp::object
		intersectT( plane_type & self, const line_type & line )
		{
			T parameter;
			return self.intersectT( line, parameter ) ?
					bp::object( parameter ) : bp::object();
		}

		static std::string
		toString(const plane_type& self)
		{
			std::ostringstream s;
			s << self;
			return s.str();
		}

		static bp::object
		getValue(plane_type &self)
		{
			return bp::make_tuple( bp::make_tuple(self.normal.x, self.normal.y, self.normal.z),
								   self.distance );
		}

		static void
		setValue(plane_type &self, const bp::object &o)
		{
			if( bp::len( o ) == 2 )
			{
				// This is the ( (x, y, z), d ) case.
				bp::tuple normal = bp::extract< bp::tuple >( o[0] );
				self.normal = vec_type( bp::extract<T>(normal[0]),
									   bp::extract<T>(normal[1]),
									   bp::extract<T>(normal[2]));
				self.distance = bp::extract<T>(o[1]);
			}
			else if( bp::len( o ) == 4 )
			{
				// This is the ( x, y, z, d ) case.
				self.normal = vec_type( bp::extract<T>(o[0]),
									   bp::extract<T>(o[1]),
									   bp::extract<T>(o[2]));
				self.distance = bp::extract<T>(o[3]);
			}
		}
	};
}

#endif
