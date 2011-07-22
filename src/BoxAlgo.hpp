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

#ifndef _PIMATH_BOXALGO__H_
#define _PIMATH_BOXALGO__H_

#include <boost/python.hpp>
#include <ImathBoxAlgo.h>
#include "util.h"

/*
 * transform, affineTransform:
 * type of scalar must be the same for the box and the matrix
 * findEntryAndExitPoints:
 * replaced with entryAndExitPoints; returns points as a tuple (or None if none found)
 * intersects:
 * replaced by intersection; returns point of intersection or None
 */

namespace pimath
{
	namespace bp = boost::python;

	template<typename T>
	struct BoxAlgoBind
	{
		typedef Imath::Vec2<T> 		vec2_type;
		typedef Imath::Vec3<T> 		vec3_type;
		BoxAlgoBind()
		{
			bp::def("closestPointInBox", &Imath::closestPointInBox<vec3_type>);
			bp::def("closestPointInBox", &Imath::closestPointInBox<vec2_type>);
			bp::def("closestPointOnBox", &Imath::closestPointOnBox<T>);
			bp::def("clip", &Imath::clip<vec2_type> );
			bp::def("clip", &Imath::clip<vec3_type> );
		}

/*		static vec2_type
		clip2( const vec2_type &p, const box2_type & b )
		{
			return Imath::clip<vec2_type>(p, b);
		}

		static vec3_type
		clip3( const vec3_type &p, const box3_type & b )
		{
			return Imath::clip<vec3_type>(p, b);
		}*/
	};



	template<typename T>
	struct BoxAlgoBindFloating
	{
		typedef Imath::Vec2<T> 		vec2_type;
		typedef Imath::Vec3<T> 		vec3_type;
		typedef Imath::Quat<T> 		quat_type;
		typedef Imath::Matrix33<T> 	mat33_type;
		typedef Imath::Matrix44<T> 	mat44_type;
		typedef Imath::Line3<T> 	line3_type;
		typedef Imath::Box< vec2_type > 	    box2_type;
		typedef Imath::Box< vec3_type > 	    box3_type;

		BoxAlgoBindFloating()
		{
			// TODO: Add different scalar combinations.
			bp::def("transform", &Imath::transform<T, T>);
			bp::def("affineTransform", &Imath::affineTransform<T, T>);
			bp::def("intersection", intersection);
			bp::def("entryAndExitPoints", entryAndExitPoints);
		}

		static bp::object
		intersection(const box3_type& box,
					 const line3_type& line)
		{
			vec3_type intersection;
			return ( Imath::intersects(box, line, intersection) ) ?
				bp::object( intersection ) : bp::object();
		}

		static bp::object
		entryAndExitPoints(const line3_type& line,
						   const box3_type box)
		{
			vec3_type entry, exit;
			return (Imath::findEntryAndExitPoints(line, box, entry, exit))?
				bp::make_tuple(entry, exit) : bp::object();
		}
	};
}

#endif
