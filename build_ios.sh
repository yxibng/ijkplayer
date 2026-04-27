#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "$0")"; pwd)";

cd $SCRIPT_DIR

sh init-ios.sh

cd $SCRIPT_DIR/ios
# build ffmpeg with ios securetransport
sh compile-ffmpeg.sh all

