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
CFLAGS="-Wall -std=c11 -Wno-unused-but-set-variable -Wno-unused-function -I $PRJ_DIR"
DFLAGS="-O0 -g"
CFLAGS="$DFLAGS $CFLAGS"


mkdir -p ./build-linux > /dev/null
pushd ./build-linux > /dev/null

if [ -n "$PROGRAM_TO_COMPILE" ]; then
    echo "Compiling program: $PROGRAM_TO_COMPILE..."
    cc \
        -pthread \
        $CFLAGS \
        $PRJ_DIR/programs/$PROGRAM_TO_COMPILE.c \
        -lm \
        -o $PROGRAM_TO_COMPILE
fi

if [ -n "$SAMPLE_TO_COMPILE" ]; then
    echo "Compiling sample: $SAMPLE_TO_COMPILE..."
    cc \
        -pthread \
        $CFLAGS \
        $PRJ_DIR/samples/$SAMPLE_TO_COMPILE.c \
        -lm \
        -o $SAMPLE_TO_COMPILE
fi

echo "done"
popd > /dev/null
popd > /dev/null
