call ndk-build
call ant debug
call adb -d uninstall com.irrlicht.irrgame
call adb -d install bin\IrrGame-debug.apk