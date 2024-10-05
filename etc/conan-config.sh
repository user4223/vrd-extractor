#!/usr/bin/env bash

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"/../
COMPILER_NAME=${1}
COMPILER_VERSION=${2}

conan profile new vrd-extractor --force --detect

if [[ -z "${COMPILER_NAME}" ]]; then
    COMPILER_NAME=$(conan profile get settings.compiler vrd-extractor)
    if [[ -z "${COMPILER_NAME}" ]]; then
        echo "ERROR: No compiler auto-dectected or passed as argument"
        exit 1
    fi
else
    conan profile update settings.compiler=${COMPILER_NAME} vrd-extractor
fi

if [[ ! -z ${COMPILER_VERSION} ]]; then
    conan profile update settings.compiler.version=${COMPILER_VERSION} vrd-extractor
fi

readonly STANDARD_LIB=$(if [[ "$COMPILER_NAME" =~ ^(clang|apple-clang)$ ]]; then echo 'libc++'; else echo 'libstdc++11'; fi)
conan profile update settings.compiler.libcxx=${STANDARD_LIB} vrd-extractor

conan profile update conf.tools.system.package_manager:mode=install vrd-extractor
conan profile show vrd-extractor
