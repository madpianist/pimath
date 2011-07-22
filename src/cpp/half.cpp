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
#include <ImathHalfLimits.h>
#include <string>
#include <sstream>


namespace bp = boost::python;

// to-python converter
struct half_to_float
{
	static PyObject* convert(const half& h) {
		return bp::incref(bp::object(h.operator float()).ptr());
	}
};

// from-python converter
struct half_from_number
{
	static void* convertible(PyObject* obj_ptr)
	{
		return (PyFloat_Check(obj_ptr) || PyInt_Check(obj_ptr))?
			obj_ptr : NULL;
	}

	static void construct(PyObject* obj_ptr, bp::converter::rvalue_from_python_stage1_data* data)
	{
		float f = (PyFloat_Check(obj_ptr))?
			static_cast<float>(PyFloat_AsDouble(obj_ptr)) :
			static_cast<float>(PyInt_AsLong(obj_ptr));

		void* storage = ((bp::converter::rvalue_from_python_storage<half>*)data)->storage.bytes;
		new(storage) half(f);
		data->convertible = storage;
	}
};


void _pimath_export_half()
{
	bp::to_python_converter<half, half_to_float>();

	bp::converter::registry::push_back(&half_from_number::convertible,
		&half_from_number::construct, bp::type_id<half>());
}








