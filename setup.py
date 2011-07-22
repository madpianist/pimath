#setup.py
from distutils.core import setup
from distutils.extension import Extension
import os.path
from os import environ
import sys

# Set this to false to dynamically link the ILM libraries.
static_link_ilmbase = True

if sys.platform == "win32" :
    include_dirs = ["C:/Boost/include/boost-1_32","."]
    libraries=["boost_python-mgw"]
    library_dirs=['C:/Boost/lib']
else :
    include_dirs = [environ['BOOST_ROOT']+"/include",
                    environ['ILMBASE_ROOT']+"/include/",
                    environ['ILMBASE_ROOT']+"/include/OpenEXR/","."]
    libraries=["pthread","dl","z","python2.5",
               "boost_python"]
    if static_link_ilmbase == False:
        libraries = libraries+["Imath","Iex","Half"]
    library_dirs=['/usr/local/lib',
                  environ['ILMBASE_ROOT']+"/lib",
                  environ['BOOST_ROOT']+"/lib"]

files = ["src/cpp/bindings.cpp","src/cpp/boxAlgo.cpp","src/cpp/box.cpp",
         "src/cpp/colorAlgo.cpp","src/cpp/color.cpp","src/cpp/euler.cpp",
         "src/cpp/exc.cpp","src/cpp/frame.cpp","src/cpp/frustum.cpp",
         "src/cpp/half.cpp","src/cpp/interval.cpp","src/cpp/lineAlgo.cpp",
         "src/cpp/line.cpp","src/cpp/matrix33.cpp","src/cpp/matrix44.cpp",
         "src/cpp/matrixAlgo.cpp","src/cpp/plane.cpp","src/cpp/quat.cpp",
         "src/cpp/random.cpp","src/cpp/roots.cpp","src/cpp/shear.cpp",
         "src/cpp/sphere.cpp","src/cpp/vec2.cpp","src/cpp/vec3.cpp",
         "src/cpp/vec4.cpp","src/cpp/vecAlgo.cpp"]

extra_objects=[]
if static_link_ilmbase == True:
    extra_objects=[environ['ILMBASE_ROOT']+"/lib/libImath.a",
                   environ['ILMBASE_ROOT']+"/lib/libHalf.a",
                   environ['ILMBASE_ROOT']+"/lib/libIex.a"]

setup(name="pimath",
      ext_modules=[
                    Extension("pimath",files,
                    library_dirs=library_dirs,
                    libraries=libraries,
                    include_dirs=include_dirs,
                    define_macros=[("BOOST_PYTHON_MAX_ARITY","17")],
                    depends=[],
                    extra_objects=extra_objects,
                    extra_link_args=[]),
                    ]
     )