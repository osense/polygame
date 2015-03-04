LOCAL_PATH:=$(call my-dir)/..
IRRLICHT_PROJECT_PATH:=$(LOCAL_PATH)

include $(CLEAR_VARS)
LOCAL_MODULE:=Irrlicht
LOCAL_SRC_FILES:=../irrlicht/lib/Android/libIrrlicht.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE:=irrPP
LOCAL_SRC_FILES:=../irrpp/lib/android/libirrPP.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE:=Polygame

LOCAL_CFLAGS:=-D_IRR_ANDROID_PLATFORM_ -pipe -fno-exceptions -fno-rtti -fstrict-aliasing

ifndef NDEBUG
LOCAL_CFLAGS+=-fexceptions -g -D_DEBUG
else
LOCAL_CFLAGS+=-fexceptions -fexpensive-optimizations -O3
endif

ifeq ($(TARGET_ARCH_ABI),x86)
LOCAL_CFLAGS+=-fno-stack-protector
endif

LOCAL_C_INCLUDES:=../irrlicht-ogl-es/include ../irrpp/include jni

LOCAL_SRC_FILES:=jni/libnoise/noisegen.cpp jni/libnoise/perlin.cpp jni/json/json_value.cpp jni/json/json_reader.cpp jni/json/json_writer.cpp jni/main.cpp jni/Settings.cpp jni/EffectRenderer.cpp jni/EffectFader.cpp jni/ShaderCBGrid.cpp jni/ShaderCBAlpha.cpp jni/ShaderCBSolid.cpp jni/ShaderCBFader.cpp jni/ShaderCBSky.cpp jni/ShaderCBTracer.cpp jni/IGUIElementLine.cpp jni/GeometryGenerator.cpp jni/Object.cpp jni/ObjectDebugInfo.cpp jni/ObjectEventReceiver.cpp jni/ObjectManager.cpp jni/ObjectUpdater.cpp jni/ObjectUpdaterThreaded.cpp jni/ObjectStateInit.cpp jni/ObjectStateGame.cpp jni/ObjectStateMenu.cpp jni/ObjectStateOptions.cpp jni/ObjectStateOptionsCalibration.cpp jni/ObjectGrid.cpp jni/ObjectGridCinematicLines.cpp jni/GridGenerator.cpp jni/ObjectHUD.cpp jni/ObjectPlayer.cpp jni/ObjectItemSpawner.cpp jni/ObjectTracerRecorder.cpp jni/ObjectTracer.cpp jni/ObjectItem.cpp jni/ObjectItemCube.cpp jni/ObjectItemPyramid.cpp jni/ObjectSky.cpp jni/ObjectCinematicCubes.cpp
LOCAL_LDLIBS:=-lEGL -llog -lGLESv1_CM -lGLESv2 -lz -landroid

LOCAL_STATIC_LIBRARIES:=android_native_app_glue Irrlicht irrPP

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
