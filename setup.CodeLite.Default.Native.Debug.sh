#!/bin/bash
DIR="$( cd "$(dirname "$0")" ; pwd -P )"
BUILD_DIR=$DIR/build/CodeLite/Debug
SOURCE_DIR=$DIR/source

mkdir -p $BUILD_DIR
pushd $BUILD_DIR
   cmake \
   -DBIN_PATH_POSTFIX=CodeLite/Debug \
   -DCMAKE_BUILD_TYPE=Debug \
   -G"CodeLite - Unix Makefiles" \
    $SOURCE_DIR 
popd

cmake --build $BUILD_DIR --config Debug
