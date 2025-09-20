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

mkdir -p ./build > /dev/null
pushd ./build > /dev/null

if [ -n "$PROGRAM_TO_COMPILE" ]; then
    echo "Compiling program: $PROGRAM_TO_COMPILE..."
    cc \
        -pthread \
        -x objective-c \
        $CFLAGS \
        -fsanitize=address \
        $PRJ_DIR/programs/$PROGRAM_TO_COMPILE.c \
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
    echo "Compiling sample: $SAMPLE_TO_COMPILE..."
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


popd > /dev/null
popd > /dev/null
