#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include <irrlicht.h>
#include "SContext.h"

using namespace irr;
using namespace core;

inline void scaleGUIPos(position2d<s32> &p, f32 gscale)
{
    p.X *= gscale;
    p.Y *= gscale;
}

inline void scaleGUIDim(dimension2d<s32> &p, f32 gscale)
{
    p.Width *= gscale;
    p.Height *= gscale;
}

inline void scaleGUIRect(rect<s32> &r, f32 gscale)
{
    scaleGUIPos(r.LowerRightCorner, gscale);
    scaleGUIPos(r.UpperLeftCorner, gscale);
}

inline gui::IGUIButton* addButton(position2d<s32> pos, dimension2d<s32> size, video::ITexture* tex, SContext* cont, s32 id = -1, gui::IGUIElement* parent = 0)
{
    scaleGUIPos(pos, cont->GUIScale);
    scaleGUIDim(size, cont->GUIScale);
    gui::IGUIButton* btn = cont->Device->getGUIEnvironment()->addButton(rect<s32>(pos, size), parent, id);
    btn->setDrawBorder(false);
    btn->setImage(tex);
    btn->setScaleImage(true);
    btn->setUseAlphaChannel(true);

    return btn;
}

inline gui::IGUIWindow* addOverlayWindow(SContext* cont)
{
    gui::IGUIEnvironment* gui = cont->Device->getGUIEnvironment();
    position2d<s32> screenSize;
    screenSize.X = cont->Device->getVideoDriver()->getScreenSize().Width;
    screenSize.Y = cont->Device->getVideoDriver()->getScreenSize().Height;

    gui::IGUIWindow* wnd = gui->addWindow(rect<s32>(position2d<s32>(0, 0), screenSize));
    wnd->setDraggable(false);
    wnd->setDrawBackground(false);
    wnd->getCloseButton()->remove();

    return wnd;
}


inline void clamp(f32 &in, f32 min, f32 max)
{
    in = in > max ? max : in < min ? min : in;
}

inline s32 signum(f32 val) {
    return (0 < val) - (val < 0);
}

inline video::SColorf addColors(video::SColorf a, video::SColorf b)
{
    a.r += b.r;
    a.g += b.g;
    a.b += b.b;

    return a;
}


#endif // FUNCTIONS_H_INCLUDED
