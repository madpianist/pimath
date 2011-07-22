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

#ifndef _PIMATH_LINE__H_
#define _PIMATH_LINE__H_

#include <ImathLine.h>
#include <ImathVec.h>
#include "util.h"

namespace pimath
{
	namespace bp = boost::python;

	template<typename T>
	struct LineBind
	{
		typedef Imath::Line3<T> 			line_type;
		typedef Imath::Vec3<T> 				vec_type;
		typedef bp::class_<line_type>		bp_class;

		LineBind(const char* name)
		{
			T (line_type::*distanceToPoint)(
								const vec_type &) const = &line_type::distanceTo;
			T (line_type::*distanceToLine)(
								const line_type &) const = &line_type::distanceTo;
			vec_type (line_type::*closestPointToPoint)(
								const vec_type &) const = &line_type::closestPointTo;
			vec_type (line_type::*closestPointToLine)(
								const line_type &) const = &line_type::closestPointTo;


			bp_class cl(name);
			cl
			.def("__init__", bp::make_constructor(defaultInit))
			.def(bp::init<vec_type, vec_type>())
			.def("__init__", bp::make_constructor(sequenceInit))
			.def("set", &line_type::set )
			.def("__call__", call)
			.def("distanceTo", distanceToPoint )
			.def("distanceTo", distanceToLine )
			.def("closestPointTo", closestPointToPoint )
			.def("closestPointTo", closestPointToLine )
			.add_property("value", getValue, setValue)
			.def("__str__", toString);
		}

		static line_type* defaultInit( )
		{
			vec_type zero(0);
			vec_type offset(0);
			offset.x = 1;
			line_type * f = new line_type( zero, offset );
			return f;
		}

		static line_type* sequenceInit( const bp::object & x )
		{
			line_type * f = new line_type();
			setValue( *f, x );
			return f;
		}

		static bp::object
		call( const line_type & self, T parameter )
		{
			return bp::object( self(parameter) );
		}

		static std::string
		toString(const line_type& self)
		{
			std::ostringstream s;
			s << self;
			return s.str();
		}

		static bp::object
		getValue(line_type &self)
		{
			return bp::make_tuple(
			            bp::make_tuple( self.pos.x, self.pos.y, self.pos.z ),
						bp::make_tuple( self.dir.x, self.dir.y, self.dir.z ) );
		}

		static void
		setValue(line_type &self, const bp::object &o)
		{
			if( bp::len( o ) == 2 )
			{
				bp::tuple posT = bp::extract<bp::tuple>(o[0]);
				self.pos = Imath::Vec3<T>( bp::extract<T>(posT[0]),
									bp::extract<T>(posT[1]),
									bp::extract<T>(posT[2]) );
				bp::tuple dirT = bp::extract<bp::tuple>(o[1]);
				self.dir = Imath::Vec3<T>( bp::extract<T>(dirT[0]),
									bp::extract<T>(dirT[1]),
									bp::extract<T>(dirT[2]) );
			}
			else if( bp::len( o ) == 6 )
			{
				self.pos = Imath::Vec3<T>( bp::extract<T>(o[0]),
									bp::extract<T>(o[1]),
									bp::extract<T>(o[2]) );
				self.dir = Imath::Vec3<T>( bp::extract<T>(o[3]),
									bp::extract<T>(o[4]),
									bp::extract<T>(o[5]) );
			}
		}
	};
}

#endif
