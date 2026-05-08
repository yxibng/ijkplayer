# FFmpeg 7.1.2 migration status

## Branches

- Main repo branch: `ffmpeg-7.1.2-migration`
- FFmpeg repo branch: `extra/ffmpeg` -> `ijk-ffmpeg-7.1.2`

## Baseline

- Old ijk FFmpeg baseline: `ff4.0--ijk0.8.8--20210426--001`
- New baseline target: `n7.1.2`

## Applied patches on top of n7.1.2

1. .gitignore: ignore DS_Store
2. avformat/hls: parse #EXT-X-MEDIA-SEQUENCE only once
3. avformat/http: read exact bytes at end of stream to avoid being blocked
4. avformat/hls: fix duration
5. Revert "Fix url_feof() for file appendings."
6. avformat/protocols: add stub for custom protocol
7. avformat/async: remove const for ff_async_protocol
8. lavu/dict: fix crash when key is null
9. Revert "libavformat: not treat 0 as EOF"

## Remaining historical patches (from ff4.0 fork)

Total remaining by subject: 39

- lavf/utils: register ijklas demuxer
- libavutils: convert pointer to string instead of using av_dict_set_int
- feat(libavformat/makefile): add the ffmpeg interface for las demo
- lavf/utils: add register demuxer method
- lavf/utils: fix compile error
- avformat/tcp: dns-cache use uri key
- lavf/utils: find stream info without decoding
- lavf/hls: add segment io protocol option
- libavformat/tcp: differentiate varieties of timeout events
- avformat/http: add filesize report
- lavf/hls: fix seek problem
- avformat/tcp: support tcp fastopen
- libavformat/flv: support HEVC
- libavformat/tcp: support dns cache
- avformat/utils: support don't get real frame rate
- avformat/pipe: enable seekable
- avformat/tcp: support option rw_timeout and connect_timeout
- avformat/protocols: add ijkio
- avformat/hls: pass down av options
- avformat/tcp: export ijk_tcp_getaddrinfo_nonblock
- avformat/tcp: add one_by_one option for getaddrinfo()
- avformat/tcp: support timeout for getaddrinfo
- avformat/http: apply control event in application
- avformat/tcp: apply control event in application
- avutil/application: support low level event callback
- avformat/ijkutils: add dummy for replacing ijk protocol
- avformat/concatdec: fetch bit_rate from internal format
- avformat/http: add option http-tcp-hook
- avformat/concatdec: pass out io error
- avformat/concat: expose a flag to indicate source switch
- avformat/concatdec: close previous segment only after new segment is open successfully
- lavf/concatdec: pass options to nested input
- lavf/avformat: add support for passing options to nested input
- avformat/http: try to fix missing Content-Range
- avformat/mov: prefer nearest sample as next sample
- avutil/Makefile: install thread.h
- avformat/Makefile: install internal.h
- avformat/Makefile: install url.h
- avformat/Makefile: install avc.h and avc.o

## Notes

- Several skipped patches modify FFmpeg 4.0 internals that were heavily refactored in 7.1.2 (especially demuxer registration and AVInputFormat internals).
- Next migration pass should prioritize ijk-specific protocol and demuxer integration first (`ijkio`, `ijklas`, `ijklivehook`, option plumbing), then behavior patches.
