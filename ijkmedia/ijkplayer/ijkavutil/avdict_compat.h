/*
 * avdict_compat.h
 *      Compatibility helpers for pointer values stored in AVDictionary.
 */

#ifndef IJKPLAYER__IJKAVUTIL__AVDICT_COMPAT_H
#define IJKPLAYER__IJKAVUTIL__AVDICT_COMPAT_H

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "libavutil/dict.h"

static inline int av_dict_set_intptr(AVDictionary **pm, const char *key, uintptr_t value, int flags)
{
    char valuestr[32];

    snprintf(valuestr, sizeof(valuestr), "%" PRIuPTR, value);
    return av_dict_set(pm, key, valuestr, flags);
}

static inline uintptr_t av_dict_strtoptr(const char *value)
{
    if (!value || !*value)
        return 0;

    return (uintptr_t)strtoull(value, NULL, 10);
}

#endif
