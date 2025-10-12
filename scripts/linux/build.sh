#!/bin/bash
set -e

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
SAMPLE_TO_COMPILE=""
PROGRAM_TO_COMPILE=""

while getopts "s:p:" o; do
    case "${o}" in
        s)
            SAMPLE_TO_COMPILE="${OPTARG}"
            ;;
        p)
            PROGRAM_TO_COMPILE="${OPTARG}"
            ;;
        *)
            ;;
    esac
done

pushd $SCRIPT_DIR/../.. > /dev/null

PRJ_DIR="$(realpath .)"
CFLAGS="-O0 -g -Wall -std=c11 -I $PRJ_DIR"

mkdir -p ./build-linux > /dev/null
pushd ./build-linux > /dev/null

if [ -n "$PROGRAM_TO_COMPILE" ]; then
    echo "Compiling program: $PROGRAM_TO_COMPILE..."
    cc \
        -pthread \
        $CFLAGS \
        $PRJ_DIR/programs/$PROGRAM_TO_COMPILE.c \
        -o $PROGRAM_TO_COMPILE
fi

if [ -n "$SAMPLE_TO_COMPILE" ]; then
    echo "Compiling sample: $SAMPLE_TO_COMPILE..."
    cc \
        -pthread \
        $CFLAGS \
        $PRJ_DIR/samples/$SAMPLE_TO_COMPILE.c \
        -lX11 -lGL \
        -o $SAMPLE_TO_COMPILE
fi

popd > /dev/null
popd > /dev/null
