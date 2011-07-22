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

#ifndef _PIMATH_QUAT__H_
#define _PIMATH_QUAT__H_

#include <ImathQuat.h>
#include <string>
#include <sstream>
#include "util.h"

namespace pimath
{
	namespace bp = boost::python;


	// templatised bindings
	template<typename Quat>
	struct QuatBind_T
	{
		typedef Quat 						quat_type;
		typedef bp::class_<quat_type> 		bp_class;

		template<typename S>
		void operator()(S s)
		{
			typedef Imath::Quat<S> s_quat_type;
			m_cl
			.def(bp::init<s_quat_type>())
			.def(bp::self == bp::other<s_quat_type>())
			.def(bp::self != bp::other<s_quat_type>())
			;
		}

		QuatBind_T(bp_class& cl):m_cl(cl){}
		bp_class m_cl;
	};


	// common bindings
	template<typename T, typename ScalarTypes>
	struct QuatBind
	{
		typedef T 						scalar_type;
		typedef Imath::Quat<T> 			quat_type;
		typedef Imath::Vec3<T> 			vec_type;
		typedef Imath::Matrix33<T> 		mat_type;
		typedef bp::class_<quat_type> 	bp_class;

		QuatBind(const char* name)
		{
			bp_class cl(name);
			cl
			.def(bp::init<>())
			.def(bp::init<scalar_type, scalar_type, scalar_type, scalar_type>())
			.def(bp::init<scalar_type, vec_type>())
			.def("__init__",bp::make_constructor(sequenceInit))
			.def_readwrite("v", &quat_type::v)
			.def("__getitem__", getItem)
			.def("__setitem__", setItem)
			.def(bp::self * bp::self)
			.def(bp::self * bp::other<scalar_type>())
			.def(bp::self * bp::other<mat_type>())
			.def(bp::other<scalar_type>() * bp::self)
			.def(bp::other<vec_type>() * bp::self)
			.def(bp::other<mat_type>() * bp::self)
			.def(bp::self / bp::self)
			.def(bp::self / bp::other<scalar_type>())
			.def(bp::self + bp::self)
			.def(bp::self - bp::self)
			.def(- bp::self)
			.def(~ bp::self)
			.def(bp::self *= bp::self)
			.def(bp::self *= bp::other<scalar_type>())
			.def(bp::self /= bp::self)
			.def(bp::self /= bp::other<scalar_type>())
			.def(bp::self += bp::self)
			.def(bp::self -= bp::self)
			.def("invert", &quat_type::invert, bp_ret_none())
			.def("inverse", &quat_type::inverse)
			.def("normalize", &quat_type::normalize, bp_ret_none())
			.def("normalized", &quat_type::normalized)
			.def("length", &quat_type::length)
			.def("rotateVector", &quat_type::rotateVector)
			.def("euclideanInnerProduct", &quat_type::euclideanInnerProduct)
			.def("setAxisAngle", &quat_type::setAxisAngle, bp_ret_none())
			.def("setRotation", &quat_type::setAxisAngle, bp_ret_none())
			.def("angle", &quat_type::angle)
			.def("axis", &quat_type::axis)
			.def("toMatrix33", &quat_type::toMatrix33)
			.def("toMatrix44", &quat_type::toMatrix44)
			.def("log", &quat_type::log)
			.def("exp", &quat_type::exp)
			.def("__str__", iostream__str__<quat_type>)
			.add_property("value", getValue, setValue)

			.def("identity", &quat_type::identity)
			.staticmethod("identity")

			// note: due to def_readwrite() not supporting custom to-python converters
			.add_property("r", bp::make_getter(&quat_type::r, bp_ret_val()),
				bp::make_setter(&quat_type::r))
			;

			quat_type (*fn_intermediate)(
				const quat_type&,const quat_type&,const quat_type&) = &Imath::intermediate;

			bp::def("slerp", &Imath::slerp<T>);
			bp::def("squad", &Imath::squad<T>);
			bp::def("spline", &Imath::spline<T>);
			bp::def("angle4D", &Imath::angle4D<T>);
			bp::def("slerpShortestArc", &Imath::slerpShortestArc<T>);
			bp::def("intermediate", fn_intermediate);

			boost::mpl::for_each<ScalarTypes>(QuatBind_T<quat_type>(cl));
		}


		static quat_type*
		sequenceInit( const bp::object & x )
		{
			quat_type * f = new quat_type();
			setValue( *f, x );
			return f;
		}

		static bp::object getValue(const quat_type &self)
		{
			return bp::make_tuple(self.r, self.v.x, self.v.y, self.v.z);
		}

		static void setValue(quat_type &self, const bp::object &o)
		{
			self.r = bp::extract<scalar_type>(o[0]);

			if( bp::len( o ) == 2 )
			{
				// This is the ( s, (x, y, z) ) case.
				bp::tuple v = bp::extract< bp::tuple >( o[1] );
				self.v = vec_type( bp::extract<T>(v[0]),
								   bp::extract<T>(v[1]),
								   bp::extract<T>(v[2]));
			}
			else if( bp::len( o ) == 4 )
			{
				// This is the ( s, x, y, z ) case.
				self.v = vec_type( bp::extract<T>(o[1]),
								   bp::extract<T>(o[2]),
								   bp::extract<T>(o[3]));
			}
		}

		static scalar_type getItem(const quat_type& self, int i)
		{
			if((i<0) || (i>=4))
				PIMATH_THROW(PyExc_IndexError, "Quat index out of range.");
			return self[i];
		}

		static void setItem(quat_type& self, int i, scalar_type val)
		{
			if((i<0) || (i>=4))
				PIMATH_THROW(PyExc_IndexError, "Quat index out of range.");
			self[i] = val;
		}
	};

}

#endif






