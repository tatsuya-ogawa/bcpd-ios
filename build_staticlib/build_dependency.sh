#!/bin/bash
set -e
MIN_IOS_VERSION=16.0

WORKDIR=$(pwd)/ios_dependencies
mkdir -p "$WORKDIR"
cd "$WORKDIR"

# OpenMP (llvm-project) clone & build
if [ ! -d llvm-project ]; then
  git clone --depth 1 https://github.com/llvm/llvm-project.git
fi

OPENMP_SRC="$WORKDIR/llvm-project/openmp"
OPENMP_BUILD="$WORKDIR/build_openmp_ios"
OPENMP_INSTALL="$WORKDIR/ios_libs/openmp"
IOS_SDK=$(xcrun --sdk iphoneos --show-sdk-path)
IOS_CC="xcrun --sdk iphoneos clang"
IOS_CFLAGS="-arch arm64 -isysroot $IOS_SDK -target arm64-apple-ios -mios-version-min=$MIN_IOS_VERSION"

mkdir -p "$OPENMP_BUILD"
cd "$OPENMP_BUILD"
cmake "$OPENMP_SRC" \
  -DCMAKE_SYSTEM_NAME=iOS \
  -DCMAKE_OSX_ARCHITECTURES=arm64 \
  -DCMAKE_OSX_SYSROOT="$IOS_SDK" \
  -DCMAKE_C_COMPILER="$(xcrun --sdk iphoneos --find clang)" \
  -DCMAKE_C_FLAGS="$IOS_CFLAGS" \
  -DCMAKE_INSTALL_PREFIX="$OPENMP_INSTALL" \
  -DLIBOMP_ENABLE_SHARED=OFF \
  -DLIBOMP_ENABLE_STATIC=ON \
  -DLIBOMP_ARCH=aarch64 \
  -DCMAKE_HAVE_THREADS_LIBRARY=1 \
  -DCMAKE_USE_WIN32_THREADS_INIT=0 \
  -DCMAKE_USE_PTHREADS_INIT=1 \
  -DCMAKE_THREAD_LIBS_INIT="-lpthread" \
  -DLIBOMP_ENABLE_TESTS=OFF 
make -j4
make install

# OpenBLAS clone & build
cd "$WORKDIR"
if [ ! -d OpenBLAS ]; then
  git clone --depth 1 https://github.com/xianyi/OpenBLAS.git
fi

OPENBLAS_SRC="$WORKDIR/OpenBLAS"
OPENBLAS_INSTALL="$WORKDIR/ios_libs/openblas"
cd "$OPENBLAS_SRC"
make TARGET=ARMV8 BINARY=64 CC="$(xcrun --sdk iphoneos --find clang)" HOSTCC=clang \
  CFLAGS="$IOS_CFLAGS -O2 -Wno-macro-redefined" NO_SHARED=1 DYNAMIC_ARCH=1 NOFORTRAN=1 libs netlib
make install PREFIX="$OPENBLAS_INSTALL"