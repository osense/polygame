#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include <irrlicht.h>
#include "SContext.h"

using namespace irr;
using namespace core;

/*inline void scaleGUIPos(position2d<s32> &p, f32 gscale)
{
    p.X = p.X * gscale.X;
    p.Y = p.Y * gscale.Y;
}*/

inline void scaleGUIRect(rect<s32> &r, f32 gscale)
{
    r.LowerRightCorner *= gscale;
    r.UpperLeftCorner *= gscale;
}

inline gui::IGUIButton* addButton(rect<s32> rect, video::ITexture* tex, SContext* cont, s32 id = -1, gui::IGUIElement* parent = 0)
{
    scaleGUIRect(rect, cont->GUIScale);
    gui::IGUIButton* btn = cont->Device->getGUIEnvironment()->addButton(rect, parent, id);
    btn->setDrawBorder(false);
    btn->setImage(tex);
    btn->setScaleImage(true);
    btn->setUseAlphaChannel(true);

    return btn;
}


inline void clamp(f32 &in, f32 min, f32 max)
{
    in = in > max ? max : in < min ? min : in;
}

inline video::SColorf addColors(video::SColorf a, video::SColorf b)
{
    a.r += b.r;
    a.g += b.g;
    a.b += b.b;

    return a;
}


#endif // FUNCTIONS_H_INCLUDED
