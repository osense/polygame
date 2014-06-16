LOCAL_PATH:=$(call my-dir)/..
IRRLICHT_PROJECT_PATH:=$(LOCAL_PATH)

include $(CLEAR_VARS)
LOCAL_MODULE:=Irrlicht
LOCAL_SRC_FILES:=../irrlicht-ogl-es/lib/Android/libIrrlicht.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE:=Polygame

LOCAL_CFLAGS:=-D_IRR_ANDROID_PLATFORM_ -pipe -fno-exceptions -fno-rtti -fstrict-aliasing

ifndef NDEBUG
LOCAL_CFLAGS+=-g -D_DEBUG
else
LOCAL_CFLAGS+=-fexpensive-optimizations -O3
endif

ifeq ($(TARGET_ARCH_ABI),x86)
LOCAL_CFLAGS+=-fno-stack-protector
endif

LOCAL_C_INCLUDES:=../irrlicht-ogl-es/include

LOCAL_SRC_FILES:=jni/main.cpp jni/ShaderCBSimple.cpp jni/ShaderCBDepth.cpp jni/Object.cpp jni/ObjectDebugInfo.cpp jni/ObjectEventReceiver.cpp jni/ObjectManager.cpp jni/ObjectStateGame.cpp jni/ObjectStateMenu.cpp jni/ObjectUpdater.cpp jni/ObjectGrid.cpp jni/ObjectPlayer.cpp
LOCAL_LDLIBS:=-lEGL -llog -lGLESv1_CM -lGLESv2 -lz -landroid

LOCAL_STATIC_LIBRARIES:=android_native_app_glue Irrlicht

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)