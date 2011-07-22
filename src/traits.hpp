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

#ifndef _PIMATH_TRAITS__H_
#define _PIMATH_TRAITS__H_

#include <ImathVec.h>
#include <ImathMatrix.h>
#include <ImathQuat.h>

namespace pimath {

	template<typename T>
	struct imath_traits{};


	template<typename T>
	struct imath_traits<Imath::Vec2<T> >
	{
		typedef T scalar_type;

		template<typename S>
		struct rebind { typedef Imath::Vec2<S> type; };
	};


	template<typename T>
	struct imath_traits<Imath::Vec3<T> >
	{
		typedef T scalar_type;

		template<typename S>
		struct rebind { typedef Imath::Vec3<S> type; };
	};


	template<typename T>
	struct imath_traits<Imath::Vec4<T> >
	{
		typedef T scalar_type;

		template<typename S>
		struct rebind { typedef Imath::Vec4<S> type; };
	};


	template<typename T>
	struct imath_traits<Imath::Matrix33<T> >
	{
		typedef T scalar_type;

		static const unsigned int _rows = 3;
		static const unsigned int _columns = 3;
		static unsigned int rows() 		{ return _rows; }
		static unsigned int columns() 	{ return _columns; }

		template<typename S>
		struct rebind { typedef Imath::Matrix33<S> type; };
	};


	template<typename T>
	struct imath_traits<Imath::Matrix44<T> >
	{
		typedef T scalar_type;

		static const unsigned int _rows = 4;
		static const unsigned int _columns = 4;
		static unsigned int rows() 		{ return _rows; }
		static unsigned int columns() 	{ return _columns; }

		template<typename S>
		struct rebind { typedef Imath::Matrix44<S> type; };
	};

	template<typename T>
	struct imath_traits<Imath::Quat<T> >
	{
		typedef T scalar_type;
	};


	template<typename T> const unsigned int imath_traits<Imath::Matrix33<T> >::_rows;
	template<typename T> const unsigned int imath_traits<Imath::Matrix33<T> >::_columns;
	template<typename T> const unsigned int imath_traits<Imath::Matrix44<T> >::_rows;
	template<typename T> const unsigned int imath_traits<Imath::Matrix44<T> >::_columns;

}


#endif







