#include "ObjectStateOptions.h"

ObjectStateOptions::ObjectStateOptions(SContext* cont) : Object(cont)
{
    Name = "ObjectStateOptions";
    setPersistent(true);
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    Context->ObjManager->getObjectFromName("ObjectEventReceiver")->registerObserver(this);
    Context->ObjManager->getObjectFromName("ObjectUpdater")->registerObserver(this);

    create_gui();

    Context->Renderer->getFader()->startFadeIn(1, 0.5);
}

ObjectStateOptions::~ObjectStateOptions()
{
    Object* eventRec = Context->ObjManager->getObjectFromName("ObjectEventReceiver");
    if (eventRec)
        eventRec->unregisterObserver(this);

    Object* updater = Context->ObjManager->getObjectFromName("ObjectUpdater");
    if (updater)
        updater->unregisterObserver(this);

    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_DIED));
}

void ObjectStateOptions::onMessage(SMessage msg)
{
    if (msg.Type == EMT_UPDATE)
    {
        Context->Device->getVideoDriver()->draw2DImageBatch(LineSegment, LinePositions, LineRects, 0, video::SColor(255, 255, 255, 255), true);
    }
    else if (msg.Type == EMT_GUI)
    {
        s32 callerID;

        if (msg.GUI.EventType == gui::EGET_BACKBUTTON_PRESSED)
            callerID = EOI_BACK;
        else
        {
            if (msg.GUI.EventType != gui::EGET_BUTTON_CLICKED)
                return;
            callerID = msg.GUI.Caller->getID();
        }


        switch (callerID)
        {
        case EOI_GLOW_OFF:
            static_cast<gui::IGUIButton*>(Window->getElementFromId(EOI_GLOW_OFF))->setPressed(true);
            static_cast<gui::IGUIButton*>(Window->getElementFromId(EOI_GLOW_LOW))->setPressed(false);
            static_cast<gui::IGUIButton*>(Window->getElementFromId(EOI_GLOW_MEDIUM))->setPressed(false);
            static_cast<gui::IGUIButton*>(Window->getElementFromId(EOI_GLOW_HIGH))->setPressed(false);
            break;

        case EOI_GLOW_LOW:
            static_cast<gui::IGUIButton*>(Window->getElementFromId(EOI_GLOW_OFF))->setPressed(false);
            static_cast<gui::IGUIButton*>(Window->getElementFromId(EOI_GLOW_LOW))->setPressed(true);
            static_cast<gui::IGUIButton*>(Window->getElementFromId(EOI_GLOW_MEDIUM))->setPressed(false);
            static_cast<gui::IGUIButton*>(Window->getElementFromId(EOI_GLOW_HIGH))->setPressed(false);
            break;

        case EOI_GLOW_MEDIUM:
            static_cast<gui::IGUIButton*>(Window->getElementFromId(EOI_GLOW_OFF))->setPressed(false);
            static_cast<gui::IGUIButton*>(Window->getElementFromId(EOI_GLOW_LOW))->setPressed(false);
            static_cast<gui::IGUIButton*>(Window->getElementFromId(EOI_GLOW_MEDIUM))->setPressed(true);
            static_cast<gui::IGUIButton*>(Window->getElementFromId(EOI_GLOW_HIGH))->setPressed(false);
            break;

        case EOI_GLOW_HIGH:
            static_cast<gui::IGUIButton*>(Window->getElementFromId(EOI_GLOW_OFF))->setPressed(false);
            static_cast<gui::IGUIButton*>(Window->getElementFromId(EOI_GLOW_LOW))->setPressed(false);
            static_cast<gui::IGUIButton*>(Window->getElementFromId(EOI_GLOW_MEDIUM))->setPressed(false);
            static_cast<gui::IGUIButton*>(Window->getElementFromId(EOI_GLOW_HIGH))->setPressed(true);
            break;

        case EOI_FXAA_OFF:
            static_cast<gui::IGUIButton*>(Window->getElementFromId(EOI_FXAA_ON))->setPressed(false);
            static_cast<gui::IGUIButton*>(Window->getElementFromId(EOI_FXAA_OFF))->setPressed(true);
            break;
        case EOI_FXAA_ON:
            static_cast<gui::IGUIButton*>(Window->getElementFromId(EOI_FXAA_ON))->setPressed(true);
            static_cast<gui::IGUIButton*>(Window->getElementFromId(EOI_FXAA_OFF))->setPressed(false);
            break;

        case EOI_BACK:
            writeSettings(Context);
            Window->remove();
            Context->State = new ObjectStateInit(Context);
            delete this;
            return;
        }

        deserialize();
    }
}


void ObjectStateOptions::create_gui()
{
    Window = addOverlayWindow(Context);

    addText(core::position2d<s32>(0, 100), core::dimension2d<s32>(270, 40), L"GLOW", Context, Window, gui::EGUIA_LOWERRIGHT);
    addButton(core::position2d<s32>(290, 102), core::dimension2d<s32>(128, 40), L"OFF", Context, EOI_GLOW_OFF, Window)->setIsPushButton(true);
    addButton(core::position2d<s32>(420, 102), core::dimension2d<s32>(128, 40), L"LOW", Context, EOI_GLOW_LOW, Window)->setIsPushButton(true);
    addButton(core::position2d<s32>(555, 102), core::dimension2d<s32>(160, 40), L"MEDIUM", Context, EOI_GLOW_MEDIUM, Window)->setIsPushButton(true);
    addButton(core::position2d<s32>(720, 102), core::dimension2d<s32>(128, 40), L"HIGH", Context, EOI_GLOW_HIGH, Window)->setIsPushButton(true);


    addText(core::position2d<s32>(0, 220), core::dimension2d<s32>(270, 40), L"ANTI", Context, Window, gui::EGUIA_LOWERRIGHT);
    addText(core::position2d<s32>(0, 250), core::dimension2d<s32>(270, 40), L"ALIASING", Context, Window, gui::EGUIA_LOWERRIGHT);
    addButton(core::position2d<s32>(290, 237), core::dimension2d<s32>(128, 40), L"OFF", Context, EOI_FXAA_OFF, Window)->setIsPushButton(true);
    addButton(core::position2d<s32>(420, 237), core::dimension2d<s32>(128, 40), L"ON", Context, EOI_FXAA_ON, Window)->setIsPushButton(true);

    addButton(core::position2d<s32>(30, 420), core::dimension2d<s32>(128, 32), L"BACK", Context, EOI_BACK, Window);
    serialize();

    // prepare stuff for the horizontal line
    LineSegment = Context->Device->getVideoDriver()->getTexture("textures/line_v.png");
    for (u32 i = LineSpacing*Context->GUIScale.Y; i <= Window->getAbsoluteClippingRect().getHeight() - LineSpacing * 1.66*Context->GUIScale.Y; i += LineSegment->getSize().Width)
    {
        LinePositions.push_back(core::position2d<s32>(280*Context->GUIScale.X, i));
        LineRects.push_back(core::rect<s32>(core::vector2d<s32>(0, 0), LineSegment->getSize()));
    }
}

void ObjectStateOptions::serialize()
{
    if (!Context->Settings->Glow)
        static_cast<gui::IGUIButton*>(Window->getElementFromId(EOI_GLOW_OFF))->setPressed(true);
    else
    {
        switch (Context->Settings->EffectQuality)
        {
        case video::EPQ_OCTOPUS:
            static_cast<gui::IGUIButton*>(Window->getElementFromId(EOI_GLOW_LOW))->setPressed(true);
            break;
        case video::EPQ_QUARTER:
            static_cast<gui::IGUIButton*>(Window->getElementFromId(EOI_GLOW_MEDIUM))->setPressed(true);
            break;
        case video::EPQ_HALF:
            static_cast<gui::IGUIButton*>(Window->getElementFromId(EOI_GLOW_HIGH))->setPressed(true);
            break;
        default:
            break;
        }
    }


    if (Context->Settings->Antialiasing)
        static_cast<gui::IGUIButton*>(Window->getElementFromId(EOI_FXAA_ON))->setPressed(true);
    else
        static_cast<gui::IGUIButton*>(Window->getElementFromId(EOI_FXAA_OFF))->setPressed(true);
}

void ObjectStateOptions::deserialize()
{
    bool oldGlow = Context->Settings->Glow;
    if (static_cast<gui::IGUIButton*>(Window->getElementFromId(EOI_GLOW_OFF))->isPressed())
        Context->Settings->Glow = false;
    else
        Context->Settings->Glow = true;

    bool oldEffectQuality = Context->Settings->EffectQuality;
    if (static_cast<gui::IGUIButton*>(Window->getElementFromId(EOI_GLOW_LOW))->isPressed())
        Context->Settings->EffectQuality = video::EPQ_OCTOPUS;
    else if (static_cast<gui::IGUIButton*>(Window->getElementFromId(EOI_GLOW_MEDIUM))->isPressed())
        Context->Settings->EffectQuality = video::EPQ_QUARTER;
    else if (static_cast<gui::IGUIButton*>(Window->getElementFromId(EOI_GLOW_HIGH))->isPressed())
        Context->Settings->EffectQuality = video::EPQ_HALF;

    bool oldFXAA = Context->Settings->Antialiasing;
    if (static_cast<gui::IGUIButton*>(Window->getElementFromId(EOI_FXAA_ON))->isPressed())
        Context->Settings->Antialiasing = true;
    else
        Context->Settings->Antialiasing = false;


    if (Context->Settings->Glow != oldGlow || oldEffectQuality != Context->Settings->EffectQuality || Context->Settings->Antialiasing != oldFXAA)
        Context->Renderer->loadPP(true);
}
