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

#ifndef _PIMATH_FRUSTUM__H_
#define _PIMATH_FRUSTUM__H_

#include <ImathFrustum.h>
#include "util.h"


/**
 * Differences:
 * Removed hither and yon (use near and far instead).
 *
 * getPlanes (both versions) returns the result as a tuple of planes.
 */

namespace pimath
{
	namespace bp = boost::python;

	template<typename T>
	struct FrustumBind
	{
		typedef Imath::Frustum<T> 				frustum_type;
		typedef Imath::Plane3<T> 				plane_type;
		typedef Imath::Matrix44<T> 				mat4_type;
		typedef bp::class_<frustum_type>		bp_class;

		FrustumBind(const char* name)
		{
			bp_class cl(name);
			cl
			.def(bp::init<>())
			.def(bp::init<T, T, T, T, T, T, bp::optional<bool> >())
			.def(bp::init<T, T, T, T, T>())
			.def("__init__", bp::make_constructor(sequenceInit))
			.def("set", setBounds)
			.def("set", setFov)
			.def("modifyNearAndFar", &frustum_type::modifyNearAndFar )
			.def("setOrthographic", &frustum_type::setOrthographic )
			.def("orthographic", &frustum_type::orthographic )
			.def("near", &frustum_type::near )
			.def("far", &frustum_type::far )
			.def("left", &frustum_type::left )
			.def("right", &frustum_type::right )
			.def("bottom", &frustum_type::bottom )
			.def("top", &frustum_type::top )
			.def("planes", getPlanes )
			.def("planes", getPlanesMatrix )
			.def("fovx", &frustum_type::fovx )
			.def("fovy", &frustum_type::fovy )
			.def("aspect", &frustum_type::aspect )
			.def("projectionMatrix", &frustum_type::projectionMatrix )
			.def("window", &frustum_type::window )
			.def("projectScreenToRay", &frustum_type::projectScreenToRay )
			.def("projectPointToScreen", &frustum_type::projectPointToScreen )
			.def("ZToDepth", &frustum_type::ZToDepth )
			.def("normalizedZToDepth", &frustum_type::normalizedZToDepth )
			.def("DepthToZ", &frustum_type::DepthToZ )
			.def("worldRadius", &frustum_type::worldRadius )
			.add_property("value", getValue, setValue)
			.def("__str__", toString);
		}

		static frustum_type* sequenceInit( const bp::object & x )
		{
			frustum_type * f = new frustum_type();
			setValue( *f, x );
			return f;
		}

		static void setBounds( frustum_type& self, T near, T far, T left, T right, T top, T bottom, bool ortho = false)
		{
			return self.set( near, far, left, right, top, bottom, ortho );
		}

		static void setFov( frustum_type& self, T near, T far, T fovx, T fovy, T aspect )
		{
			return self.set( near, far, fovx, fovy, aspect );
		}

		static bp::object getPlanes( frustum_type& self )
		{
			plane_type p[6];
			self.planes( p );
			return bp::make_tuple( p[0], p[1], p[2], p[3], p[4], p[5] );
		}

		static bp::object getPlanesMatrix( frustum_type& self, const mat4_type & matrix )
		{
			plane_type p[6];
			self.planes( p, matrix );
			return bp::make_tuple( p[0], p[1], p[2], p[3], p[4], p[5] );
		}

		static std::string
		toString(const frustum_type& self)
		{
			std::ostringstream s;
			s << '(';
			s << self.near() << ' ';
			s << self.far() << ' ';
			s << self.left() << ' ';
			s << self.right() << ' ';
			s << self.top() << ' ';
			s << self.bottom() << ' ';
			s << (self.orthographic()?"True":"False") << ')';
			return s.str();
		}

		static bp::object
		getValue(frustum_type &self)
		{
			return bp::make_tuple( self.near(),
			                       self.far(),
			                       self.left(),
								   self.right(),
								   self.top(),
								   self.bottom(),
								   self.orthographic() );
		}

		static void
		setValue(frustum_type &self, const bp::object &o)
		{
			if( bp::len( o ) == 5 )
			{
				// near/far/fovx/fovy/aspect
				self.set( bp::extract<T>(o[0]),
						  bp::extract<T>(o[1]),
						  bp::extract<T>(o[2]),
						  bp::extract<T>(o[3]),
						  bp::extract<T>(o[4]) );
			}
			else if( bp::len( o ) == 6 )
			{
				// near/far/left/right/top/bottom
				self.set( bp::extract<T>(o[0]),
						  bp::extract<T>(o[1]),
						  bp::extract<T>(o[2]),
						  bp::extract<T>(o[3]),
						  bp::extract<T>(o[4]),
						  bp::extract<T>(o[5]) );
			}
			else if( bp::len( o ) == 7 )
			{
				// near/far/left/right/top/bottom/ortho
				self.set( bp::extract<T>(o[0]),
						  bp::extract<T>(o[1]),
						  bp::extract<T>(o[2]),
						  bp::extract<T>(o[3]),
						  bp::extract<T>(o[4]),
						  bp::extract<T>(o[5]),
						  bp::extract<bool>(o[6]) );
			}
		}
	};
}

#endif

