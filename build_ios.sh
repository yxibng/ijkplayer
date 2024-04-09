#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "$0")"; pwd)";

cd $SCRIPT_DIR

git checkout $SCRIPT_DIR
git clean -dff


sh init-ios.sh
sh init-ios-openssl.sh

cd $SCRIPT_DIR/ios

# build openssl first
sh compile-openssl.sh clean
sh compile-openssl.sh all
# build ffmpeg with openssl
sh compile-ffmpeg.sh all

