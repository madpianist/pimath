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

#ifndef _PIMATH_VEC__H_
#define _PIMATH_VEC__H_

#include <ImathVec.h>
#include <ImathColor.h>
#include <ImathMatrix.h>
#include "util.h"


namespace pimath
{
	namespace bp = boost::python;


	template<typename Vec, typename ScalarTypes>
	struct VecNBind{};

	template<typename Vec>
	struct VecNBind_ {
		typedef VecNBind<Vec, boost::mpl::vector<> > type;
	};


	// Vec2-specific bindings
	template<typename T, typename ScalarTypes>
	struct VecNBind<Imath::Vec2<T>, ScalarTypes>
	{
		typedef Imath::Vec2<T> 				vec_type;
		typedef T 							scalar_type;
		typedef bp::class_<vec_type> 		bp_class;

		struct Bind_T
		{
			template<typename S>
			void operator()(S s) {
				this->bind_matrix_funcs<S>();
			}

			template<typename S>
			void bind_matrix_funcs(typename boost::enable_if<boost::is_integral<S> >::type* dummy = 0) {}

			template<typename S>
			void bind_matrix_funcs(typename boost::disable_if<boost::is_integral<S> >::type* dummy = 0) {
				m_cl.def(bp::self *= bp::other<Imath::Matrix33<S> >());
			}

			Bind_T(bp_class& cl):m_cl(cl){}
			bp_class m_cl;
		};

		static void bind(bp_class& cl)
		{
			cl
			.def(bp::init<scalar_type, scalar_type>())
			.def("cross", &vec_type::cross)
			.def(bp::self % bp::self)
			;

			boost::mpl::for_each<ScalarTypes>(Bind_T(cl));
		}

		static bp::tuple getValue(const vec_type &self) {
			return bp::make_tuple(self.x, self.y);
		}
	};


	// Vec3-specific bindings
	template<typename T, typename ScalarTypes>
	struct VecNBind<Imath::Vec3<T>, ScalarTypes>
	{
		typedef Imath::Vec3<T> 				vec_type;
		typedef T 							scalar_type;
		typedef bp::class_<vec_type> 		bp_class;

		struct Bind_T
		{
			template<typename S>
			void operator()(S s) {
				m_cl.def(bp::init<Imath::Vec4<S> >());
				this->bind_matrix_funcs<S>();
			}

			template<typename S>
			void bind_matrix_funcs(typename boost::enable_if<boost::is_integral<S> >::type* dummy = 0) {}

			template<typename S>
			void bind_matrix_funcs(typename boost::disable_if<boost::is_integral<S> >::type* dummy = 0) {
				m_cl.def(bp::self *= bp::other<Imath::Matrix33<S> >());
				m_cl.def(bp::self *= bp::other<Imath::Matrix44<S> >());
			}

			Bind_T(bp_class& cl):m_cl(cl){}
			bp_class m_cl;
		};

		static void bind(bp_class& cl)
		{
			cl
			.def(bp::init<scalar_type, scalar_type, scalar_type>())
			.def("cross", &vec_type::cross)
			.def(bp::self % bp::self)
			.add_property("z", bp::make_getter(&vec_type::z, bp_ret_val()),
				bp::make_setter(&vec_type::z))
			;

			boost::mpl::for_each<ScalarTypes>(Bind_T(cl));
		}

		static bp::tuple getValue(const vec_type &self) {
			return bp::make_tuple(self.x, self.y, self.z);
		}
	};


	// Vec4-specific bindings
	template<typename T, typename ScalarTypes>
	struct VecNBind<Imath::Vec4<T>, ScalarTypes>
	{
		typedef Imath::Vec4<T> 				vec_type;
		typedef T 							scalar_type;
		typedef bp::class_<vec_type> 		bp_class;

		struct Bind_T
		{
			template<typename S>
			void operator()(S s) {
				m_cl.def(bp::init<Imath::Vec3<S> >());
				this->bind_matrix_funcs<S>();
			}

			template<typename S>
			void bind_matrix_funcs(typename boost::enable_if<boost::is_integral<S> >::type* dummy = 0) {}

			template<typename S>
			void bind_matrix_funcs(typename boost::disable_if<boost::is_integral<S> >::type* dummy = 0) {
				m_cl.def(bp::self *= bp::other<Imath::Matrix44<S> >());
			}

			Bind_T(bp_class& cl):m_cl(cl){}
			bp_class m_cl;
		};

		static void bind(bp_class& cl)
		{
			cl
			.def(bp::init<scalar_type, scalar_type, scalar_type, scalar_type>())
			.add_property("z", bp::make_getter(&vec_type::z, bp_ret_val()),
				bp::make_setter(&vec_type::z))
			.add_property("w", bp::make_getter(&vec_type::w, bp_ret_val()),
				bp::make_setter(&vec_type::w))
			;

			boost::mpl::for_each<ScalarTypes>(Bind_T(cl));
		}

		static bp::tuple getValue(const vec_type &self) {
			return bp::make_tuple(self.x, self.y, self.z, self.w);
		}
	};


	// Vec-common templatised bindings
	template<typename Vec>
	struct VecBind_T
	{
		typedef Vec 						vec_type;
		typedef bp::class_<vec_type> 		bp_class;

		template<typename S>
		void operator()(S s)
		{
			typedef typename imath_traits<vec_type>::template rebind<S>::type s_vec_type;

			m_cl
			.def(bp::init<s_vec_type>())
			.def(bp::self == bp::other<s_vec_type>())
			.def(bp::self != bp::other<s_vec_type>())
			;
		}

		VecBind_T(bp_class& cl):m_cl(cl){}
		bp_class m_cl;
	};


	// Vec-common bindings
	template<typename Vec, typename ScalarTypes>
	struct VecBind
	{
		typedef Vec 							vec_type;
		typedef typename vec_type::BaseType 	scalar_type;
		typedef bp::class_<vec_type> 			bp_class;

		VecBind(const char* name)
		{
			bp_class cl(name);
			cl
			.def(bp::init<scalar_type>())
			.def("__init__", bp::make_constructor(defaultInit))
			.def("__init__", bp::make_constructor(sequenceInit))
			.def("__getitem__", getItem)
			.def("__setitem__", setItem)
			.add_property("value", VecNBind<vec_type,ScalarTypes>::getValue, setValue)
			.def(bp::self + bp::self)
			.def(bp::self += bp::self)
			.def(bp::self - bp::self)
			.def(bp::self -= bp::self)
			.def(bp::self * bp::self)
			.def(bp::self *= bp::self)
			.def(bp::self / bp::self)
			.def(bp::self /= bp::self)
			.def(bp::self * bp::other<scalar_type>())
			.def(bp::self *= bp::other<scalar_type>())
			.def(bp::self / bp::other<scalar_type>())
			.def(bp::self /= bp::other<scalar_type>())
			.def(bp::self ^ bp::self)
			.def(- bp::self)
			.def("dot", &vec_type::dot)
			.def("negate", &vec_type::negate, bp_ret_none())
			.def("normalize", &vec_type::normalize, bp_ret_none())
			.def("normalizeExc", &vec_type::normalizeExc, bp_ret_none())
			.def("normalized", &vec_type::normalized)
			.def("equalWithAbsError", &vec_type::equalWithAbsError)
			.def("equalWithRelError", &vec_type::equalWithRelError)
			.def("length", &vec_type::length)
			.def("length2", &vec_type::length2)
			.def("__str__", iostream__str__<vec_type>)

			.def("dimensions", &vec_type::dimensions)
			.staticmethod("dimensions")

			// note: due to def_readwrite() not supporting custom to-python converters
			.add_property("x", bp::make_getter(&vec_type::x, bp_ret_val()),
				bp::make_setter(&vec_type::x))
			.add_property("y", bp::make_getter(&vec_type::y, bp_ret_val()),
				bp::make_setter(&vec_type::y))
			;

			bindBaseType<bp_class, vec_type>(cl);
			VecNBind<vec_type, ScalarTypes>::bind(cl);
			boost::mpl::for_each<ScalarTypes>(VecBind_T<vec_type>(cl));
		}

		static vec_type* defaultInit() {
			return new vec_type(scalar_type(0));
		}

		static vec_type* sequenceInit(const bp::object &o)
		{
			vec_type* v = new vec_type();
			setValue(*v, o);
			return v;
		}

		static scalar_type getItem(const vec_type& self, int i)
		{
			if((i<0) || (i>=(int)vec_type::dimensions()))
				PIMATH_THROW(PyExc_IndexError, "Vector index out of range.");
			return self[i];
		}

		static void setItem(vec_type& self, int i, scalar_type val)
		{
			if((i<0) || (i>=(int)vec_type::dimensions()))
				PIMATH_THROW(PyExc_IndexError, "Vector index out of range.");
			self[i] = val;
		}

		static void setValue(vec_type &self, const bp::object &o)
		{
			for(unsigned int i=0; i<vec_type::dimensions(); ++i)
				self[i] = bp::extract<scalar_type>(o[i]);
		}
	};

	template<typename Vec>
	struct VecBind_ {
		typedef VecBind<Vec, boost::mpl::vector<> > type;
	};
}

#endif








