#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include <irrlicht.h>
#include "SContext.h"

using namespace irr;
using namespace core;

inline void scaleGUIPos(position2d<s32> &p, vector2df gscale)
{
    p.X *= gscale.X;
    p.Y *= gscale.Y;
}

inline void scaleGUIDim(dimension2d<s32> &p, vector2df gscale)
{
    p.Width *= gscale.X;
    p.Height *= gscale.Y;
}

inline void scaleGUIRect(rect<s32> &r, vector2df gscale)
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

inline gui::IGUIButton* addButton(position2d<s32> pos, dimension2d<s32> size, core::stringw text, SContext* cont, s32 id = -1, gui::IGUIElement* parent = 0)
{
    scaleGUIPos(pos, cont->GUIScale);
    scaleGUIDim(size, cont->GUIScale);
    gui::IGUIButton* btn = cont->Device->getGUIEnvironment()->addButton(rect<s32>(pos, size), parent, id);
    btn->setDrawBorder(false);
    btn->setText(text.c_str());

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

/** Converts an RGB color to it's HSV representation
 *  @param rgb RGB color to convert
 *  @return The HSV encoded color, as an irr::core::vector3df where X is Hue [0, 360); Y is Saturation [0, 1]; Z is Value [0, 1].
 */
inline core::vector3df RGBftoHSV(video::SColorf rgb)
{
    core::vector3df hsv;

    f32 M = max_(max_(rgb.getRed(), rgb.getGreen()), max_(rgb.getGreen(), rgb.getBlue()));
    f32 m = min_(min_(rgb.getRed(), rgb.getGreen()), min_(rgb.getGreen(), rgb.getBlue()));
    f32 C = M - m;

    if (C == 0)
        hsv.X = 0;
    else if (M <= rgb.getRed())
        hsv.X = ((rgb.getGreen() - rgb.getBlue()) / C);
    else if (M <= rgb.getGreen())
        hsv.X = ((rgb.getBlue() - rgb.getRed()) / C) + 2;
    else if (M <= rgb.getBlue())
        hsv.X = ((rgb.getRed() - rgb.getGreen()) / C) + 4;

    hsv.X *= 60;
    if (hsv.X < 0)
        hsv.X += 360;

    hsv.Y = M;

    if (hsv.Y == 0)
        hsv.Z = 0;
    else
        hsv.Z = C / hsv.Y;

    return hsv;
}

inline core::vector3df RGBtoHSV(video::SColor rgb)
{
    return RGBftoHSV(video::SColorf(rgb));
}

inline video::SColorf HSVtoRGBf(core::vector3df hsv)
{
    video::SColorf rgb;

    f32 C = hsv.Z * hsv.Y;
    f32 H = hsv.X / 60;
    f32 Hmod = H - (2 * (int(H) / 2)); // same as H = fmod(H, 2)
    f32 X = C * (1 - abs_(Hmod - 1));

    if (H < 1)
        rgb = video::SColorf(C, X, 0);
    else if (H < 2)
        rgb = video::SColorf(X, C, 0);
    else if (H < 3)
        rgb = video::SColorf(0, C, X);
    else if (H < 4)
        rgb = video::SColorf(0, X, C);
    else if (H < 5)
        rgb = video::SColorf(X, 0, C);
    else if (H < 6)
        rgb = video::SColorf(C, 0, X);

    f32 m = hsv.Z - C;
    rgb.r += m;
    rgb.g += m;
    rgb.b += m;

    return rgb;
}

inline video::SColor HSVtoRGB(core::vector3df hsv)
{
    return HSVtoRGBf(hsv).toSColor();
}

inline video::SColorf hueShift(video::SColorf col, f32 shift)
{
    f32 a = col.a;

    core::vector3df hsvCol = RGBftoHSV(col);
    hsvCol.X += shift;
    while (hsvCol.X >= 360)
        hsvCol.X -= 360;
    while (hsvCol.X < 0)
        hsvCol += 360;

    video::SColorf comp = HSVtoRGBf(hsvCol);
    comp.a = a;
    return comp;
}

inline video::SColor hueShift(video::SColor col, f32 shift)
{
    return hueShift(video::SColorf(col), shift).toSColor();
}


#endif // FUNCTIONS_H_INCLUDED
