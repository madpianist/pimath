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

#ifndef _PIMATH_MATRIXALGO__H_
#define _PIMATH_MATRIXALGO__H_

#include <boost/python.hpp>
#include <ImathMatrixAlgo.h>
#include "util.h"

/*
 * extractSHRT,
 * extractScaling,
 * extractScalingAndShear:
 * Return the value(s) rather than a bool, or None where Imath would return False.
 *
 * removeScaling,
 * removeScalingAndShear,
 * sansScaling,
 * sansScalingAndShear:
 * Not implemented. Use 'm = withoutScaling(AndShear)(m)' instead.
 *
 * withoutScaling(AndShear) replaces sansScaling(AndShear), but behaviour is slightly different.
 * withoutScaling will return None, where removeScaling would have returned False. Otherwise,
 * it will return the same value as sansScaling.
 *
 * extractAndRemoveScalingAndShear:
 * Not implemented, use a combination of extractScalingAndShear and withoutScalingAndShear instead.
 *
 * extractSHRT:
 * The form of this function which takes args (M44, Vec3, Vec3, Euler, Vec3) is available
 * as the function 'extractEulerSHRT'.
 */

namespace pimath
{
	namespace bp = boost::python;

	template<typename T>
	struct MatrixAlgoBind
	{
		typedef Imath::Vec2<T> 		vec2_type;
		typedef Imath::Vec3<T> 		vec3_type;
		typedef Imath::Quat<T> 		quat_type;
		typedef Imath::Matrix33<T> 	mat33_type;
		typedef Imath::Matrix44<T> 	mat44_type;
		typedef Imath::Euler<T> 	euler_type;

		MatrixAlgoBind()
		{
			bp::def("extractQuat", &Imath::extractQuat<T>);
			bp::def("rotationMatrix", &Imath::rotationMatrix<T>);
			bp::def("rotationMatrixWithUpDir", &Imath::rotationMatrixWithUpDir<T>);
			bp::def("alignZAxisWithTargetDir", &Imath::alignZAxisWithTargetDir<T>);

			bp::def("extractScaling", extractScaling<mat33_type>);
			bp::def("extractScaling", extractScaling_<mat33_type>);
			bp::def("extractScaling", extractScaling<mat44_type>);
			bp::def("extractScaling", extractScaling_<mat44_type>);

			bp::def("withoutScaling", removeScaling<mat33_type>);
			bp::def("withoutScaling", removeScaling_<mat33_type>);
			bp::def("withoutScaling", removeScaling<mat44_type>);
			bp::def("withoutScaling", removeScaling_<mat44_type>);

			bp::def("withoutScalingAndShear", removeScalingAndShear<mat33_type>);
			bp::def("withoutScalingAndShear", removeScalingAndShear_<mat33_type>);
			bp::def("withoutScalingAndShear", removeScalingAndShear<mat44_type>);
			bp::def("withoutScalingAndShear", removeScalingAndShear_<mat44_type>);

			bp::def("extractScalingAndShear", extractScalingAndShear33);
			bp::def("extractScalingAndShear", extractScalingAndShear33_);
			bp::def("extractScalingAndShear", extractScalingAndShear44);
			bp::def("extractScalingAndShear", extractScalingAndShear44_);

			bp::def("extractSHRT", extractSHRT33);
			bp::def("extractSHRT", extractSHRT33_);

			bp::def("extractSHRT", extractSHRT44);
			bp::def("extractSHRT", extractSHRT44_);
			bp::def("extractSHRT", extractSHRT44__);

			bp::def("extractEulerSHRT", extractEulerSHRT);
			bp::def("extractEulerSHRT", extractEulerSHRT_);

			bp::def("extractEuler", extractEuler);
			bp::def("extractEulerXYZ", extractEulerXYZ);
			bp::def("extractEulerZYX", extractEulerZYX);
		}

		static bp::object extractEulerSHRT(const mat44_type& mat, bool exc)
		{
			vec3_type s, h, t;
			euler_type r;
			return (Imath::extractSHRT(mat, s, h, r, t, exc))?
				bp::make_tuple(s,h,r,t) : bp::object();
		}

		static bp::object extractEulerSHRT_(const mat44_type& mat) {
			return extractEulerSHRT(mat, true);
		}

		static bp::object extractSHRT44(const mat44_type& mat,
			bool exc, typename euler_type::Order order)
		{
			vec3_type s, h, r, t;
			return (Imath::extractSHRT(mat, s, h, r, t, exc, order))?
				bp::make_tuple(s,h,r,t) : bp::object();
		}

		static bp::object extractSHRT44_(const mat44_type& mat, bool exc) {
			return extractSHRT44(mat, exc, euler_type::XYZ);
		}

		static bp::object extractSHRT44__(const mat44_type& mat) {
			return extractSHRT44(mat, true, euler_type::XYZ);
		}

		static bp::object extractSHRT33(const mat33_type& mat, bool exc)
		{
			vec2_type s, t;
			T h, r;
			return (Imath::extractSHRT(mat, s, h, r, t, exc))?
				bp::make_tuple(s,h,r,t) : bp::object();
		}

		static bp::object extractSHRT33_(const mat33_type& mat) {
			return extractSHRT33(mat, true);
		}

		static bp::object extractScalingAndShear33(const mat33_type& mat, bool exc)
		{
			vec2_type scl;
			T h;
			return (Imath::extractScalingAndShear(mat, scl, h, exc))?
				bp::make_tuple(scl, h) : bp::object();
		}

		static bp::object extractScalingAndShear33_(const mat33_type& mat) {
			return extractScalingAndShear33(mat, true);
		}

		static bp::object extractScalingAndShear44(const mat44_type& mat, bool exc)
		{
			vec3_type scl, shr;
			return (Imath::extractScalingAndShear(mat, scl, shr, exc))?
				bp::make_tuple(scl, shr) : bp::object();
		}

		static bp::object extractScalingAndShear44_(const mat44_type& mat) {
			return extractScalingAndShear44(mat, true);
		}

		template<typename Matrix>
		static bp::object removeScalingAndShear(const Matrix& mat, bool exc)
		{
			Matrix m(mat);
			return (Imath::removeScalingAndShear(m, exc))?
				bp::object(m) : bp::object();
		}

		template<typename Matrix>
		static bp::object removeScalingAndShear_(const Matrix& mat, bool exc) {
			return removeScalingAndShear(mat, true);
		}

		template<typename Matrix>
		static bp::object removeScaling(const Matrix& mat, bool exc)
		{
			Matrix m(mat);
			return (Imath::removeScaling(m, exc))?
				bp::object(m) : bp::object();
		}

		template<typename Matrix>
		static bp::object removeScaling_(const Matrix& mat, bool exc) {
			return removeScaling(mat, true);
		}

		template<typename Matrix>
		static bp::object extractScaling(const Matrix& mat, bool exc)
		{
			typedef imath_traits<Matrix> 								mat_traits;
			typedef typename make_vec<mat_traits::_columns-1, T>::type 	vec_less1_type;

			vec_less1_type v;
			return (Imath::extractScaling(mat, v, exc))?
				bp::object(v) : bp::object();
		}

		template<typename Matrix>
		static bp::object extractScaling_(const Matrix& mat) {
			return extractScaling(mat, true);
		}

		static T extractEuler(const mat33_type &mat)
		{
			T rot;
			Imath::extractEuler(mat, rot);
			return rot;
		}

		static vec3_type extractEulerXYZ(const mat44_type &mat)
		{
			vec3_type rot;
			Imath::extractEulerXYZ(mat, rot);
			return rot;
		}

		static vec3_type extractEulerZYX(const mat44_type &mat)
		{
			vec3_type rot;
			Imath::extractEulerZYX(mat, rot);
			return rot;
		}
	};

}

#endif
