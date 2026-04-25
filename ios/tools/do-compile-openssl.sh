#!/usr/bin/env bash
set -e

# Real do-compile-openssl: builds OpenSSL for iOS device and simulator
# Usage: do-compile-openssl.sh <arch|arch-sim>

ARCH_IN="$1"
ROOT=$(cd "$(dirname "$0")/.." && pwd)
if [ -z "$ARCH_IN" ]; then
    echo "Usage: do-compile-openssl.sh <arch|arch-sim>"
    exit 1
fi

# map simulator suffix
IS_SIM=0
case "$ARCH_IN" in
    *-sim)
        IS_SIM=1
        ARCH="${ARCH_IN%-sim}"
        ;;
    *)
        ARCH="$ARCH_IN"
        ;;
esac

echo "[openssl] building for arch=$ARCH (sim=$IS_SIM)"

FF_BUILD_ROOT="$ROOT"
FF_BUILD_NAME="openssl-$ARCH"
if [ "$IS_SIM" -eq 1 ]; then
    FF_BUILD_NAME="openssl-$ARCH-sim"
fi

FF_BUILD_SOURCE="$FF_BUILD_ROOT/$FF_BUILD_NAME"
FF_BUILD_PREFIX="$FF_BUILD_ROOT/build/$FF_BUILD_NAME/output"

mkdir -p "$FF_BUILD_PREFIX"

# determine platform and SDK
if [ "$IS_SIM" -eq 1 ] || [ "$ARCH" = "x86_64" ]; then
    FF_XCRUN_PLATFORM="iPhoneSimulator"
    FF_XCRUN_OSVERSION="-mios-simulator-version-min=15.0"
    case "$ARCH" in
        x86_64)
            OPENSSL_TARGET="darwin64-x86_64-cc"
            ;;
        arm64)
            # openssl target for arm64 simulator (supported in Configurations)
            OPENSSL_TARGET="iossimulator-arm64-xcrun"
            ;;
        *)
            echo "Unsupported simulator arch: $ARCH"
            exit 1
            ;;
    esac
else
    FF_XCRUN_PLATFORM="iPhoneOS"
    FF_XCRUN_OSVERSION="-miphoneos-version-min=15.0"
    case "$ARCH" in
        arm64)
            OPENSSL_TARGET="ios64-cross"
            ;;
        *)
            echo "Unsupported device arch: $ARCH"
            exit 1
            ;;
    esac
fi

FF_XCRUN_SDK=$(echo "$FF_XCRUN_PLATFORM" | tr '[:upper:]' '[:lower:]')
FF_XCRUN_DEVELOPER=`xcode-select -print-path`
if [ ! -d "$FF_XCRUN_DEVELOPER" ]; then
    echo "xcode path is not set correctly: $FF_XCRUN_DEVELOPER"
    exit 1
fi

FF_XCRUN_CC="xcrun -sdk $FF_XCRUN_SDK clang"
FF_XCRUN_SDK_PATH=`xcrun -sdk $FF_XCRUN_SDK --show-sdk-path`
FF_XCRUN_SDK_PLATFORM_PATH=`xcrun -sdk $FF_XCRUN_SDK --show-sdk-platform-path`

export CROSS_TOP="$FF_XCRUN_SDK_PLATFORM_PATH/Developer"
export CROSS_SDK=`echo ${FF_XCRUN_SDK_PATH/#$CROSS_TOP\/SDKs\//}`
export BUILD_TOOL="$FF_XCRUN_DEVELOPER"
export CC="$FF_XCRUN_CC -arch $ARCH $FF_XCRUN_OSVERSION -isysroot $FF_XCRUN_SDK_PATH"

echo "build_source: $FF_BUILD_SOURCE"
echo "build_prefix: $FF_BUILD_PREFIX"
echo "CC: $CC"

# Ensure source dir exists
if [ ! -d "$FF_BUILD_SOURCE" ]; then
    # try to copy from any existing openssl-* source in the ios dir
    echo "OpenSSL source directory not found: $FF_BUILD_SOURCE"
    ALT_SRC=$(ls -d "$ROOT"/openssl-* 2>/dev/null | head -n 1 || true)
    if [ -n "$ALT_SRC" ]; then
        echo "Found alternate OpenSSL source $ALT_SRC — copying to $FF_BUILD_SOURCE"
        rm -rf "$FF_BUILD_SOURCE"
        cp -R "$ALT_SRC" "$FF_BUILD_SOURCE"
    else
        echo "Please run init-ios.sh to fetch sources first."
        exit 1
    fi
fi

cd "$FF_BUILD_SOURCE"

OPENSSL_CFG_FLAGS="$OPENSSL_TARGET --openssldir=$FF_BUILD_PREFIX"

if [ -f "Makefile" ]; then
    echo "Makefile exists — reconfiguring to ensure correct CC/isysroot"
    ./Configure $OPENSSL_CFG_FLAGS
    make clean || true
else
    echo "config: $OPENSSL_CFG_FLAGS"
    ./Configure $OPENSSL_CFG_FLAGS
    make clean || true
fi

echo "compiling OpenSSL..."
make -j$(sysctl -n hw.ncpu || echo 4)
echo "installing OpenSSL to $FF_BUILD_PREFIX"
make install_sw INSTALLTOP="$FF_BUILD_PREFIX"

echo "done: $FF_BUILD_PREFIX"
