#ifndef OBJECTTRACER_H
#define OBJECTTRACER_H

#include <irrlicht.h>
#include <vector>
#include "json/value.h"
#include "Object.h"
#include "ObjectManager.h"

using namespace irr;


class ObjectTracer : public Object
{
    public:
        ObjectTracer(SContext* cont, Json::Value& jsonData);
        virtual ~ObjectTracer();

        virtual void onMessage(SMessage msg);

    private:
        std::vector<core::vector3df> Positions;
};

#endif // OBJECTTRACER_H
