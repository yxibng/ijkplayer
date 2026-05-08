/*
 * ff_ffinc.h
 *      ffmpeg headers
 *
 * Copyright (c) 2013 Bilibili
 * Copyright (c) 2013 Zhang Rui <bbcallen@gmail.com>
 *
 * This file is part of ijkPlayer.
 *
 * ijkPlayer is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * ijkPlayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with ijkPlayer; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef FFPLAY__FF_FFINC_H
#define FFPLAY__FF_FFINC_H

#include <stdbool.h>
#include <assert.h>
#include "libavutil/avstring.h"
#include "libavutil/time.h"
#include "libavformat/avformat.h"
#include "libavcodec/avfft.h"
#include "libswscale/swscale.h"
#include "ijkavutil/application.h"
#include "libavutil/base64.h"
#include "libavutil/channel_layout.h"
#include "libavutil/error.h"
#include "libavutil/opt.h"
#include "libavutil/version.h"
#include "libswresample/swresample.h"

#include "ijkavutil/avdict_compat.h"
#include "ijksdl/ijksdl.h"

#if LIBAVCODEC_VERSION_MAJOR >= 59
static inline int av_copy_packet(AVPacket *dst, const AVPacket *src)
{
    return av_packet_ref(dst, src);
}

static inline int avcodec_decode_video2(AVCodecContext *avctx, AVFrame *frame, int *got_picture_ptr, const AVPacket *avpkt)
{
    int ret;

    if (got_picture_ptr)
        *got_picture_ptr = 0;

    ret = avcodec_send_packet(avctx, avpkt);
    if (ret < 0 && ret != AVERROR(EAGAIN))
        return ret;

    ret = avcodec_receive_frame(avctx, frame);
    if (ret == 0) {
        if (got_picture_ptr)
            *got_picture_ptr = 1;
        return avpkt ? avpkt->size : 0;
    }

    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
        return avpkt ? avpkt->size : 0;

    return ret;
}

static inline int av_get_channel_layout_nb_channels(uint64_t channel_layout)
{
    AVChannelLayout ch_layout;
    int nb_channels = 0;

    if (av_channel_layout_from_mask(&ch_layout, channel_layout) < 0)
        return 0;

    nb_channels = ch_layout.nb_channels;
    av_channel_layout_uninit(&ch_layout);
    return nb_channels;
}

static inline int64_t av_get_default_channel_layout(int nb_channels)
{
    AVChannelLayout ch_layout;
    int64_t channel_layout = 0;

    av_channel_layout_default(&ch_layout, nb_channels);
    if (ch_layout.order == AV_CHANNEL_ORDER_NATIVE)
        channel_layout = (int64_t)ch_layout.u.mask;

    av_channel_layout_uninit(&ch_layout);
    return channel_layout;
}

static inline void av_codec_set_pkt_timebase(AVCodecContext *avctx, AVRational val)
{
    avctx->pkt_timebase = val;
}

static inline int av_codec_get_max_lowres(const AVCodec *codec)
{
    return codec ? codec->max_lowres : 0;
}

static inline void av_codec_set_lowres(AVCodecContext *avctx, int lowres)
{
    avctx->lowres = lowres;
}
#endif

#ifndef AV_PKT_FLAG_DISCONTINUITY
#define AV_PKT_FLAG_DISCONTINUITY 0x0100
#endif
#ifndef AV_PKT_FLAG_NEW_SEG
#define AV_PKT_FLAG_NEW_SEG AV_PKT_FLAG_DISCONTINUITY
#endif

#ifndef av_packet_split_side_data
static inline int av_packet_split_side_data(AVPacket *pkt)
{
    (void)pkt;
    return 0;
}
#endif

typedef int (*ijk_inject_callback)(void *opaque, int type, void *data, size_t data_size);

#define FFP_OPT_CATEGORY_FORMAT 1
#define FFP_OPT_CATEGORY_CODEC  2
#define FFP_OPT_CATEGORY_SWS    3
#define FFP_OPT_CATEGORY_PLAYER 4
#define FFP_OPT_CATEGORY_SWR    5

/* FFmpeg 7 compatibility: ff_avc_parse_nal_units / ff_avc_find_startcode
 * were removed from public avc.h; implementations live in allformats.c. */
#include "libavformat/avio.h"
const uint8_t *ff_avc_find_startcode(const uint8_t *p, const uint8_t *end);
int ff_avc_parse_nal_units(AVIOContext *pb, const uint8_t *buf_in, int size);

#endif
