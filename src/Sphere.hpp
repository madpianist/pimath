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

#ifndef _PIMATH_SPHERE__H_
#define _PIMATH_SPHERE__H_

#include <ImathSphere.h>
#include "util.h"

/**
 * Intersect/IntersectT now return the result (or None if no intersection)
 */

namespace pimath
{
	namespace bp = boost::python;

	template<typename T>
	struct SphereBind
	{
		typedef T							scalar_type;
		typedef Imath::Sphere3<T> 			sphere_type;
		typedef Imath::Vec3<T> 				vec_type;
		typedef Imath::Line3<T> 			line_type;
		typedef bp::class_<sphere_type>		bp_class;

		SphereBind(const char* name)
		{
			bp_class cl(name);
			cl
			.add_property("center", bp::make_getter(&sphere_type::center, bp_ret_val()),
				bp::make_setter(&sphere_type::center))
			.add_property("radius", bp::make_getter(&sphere_type::radius, bp_ret_val()),
				bp::make_setter(&sphere_type::radius))
			.def(bp::init<>())
			.def(bp::init<vec_type, T>())
			.def("__init__", bp::make_constructor(sequenceInit))
			.add_property("value", getValue, setValue)
			.def("circumscribe",&sphere_type::circumscribe)
			.def("intersect",intersect)
			.def("intersectT",intersectT)
			.def("__str__", toString);
		}

		static sphere_type*
		sequenceInit(const bp::object &o)
		{
			sphere_type* v = new sphere_type();
			setValue(*v, o);
			return v;
		}

		static bp::object
		intersect( const sphere_type & self, const line_type& line)
		{
			vec_type rv;
			return self.intersect( line, rv ) ?
					bp::object( rv ) : bp::object();
		}

		static bp::object
		intersectT( const sphere_type & self, const line_type& line)
		{
			T rv;
			return self.intersectT( line, rv ) ?
					bp::object( rv ) : bp::object();
		}

		static std::string
		toString(const sphere_type& self)
		{
			std::ostringstream s;
			s << "(" << self.center << " " << self.radius << ")";
			return s.str();
		}

		static bp::object
		getValue(sphere_type &self)
		{
			return bp::make_tuple( bp::make_tuple(self.center.x, self.center.y, self.center.z),
			                       self.radius );
		}

		static void setValue(sphere_type &self, const bp::object &o)
		{
			if( bp::len( o ) == 2 )
			{
				// This is the ( (x, y, z), r ) case.
				bp::tuple center = bp::extract< bp::tuple >( o[0] );
				self.center = vec_type( bp::extract<T>(center[0]),
									   bp::extract<T>(center[1]),
									   bp::extract<T>(center[2]));
				self.radius = bp::extract<T>(o[1]);
			}
			else if( bp::len( o ) == 4 )
			{
				// This is the ( x, y, z, r ) case.
				self.center = vec_type( bp::extract<T>(o[0]),
									   bp::extract<T>(o[1]),
									   bp::extract<T>(o[2]));
				self.radius = bp::extract<T>(o[3]);
			}
		}
	};
}

#endif
