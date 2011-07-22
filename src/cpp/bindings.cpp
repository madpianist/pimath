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

#include <boost/python.hpp>
#include <ImathPlatform.h>


extern void _pimath_export_half();
extern void _pimath_export_box();
extern void _pimath_export_boxAlgo();
extern void _pimath_export_color();
extern void _pimath_export_colorAlgo();
extern void _pimath_export_frame();
extern void _pimath_export_frustum();
extern void _pimath_export_interval();
extern void _pimath_export_line();
extern void _pimath_export_lineAlgo();
extern void _pimath_export_plane();
extern void _pimath_export_quat();
extern void _pimath_export_random();
extern void _pimath_export_roots();
extern void _pimath_export_shear();
extern void _pimath_export_sphere();
extern void _pimath_export_vec2();
extern void _pimath_export_vec3();
extern void _pimath_export_vec4();
extern void _pimath_export_vecAlgo();
extern void _pimath_export_matrix33();
extern void _pimath_export_matrix44();
extern void _pimath_export_euler();
extern void _pimath_export_matrixAlgo();
extern void _pimath_export_exc();

namespace bp = boost::python;

BOOST_PYTHON_MODULE(pimath)
{
	bp::scope().attr("M_PI") = M_PI;
	bp::scope().attr("M_PI_2") = M_PI_2;

	_pimath_export_half();
	_pimath_export_box();
	_pimath_export_boxAlgo();
	_pimath_export_vec3();
	_pimath_export_color();
	_pimath_export_colorAlgo();
	_pimath_export_frame();
	_pimath_export_frustum();
	_pimath_export_interval();
	_pimath_export_line();
	_pimath_export_lineAlgo();
	_pimath_export_plane();
	_pimath_export_quat();
	_pimath_export_random();
	_pimath_export_roots();
	_pimath_export_shear();
	_pimath_export_sphere();
	_pimath_export_vec2();
	_pimath_export_vec4();
	_pimath_export_vecAlgo();
	_pimath_export_matrix33();
	_pimath_export_matrix44();
	_pimath_export_euler();
	_pimath_export_matrixAlgo();
	_pimath_export_exc();
}
