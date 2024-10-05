#!/usr/bin/env bash

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"/../
readonly BUILD_TYPE=${1:-Release}
readonly ENABLE_COVERAGE=${2:-OFF}

# when presets are working for sure due to cmake >= 3.23, use: --preset release instead of -DCMAKE_TOOLCHAIN_FILE=...
cmake -S ${WORKSPACE_ROOT} \
    -B ${WORKSPACE_ROOT}/build/${BUILD_TYPE} \
    -DCMAKE_TOOLCHAIN_FILE=build/${BUILD_TYPE}/generators/conan_toolchain.cmake \
    -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
    -DENABLE_COVERAGE=${ENABLE_COVERAGE} \
    ${@:2}
