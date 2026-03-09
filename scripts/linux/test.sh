#!/bin/bash
set -e

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
pushd $SCRIPT_DIR/../.. > /dev/null
mkdir -p ./build-test > /dev/null

PRJ_DIR="$(realpath .)"
CFLAGS="-Wall -pthread -std=c99 -Wno-missing-braces -Wno-unused-but-set-variable -Wno-unused-function -I $PRJ_DIR "
DFLAGS="-O0 -g"
CFLAGS="$DFLAGS $CFLAGS"

pushd ./build-test > /dev/null
c++ $CFLAGS $PRJ_DIR/tests/vd_fw_test.cpp -o vd_fw_test
popd
popd
