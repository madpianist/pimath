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

#ifndef _PIMATH_COLOR__H_
#define _PIMATH_COLOR__H_

#include <ImathVec.h>
#include <ImathMatrix.h>
#include <ImathColor.h>
#include "util.h"


namespace pimath
{
	namespace bp = boost::python;

	// Color3 uses the Vec3 binding as it has an identical interface.

	// templatised bindings
	template<typename Color>
	struct Color3Bind_T
	{
		typedef Color 						color_type;
		typedef typename color_type::BaseType 	scalar_type;
		typedef Imath::Vec3<scalar_type>		vec_type;
		typedef bp::class_<color_type, bp::bases<vec_type> > 	bp_class;

		template<typename S>
		void operator()(S s)
		{
			m_cl
			.def(bp::init<Imath::Vec3<S> >());
		}

		Color3Bind_T(bp_class& cl):m_cl(cl){}
		bp_class m_cl;
	};

	template<typename Color, typename ScalarTypes>
	struct Color3Bind
	{
		typedef Color 							color_type;
		typedef typename color_type::BaseType 	scalar_type;
		typedef Imath::Vec3<scalar_type>		vec_type;
		typedef bp::class_<color_type, bp::bases<vec_type> > 	bp_class;

		Color3Bind( const char * name )
		{
			bp_class cl = bp::class_<color_type, bp::bases<vec_type> >(name);
			cl
				.def("__init__", bp::make_constructor(defaultInit))
				.def(bp::init<scalar_type>())
				.def(bp::init<scalar_type, scalar_type, scalar_type>())
				.def("__init__", bp::make_constructor(sequenceInit));

			boost::mpl::for_each<ScalarTypes>(Color3Bind_T<color_type>(cl));
		}

		static color_type* defaultInit()
		{
			return new color_type(0);
		}

		static color_type* sequenceInit(const bp::object &o)
		{
			color_type* v = new color_type();
			setValue(*v, o);
			return v;
		}

		static void setValue(color_type &self, const bp::object &o)
		{
			for(unsigned int i=0; i<3; ++i)
				self[i] = bp::extract<scalar_type>(o[i]);
		}

	};

	// Color4, however, needs a unique interface:


	// templatised bindings
	template<typename Color>
	struct Color4Bind_T
	{
		typedef Color 						color_type;
		typedef bp::class_<color_type> 		bp_class;

		template<typename S>
		void operator()(S s)
		{
			m_cl
			.def(bp::init<Imath::Color4<S> >());
		}

		Color4Bind_T(bp_class& cl):m_cl(cl){}
		bp_class m_cl;
	};

	// Vec-common bindings
	template<typename Color, typename ScalarTypes>
	struct ColorBind
	{
		typedef Color							color_type;
		typedef typename color_type::BaseType 	scalar_type;
		typedef bp::class_<color_type> 			bp_class;

		ColorBind(const char* name)
		{
			bp_class cl(name);
			cl
			.def("__init__",bp::make_constructor(defaultInit))
			.def(bp::init<scalar_type, scalar_type, scalar_type, scalar_type>())
			.def("__init__", bp::make_constructor(sequenceInit))
			.def("__getitem__", getItem)
			.def("__setitem__", setItem)
			.add_property("value", getValue, setValue)
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
			.def(bp::init<color_type>())
			.def(bp::self == bp::other<color_type>())
			.def(bp::self != bp::other<color_type>())
			.def(- bp::self)
			.def("negate", &color_type::negate, bp_ret_none())
			.def("__str__", iostream__str__<color_type>)

			.def("dimensions", &color_type::dimensions)
			.staticmethod("dimensions")

			// note: due to def_readwrite() not supporting custom to-python converters
			.add_property("r", bp::make_getter(&color_type::r, bp_ret_val()),
				bp::make_setter(&color_type::r))
			.add_property("g", bp::make_getter(&color_type::g, bp_ret_val()),
				bp::make_setter(&color_type::g))
			.add_property("b", bp::make_getter(&color_type::b, bp_ret_val()),
				bp::make_setter(&color_type::b))
			.add_property("a", bp::make_getter(&color_type::a, bp_ret_val()),
				bp::make_setter(&color_type::a))
			;

			bindBaseType<bp_class, color_type>(cl);

			boost::mpl::for_each<ScalarTypes>(Color4Bind_T<color_type>(cl));
		}

		static color_type* defaultInit() {
			return new color_type(scalar_type(0));
		}

		static color_type* sequenceInit(const bp::object &o)
		{
			color_type* v = new color_type();
			setValue(*v, o);
			return v;
		}

		static scalar_type getItem(const color_type& self, int i)
		{
			if((i<0) || (i>=(int)color_type::dimensions()))
				PIMATH_THROW(PyExc_IndexError, "Vector index out of range.");
			return self[i];
		}

		static void setItem(color_type& self, int i, scalar_type val)
		{
			if((i<0) || (i>=(int)color_type::dimensions()))
				PIMATH_THROW(PyExc_IndexError, "Vector index out of range.");
			self[i] = val;
		}

		static void setValue(color_type &self, const bp::object &o)
		{
			for(unsigned int i=0; i<color_type::dimensions(); ++i)
				self[i] = bp::extract<scalar_type>(o[i]);
		}

		static bp::tuple getValue(const color_type &self) {
			return bp::make_tuple(self.r, self.g, self.b, self.a);
		}
	};
}

#endif








