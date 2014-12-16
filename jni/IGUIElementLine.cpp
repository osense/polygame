#include "IGUIElementLine.h"

IGUIElementLine::IGUIElementLine(irr::gui::IGUIEnvironment* environment, irr::gui::IGUIElement* parent, irr::s32 id, const irr::core::rect< s32 >& rectangle, irr::video::ITexture* tex, bool vertical)
    :IGUIElement(irr::gui::EGUIET_ELEMENT, environment, parent, id, rectangle)
{
    Texture = tex;

    u32 start = vertical ? rectangle.UpperLeftCorner.Y : rectangle.UpperLeftCorner.X;
    u32 end = vertical ? rectangle.LowerRightCorner.Y : rectangle.LowerRightCorner.X;
    u32 mid = vertical ? rectangle.UpperLeftCorner.X : rectangle.UpperLeftCorner.Y;

    for (u32 i = start; i < end; i += Texture->getSize().Width-2)
    {
        LinePositions.push_back(vertical ? core::position2d<s32>(mid, i) : core::position2d<s32>(i, mid));
        LineRects.push_back(core::rect<s32>(core::vector2d<s32>(0, 0), Texture->getSize()));
    }
}

IGUIElementLine::~IGUIElementLine()
{

}

void IGUIElementLine::draw()
{
    if (isVisible())
    {
        Environment->getVideoDriver()->draw2DImageBatch(Texture, LinePositions, LineRects, 0, irr::video::SColor(255, 255, 255, 255), true);
    }
}