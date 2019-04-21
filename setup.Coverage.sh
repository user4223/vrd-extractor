#!/bin/bash
DIR="$( cd "$(dirname "$0")" ; pwd -P )"
BUILD_DIR=$DIR/build/Coverage
SOURCE_DIR=$DIR/source

rm -rf $BUILD_DIR
mkdir -p $BUILD_DIR
pushd $BUILD_DIR
   cmake \
      -DBIN_PATH_POSTFIX=Coverage \
      -DCMAKE_BUILD_TYPE=Coverage \
      $SOURCE_DIR 

   cmake --build .

   $DIR/bin/Coverage/vrdlib.test

   gcovr -r $DIR \
         -e ".+(gmock|gtest).*[.](cpp|h)$" \
         -e ".+(Test|TestBase|Mock|main)[.](cpp|h)$"
popd

