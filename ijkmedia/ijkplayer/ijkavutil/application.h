/*
 * application.h
 *   IJK application context – ijk-local replacement for Bilibili's
 *   patched libavutil/application.h (not in standard FFmpeg 7.x).
 *
 * Copyright (c) 2015 Bilibili
 * Copyright (c) 2015 Zhang Rui <bbcallen@gmail.com>
 *
 * This file is part of ijkPlayer.
 *
 * ijkPlayer is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 */

#ifndef IJKPLAYER__APPLICATION_H
#define IJKPLAYER__APPLICATION_H

#include <stdint.h>
#include "libavutil/mem.h"

/* ---- Event / Control type codes ---- */
#define AVAPP_EVENT_WILL_HTTP_OPEN              1 /* AVAppHttpEvent */
#define AVAPP_EVENT_DID_HTTP_OPEN               2 /* AVAppHttpEvent */
#define AVAPP_EVENT_WILL_HTTP_SEEK              3 /* AVAppHttpEvent */
#define AVAPP_EVENT_DID_HTTP_SEEK               4 /* AVAppHttpEvent */

#define AVAPP_CTRL_WILL_HTTP_OPEN               0x20001 /* AVAppIOControl */
#define AVAPP_CTRL_WILL_LIVE_OPEN               0x20002 /* AVAppIOControl */
#define AVAPP_CTRL_WILL_CONCAT_SEGMENT_OPEN     0x20003 /* AVAppIOControl */
#define AVAPP_CTRL_WILL_TCP_OPEN                0x20100 /* AVAppTcpIOControl */
#define AVAPP_CTRL_DID_TCP_OPEN                 0x20101 /* AVAppTcpIOControl */

#define AVAPP_EVENT_IO_TRAFFIC                  0x30000 /* AVAppIOTraffic */
#define AVAPP_EVENT_ASYNC_STATISTIC             0x40000 /* AVAppAsyncStatistic */
#define AVAPP_EVENT_ASYNC_READ_SPEED            0x40001 /* AVAppAsyncReadSpeed */

/* ---- HTTP event ---- */
typedef struct AVAppHttpEvent {
    void        *obj;
    char         url[4096];
    int64_t      offset;
    int          error;
    int          http_code;
    int64_t      filesize;
} AVAppHttpEvent;

/* ---- IO control (used for URL override hooks) ---- */
typedef struct AVAppIOControl {
    size_t       size;
    char         url[4096];
    int          segment_index;
    int          retry_counter;
    int          is_handled;
    int          is_url_changed;
} AVAppIOControl;

/* ---- TCP IO control ---- */
typedef struct AVAppTcpIOControl {
    int          error;
    int          family;
    char         ip[96];
    int          port;
    int          fd;
} AVAppTcpIOControl;

/* ---- IO traffic ---- */
typedef struct AVAppIOTraffic {
    void        *obj;
    int          bytes;
} AVAppIOTraffic;

/* ---- Async buffer statistics ---- */
typedef struct AVAppAsyncStatistic {
    size_t       size;
    int64_t      buf_forwards;
    int64_t      buf_backwards;
    int64_t      buf_capacity;
} AVAppAsyncStatistic;

/* ---- Async read speed ---- */
typedef struct AVAppAsyncReadSpeed {
    size_t       size;
    int          is_full_speed;
    int64_t      io_bytes;
    int64_t      elapsed_milli;
} AVAppAsyncReadSpeed;

/* ---- Application context ---- */
typedef struct AVApplicationContext AVApplicationContext;
typedef int (*ijk_app_func_on_event)(AVApplicationContext *h, int message, void *data, size_t data_size);

struct AVApplicationContext {
    void                      *opaque;
    ijk_app_func_on_event      func_on_app_event;
};

/* ---- API (implemented in allformats.c) ---- */
int  av_application_alloc(AVApplicationContext **ph, void *opaque);
int  av_application_open(AVApplicationContext **ph, void *opaque);
void av_application_close(AVApplicationContext *h);
void av_application_closep(AVApplicationContext **ph);

void av_application_on_http_event(AVApplicationContext *h, int event_type, AVAppHttpEvent *event);
void av_application_will_http_open(AVApplicationContext *h, void *obj, const char *url);
void av_application_did_http_open(AVApplicationContext *h, void *obj, const char *url, int error, int http_code, int64_t filesize);
void av_application_will_http_seek(AVApplicationContext *h, void *obj, const char *url, int64_t offset);
void av_application_did_http_seek(AVApplicationContext *h, void *obj, const char *url, int64_t offset, int error, int http_code);
void av_application_did_io_tcp_read(AVApplicationContext *h, void *obj, int bytes);
int  av_application_on_io_control(AVApplicationContext *h, int event_type, AVAppIOControl *control);
int  av_application_on_tcp_will_open(AVApplicationContext *h);
int  av_application_on_tcp_did_open(AVApplicationContext *h, int error, int fd, AVAppTcpIOControl *control);
void av_application_on_async_statistic(AVApplicationContext *h, AVAppAsyncStatistic *statistic);
void av_application_on_async_read_speed(AVApplicationContext *h, AVAppAsyncReadSpeed *speed);

#endif /* IJKPLAYER__APPLICATION_H */
