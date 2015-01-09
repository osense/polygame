~/dev/android-ndk-r10d/ndk-build -j 2
if [ $? -ne 0 ]; then
	echo "Build failed, aborting..."
	exit -1;
fi
ant debug
#adb -d uninstall com.entity.polygame
#adb -d install bin/Polygame-debug.apk
