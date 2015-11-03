LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
ifeq (1,$(strip $(shell expr $(PLATFORM_VERSION) \>= 6.0)))
LOCAL_MODULE := libisp_silicomimageisp_api_6x
else
LOCAL_MODULE := libisp_silicomimageisp_api_5x
endif

ifeq ($(strip $(TARGET_BOARD_PLATFORM)), rk3368)
#include $(CLEAR_VARS)
ifneq (1,$(strip $(shell expr $(PLATFORM_VERSION) \>= 5.0)))
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)
else
ifneq ($(strip $(TARGET_2ND_ARCH)), )
LOCAL_MULTILIB := both
endif
LOCAL_MODULE_RELATIVE_PATH :=
endif
#LOCAL_MODULE := libisp_silicomimageisp_api
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_MODULE_STEM := $(LOCAL_MODULE)
LOCAL_SRC_FILES_arm := $(LOCAL_MODULE)_32bit.so
LOCAL_SRC_FILES_arm64 := $(LOCAL_MODULE)_64bit.so
LOCAL_MODULE_SUFFIX := .so
include $(BUILD_PREBUILT)
endif

ifeq ($(strip $(TARGET_BOARD_PLATFORM)),rk3288)
#include $(CLEAR_VARS)
#LOCAL_MODULE := libisp_silicomimageisp_api
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_MODULE_STEM := $(LOCAL_MODULE)
LOCAL_SRC_FILES := $(LOCAL_MODULE)_32bit.so
LOCAL_MODULE_SUFFIX := .so
include $(BUILD_PREBUILT)
endif

ifeq ($(strip $(TARGET_BOARD_PLATFORM)),rk312x)
#include $(CLEAR_VARS)
#LOCAL_MODULE := libisp_silicomimageisp_api
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_MODULE_STEM := $(LOCAL_MODULE)
LOCAL_SRC_FILES := $(LOCAL_MODULE)_32bit.so
LOCAL_MODULE_SUFFIX := .so
include $(BUILD_PREBUILT)
endif

ifeq ($(strip $(TARGET_BOARD_PLATFORM)),rk3036)
#include $(CLEAR_VARS)
#LOCAL_MODULE := libisp_silicomimageisp_api
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_MODULE_STEM := $(LOCAL_MODULE)
LOCAL_SRC_FILES := $(LOCAL_MODULE)_32bit.so
LOCAL_MODULE_SUFFIX := .so
include $(BUILD_PREBUILT)
endif

ifeq ($(strip $(TARGET_BOARD_PLATFORM)),rk3188)
#include $(CLEAR_VARS)
#LOCAL_MODULE := libisp_silicomimageisp_api
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_MODULE_STEM := $(LOCAL_MODULE)
LOCAL_SRC_FILES := $(LOCAL_MODULE)_32bit.so
LOCAL_MODULE_SUFFIX := .so
include $(BUILD_PREBUILT)
endif

