#ifndef OBJECTTRACERRECORDER_H
#define OBJECTTRACERRECORDER_H

#include "Object.h"
#include "ObjectManager.h"
#include "Settings.h"
#include "irrlicht.h"
#include "json/value.h"
#include "functions.h"
#include <vector>
#include <ctime>

using namespace irr;


class ObjectTracerRecorder : public Object
{
public:
    ObjectTracerRecorder(SContext* cont);
    virtual ~ObjectTracerRecorder();

    virtual void onMessage(SMessage msg);

private:
    std::vector<core::vector3df> Positions;

    void writeToStorage() const;

    static constexpr f32 RecordEvery = 1;
};

#endif // OBJECTTRACERRECORDER_H
