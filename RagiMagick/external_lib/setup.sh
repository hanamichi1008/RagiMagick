#!/bin/bash

while getopts p: OPT
do
    case $OPT in
        p)  PLATFORM=$OPTARG
            ;;
    esac
done

CMAKE_OPTION=""

if [ "${PLATFORM}" = "win" ]; then
    CMAKE_OPTION="-T llvm"
fi

# libjpeg-turbo
LIBJPEG_TURBO_ROOT="$(pwd)/libjpeg-turbo"
LIBJPEG_TURBO_SRC=${LIBJPEG_TURBO_ROOT}/src
LIBJPEG_TURBO_CACHE=${LIBJPEG_TURBO_ROOT}/build
# libpng
LIBPNG_ROOT="$(pwd)/libpng"
LIBPNG_SRC=${LIBPNG_ROOT}/src
LIBPNG_CACHE=${LIBPNG_ROOT}/build
# zlib
LIBZLIB_ROOT="$(pwd)/zlib"
LIBZLIB_SRC=${LIBZLIB_ROOT}/src
LIBZLIB_CACHE=${LIBZLIB_ROOT}/build

# libjpeg-turbo
if [ -e ${LIBJPEG_TURBO_CACHE} ]; then
    rm -rf ${LIBJPEG_TURBO_CACHE}
fi
mkdir ${LIBJPEG_TURBO_CACHE}
pushd ${LIBJPEG_TURBO_CACHE}
cmake ${LIBJPEG_TURBO_SRC} ${CMAKE_OPTION}
cmake --build . -j 4
popd

# zlib
if [ -e ${LIBZLIB_CACHE} ]; then
    rm -rf ${LIBZLIB_CACHE}
fi
mkdir ${LIBZLIB_CACHE}
pushd ${LIBZLIB_CACHE}
cmake ${LIBZLIB_SRC} ${CMAKE_OPTION}
cmake --build . -j 4
pushd ${LIBZLIB_SRC}
git reset --hard
git clean -f
popd
popd

# libpng
if [ -e ${LIBPNG_CACHE} ]; then
    rm -rf ${LIBPNG_CACHE}
fi
mkdir ${LIBPNG_CACHE}
pushd ${LIBPNG_CACHE}
if [ "${PLATFORM}" = "win" ]; then
    cmake ${LIBPNG_SRC} -DZLIB_LIBRARY="${LIBZLIB_CACHE}/zlibstatic" -DZLIB_INCLUDE_DIR="${LIBZLIB_SRC};${LIBZLIB_CACHE}" ${CMAKE_OPTION}
else
    cmake ${LIBPNG_SRC} -DZLIB_LIBRARY="${LIBZLIB_CACHE}/libz.a" -DZLIB_INCLUDE_DIR="${LIBZLIB_SRC};${LIBZLIB_CACHE}" ${CMAKE_OPTION}
fi
cmake --build . -j 4
popd

echo "complete!!!"
