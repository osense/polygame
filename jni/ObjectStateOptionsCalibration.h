#ifndef OBJECTSTATEOPTIONSCALIBRATION_H
#define OBJECTSTATEOPTIONSCALIBRATION_H

#include "Object.h"
#include "ObjectManager.h"
#include "Settings.h"
#include "functions.h"
#include <irrlicht.h>
#include <vector>

using namespace irr;


class ObjectStateOptionsCalibration : public Object
{
public:
    
    ObjectStateOptionsCalibration(SContext* cont);
    
    ~ObjectStateOptionsCalibration();
    
    void onMessage(SMessage msg);
    
private:
    void createGUI();
    
    std::vector<f32> Samples;
    gui::IGUIWindow* Window;
    f32 TimeElapsed = 0;
    
    static constexpr f32 CalibrateTime = 3.5;
};

#endif