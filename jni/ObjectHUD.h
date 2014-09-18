#ifndef OBJECTHUD_H
#define OBJECTHUD_H

#include "Object.h"
#include "ObjectManager.h"
#include "ObjectPlayer.h"
#include "functions.h"


class ObjectHUD : public Object
{
public:
    ObjectHUD(SContext* cont);
    virtual ~ObjectHUD();

    virtual void onMessage(SMessage msg);

private:
    video::SColor HUDColor;

    gui::IGUIStaticText* DistTravelledText;
    f32 DT = 0;
};

#endif // OBJECTHUD_H
