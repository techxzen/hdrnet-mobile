# Android.mk
# Usage: to build NDK shared library

# Convention
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := main

# Target Arch Info
#TARGET_PLATFORM := android-27
#TARGET_ARCH_ABI := arm64-v8a

# Include Files
MY_PROJECT_PATH := $(LOCAL_PATH)/../../../
LOCAL_C_INCLUDES := $(MY_PROJECT_PATH)/include/

# Src Files
LOCAL_SRC_FILES := $(MY_PROJECT_PATH)/bin/main.cpp

# Build Options
LOCAL_CFLAGS += -std=c++11


# Needed Shared Libs
LOCAL_LDLIBS := -L$(MY_PROJECT_PATH)/ndk/dll/libs/arm64-v8a/ -lhdrnet_api


include $(BUILD_EXECUTABLE)