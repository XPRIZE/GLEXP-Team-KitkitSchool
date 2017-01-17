LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

CLASSES := $(wildcard $(LOCAL_PATH)/../../Classes/*.cpp)
CLASSES += $(wildcard $(LOCAL_PATH)/../../Classes/**/*.cpp)
CLASSES += $(wildcard $(LOCAL_PATH)/../../Classes/**/*.c)
CLASSES += $(wildcard $(LOCAL_PATH)/../../Classes/**/**/*.cpp)
CLASSES += $(wildcard $(LOCAL_PATH)/../../Classes/**/**/**/*.cpp)
CLASSES := $(CLASSES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES := hellocpp/main.cpp
LOCAL_SRC_FILES += $(CLASSES)


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
