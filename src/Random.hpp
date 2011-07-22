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

#ifndef _PIMATH_RANDOM__H_
#define _PIMATH_RANDOM__H_

#include <ImathRandom.h>
#include "Vec.hpp"

/**
 * There are variants of solidSphereRand and hollowSphereRand and gaussSphererand
 * that vary only by return type. These have been given unique names.
 *
 * TODO: Direct fn poniter for constructor, nextf
 * TODO: Separate type list for sphere algo
 */

namespace pimath
{
	namespace bp = boost::python;

	// templatised bindings
	template<typename T>
	struct RandBind_T
	{
		typedef T 							rand_type;
		typedef bp::class_<rand_type> 		bp_class;

		template<typename S>
		char * getSuffix(S s)
		{
			return "";
		}

		template<typename S>
		char * getSuffix(Imath::V2f s)
		{
			return "2f";
		}

		template<typename S>
		char * getSuffix(Imath::V2d s)
		{
			return "2d";
		}

		template<typename S>
		char * getSuffix(Imath::Vec2<half> s)
		{
			return "2h";
		}

		template<typename S>
		char * getSuffix(Imath::V3f s)
		{
			return "3f";
		}

		template<typename S>
		char * getSuffix(Imath::V3d s)
		{
			return "3d";
		}

		template<typename S>
		char * getSuffix(Imath::Vec3<half> s)
		{
			return "3h";
		}

		template<typename S>
		char * getSuffix(Imath::V4f s)
		{
			return "4f";
		}

		template<typename S>
		char * getSuffix(Imath::V4d s)
		{
			return "4d";
		}

		template<typename S>
		char * getSuffix(Imath::Vec4<half> s)
		{
			return "4h";
		}

		template<typename S>
		void operator()(S s)
		{
			char * suffix = getSuffix<S>(s);
			std::string str("solidSphereRand");
			str += suffix;
			bp::def(str.c_str(), &Imath::solidSphereRand<S, T>);

			str = std::string("hollowSphereRand");
			str += suffix;
			bp::def(str.c_str(), &Imath::hollowSphereRand<S, T>);

			str = std::string("gaussSphereRand");
			str += suffix;
			bp::def(str.c_str(), &Imath::gaussSphereRand<S, T>);
		}

		RandBind_T(bp_class& cl):m_cl(cl){}
		bp_class m_cl;
	};

	template<typename T, typename VecTypes, typename NextFType >
	struct RandBind
	{

		typedef bp::class_<T> 			bp_class;
		RandBind( const char* name )
		{

			NextFType (T::*nextf)() = &T::nextf;
			NextFType (T::*nextfRange)(NextFType rangeMin, NextFType rangeMax) = &T::nextf;

			bp_class cl(name);
			cl
			.def(bp::init<>())
			.def(bp::init<unsigned long int>())
			.def("__init__", bp::make_constructor(sequenceInit))
			.def("init", &T::init )
			.def("nextb", &T::nextb )
			.def("nexti", &T::nexti )
			.def("nextf", nextf )
			.def("nextf", nextfRange )
			;

			boost::mpl::for_each<VecTypes>(RandBind_T<T>(cl));

			bp::def("gaussRand", &Imath::gaussRand<T>);
		}

		static T* sequenceInit(const bp::object &o)
		{
			T* b = new T();
			setValue(*b, o);
			return b;
		}

		static void setValue(T &self, const bp::object &x)
		{
			self.init( bp::extract<unsigned long int>(x[0]));
		}
	};
}

#endif








