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

#ifndef _PIMATH_SHEAR__H_
#define _PIMATH_SHEAR__H_

#include <ImathShear.h>
#include "util.h"


namespace pimath
{
	namespace bp = boost::python;

	template<typename T, typename ScalarTypes>
	struct ShearBind
	{
		typedef T							scalar_type;
		typedef Imath::Shear6<T> 			shear_type;
		typedef Imath::Vec3<T> 				vec_type;
		typedef bp::class_<shear_type>		bp_class;

		ShearBind(const char* name)
		{
			bp_class cl(name);
			cl
			.add_property("xy", bp::make_getter(&shear_type::xy, bp_ret_val()),
				bp::make_setter(&shear_type::xy))
			.add_property("xz", bp::make_getter(&shear_type::xz, bp_ret_val()),
				bp::make_setter(&shear_type::xz))
			.add_property("yz", bp::make_getter(&shear_type::yz, bp_ret_val()),
				bp::make_setter(&shear_type::yz))
			.add_property("yx", bp::make_getter(&shear_type::yx, bp_ret_val()),
				bp::make_setter(&shear_type::yx))
			.add_property("zx", bp::make_getter(&shear_type::zx, bp_ret_val()),
				bp::make_setter(&shear_type::zx))
			.add_property("zy", bp::make_getter(&shear_type::zy, bp_ret_val()),
				bp::make_setter(&shear_type::zy))
			.def(bp::init<>())
			.def(bp::init<T,T,T>())
			.def(bp::init<vec_type>())
			.def(bp::init<T,T,T,T,T,T>())
			.def("__init__", bp::make_constructor(sequenceInit))
			.add_property("value", getValue, setValue)
			.def("equalWithAbsError", &shear_type::equalWithAbsError)
			.def("equalWithRelError", &shear_type::equalWithRelError)
			.def(bp::self + bp::self)
			.def(bp::self += bp::self)
			.def(bp::self - bp::self)
			.def(bp::self -= bp::self)
			.def(bp::self == bp::self)
			.def(bp::self != bp::self)
			.def(- bp::self)
			.def("negate", &vec_type::negate, bp_ret_none())
			.def(bp::self * bp::self)
			.def(bp::self *= bp::self)
			.def(bp::self / bp::self)
			.def(bp::self /= bp::self)
			.def(bp::self * bp::other<scalar_type>())
			.def(bp::self *= bp::other<scalar_type>())
			.def(bp::self / bp::other<scalar_type>())
			.def(bp::self /= bp::other<scalar_type>())
			.def(bp::other<scalar_type>() * bp::self)
			.def("__str__", toString );
		}

		static shear_type*
		sequenceInit(const bp::object &o)
		{
			shear_type* v = new shear_type();
			setValue(*v, o);
			return v;
		}

		static bp::object
		getValue(shear_type &self)
		{
			return bp::make_tuple(self.xy, self.xz, self.yz, self.yx, self.zx, self.zy);
		}

		static void setValue(shear_type &self, const bp::object &o)
		{
			for(unsigned int i=0; i<6; ++i)
				self[i] = bp::extract<T>(o[i]);
		}

		static std::string
		toString(const shear_type& self)
		{
			std::ostringstream s;
			s << "(" << self.xy << " " << self.xz << " " << self.yz
			  << " " << self.yx << " " << self.zx << " " << self.zy << ")";
			return s.str();
		}
	};
}

#endif
