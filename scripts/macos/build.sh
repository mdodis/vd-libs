#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SAMPLE_TO_COMPILE="ui_basic"

while getopts "s:a" o; do
    case "${o}" in
        s)
            SAMPLE_TO_COMPILE="${OPTARG}"
            ;;
        a)
            ;;
        *)
            ;;
    esac
done

pushd $SCRIPT_DIR/../..

PRJ_DIR="$(realpath .)"
CFLAGS="-O0 -g -Wall -std=c11 -I $PRJ_DIR"

mkdir -p ./build
pushd ./build

echo "Compiling $SAMPLE_TO_COMPILE ..."

cc \
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


popd
popd
