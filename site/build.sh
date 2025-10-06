#!/bin/bash
SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)

pushd $SCRIPT_DIR/.. > /dev/null
PRJ_DIR=$(pwd)
SRC_DIR=$PRJ_DIR/site

mkdir -p ./build > /dev/null
pushd ./build > /dev/null

echo $PRJ_DIR
echo $SRC_DIR

cc -std=c99 -g -O0 -I $PRJ_DIR $SRC_DIR/docubuild.c -o docubuild

popd
popd