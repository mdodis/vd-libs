#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(cd "$SCRIPT_DIR/../.." && pwd)"

mkdir -p "$PROJECT_DIR/build-test/"

pushd "$PROJECT_DIR/build-test/"

cc -std=c99 -O0 -g -Wall -o run_tests  $PROJECT_DIR/test.c

popd