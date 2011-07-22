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

#ifndef _PIMATH_UTIL__H_
#define _PIMATH_UTIL__H_

#include <ImathVec.h>
#include <vector>
#include <sstream>
#include <boost/python.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/remove_if.hpp>
#include <boost/type_traits/is_integral.hpp>
#include "traits.hpp"


namespace bp = boost::python;

#define PIMATH_THROW(ExcType, Msg) { 				\
	std::stringstream strm;							\
	strm << Msg;									\
	PyErr_SetString(ExcType, strm.str().c_str());	\
	bp::throw_error_already_set();					\
}

namespace pimath {

	typedef bp::return_value_policy<bp::return_by_value>		bp_ret_val;
	typedef bp::return_value_policy<bp::detail::return_none>	bp_ret_none;


	template<typename BpClass, typename T>
	void bindBaseType(BpClass& cl)
	{
		cl
		.def("baseTypeMin", 		&T::baseTypeMin)		.staticmethod("baseTypeMin")
		.def("baseTypeMax", 		&T::baseTypeMax)		.staticmethod("baseTypeMax")
		.def("baseTypeSmallest", 	&T::baseTypeSmallest)	.staticmethod("baseTypeSmallest")
		.def("baseTypeEpsilon", 	&T::baseTypeEpsilon)	.staticmethod("baseTypeEpsilon")
		;
	}


	template<typename T>
	std::string iostream__str__(const T& self)
	{
		std::ostringstream s;
		s << self;
		return s.str();
	}


	template<int Dims, typename T> struct make_vec{};
	template<typename T> struct make_vec<2,T> { typedef Imath::Vec2<T> type; };
	template<typename T> struct make_vec<3,T> { typedef Imath::Vec3<T> type; };
	template<typename T> struct make_vec<4,T> { typedef Imath::Vec4<T> type; };
}

#endif












