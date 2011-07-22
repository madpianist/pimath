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

#include "../exception_translators.hpp"

using namespace pimath;
namespace bp = boost::python;


ExcBind::ExcBind()
{
	bp::class_< ::Iex::MathExc> exc("MathExc");
	exc.def( "__str__", &::Iex::MathExc::what);
	bp::register_exception_translator< ::Iex::MathExc>(&translate);
	m_excType = exc.ptr();
}


void ExcBind::translate(const ::Iex::MathExc& e)
{
	bp::object pythonExceptionInstance(e);
	PyErr_SetObject(m_excType, pythonExceptionInstance.ptr());
}


PyObject* ExcBind::m_excType(NULL);


void _pimath_export_exc()
{
	ExcBind();

	ExcDerivedBind<Imath::NullVecExc>			("NullVecExc");
	ExcDerivedBind<Imath::InfPointExc>			("InfPointExc");
	ExcDerivedBind<Imath::NullQuatExc>			("NullQuatExc");
	ExcDerivedBind<Imath::SingMatrixExc>		("SingMatrixExc");
	ExcDerivedBind<Imath::ZeroScaleExc>			("ZeroScaleExc");
	ExcDerivedBind<Imath::IntVecNormalizeExc>	("IntVecNormalizeExc");
}







