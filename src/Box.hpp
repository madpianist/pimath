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

#ifndef _PIMATH_BOX__H_
#define _PIMATH_BOX__H_

#include <ImathBox.h>
#include "Vec.hpp"

namespace pimath
{
	namespace bp = boost::python;

	template<typename T >
	struct BoxBind
	{
		typedef typename T::BaseType 	value_type;
		typedef T 						vec_type;
		typedef Imath::Box<T> 			box_type;

		BoxBind( const char* name )
		{
			void (box_type::*fn_extendByVec)(const vec_type&) = &box_type::extendBy;
			void (box_type::*fn_extendByBox)(const box_type&) = &box_type::extendBy;
			bool (box_type::*fn_intersectsVec)(const vec_type&) const = &box_type::intersects;
			bool (box_type::*fn_intersectsBox)(const box_type&) const = &box_type::intersects;

			bp::class_<box_type>(name)
			.def("__init__", bp::make_constructor(sequenceInit))
			.def_readwrite("min", &box_type::min)
			.def_readwrite("max", &box_type::max)
			.add_property("value", getValue, setValue)
			.def(bp::init<vec_type>())
			.def(bp::init<vec_type, vec_type>())
			.def(bp::init<box_type>())
			.def(bp::self == bp::self)
			.def(bp::self != bp::self)
			.def("makeEmpty", &box_type::makeEmpty)
			.def("extendBy", fn_extendByVec)
			.def("extendBy", fn_extendByBox)
			.def("intersects", fn_intersectsVec)
			.def("intersects", fn_intersectsBox)
			.def("size", &box_type::size)
			.def("center", &box_type::center)
			.def("majorAxis", &box_type::majorAxis)
			.def("isEmpty", &box_type::isEmpty)
			.def("hasVolume", &box_type::hasVolume)
			.def("__str__", toString)
			;
		}

		static box_type* sequenceInit(const bp::object &o)
		{
			box_type* b = new box_type();
			setValue(*b, o);
			return b;
		}

		static std::string toString(const box_type& self)
		{
			std::ostringstream s;
			s << '(' << self.min << ' ' << self.max << ')';
			return s.str();
		}

		static bp::tuple getValue(const box_type &self)
		{
			typedef typename VecNBind_<vec_type>::type vec_bind;
			return bp::make_tuple(vec_bind::getValue(self.min), vec_bind::getValue(self.max));
		}

		static void setValue(box_type &self, const bp::object &x)
		{
			typedef typename VecBind_<vec_type>::type vec_bind;
			vec_bind::setValue(self.min, bp::extract<bp::tuple>(x[0]));
			vec_bind::setValue(self.max, bp::extract<bp::tuple>(x[1]));
		}
	};
}

#endif








