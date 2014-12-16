#ifndef IGUIELEMENTLINE_H
#define IGUIELEMENTLINE_H

#include <irrlicht.h>

using namespace irr;

class IGUIElementLine :public gui::IGUIElement
{
public:
    IGUIElementLine(gui::IGUIEnvironment *environment, gui::IGUIElement *parent, s32 id, const core::rect<s32> &rectangle, video::ITexture* tex, bool vertical = false);

    ~IGUIElementLine();

    void draw();

private:
    video::ITexture* Texture;
    core::array<core::position2d< s32 > > LinePositions;
    core::array<core::rect< s32 > > LineRects;
};

#endif // IGUIELEMENTLINE_H
