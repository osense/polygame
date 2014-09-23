#include "ObjectStateOptionsCalibration.h"

ObjectStateOptionsCalibration::ObjectStateOptionsCalibration(SContext* cont) : Object(cont)
{
    Name = "ObjectStateOptionsCalibration";
    setPersistent(true);
    
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));
    
    Context->ObjManager->getObjectFromName("ObjectUpdater")->registerObserver(this);
    Context->ObjManager->getObjectFromName("ObjectEventReceiver")->registerObserver(this);
    
    Samples.reserve(512);
    createGUI();
}
    
ObjectStateOptionsCalibration::~ObjectStateOptionsCalibration()
{
    Window->remove();
    
    Object* obj = Context->ObjManager->getObjectFromName("ObjectUpdater");
    if (obj)
        obj->unregisterObserver(this);
    
    obj = Context->ObjManager->getObjectFromName("ObjectEventReceiver");
    if (obj)
        obj->unregisterObserver(this);
    
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_DIED));
}
    
void ObjectStateOptionsCalibration::onMessage(SMessage msg)
{
    if (msg.Type == EMT_UPDATE)
    {
        TimeElapsed += msg.Update.fDelta;
        
        if (TimeElapsed >= CalibrateTime)
        {
            u32 numSamples = Samples.size();
            
            if (numSamples > 0)
            {
                f64 meanAcc = 0;
                
                for (u32 i = 0; i < numSamples; i++)
                {
                    meanAcc += Samples[i] * (f64(i) / numSamples);
                }
                
                meanAcc = (meanAcc / f64(numSamples)) * 2.0;
                Context->Sets->AccelXBias -= meanAcc;
            }
            
            delete this;
            return;
        }
    }
    else if (msg.Type == EMT_ACC)
    {
        Samples.push_back(msg.Acc.X);
    }
}

void ObjectStateOptionsCalibration::createGUI()
{
    Window = addOverlayWindow(Context);
    Window->setDrawBackground(true);
    Window->setDrawTitlebar(false);
    
    addText(core::position2di(0, 160), core::dimension2di(840, 40), "CALIBRATING", Context, Window);
    addText(core::position2di(0, 220), core::dimension2di(840, 40), "PLEASE    HOLD  STILL", Context, Window);
}