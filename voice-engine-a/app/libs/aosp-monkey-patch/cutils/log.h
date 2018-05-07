// NB(xenosoz, 2018): Fake log.h to make jobs done.

#pragma once

#define LOG_ALWAYS_FATAL_IF(...) 0

#ifndef ALOGE
#define ALOGE(...) 0
#endif

#ifndef ALOGW
#define ALOGW(...) 0
#endif

#ifndef ALOGD
#define ALOGD(...) 0
#endif

#ifndef ALOG
#define ALOG(...) 0
#endif

#ifndef __android_log_print
#define __android_log_print(...) 0
#endif

#ifndef __android_log_write
#define __android_log_write(...) 0
#endif
