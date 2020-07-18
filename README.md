![build](https://github.com/karlheinzkurt/vrd-extractor/workflows/build/badge.svg)

# Summary
Tool to extract VRD recipe data out of CR2/CRW/VRD/DR4 files created while 
using DPP. The extracted information is written or merged into XMP sidecar files to be used
by Darktable. Currently supported tags:

* Rating - Is merged togehter from 'Check Mark' and 'Star Rating' 
  information from VRD/DR version 1/2/4 or Exif/XMP data.
  Remember: 'Check Mark' in DPP is the internally used rating, while
  'Star Rating' is there for compatibility with other software, 
  Photographers might have used both independently or mixed over time.

When conflicts between different sources or destination occur, 
the user is able to solve it manually.

# Motivation
First of all: Actually I like the results when using DPP to process my images and I
have used DPP for years (10+) to do so because it is simple and straight forward, BUT:

* DPP is using proprietary formats for recipes
* It is build from proprietary source code
* It is not running on Linux natively

I discovered Darktable recently as a very good replacement running 
natively on Linux. It is not that hard to switch over the software, 
but all the modification are lost and it is a pain to regain the best images. 
I guess to convert all recipe information seamlessly from one format to another is just 
a dream, but to convert at least rating and check marks to find selected images 
in the archives again would be a good starting point. I didn't find an existing 
convertion tool, so I've started to write my own.

# Thanks
A lot of required information could I retrieve from
here: https://sno.phy.queensu.ca/~phil/exiftool/ . So almost all of the 
knowledge about the structure of recipe data is taken from there. Thank you Phil!

# Requirements

* C++14 enabled compiler
* cmake (> 3.1.2)  
* log4cxx  (>= 0.10.0, there is no conan package)
* libexiv2 (>= 0.25.2, no conan package)
* python3 as requirement for conan
* conan package manager (See: https://docs.conan.io/en/latest/installation.html)

(boost, gtest gets installed via conan)

# Build Instructions
```
mkdir build
pushd build

# Install dependencies
#
conan install --build missing ../

# Build lib, executable and tests
#
cmake -DCMAKE_BUILD_TYPE=Release ../source
cmake --build .

# Run tests
#
bin/vrdlib.test

popd
```

# Installation instructions for Debian like systems
```
pushd build

# Create package and check content
#
cpack
dpkg --contents vrdextractor*.deb

# Install package
#
sudo dpkg -i vrdextractor*.deb

popd
```

