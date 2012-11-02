# -----------------------------------
# Build static library libmuriel.a 
# -----------------------------------

# Clear all LOCAL_XXX variables
include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm

muriel_src := \
	../muriel/src/Url.cpp \
	../muriel/src/ByteRange.cpp

local_c_includes := $(LOCAL_PATH)/../muriel/include 
	
LOCAL_SRC_FILES := $(muriel_src)
LOCAL_MODULE := libmuriel

RUNTIME_STL_PATH :=$(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++

LOCAL_C_INCLUDES += $(RUNTIME_STL_PATH)/include $(RUNTIME_STL_PATH)/libs/$(TARGET_ARCH_ABI)/include 

LOCAL_C_INCLUDES += $(local_c_includes)

include $(BUILD_STATIC_LIBRARY)
