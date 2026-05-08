/*
 * Copyright (c) 2003 Bilibili
 * Copyright (c) 2003 Fabrice Bellard
 * Copyright (c) 2015 Zhang Rui <bbcallen@gmail.com>
 *
 * This file is part of ijkPlayer.
 * Based on libavformat/allformats.c
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "libavformat/avformat.h"
#include "libavformat/url.h"
#include "libavformat/version.h"
#include "libavutil/mem.h"
#include "libavutil/error.h"
#include "libavutil/avstring.h"
#include "../ijkavutil/application.h"

/* ============================================================
 * FFmpeg 7 compatibility: av_application_* implementations
 * (Bilibili custom extensions – not compiled into standard FFmpeg)
 * ============================================================ */

int av_application_alloc(AVApplicationContext **ph, void *opaque)
{
    AVApplicationContext *h = av_mallocz(sizeof(AVApplicationContext));
    if (!h)
        return AVERROR(ENOMEM);
    h->opaque = opaque;
    *ph = h;
    return 0;
}

int av_application_open(AVApplicationContext **ph, void *opaque)
{
    return av_application_alloc(ph, opaque);
}

void av_application_close(AVApplicationContext *h)
{
    av_free(h);
}

void av_application_closep(AVApplicationContext **ph)
{
    if (ph && *ph) {
        av_application_close(*ph);
        *ph = NULL;
    }
}

void av_application_on_http_event(AVApplicationContext *h, int event_type, AVAppHttpEvent *event)
{
    if (h && h->func_on_app_event)
        h->func_on_app_event(h, event_type, event, sizeof(*event));
}

void av_application_will_http_open(AVApplicationContext *h, void *obj, const char *url)
{
    if (h && h->func_on_app_event) {
        AVAppHttpEvent event = { .obj = obj, .offset = 0, .error = 0, .http_code = 0, .filesize = 0 };
        av_strlcpy(event.url, url ? url : "", sizeof(event.url));
        h->func_on_app_event(h, AVAPP_EVENT_WILL_HTTP_OPEN, &event, sizeof(event));
    }
}

void av_application_did_http_open(AVApplicationContext *h, void *obj, const char *url, int error, int http_code, int64_t filesize)
{
    if (h && h->func_on_app_event) {
        AVAppHttpEvent event = { .obj = obj, .offset = 0, .error = error, .http_code = http_code, .filesize = filesize };
        av_strlcpy(event.url, url ? url : "", sizeof(event.url));
        h->func_on_app_event(h, AVAPP_EVENT_DID_HTTP_OPEN, &event, sizeof(event));
    }
}

void av_application_will_http_seek(AVApplicationContext *h, void *obj, const char *url, int64_t offset)
{
    if (h && h->func_on_app_event) {
        AVAppHttpEvent event = { .obj = obj, .offset = offset, .error = 0, .http_code = 0, .filesize = 0 };
        av_strlcpy(event.url, url ? url : "", sizeof(event.url));
        h->func_on_app_event(h, AVAPP_EVENT_WILL_HTTP_SEEK, &event, sizeof(event));
    }
}

void av_application_did_http_seek(AVApplicationContext *h, void *obj, const char *url, int64_t offset, int error, int http_code)
{
    if (h && h->func_on_app_event) {
        AVAppHttpEvent event = { .obj = obj, .offset = offset, .error = error, .http_code = http_code, .filesize = 0 };
        av_strlcpy(event.url, url ? url : "", sizeof(event.url));
        h->func_on_app_event(h, AVAPP_EVENT_DID_HTTP_SEEK, &event, sizeof(event));
    }
}

void av_application_did_io_tcp_read(AVApplicationContext *h, void *obj, int bytes)
{
    if (h && h->func_on_app_event) {
        AVAppIOTraffic event = { .obj = obj, .bytes = bytes };
        h->func_on_app_event(h, AVAPP_EVENT_IO_TRAFFIC, &event, sizeof(event));
    }
}

int av_application_on_io_control(AVApplicationContext *h, int event_type, AVAppIOControl *control)
{
    if (h && h->func_on_app_event)
        return h->func_on_app_event(h, event_type, control, sizeof(*control));
    return 0;
}

int av_application_on_tcp_will_open(AVApplicationContext *h)
{
    if (h && h->func_on_app_event) {
        AVAppTcpIOControl control = {0};
        return h->func_on_app_event(h, AVAPP_CTRL_WILL_TCP_OPEN, &control, sizeof(control));
    }
    return 0;
}

int av_application_on_tcp_did_open(AVApplicationContext *h, int error, int fd, AVAppTcpIOControl *control)
{
    if (h && h->func_on_app_event) {
        if (control) {
            control->error = error;
            control->fd    = fd;
        }
        return h->func_on_app_event(h, AVAPP_CTRL_DID_TCP_OPEN, control, sizeof(*control));
    }
    return 0;
}

void av_application_on_async_statistic(AVApplicationContext *h, AVAppAsyncStatistic *statistic)
{
    if (h && h->func_on_app_event)
        h->func_on_app_event(h, AVAPP_EVENT_ASYNC_STATISTIC, statistic, sizeof(*statistic));
}

void av_application_on_async_read_speed(AVApplicationContext *h, AVAppAsyncReadSpeed *speed)
{
    if (h && h->func_on_app_event)
        h->func_on_app_event(h, AVAPP_EVENT_ASYNC_READ_SPEED, speed, sizeof(*speed));
}

/* ============================================================
 * FFmpeg 7 compatibility: ijkav_register_*_protocol stubs
 * (Bilibili custom FFmpeg patched API – not in standard FFmpeg 7)
 * ============================================================ */

int ijkav_register_async_protocol(URLProtocol *protocol, int protocol_size)          { (void)protocol; (void)protocol_size; return 0; }
int ijkav_register_ijkio_protocol(URLProtocol *protocol, int protocol_size)          { (void)protocol; (void)protocol_size; return 0; }
int ijkav_register_ijklongurl_protocol(URLProtocol *protocol, int protocol_size)     { (void)protocol; (void)protocol_size; return 0; }
int ijkav_register_ijktcphook_protocol(URLProtocol *protocol, int protocol_size)     { (void)protocol; (void)protocol_size; return 0; }
int ijkav_register_ijkhttphook_protocol(URLProtocol *protocol, int protocol_size)    { (void)protocol; (void)protocol_size; return 0; }
int ijkav_register_ijksegment_protocol(URLProtocol *protocol, int protocol_size)     { (void)protocol; (void)protocol_size; return 0; }
#ifdef __ANDROID__
int ijkav_register_ijkmediadatasource_protocol(URLProtocol *protocol, int protocol_size) { (void)protocol; (void)protocol_size; return 0; }
#endif


/* ============================================================
 * FFmpeg 7 compatibility: ff_avc_parse_nal_units / ff_isom_write_avcc
 * (libavformat/avc.c not compiled into the FFmpeg 7 build)
 * ============================================================ */
#include "libavformat/avio.h"
#include "libavutil/intreadwrite.h"
#include <string.h>

const uint8_t *ff_avc_find_startcode(const uint8_t *p, const uint8_t *end)
{
    const uint8_t *a = p + 4 - ((intptr_t)p & 3);
    for (end -= 3; p < a && p < end; p++) {
        if (p[0] == 0 && p[1] == 0 && p[2] == 1)
            return p;
    }
    for (end += 3; p < end; p += 4) {
        uint32_t x = AV_RN32(p);
        if ((x - 0x01010101) & (~x) & 0x80808080) {
            if (p[1] == 0) {
                if (p[0] == 0 && p[2] == 1)
                    return p;
                if (p[2] == 0 && p[3] == 1)
                    return p + 1;
            }
            if (p[3] == 0) {
                if (p[2] == 0 && p[4] == 1)
                    return p + 2;
                if (p[4] == 0 && p[5] == 1)
                    return p + 3;
            }
        }
    }
    for (end += 3; p < end; p++) {
        if (p[0] == 0 && p[1] == 0 && p[2] == 1)
            return p;
    }
    return end + 3;
}

int ff_avc_parse_nal_units(AVIOContext *pb, const uint8_t *buf, int size)
{
    const uint8_t *p = buf;
    const uint8_t *end = p + size;
    const uint8_t *nal_start, *nal_end;

    nal_start = ff_avc_find_startcode(p, end);
    for (;;) {
        while (nal_start < end && !*(nal_start++));
        if (nal_start == end)
            break;
        nal_end = ff_avc_find_startcode(nal_start, end);
        avio_wb32(pb, (int)(nal_end - nal_start));
        avio_write(pb, nal_start, (int)(nal_end - nal_start));
        nal_start = nal_end;
    }
    return 0;
}

int ff_isom_write_avcc(AVIOContext *pb, const uint8_t *data, int len)
{
    /* If data is already in AVCC format (not Annex B), write directly */
    if (len > 6 && data[0] == 1) {
        /* looks like AVCC extradata already */
        avio_write(pb, data, len);
        return 0;
    }
    /* Convert Annex B to AVCC */
    const uint8_t *p = data, *end = data + len;
    const uint8_t *sps = NULL, *pps = NULL;
    int sps_size = 0, pps_size = 0;
    const uint8_t *r;

    r = ff_avc_find_startcode(p, end);
    while (r < end) {
        const uint8_t *r1;
        while (!*(r++));
        r1 = ff_avc_find_startcode(r, end);
        int nal_type = r[0] & 0x1f;
        if (nal_type == 7 && !sps) { /* SPS */
            sps = r;
            sps_size = (int)(r1 - r);
            /* trim trailing zeros */
            while (sps_size > 0 && sps[sps_size-1] == 0)
                sps_size--;
        } else if (nal_type == 8 && !pps) { /* PPS */
            pps = r;
            pps_size = (int)(r1 - r);
            while (pps_size > 0 && pps[pps_size-1] == 0)
                pps_size--;
        }
        r = r1;
    }
    if (!sps || sps_size < 4 || !pps)
        return AVERROR_INVALIDDATA;

    avio_w8(pb, 1);           /* version */
    avio_w8(pb, sps[1]);      /* profile */
    avio_w8(pb, sps[2]);      /* profile compat */
    avio_w8(pb, sps[3]);      /* level */
    avio_w8(pb, 0xff);        /* 6 bits reserved + 2 bits nal size - 1 */
    avio_w8(pb, 0xe1);        /* 3 bits reserved + 5 bits number of sps */
    avio_wb16(pb, sps_size);
    avio_write(pb, sps, sps_size);
    avio_w8(pb, 1);           /* number of pps */
    avio_wb16(pb, pps_size);
    avio_write(pb, pps, pps_size);
    return 0;
}


#define IJK_REGISTER_DEMUXER(x)                                         \
    {                                                                   \
        extern AVInputFormat ijkff_##x##_demuxer;                       \
        int ijkav_register_##x##_demuxer(AVInputFormat *demuxer, int demuxer_size);   \
        ijkav_register_##x##_demuxer(&ijkff_##x##_demuxer, sizeof(AVInputFormat));    \
    }

#define IJK_REGISTER_PROTOCOL(x)                                        \
    {                                                                   \
        extern URLProtocol ijkimp_ff_##x##_protocol;                        \
        int ijkav_register_##x##_protocol(URLProtocol *protocol, int protocol_size);\
        ijkav_register_##x##_protocol(&ijkimp_ff_##x##_protocol, sizeof(URLProtocol));  \
    }

static struct AVInputFormat *ijkav_find_input_format(const char *iformat_name)
{
    if (!iformat_name)
        return NULL;

#if LIBAVFORMAT_VERSION_MAJOR >= 58
    const AVInputFormat *fmt = NULL;
    void *opaque = NULL;
    while ((fmt = av_demuxer_iterate(&opaque))) {
        if (!fmt->name)
            continue;
        if (!strcmp(iformat_name, fmt->name))
            return (AVInputFormat *)fmt;
    }
#else
    AVInputFormat *fmt = NULL;
    while ((fmt = av_iformat_next(fmt))) {
        if (!fmt->name)
            continue;
        if (!strcmp(iformat_name, fmt->name))
            return fmt;
    }
#endif
    return NULL;
}

static void ijkav_register_input_format(AVInputFormat *iformat)
{
#if LIBAVFORMAT_VERSION_MAJOR >= 58
    (void)iformat;
    av_log(NULL, AV_LOG_WARNING, "register demuxer is disabled on modern FFmpeg API\n");
#else
    if (ijkav_find_input_format(iformat->name)) {
        av_log(NULL, AV_LOG_WARNING, "skip     demuxer : %s (duplicated)\n", iformat->name);
    } else {
        av_log(NULL, AV_LOG_INFO,    "register demuxer : %s\n", iformat->name);
        av_register_input_format(iformat);
    }
#endif
}


void ijkav_register_all(void)
{
    static int initialized;

    if (initialized)
        return;
    initialized = 1;

#if LIBAVFORMAT_VERSION_MAJOR < 58
    av_register_all();
#endif

    /* protocols */
    av_log(NULL, AV_LOG_INFO, "===== custom modules begin =====\n");
#ifdef __ANDROID__
    IJK_REGISTER_PROTOCOL(ijkmediadatasource);
#endif
    IJK_REGISTER_PROTOCOL(ijkio);
    IJK_REGISTER_PROTOCOL(async);
    IJK_REGISTER_PROTOCOL(ijklongurl);
    IJK_REGISTER_PROTOCOL(ijktcphook);
    IJK_REGISTER_PROTOCOL(ijkhttphook);
    IJK_REGISTER_PROTOCOL(ijksegment);
    /* demuxers */
#if LIBAVFORMAT_VERSION_MAJOR < 60
    IJK_REGISTER_DEMUXER(ijklivehook);
    IJK_REGISTER_DEMUXER(ijklas);
#endif
    av_log(NULL, AV_LOG_INFO, "===== custom modules end =====\n");
}
