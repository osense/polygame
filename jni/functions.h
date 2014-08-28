#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include <irrlicht.h>
#include <ctime>
#include "circular_buffer.h"
#include "SContext.h"
#include "json/json.h"

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

inline gui::IGUIFont* getFont(SContext* cont)
{
    gui::IGUIFont* font = 0;

    switch(cont->ScreenSize)
    {
    case ESS_SMALL:
        font = cont->Device->getGUIEnvironment()->getFont("getvoip_18.xml");
        break;
    case ESS_NORMAL:
        font = cont->Device->getGUIEnvironment()->getFont("getvoip_20.xml");
        break;
    case ESS_LARGE:
        font = cont->Device->getGUIEnvironment()->getFont("getvoip_28.xml");
        break;
    case ESS_XLARGE:
        font = cont->Device->getGUIEnvironment()->getFont("getvoip_36.xml");
        break;
    }

    return font;
}

inline gui::IGUIFont* getOverlayFont(SContext* cont)
{
    gui::IGUIFont* font = 0;

    switch(cont->ScreenSize)
    {
    case ESS_SMALL:
        font = cont->Device->getGUIEnvironment()->getFont("douar_20.xml");
        break;
    case ESS_NORMAL:
        font = cont->Device->getGUIEnvironment()->getFont("douar_22.xml");
        break;
    case ESS_LARGE:
        font = cont->Device->getGUIEnvironment()->getFont("douar_36.xml");
        break;
    case ESS_XLARGE:
        font = cont->Device->getGUIEnvironment()->getFont("douar_48.xml");
        break;
    }

    return font;
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

inline gui::IGUIStaticText* addText(position2d<s32> pos, dimension2d<s32> size, core::stringw text, SContext* cont, gui::IGUIElement* parent = 0, gui::EGUI_ALIGNMENT al = gui::EGUIA_CENTER)
{
    scaleGUIPos(pos, cont->GUIScale);
    scaleGUIDim(size, cont->GUIScale);
    gui::IGUIStaticText* staticText = cont->Device->getGUIEnvironment()->addStaticText(text.c_str(), rect<s32>(pos, size), false, false, parent, -1);
    staticText->setOverrideFont(getOverlayFont(cont));
    staticText->setTextAlignment(al, gui::EGUIA_CENTER);

    return staticText;
}

inline gui::IGUIWindow* addOverlayWindow(SContext* cont, f32 startYVolume = 1, f32 endYVolume = 1)
{
    gui::IGUIEnvironment* gui = cont->Device->getGUIEnvironment();
    position2d<s32> screenSize;
    screenSize.X = cont->Device->getVideoDriver()->getScreenSize().Width;
    screenSize.Y = cont->Device->getVideoDriver()->getScreenSize().Height;
    position2d<s32> startPos(0, screenSize.Y - screenSize.Y*startYVolume);
    position2d<s32> endPos(screenSize.X, screenSize.Y*endYVolume);

    gui::IGUIWindow* wnd = gui->addWindow(rect<s32>(startPos, endPos));
    wnd->setDraggable(false);
    wnd->setDrawBackground(false);
    wnd->getCloseButton()->remove();

    return wnd;
}


inline void clamp(f32 &in, f32 min, f32 max)
{
    in = in > max ? max : in < min ? min : in;
}

inline s32 signum(f32 val)
{
    return (0 < val) - (val < 0);
}

inline vector3df interpCR(vector3df p0, vector3df p1, vector3df p2, vector3df p3, f32 posZ)
{
   vector3df a0, a1, a2, a3;
   f32 t = (posZ - p1.Z) / (p2.Z - p1.Z);
   f32 t2 = t * t;
   /*a0 = y3 - y2 - y0 + y1;
   a1 = y0 - y1 - a0;
   a2 = y2 - y0;
   a3 = y1;*/
   a0 = -0.5*p0 + 1.5*p1 - 1.5*p2 + 0.5*p3;
   a1 = p0 - 2.5*p1 + 2*p2 - 0.5*p3;
   a2 = -0.5*p0 + 0.5*p2;
   a3 = p1;

   return (a0*t*t2 + a1*t2 + a2*t + a3);
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
        hsvCol.X += 360;

    video::SColorf comp = HSVtoRGBf(hsvCol);
    comp.a = a;
    return comp;
}

inline video::SColor hueShift(video::SColor col, f32 shift)
{
    return hueShift(video::SColorf(col), shift).toSColor();
}

inline void writeJson(SContext* cont, Json::Value& root, core::stringc path)
{
    Json::StyledWriter jsonWriter;
    io::IWriteFile* file = cont->Device->getFileSystem()->createAndWriteFile(path.c_str());

    std::string jsonStr = jsonWriter.write(root);

    file->write(jsonStr.c_str(), jsonStr.length());

    file->drop();
}

inline Json::Value readJson(SContext* cont, core::stringc path)
{
    Json::Value root;
    io::IReadFile* file = cont->Device->getFileSystem()->createAndOpenFile(path.c_str());
    if (!file)
        return root;

    char* buff = new char[file->getSize()];
    file->read(buff, file->getSize());
    buff[file->getSize()-1] = '\0';
    file->drop();
    std::string jsonStr(buff);

    Json::Reader reader;
    reader.parse(jsonStr, root);

    delete[] buff;
    return root;
}

inline void storeVector3df(const core::vector3df& vec, Json::Value& val, const core::stringc name)
{
    Json::Value vecVal;
    vecVal.append(vec.X);
    vecVal.append(vec.Y);
    vecVal.append(vec.Z);
    val[name.c_str()] = vecVal;
}

inline Json::Value storeVector3df(const core::vector3df& vec)
{
    Json::Value vecVal;
    vecVal.append(vec.X);
    vecVal.append(vec.Y);
    vecVal.append(vec.Z);
    return vecVal;
}

inline core::vector3df parseVector3df(Json::Value& val, const core::stringc name)
{
    core::vector3df vec;
    vec.X = val[name.c_str()][0].asDouble();
    vec.Y = val[name.c_str()][1].asDouble();
    vec.Z = val[name.c_str()][2].asDouble();

    return vec;
}

inline core::vector3df parseVector3df(Json::Value& val)
{
    core::vector3df vec;
    vec.X = val[0].asDouble();
    vec.Y = val[1].asDouble();
    vec.Z = val[2].asDouble();

    return vec;
}

inline Json::Value serializeSColorf(video::SColorf& col)
{
    Json::Value root;
    root.append(col.getRed());
    root.append(col.getGreen());
    root.append(col.getBlue());
    root.append(col.getAlpha());

    return root;
}

inline video::SColorf deserializeSColorf(Json::Value& val)
{
    video::SColorf col;
    col.r = val[0].asDouble();
    col.g = val[1].asDouble();
    col.b = val[2].asDouble();
    col.a = val[3].asDouble();

    return col;
}

inline Json::Value serializeCircularBuffer(const circular_buffer<f32>& buff)
{
    Json::Value root;
    root["index"] = buff.getIndex();

    for (u32 i = 0; i < buff.getSize(); i++)
            root["values"].append(buff[i]);

    return root;
}

inline void deserializeCircularBuffer(circular_buffer<f32>& buff, Json::Value& val)
{
    for (u32 i = 0; i < buff.getSize(); i++)
            buff.push_back(val["values"][i].asDouble());

    buff.setIndex(val["index"].asUInt());
}

inline Json::Value serializeDate(std::tm* date)
{
    Json::Value d;
    d["year"] = date->tm_year;
    d["month"] = date->tm_mon;
    d["day"] = date->tm_mday;
    d["hour"] = date->tm_hour;
    d["minute"] = date->tm_min;

    return d;
}

inline std::tm deserializeDate(Json::Value& date)
{
    std::tm d;
    d.tm_year = date["year"].asInt();
    d.tm_mon = date["month"].asInt();
    d.tm_mday = date["day"].asInt();
    d.tm_hour = date["hour"].asInt();
    d.tm_min = date["minute"].asInt();

    return d;
}

#endif // FUNCTIONS_H_INCLUDED
