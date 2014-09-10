#include "ObjectWakeLock.h"

ObjectWakeLock::ObjectWakeLock(SContext* cont) : Object(cont)
{
    Name = "ObjectWakeLock";
    setPersistent(true);
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));


    JNIEnv* jni = 0;
    Context->App->activity->vm->AttachCurrentThread(&jni, NULL);
    if (jni)
    {
        jclass classNativeActivity = jni->FindClass("android/app/NativeActivity");
        jclass classPowerManager = jni->FindClass("android/os/PowerManager");
        if (classPowerManager)
        {
            jmethodID idNativeActivity_getAppContext = jni->GetMethodID(classNativeActivity, "getApplicationContext", "()Landroid/content/Context;");

            jobject AppContext = jni->CallObjectMethod(Context->App->activity->clazz, idNativeActivity_getAppContext);
            if (AppContext)
            {
                jmethodID idNativeActivity_getSystemService = jni->GetMethodID(classNativeActivity, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
                jstring POWER_SERVICE = jni->NewStringUTF("power");
                jobject PowerManager = jni->CallObjectMethod(AppContext, idNativeActivity_getSystemService, POWER_SERVICE);
                if (PowerManager)
                {
                    jmethodID idPowerManager_newWakeLock = jni->GetMethodID(classPowerManager, "newWakeLock", "(ILjava/lang/String;)Landroid/os/PowerManager$WakeLock;");
                    int SCREEN_BRIGHT_WAKE_LOCK = 0x0000000a;
                    long int ON_AFTER_RELEASE = 0x20000000;
                    jstring jWakeLockTag = jni->NewStringUTF("com.entity.Polygame.MY_BRIGHT_SCREEN_LOCK_LOL");

                    WakeLock = jni->CallObjectMethod(PowerManager, idPowerManager_newWakeLock, SCREEN_BRIGHT_WAKE_LOCK | ON_AFTER_RELEASE, jWakeLockTag);
                    debugLog("constructed wake lock");

                    jni->DeleteLocalRef(jWakeLockTag);
                }

                jni->DeleteLocalRef(POWER_SERVICE);
            }

        }

        Context->App->activity->vm->DetachCurrentThread();
    }
}

ObjectWakeLock::~ObjectWakeLock()
{
    setLocked(false);

    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_DIED));
}

void ObjectWakeLock::onMessage(SMessage msg)
{
    if (msg.Type == EMT_WAKE_LOCK)
    {
        setLocked(true);
    }
    else if (msg.Type == EMT_WAKE_UNLOCK)
    {
        setLocked(false);
    }
}


void ObjectWakeLock::setLocked(bool locked)
{
    if (Active == locked)
    {
        return;
    }

    JNIEnv* jni = 0;
    Context->App->activity->vm->AttachCurrentThread(&jni, NULL);
    if (jni)
    {
        jclass wakeLock = jni->FindClass("android/os/PowerManager$WakeLock");

        if (locked)
        {
            jmethodID idWakeLock_acquire = jni->GetMethodID(wakeLock, "acquire", "()V");

            jni->CallVoidMethod(WakeLock, idWakeLock_acquire);

            debugLog("acquired wake lock");
        }
        else
        {
            jmethodID idWakeLock_release = jni->GetMethodID(wakeLock, "release", "()V");

            jni->CallVoidMethod(WakeLock, idWakeLock_release);

            debugLog("released wake lock");
        }

        Context->App->activity->vm->DetachCurrentThread();
        Active = locked;
    }
}
