#! /bin/bash

SCRIPT_FILE=$(which $0)
SCRIPT_ROOT=$(dirname $(realpath "$SCRIPT_FILE"))
PROJECT_ROOT=$(dirname "${SCRIPT_ROOT}")
HOST_ARCH=$(uname -m)
BUILD_ROOT=build/linux-$HOST_ARCH-release
OBJECT_ROOT=$BUILD_ROOT/object
OUTPUT_ROOT=$BUILD_ROOT/output

pushd "$PROJECT_ROOT"

mkdir -p ${OBJECT_ROOT}

# build curl
if [ -e "${OUTPUT_ROOT}/lib/libcurl.a" ]; then
    echo "libcurl already built, skip..."
else
    mkdir -p ${OBJECT_ROOT}/curl
    pushd ${OBJECT_ROOT}/curl

    ${PROJECT_ROOT}/3rdparty/curl-7.41.0/configure --prefix=$PROJECT_ROOT/$OUTPUT_ROOT || exit 1
    make || exit 1
    make install || exit 1

    popd
fi

# build freetype2
if [ -e "${OUTPUT_ROOT}/lib/libfreetype.a" ]; then
    echo "libfreetype2 already built, skip.."
else
    mkdir -p ${OBJECT_ROOT}/freetype2
    pushd ${OBJECT_ROOT}/freetype2

    ${PROJECT_ROOT}/3rdparty/freetype-2.5.5/configure --prefix=$PROJECT_ROOT/$OUTPUT_ROOT || exit 1
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

    ${PROJECT_ROOT}/3rdparty/libpng-1.6.17/configure --prefix=$PROJECT_ROOT/$OUTPUT_ROOT || exit 1
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

    ${PROJECT_ROOT}/3rdparty/libusb-1.0.19/configure --prefix=$PROJECT_ROOT/$OUTPUT_ROOT --disable-udev || exit 1
    make || exit 1
    make install || exit 1

    popd
fi

# build rpairmon
./onemake/onemake.py concurrent=8 scheme=release

# all done

popd

