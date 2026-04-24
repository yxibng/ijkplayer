# ijkplayer 依赖库与版本

## C/C++ 依赖

- **FFmpeg**: n8.1 (https://github.com/FFmpeg/FFmpeg.git)
- **OpenSSL**: openssl-4.0.0 (https://github.com/openssl/openssl.git)
- **SoundTouch**: soundtouch-2.1.0 (https://codeberg.org/soundtouch/soundtouch.git)
- **libyuv**: main / 最新版 (https://chromium.googlesource.com/libyuv/libyuv)
- **soxr**: 0.1.3 (https://sourceforge.net/p/soxr/code/ci/master/tree/)

## Android 依赖

### Gradle / 构建工具
- **Gradle Version**: 8.7 (最新稳定版)
- **Android Gradle Plugin**: 8.4.0 (最新稳定版)
- **Android Compile SDK Version**: 34
- **Android Build Tools Version**: 34.0.0

### Java / Android 库
- **ExoPlayer**: 2.19.1 (`com.google.android.exoplayer:exoplayer:2.19.1`) / Media3: 1.3.1 (`androidx.media3:media3-exoplayer:1.3.1`)
- **AndroidX Core**: 1.13.1 (`androidx.core:core-ktx:1.13.1`)
- **Otto**: 已弃用，建议替换为 RxJava 或 EventBus (例如 EventBus 3.3.1 `org.greenrobot:eventbus:3.3.1`)
