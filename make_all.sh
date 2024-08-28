#!/bin/bash

set -e
set -v

# build eol lib
mkdir -p build/eol && pushd build/eol
cmake ../../code -DCMAKE_BUILD_TYPE=DEBUG
make && popd

# build google test lib
mkdir -p build/googletest && pushd build/googletest
cmake ../../test/googletest -DBUILD_GMOCK=OFF
make && popd

# build test
mkdir -p build/test && pushd build/test
cmake ../../test -DCMAKE_BUILD_TYPE=DEBUG
make VERBOSE=1 && popd
