#ifndef OBJECTWAKELOCK_H
#define OBJECTWAKELOCK_H

#include <android_native_app_glue.h>

#include "Object.h"
#include "ObjectManager.h"


class ObjectWakeLock : public Object
{
    public:
        ObjectWakeLock(SContext* cont);

        virtual ~ObjectWakeLock();

        virtual void onMessage(SMessage msg);

    private:
        jobject WakeLock;
        bool Active = false;

        void setLocked(bool locked);
};

#endif // OBJECTWAKELOCK_H
