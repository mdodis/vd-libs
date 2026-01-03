#!/bin/bash
SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)

pushd $SCRIPT_DIR/../.. > /dev/null
PRJ_DIR="$(realpath .)"

mkdir -p ./build> /dev/null
pushd ./build > /dev/null
cc \
    -pthread \
    -x objective-c \
    $CFLAGS \
    -fsanitize=address \
    -I $PRJ_DIR \
    -I $PRJ_DIR/ext \
    $PRJ_DIR/programs/gamecontrollerdb_gen.c \
    -o gamecontrollerdb_gen \
    -framework Cocoa \
    -framework Metal \
    -framework QuartzCore \
    -framework CoreGraphics \
    -framework IOSurface \
    -framework IOKit \
    -framework Foundation \
    -framework OpenGL
popd

RGCDBGEN=$PRJ_DIR/build/gamecontrollerdb_gen

$RGCDBGEN assets/builtin.rgcdb "Mac Os X" > builtin.rgcdb.c
popd
