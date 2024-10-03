#!/usr/bin/env bash

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"

${WORKSPACE_ROOT}/etc/conan-config.sh
${WORKSPACE_ROOT}/etc/conan-install.sh Release
${WORKSPACE_ROOT}/etc/cmake-config.sh Release
${WORKSPACE_ROOT}/etc/cmake-build.sh Release $@

pushd ${WORKSPACE_ROOT}/build/Release/
    cpack .
popd
