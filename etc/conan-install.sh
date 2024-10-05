#!/usr/bin/env bash

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"/../
readonly BUILD_TYPE=${1:-Release}

conan install ${WORKSPACE_ROOT} \
    -if ${WORKSPACE_ROOT}/build/${BUILD_TYPE} \
    -pr vrd-extractor -pr:b vrd-extractor \
    -s build_type=${BUILD_TYPE} \
    --build missing \
    ${@:2}

conan remove '*' --build --src --force
