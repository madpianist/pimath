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

#ifndef _PIMATH_INTERVAL__H_
#define _PIMATH_INTERVAL__H_

#include <ImathInterval.h>
#include "util.h"

namespace pimath
{
	namespace bp = boost::python;

	template<typename T>
	struct IntervalBind
	{
		typedef Imath::Interval<T> 				interval_type;
		typedef bp::class_<interval_type>		bp_class;

		IntervalBind(const char* name)
		{

			void (interval_type::*extendByPoint)(
					const T &) = &interval_type::extendBy;

			void (interval_type::*extendByInterval)(
					const interval_type &) = &interval_type::extendBy;

			bool (interval_type::*intersectsPoint)(
					const T &) const = &interval_type::intersects;

			bool (interval_type::*intersectsInterval)(
					const interval_type &) const = &interval_type::intersects;

			bp_class cl(name);
			cl
			.def(bp::init<>())
			.def(bp::init<const T&>())
			.def(bp::init<const T&, const T&>())
			.def("__init__", bp::make_constructor(sequenceInit))
			.def(bp::self == bp::self)
			.def("makeEmpty", &interval_type::makeEmpty)
			.def("extendBy", extendByPoint)
			.def("extendBy", extendByInterval)
			.def("size", &interval_type::size)
			.def("center", &interval_type::center)
			.def("intersects", intersectsPoint)
			.def("intersects", intersectsInterval)
			.def("hasVolume", &interval_type::hasVolume)
			.def("isEmpty", &interval_type::isEmpty)
			.add_property("value", getValue, setValue)
			.def("__str__", toString);
		}


		static interval_type* sequenceInit( const bp::object & x )
		{
			interval_type * f = new interval_type();
			setValue( *f, x );
			return f;
		}

		static std::string
		toString(const interval_type& self)
		{
			std::ostringstream s;
			s << '(';
			s << self.min << ' ';
			s << self.max << ')';
			return s.str();
		}

		static bp::object
		getValue(interval_type &self)
		{
			return bp::make_tuple( self.min,
								   self.max );
		}

		static void
		setValue(interval_type &self, const bp::object &o)
		{
			if( bp::len( o ) == 1 )
			{
				// set by point
				self.min = bp::extract<T>(o[0]);
				self.max = self.min;
			}
			else if( bp::len( o ) == 2 )
			{
				// set by min & max
				self.min = bp::extract<T>(o[0]);
				self.max = bp::extract<T>(o[1]);
			}
		}
	};
}

#endif
