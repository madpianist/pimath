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

#ifndef _PIMATH_EULER__H_
#define _PIMATH_EULER__H_

#include <boost/python.hpp>
#include <ImathQuat.h>
#include <ImathVec.h>
#include <ImathEuler.h>
#include <ImathMatrix.h>
#include <string>
#include <sstream>

namespace pimath
{
	namespace bp = boost::python;

	template<typename T>
	struct EulerBind
	{
		typedef T 									scalar_type;
		typedef Imath::Euler<T> 					euler_type;
		typedef typename euler_type::Order 			order_type;
		typedef typename euler_type::Axis 			axis_type;
		typedef typename euler_type::InputLayout 	input_layout_type;

		typedef Imath::Vec3<T> 						vec_type;
		typedef Imath::Quat<T> 						quat_type;
		typedef Imath::Matrix33<T> 					mat33_type;
		typedef Imath::Matrix44<T> 					mat44_type;

		EulerBind(const char* name)
		{
			void (euler_type::*fn_extractM33)(const mat33_type&) = &euler_type::extract;
			void (euler_type::*fn_extractM44)(const mat44_type&) = &euler_type::extract;
			void (euler_type::*fn_extractQuat)(const quat_type&) = &euler_type::extract;

			bp::scope eulerScope =
				bp::class_<euler_type, bp::bases<vec_type> >(name)
				.def(bp::init<>())
				.def(bp::init<euler_type>())
				.def(bp::init<order_type>())
				.def(bp::init<vec_type, bp::optional<order_type, input_layout_type> >())
				.def(bp::init<T, T, T, bp::optional<order_type, input_layout_type> >())
				.def(bp::init<euler_type, bp::optional<order_type> >())
				.def(bp::init<mat33_type, bp::optional<order_type> > ())
				.def(bp::init<mat44_type, bp::optional<order_type> > ())

				.def("setXYZVector", &euler_type::setXYZVector)
				.add_property("order", &euler_type::order, &euler_type::setOrder)
				.def("set", &euler_type::set)
				.def("extract", fn_extractM33)
				.def("extract", fn_extractM44)
				.def("extract", fn_extractQuat)
				.def("toMatrix33", &euler_type::toMatrix33)
				.def("toMatrix44", &euler_type::toMatrix44)
				.def("toQuat", &euler_type::toQuat)
				.def("toXYZVector", &euler_type::toXYZVector)
				.def("angleOrder", angleOrder)
				.def("angleMapping", angleMapping)
				.def("makeNear", &euler_type::makeNear)
				.def("frameStatic", &euler_type::frameStatic)
				.def("initialRepeated", &euler_type::initialRepeated)
				.def("parityEven", &euler_type::parityEven)
				.def("initialAxis", &euler_type::initialAxis)

				.def("legal", &euler_type::legal)
				.def("angleMod", &euler_type::angleMod)
				.def("simpleXYZRotation", simpleXYZRotation)
				.def("nearestRotation", nearestRotation)
				.def("nearestRotation", nearestRotation_)
				.staticmethod("legal")
				.staticmethod("angleMod")
				.staticmethod("simpleXYZRotation")
				.staticmethod("nearestRotation")
				;

			bp::enum_<order_type>("Order")
				.value("XYZ", euler_type::XYZ)
				.value("XZY", euler_type::XZY)
				.value("YZX", euler_type::YZX)
				.value("YXZ", euler_type::YXZ)
				.value("ZXY", euler_type::ZXY)
				.value("ZYX", euler_type::ZYX)

				.value("XZX", euler_type::XZX)
				.value("XYX", euler_type::XYX)
				.value("YXY", euler_type::YXY)
				.value("YZY", euler_type::YZY)
				.value("ZYZ", euler_type::ZYZ)
				.value("ZXZ", euler_type::ZXZ)

				.value("XYZr", euler_type::XYZr)
				.value("XZYr", euler_type::XZYr)
				.value("YZXr", euler_type::YZXr)
				.value("YXZr", euler_type::YXZr)
				.value("ZXYr", euler_type::ZXYr)
				.value("ZYXr", euler_type::ZYXr)

				.value("XZXr", euler_type::XZXr)
				.value("XYXr", euler_type::XYXr)
				.value("YXYr", euler_type::YXYr)
				.value("YZYr", euler_type::YZYr)
				.value("ZYZr", euler_type::ZYZr)
				.value("ZXZr", euler_type::ZXZr)
				;

			bp::enum_<axis_type>("Axis")
				.value("X", euler_type::X)
				.value("Y", euler_type::Y)
				.value("Z", euler_type::Z)
				;

			bp::enum_<input_layout_type>("InputLayout")
				.value("XYZLayout", euler_type::XYZLayout)
				.value("IJKLayout", euler_type::IJKLayout)
				;
		}

		static bp::tuple angleOrder(const euler_type& self)
		{
			int i, j, k;
			self.angleOrder(i,j,k);
			return bp::make_tuple(i,j,k);
		}

		static bp::tuple angleMapping(const euler_type& self)
		{
			int i, j, k;
			self.angleMapping(i,j,k);
			return bp::make_tuple(i,j,k);
		}

		static vec_type simpleXYZRotation(const vec_type& xyzRot, const vec_type& targetXyzRot)
		{
			vec_type v(xyzRot);
			euler_type::simpleXYZRotation(v, targetXyzRot);
			return v;
		}

		static vec_type nearestRotation(const vec_type& xyzRot, const vec_type& targetXyzRot,
			order_type order)
		{
			vec_type v(xyzRot);
			euler_type::nearestRotation(v, targetXyzRot, order);
			return v;
		}

		static vec_type nearestRotation_(const vec_type& xyzRot, const vec_type& targetXyzRot) {
			return nearestRotation(xyzRot, targetXyzRot, euler_type::XYZ);
		}
	};

}

#endif




