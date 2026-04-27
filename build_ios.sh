#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "$0")"; pwd)";

cd $SCRIPT_DIR

sh init-ios.sh
# sh init-ios-openssl.sh

cd $SCRIPT_DIR/ios

# build openssl first
# sh compile-openssl.sh clean
# sh compile-openssl.sh all
# build ffmpeg with openssl
sh compile-ffmpeg.sh all

