#ifndef OBJECTHUD_H
#define OBJECTHUD_H

#include "Object.h"
#include "ObjectManager.h"
#include "functions.h"


class ObjectHUD : public Object
{
    public:
        ObjectHUD(SContext* cont);
        virtual ~ObjectHUD();

        virtual void onMessage(SMessage msg);

    private:

        gui::IGUIStaticText* DistTravelledText;
        f32 DT;
};

#endif // OBJECTHUD_H
