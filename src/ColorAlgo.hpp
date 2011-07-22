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

/**
 * This differs from the API in that the Color3-related functions now return
 * Color3 objects rather than Vec3s. This is so that equality works as expected.
 * Note they will still accept any Vec3.
 */

#ifndef _PIMATH_COLORALGO__H_
#define _PIMATH_COLORALGO__H_

#include <boost/python.hpp>
#include <ImathColorAlgo.h>
#include <ImathColor.h>
#include "util.h"

namespace pimath
{
	namespace bp = boost::python;

	struct ColorAlgoBindUnteplated
	{
		ColorAlgoBindUnteplated()
		{
			Imath::Color4<double> (*hsv2rgb_d4)(
					const Imath::Color4<double> &) = &Imath::hsv2rgb_d;
			Imath::Color4<double> (*rgb2hsv_d4)(
					const Imath::Color4<double> &) = &Imath::rgb2hsv_d;

			bp::def("hsv2rgb_d", hsv2rgb_d3);
			bp::def("hsv2rgb_d", hsv2rgb_d4);
			bp::def("rgb2hsv_d", rgb2hsv_d3);
			bp::def("rgb2hsv_d", rgb2hsv_d4);
		}

		static Imath::Color3<double> hsv2rgb_d3(
				const Imath::Vec3<double> & hsv)
		{
			return Imath::Color3<double>( Imath::hsv2rgb_d( hsv ));
		}

		static Imath::Color3<double> rgb2hsv_d3(
				const Imath::Vec3<double> & rgb)
		{
			return Imath::Color3<double>( Imath::rgb2hsv_d( rgb ));
		}
	};

	template<typename T>
	struct ColorAlgoBind
	{
		typedef Imath::Vec3<T> 		vec3_type;
		typedef Imath::Color3<T>	col3_type;
		typedef Imath::Color4<T>	col4_type;
		ColorAlgoBind()
		{
			col4_type (*hsv2rgb4)(
					const col4_type &) = &Imath::hsv2rgb;
			col4_type (*rgb2hsv4)(
					const col4_type &) = &Imath::rgb2hsv;
			Imath::PackedColor (*rgb2packed3)(
					const vec3_type &) = &Imath::rgb2packed;
			Imath::PackedColor (*rgb2packed4)(
					const col4_type &) = &Imath::rgb2packed;

			bp::def("hsv2rgb", hsv2rgb3);
			bp::def("hsv2rgb", hsv2rgb4);
			bp::def("rgb2hsv", rgb2hsv3);
			bp::def("rgb2hsv", rgb2hsv4);
			bp::def("rgb2packed", rgb2packed3);
			bp::def("rgb2packed", rgb2packed4);
			bp::def("packed2rgb", packed2rgb );
			bp::def("packed2rgba", packed2rgba );
		}

		static col3_type hsv2rgb3( const vec3_type & vec )
		{
			return col3_type( Imath::hsv2rgb( vec ) );
		}

		static col3_type rgb2hsv3( const vec3_type & vec )
		{
			return col3_type( Imath::rgb2hsv( vec ) );
		}

		static bp::object
		packed2rgb( Imath::PackedColor packed )
		{
			col3_type rv;
			Imath::packed2rgb<T>( packed, rv );
			return  bp::object( rv );
		}

		static bp::object
		packed2rgba( Imath::PackedColor packed )
		{
			col4_type rv;
			Imath::packed2rgb<T>( packed, rv);
			return  bp::object( rv );
		}
	};
}

#endif

