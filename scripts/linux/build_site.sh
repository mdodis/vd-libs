#!/bin/bash
set -e
SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)

pushd $SCRIPT_DIR/../.. > /dev/null

mkdir -p build-site-temp/
pushd build-site-temp

gcc -g -O0 -o docubuild -I ../ ../site/docubuild.c
popd

./build-site-temp/docubuild ./site/ -o ./site-base/
popd
