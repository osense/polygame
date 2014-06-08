~/dev/android-ndk-r9d/ndk-build
ant debug
adb -d uninstall com.entity.polygame
adb -d install bin/Polygame-debug.apk