# Android.mk
# Usage: to build NDK shared library

# Convention
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := hdrnet_api

# Target Arch Info
#TARGET_PLATFORM := android-27
#TARGET_ARCH_ABI := arm64-v8a

# Include Files
MY_PROJECT_PATH := $(LOCAL_PATH)/../../../
LOCAL_C_INCLUDES := $(MY_PROJECT_PATH)/include/

# Src Files
SRC_DIR = $(MY_PROJECT_PATH)/src/
HDRNET_SRC = $(wildcard ${SRC_DIR}/hdrnet/*.cpp)
UTILS_SRC = $(wildcard ${SRC_DIR}/utils/*.cpp)
CNN_SRC = $(wildcard ${SRC_DIR}/cnn/*.cpp)
LOCAL_SRC_FILES := $(HDRNET_SRC) $(UTILS_SRC) $(CNN_SRC)

# Build Options
LOCAL_CFLAGS += -std=c++11


# Needed Shared Libs
#LOCAL_LDLIBS := 

include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_STATIC_LIBRARY)
#include $(BUILD_EXECUTABLE)