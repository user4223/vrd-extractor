#!/usr/bin/env bash

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"

${WORKSPACE_ROOT}/etc/conan-config.sh
${WORKSPACE_ROOT}/etc/conan-install.sh Debug
${WORKSPACE_ROOT}/etc/cmake-config.sh Debug ON
${WORKSPACE_ROOT}/etc/cmake-build.sh Debug -- -Bj

pushd ${WORKSPACE_ROOT}/build/Debug

bin/vrdlib.test

gcovr -r $WORKSPACE_ROOT \
      -e ".+(gmock|gtest).*[.](cpp|h)$" \
      -e ".+(Test|TestBase|Mock|main)[.](cpp|h)$" \
      --html-details coverage.html

popd

open ${WORKSPACE_ROOT}/build/Debug/coverage.html
