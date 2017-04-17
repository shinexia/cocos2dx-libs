LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := commons_static

LOCAL_MODULE_FILENAME := libcommon

#traverse all the directory and subdirectory
define walk
  $(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))
endef

#traverse Classes Directory
TRAVERSE_CLASSES = $(call walk, $(LOCAL_PATH)/commons)

FILE_LIST := $(filter %.cpp, $(TRAVERSE_CLASSES))
FILE_LIST += $(filter %.hpp, $(TRAVERSE_CLASSES))

#source file will be compiled
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

LOCAL_C_INCLUDES := $(LOCAL_PATH)

LOCAL_STATIC_LIBRARIES := libboost_armeabi
LOCAL_STATIC_LIBRARIES += cocos2dx_static

include $(BUILD_STATIC_LIBRARY)

$(call import-module, boost4android)
