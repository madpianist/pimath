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

#ifndef _PIMATH_MATRIX__H_
#define _PIMATH_MATRIX__H_

/*
 * The [] operator can take either an integer, or an (int,int) pair. In the former case,
 * that row is returned in a Vec (for eg, M33f()[0] will return a V3f - a copy of the
 * first row of the matrix). In the latter case, that (row,column) scalar entry will be
 * returned.
 *
 * The methods 'setTranslation', 'setScale', 'setShear' are renamed to 'setToTranslation',
 * 'setToScale', 'setToShear'. Specifically in the case of Matrix44 'setScale' for eg is
 * misleading - it sets the whole matrix to a scale matrix, but one might expect it to
 * just set the scale component of that matrix.
 *
 * The free function 'multiply' is not bound, as the * operator will suffice.
 */

#include <ImathMatrix.h>
#include <ImathShear.h>
#include "util.h"


namespace pimath
{
	namespace bp = boost::python;


	template<typename Matrix, typename ScalarTypes>
	struct MatrixNNBind{};


	// Matrix33-specific bindings
	template<typename T, typename ScalarTypes>
	struct MatrixNNBind<Imath::Matrix33<T>, ScalarTypes>
	{
		typedef Imath::Matrix33<T> 			mat_type;
		typedef T 							scalar_type;
		typedef bp::class_<mat_type> 		bp_class;

		static void bind(bp_class& cl)
		{
			const mat_type& (mat_type::*fn_setRotation)(const double) = &mat_type::setRotation;
			const mat_type& (mat_type::*fn_rotate)(const double) = &mat_type::rotate;
			const mat_type& (mat_type::*fn_setShear)(const double&) = &mat_type::setShear;
			const mat_type& (mat_type::*fn_shear)(const double&) = &mat_type::shear;

			cl
			.def(bp::init<
				scalar_type, scalar_type, scalar_type,
				scalar_type, scalar_type, scalar_type,
				scalar_type, scalar_type, scalar_type>())
			.def("setToRotation", fn_setRotation, bp_ret_none())
			.def("rotate", fn_rotate, bp_ret_none())
			.def("setToShear", fn_setShear, bp_ret_none())
			.def("shear", fn_shear, bp_ret_none())
			;
		}

		static bp::tuple getValue(const mat_type &self)
		{
			return bp::make_tuple(
				bp::make_tuple(self.x[0][0], self.x[0][1], self.x[0][2]),
				bp::make_tuple(self.x[1][0], self.x[1][1], self.x[1][2]),
				bp::make_tuple(self.x[2][0], self.x[2][1], self.x[2][2])
			);
		}
	};


	// Matrix44-specific bindings
	template<typename T, typename ScalarTypes>
	struct MatrixNNBind<Imath::Matrix44<T>, ScalarTypes>
	{
		typedef Imath::Matrix44<T> 			mat_type;
		typedef T 							scalar_type;
		typedef bp::class_<mat_type> 		bp_class;

		struct Bind_T
		{
			template<typename S>
			void operator()(S s)
			{
				const mat_type& (mat_type::*fn_setEuler)(const Imath::Vec3<S>&) = &mat_type::setEulerAngles;
				const mat_type& (mat_type::*fn_setAxis)(const Imath::Vec3<S>&, S) = &mat_type::setAxisAngle;
				const mat_type& (mat_type::*fn_setShear)(const Imath::Shear6<S>&) = &mat_type::setShear;
				const mat_type& (mat_type::*fn_shear)(const Imath::Shear6<S>&) = &mat_type::shear;

				m_cl.def("setEulerAngles", fn_setEuler, bp_ret_none());
				m_cl.def("setAxisAngle", fn_setAxis, bp_ret_none());
				m_cl.def("setShear", fn_setShear, bp_ret_none());
				m_cl.def("shear", fn_shear, bp_ret_none());
			}

			Bind_T(bp_class& cl):m_cl(cl){}
			bp_class m_cl;
		};

		static void bind(bp_class& cl)
		{
			cl
			.def(bp::init<
				scalar_type, scalar_type, scalar_type, scalar_type,
				scalar_type, scalar_type, scalar_type, scalar_type,
				scalar_type, scalar_type, scalar_type, scalar_type,
				scalar_type, scalar_type, scalar_type, scalar_type>())
			;

			boost::mpl::for_each<ScalarTypes>(Bind_T(cl));
		}

		static bp::tuple getValue(const mat_type &self)
		{
			return  bp::make_tuple(
				bp::make_tuple(self.x[0][0], self.x[0][1], self.x[0][2], self.x[0][3]),
				bp::make_tuple(self.x[1][0], self.x[1][1], self.x[1][2], self.x[1][3]),
				bp::make_tuple(self.x[2][0], self.x[2][1], self.x[2][2], self.x[2][3]),
				bp::make_tuple(self.x[3][0], self.x[3][1], self.x[3][2], self.x[3][3])
			);
		}
	};


	// Matrix-common templatised bindings
	template<typename Matrix>
	struct MatrixBind_T
	{
		typedef Matrix 						mat_type;
		typedef bp::class_<mat_type> 		bp_class;

		template<typename S>
		void operator()(S s)
		{
			typedef imath_traits<mat_type> mat_traits;
			typedef typename mat_traits::template rebind<S>::type s_mat_type;
			typedef typename make_vec<mat_traits::_rows, S>::type s_vec_type;
			typedef typename make_vec<mat_traits::_rows-1, S>::type s_vec_less1_type;

			s_vec_less1_type (*fn_multVecMatrix)(const mat_type&, const s_vec_less1_type&) = multVecMatrix<s_vec_less1_type>;
			s_vec_less1_type (*fn_multDirMatrix)(const mat_type&, const s_vec_less1_type&) = multDirMatrix<s_vec_less1_type>;

			const mat_type& (mat_type::*fn_setScale)(const s_vec_less1_type&) 	= &mat_type::setScale;
			const mat_type& (mat_type::*fn_scale)(const s_vec_less1_type&) 		= &mat_type::scale;
			const mat_type& (mat_type::*fn_setTransl)(const s_vec_less1_type&) 	= &mat_type::setTranslation;
			const mat_type& (mat_type::*fn_transl)(const s_vec_less1_type&) 	= &mat_type::translate;
			const mat_type& (mat_type::*fn_setShear)(const s_vec_less1_type&) 	= &mat_type::setShear;
			const mat_type& (mat_type::*fn_shear)(const s_vec_less1_type&) 		= &mat_type::shear;

			m_cl
			.def(bp::init<s_mat_type>())
			.def("multVecMatrix", fn_multVecMatrix)
			.def("multDirMatrix", fn_multDirMatrix)
			.def("setToScale", fn_setScale, bp_ret_none())
			.def("scale", fn_scale, bp_ret_none())
			.def("setToTranslation", fn_setTransl, bp_ret_none())
			.def("translate", fn_transl, bp_ret_none())
			.def("setToShear", fn_setShear, bp_ret_none())
			.def("shear", fn_shear, bp_ret_none())
			.def(bp::other<s_vec_less1_type>() * bp::self)
			.def(bp::other<s_vec_type>() * bp::self)
			;
		}

		template<typename Vec>
		static Vec multVecMatrix(const mat_type& self, const Vec& vsrc)
		{
			Vec vdest;
			self.multVecMatrix(vsrc, vdest);
			return vdest;
		}

		template<typename Vec>
		static Vec multDirMatrix(const mat_type& self, const Vec& vsrc)
		{
			Vec vdest;
			self.multDirMatrix(vsrc, vdest);
			return vdest;
		}

		MatrixBind_T(bp_class& cl):m_cl(cl){}
		bp_class m_cl;
	};


	// Matrix-common bindings
	template<typename Matrix, typename ScalarTypes>
	struct MatrixBind
	{
		typedef Matrix 														mat_type;
		typedef imath_traits<mat_type> 										mat_traits;
		typedef typename mat_traits::scalar_type 							scalar_type;
		typedef typename make_vec<mat_traits::_columns, scalar_type>::type 	vec_type;
		typedef bp::class_<mat_type> 										bp_class;

		BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ol_inverse, 		inverse, 	0, 1);
		BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ol_invert, 		invert, 	0, 1);
		BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ol_gjInverse, 	gjInverse, 	0, 1);
		BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ol_gjInvert, 	gjInvert, 	0, 1);

		MatrixBind(const char* name)
		{
			const mat_type& (mat_type::*fn_setScale)(const scalar_type) = &mat_type::setScale;

			bp_class cl(name);
			cl
			.def(bp::init<scalar_type>())
			.def("__init__", bp::make_constructor(sequenceInit))
			.def("__getitem__", getItem)
			.def("__setitem__", setItem)
			.add_property("value", MatrixNNBind<mat_type,ScalarTypes>::getValue, setValue)
			.def(bp::self == bp::self)
			.def(bp::self != bp::self)
			.def(bp::self += bp::self)
			.def(bp::self += scalar_type())
			.def(bp::self + bp::self)
			.def(bp::self -= bp::self)
			.def(bp::self -= scalar_type())
			.def(bp::self - bp::self)
			.def(bp::self * bp::self)
			.def(bp::self *= bp::self)
			.def(bp::self * scalar_type())
			.def(bp::self *= scalar_type())
			.def(bp::self / scalar_type())
			.def(bp::self /= scalar_type())
			.def(- bp::self)
			.def("negate", &mat_type::negate, bp_ret_none())

			.def("inverse", &mat_type::inverse, ol_inverse())
			.def("invert", &mat_type::invert, bp_ret_none(), ol_invert())
			.def("gjInverse", &mat_type::gjInverse, ol_gjInverse())
			.def("gjInvert", &mat_type::gjInvert, bp_ret_none(), ol_gjInvert())

			.def("makeIdentity", &mat_type::makeIdentity)
			.def("transpose", &mat_type::transpose, bp_ret_none())
			.def("transposed", &mat_type::transposed)
			.def("equalWithAbsError", &mat_type::equalWithAbsError)
			.def("equalWithRelError", &mat_type::equalWithRelError)
			.def("setToScale", fn_setScale, bp_ret_none())
			.def("translation", &mat_type::translation)
			.def("__str__", toString)

			.def("dimensions", _dimensions)
			.staticmethod("dimensions")
			;

			bindBaseType<bp_class, mat_type>(cl);
			MatrixNNBind<mat_type, ScalarTypes>::bind(cl);
			boost::mpl::for_each<ScalarTypes>(MatrixBind_T<mat_type>(cl));
		}

		static std::string toString(const mat_type& self)
		{
			std::ostringstream s;
			bool firstr = true;
			s << '(';

			for(unsigned int i=0; i<imath_traits<mat_type>::_rows; ++i, firstr=false)
			{
				if(!firstr)
					s << ' ';
				s << '(';
				bool firstc = true;
				for(unsigned int j=0; j<imath_traits<mat_type>::_columns; ++j, firstc=false)
				{
					if(!firstc)
						s << ' ';
					s << self.x[i][j];
				}
				s << ')';
			}
			s << ')';

			return s.str();
		}

		static bp::tuple _dimensions() {
			return bp::make_tuple(imath_traits<mat_type>::_rows,
				imath_traits<mat_type>::_columns);
		}

		static mat_type* sequenceInit(const bp::object& x)
		{
			mat_type* m = new mat_type();
			setValue(*m, x);
			return m;
		}

		static bp::object getItem(const mat_type& self, const bp::object& index)
		{
			bp::extract<int> getint(index);
			if(getint.check())
			{
				int r = getint();
				if((r<0) || (r>=(int)imath_traits<mat_type>::_rows))
					PIMATH_THROW(PyExc_IndexError, "Matrix index out of range.");

				vec_type v;
				std::copy(self[r], self[r]+imath_traits<mat_type>::_columns, &v.x);
				return bp::object(v);
			}
			else if(bp::len(index) == 2)
			{
				int r = bp::extract<int>(index[0]);
				int c = bp::extract<int>(index[1]);
				if(	(r<0) || (r>=(int)imath_traits<mat_type>::_rows) ||
					(c<0) || (c>=(int)imath_traits<mat_type>::_columns))
					PIMATH_THROW(PyExc_IndexError, "Matrix index out of range.");
				return bp::object(self.x[r][c]);
			}
			else
			{
				PIMATH_THROW(PyExc_KeyError, "Only integer and 2-tuple indexing supported on matrices");
				return bp::object();
			}
		}


		static void setItem(mat_type& self, const bp::object& index, const bp::object& val)
		{
			bp::extract<int> getint(index);
			if(getint.check())
			{
				int r = getint();
				if((r<0) || (r>=(int)imath_traits<mat_type>::_rows))
					PIMATH_THROW(PyExc_IndexError, "Matrix index out of range.");

				vec_type v = bp::extract<vec_type>(val);
				std::copy(&v.x, &v.x+imath_traits<mat_type>::_columns, self[r]);
			}
			else if(bp::len(index) == 2)
			{
				int r = bp::extract<int>(index[0]);
				int c = bp::extract<int>(index[1]);
				if(	(r<0) || (r>=(int)imath_traits<mat_type>::_rows) ||
					(c<0) || (c>=(int)imath_traits<mat_type>::_columns))
					PIMATH_THROW(PyExc_IndexError, "Matrix index out of range.");
				self.x[r][c] = bp::extract<scalar_type>(val);
			}
			else
				PIMATH_THROW(PyExc_KeyError, "Only integer and 2-tuple indexing supported on matrices");
		}

		static void setValue(mat_type &self, const bp::object &x)
		{
			for(unsigned int i=0; i<imath_traits<mat_type>::_rows; ++i)
			{
				bp::tuple row = bp::extract<bp::tuple>(x[i]);
				for(unsigned int j=0; j<imath_traits<mat_type>::_columns; ++j)
					self.x[i][j] = bp::extract<scalar_type>(row[j]);
			}
		}
	};

}

#endif






