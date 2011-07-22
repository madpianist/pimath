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

#include <ImathHalfLimits.h>
#include "../Color.hpp"
#include "../Vec.hpp"

using namespace pimath;
namespace bp = boost::python;

void _pimath_export_color()
{
	typedef boost::mpl::vector<int,float,double,half> _types;

	// Color3s are the same as normal Vec3s
	Color3Bind<Imath::Color3<float>, _types>	("C3f");
	Color3Bind<Imath::Color3<half>, _types>	("C3h");

	// Note: If you want to use this type, you'll also need to uncomment
	// Vec3<unsigned char> in vec3.cpp. This will cause warnings during the compile
	// as some methods of Vec3 are not intended to run on unsigned chars.
	//Color3Bind<Imath::Color3<unsigned char>, _types>	("C3c");

	// Color4s are a unique type
    ColorBind<Imath::Color4<unsigned char>, _types>	("C4c");
	ColorBind<Imath::Color4<float>, _types>	("C4f");
	ColorBind<Imath::Color4<half>, _types>	("C4h");
}
