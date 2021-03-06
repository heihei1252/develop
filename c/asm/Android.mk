LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    c-asm.c

LOCAL_CFLAGS += -Wall -Werror -Wunused -Wunreachable-code -Werror=unused-parameter

#LOCAL_SHARED_LIBRARIES := \
    libcutils \
    liblog \
    libandroidfw \
    libutils \
    libbinder \
    libui \
    libskia \
    libEGL \
    libGLESv1_CM \
    libgui \
    libtinyalsa

LOCAL_MODULE:= arm-asm-test

LOCAL_32_BIT_ONLY := true

include $(BUILD_EXECUTABLE)
