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

echo "SAMPLE_TO_COMPILE is $SAMPLE_TO_COMPILE"
pushd $SCRIPT_DIR/../..

PRJ_DIR="$(realpath .)"
CFLAGS="-O0 -g -Wall -std=c11 -I $PRJ_DIR"

mkdir -p ./build
pushd ./build

if [ -n "$PROGRAM_TO_COMPILE" ]; then
    cc \
        -pthread \
        -x objective-c \
        $CFLAGS \
        -fsanitize=address \
        $PRJ_DIR/program/$PROGRAM_TO_COMPILE.c \
        -o $PROGRAM_TO_COMPILE \
        -framework Cocoa \
        -framework Metal \
        -framework QuartzCore \
        -framework CoreGraphics \
        -framework IOSurface \
        -framework IOKit \
        -framework Foundation \
        -framework OpenGL
fi

if [ -n "$SAMPLE_TO_COMPILE" ]; then
    cc \
        -pthread \
        -x objective-c \
        $CFLAGS \
        -fsanitize=address \
        $PRJ_DIR/samples/$SAMPLE_TO_COMPILE.c \
        -o $SAMPLE_TO_COMPILE \
        -framework Cocoa \
        -framework Metal \
        -framework QuartzCore \
        -framework CoreGraphics \
        -framework IOSurface \
        -framework IOKit \
        -framework Foundation \
        -framework OpenGL
fi


popd
popd
