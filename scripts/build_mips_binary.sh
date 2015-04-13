#! /bin/bash

SCRIPT_FILE=$(which $0)
SCRIPT_ROOT=$(dirname $(realpath "$SCRIPT_FILE"))
PROJECT_ROOT=$(dirname "${SCRIPT_ROOT}")
HOST_ARCH=$(uname -m)
TARGET_ARCH=mips
BUILD_ROOT=build/openwrt-$TARGET_ARCH-release
OBJECT_ROOT=$BUILD_ROOT/object
OUTPUT_ROOT=$BUILD_ROOT/output
CROSS_FLAGS="--build=${HOST_ARCH}-linux --host=mips-openwrt-linux"
export CC=mips-openwrt-linux-gcc
export CFLAGS="-I${PROJECT_ROOT}/${OUTPUT_ROOT}/include"
export CPPFLAGS="-I${PROJECT_ROOT}/${OUTPUT_ROOT}/include"
export LDFLAGS="-L${PROJECT_ROOT}/${OUTPUT_ROOT}/lib"

pushd "$PROJECT_ROOT"

mkdir -p ${OBJECT_ROOT}

# build curl
if [ -e "${OUTPUT_ROOT}/lib/libcurl.a" ]; then
    echo "libcurl already built, skip..."
else
    mkdir -p ${OBJECT_ROOT}/curl
    pushd ${OBJECT_ROOT}/curl

    ${PROJECT_ROOT}/3rdparty/curl-7.41.0/configure --prefix=$PROJECT_ROOT/$OUTPUT_ROOT $CROSS_FLAGS || exit 1
    make || exit 1
    make install || exit 1

    popd
fi

# build zlib
if [ -e "${OUTPUT_ROOT}/lib/libz.a" ]; then
    echo "libz already built, skip..."
else
    mkdir -p ${OBJECT_ROOT}/zlib
    pushd ${PROJECT_ROOT}/3rdparty/zlib-1.2.8

    ./configure --prefix=$PROJECT_ROOT/$OUTPUT_ROOT || exit 1
    make || exit 1
    make install || exit 1

    popd
fi

# build libpng
if [ -e "${OUTPUT_ROOT}/lib/libpng16.a" ]; then
    echo "libpng16 already built, skip..."
else
    mkdir -p ${OBJECT_ROOT}/libpng16
    pushd ${OBJECT_ROOT}/libpng16

    echo "${CFLAGS}"

    ${PROJECT_ROOT}/3rdparty/libpng-1.6.17/configure --prefix=$PROJECT_ROOT/$OUTPUT_ROOT $CROSS_FLAGS || exit 1
    make CFLAGS="$CFLAGS" LDFLAGS="$LDFLAGS"  || exit 1
    make install CFLAGS="$CFLAGS" LDFLAGS="$LDFLAGS" || exit 1

    popd
fi

# build freetype2
if [ -e "${OUTPUT_ROOT}/lib/libfreetype.a" ]; then
    echo "libfreetype2 already built, skip.."
else
    mkdir -p ${OBJECT_ROOT}/freetype2
    pushd ${OBJECT_ROOT}/freetype2

    ${PROJECT_ROOT}/3rdparty/freetype-2.5.5/configure --prefix=$PROJECT_ROOT/$OUTPUT_ROOT --with-png=no --with-harfbuzz=no $CROSS_FLAGS || exit 1
    make || exit 1
    make install || exit 1

    popd
fi

# build libusb
if [ -e "${OUTPUT_ROOT}/lib/libusb-1.0.a" ]; then
    echo "libusb-1.0 already built, skip..."
else
    mkdir -p ${OBJECT_ROOT}/libusb
    pushd ${OBJECT_ROOT}/libusb

    ${PROJECT_ROOT}/3rdparty/libusb-1.0.19/configure --prefix=$PROJECT_ROOT/$OUTPUT_ROOT --disable-udev $CROSS_FLAGS || exit 1
    make || exit 1
    make install || exit 1

    popd
fi

# build rpairmon
./onemake/onemake.py concurrent=8 scheme=release target_arch=mips target_platform=openwrt

# all done

popd

