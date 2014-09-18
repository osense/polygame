#ifndef OBJECTITEM_H
#define OBJECTITEM_H

#include "Object.h"

enum E_ITEM_STATE
{
    EIS_ITEM,
    EIS_EFFECT_FADEIN,
    EIS_EFFECT,
    EIS_EFFECT_FADEOUT
};

class ObjectItem : public Object
{
public:
    ObjectItem(SContext* cont);

protected:
    E_ITEM_STATE State;
};

#endif // OBJECTITEM_H
