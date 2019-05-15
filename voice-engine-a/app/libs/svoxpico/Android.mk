# Build Base Generic SVOX Pico Library

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := libsvoxpico
LOCAL_MULTILIB := 32

LOCAL_SRC_FILES := \
	picoacph.c \
	picoapi.c \
	picobase.c \
	picocep.c \
	picoctrl.c \
	picodata.c \
	picodbg.c \
	picoextapi.c \
	picofftsg.c \
	picokdbg.c \
	picokdt.c \
	picokfst.c \
	picoklex.c \
	picoknow.c \
	picokpdf.c \
	picokpr.c \
	picoktab.c \
	picoos.c \
	picopal.c \
	picopam.c \
	picopr.c \
	picorsrc.c \
	picosa.c \
	picosig.c \
	picosig2.c \
	picospho.c \
	picotok.c \
	picotrns.c \
	picowa.c

LOCAL_CFLAGS+= $(TOOL_CFLAGS)
LOCAL_LDFLAGS+= $(TOOL_LDFLAGS)

LOCAL_CFLAGS += \
    -Wall -Werror \
    -Wno-error=infinite-recursion \
    -Wno-parentheses-equality \
    -Wno-self-assign \
    -Wno-sign-compare \
    -Wno-unneeded-internal-declaration \
    -Wno-unused-parameter \

include $(BUILD_STATIC_LIBRARY)




