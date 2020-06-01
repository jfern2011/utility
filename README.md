# utility
General purpose C++ utilities


## bitops

bitops.h contains functions for manipulating bits in generic integral types.
See the Doxygen pages for details


## filesys

filesys.h contains functions relating to filesystem operations. C++17
introduces std::filesystem, which deprecates these functions. However, they
remain here for projects that predate the C++17 standard. See the Doxygen
page for details


## superstring

The superstring class is a simple std::string wrapper which augments the
string manipulation capabilities available via the standard library. See the
Doxygen pages for details


## Usage

cd utility/  
mkdir build && cd build  
cmake ..  
make


## cpplint

This project imports Google's cpplint style checker as a submodule: https://github.com/cpplint/cpplint

### To run the linter

python utility/cpplint/cpplint.py --root=utility/include/superstring utility/include/superstring/superstring.h
python utility/cpplint/cpplint.py --root=utility/include/bitops utility/include/bitops/bitops.h
python utility/cpplint/cpplint.py --root=utility/include/filesys utility/include/filesys/filesys.h
python utility/cpplint/cpplint.py utility/src/filesys/filesys.cc utility/src/superstring/superstring.cc

python utility/cpplint/cpplint.py utility/tests/*


## Contact

Feel free to email me with bug reports or suggestions:
jfernandez3@gatech.edu