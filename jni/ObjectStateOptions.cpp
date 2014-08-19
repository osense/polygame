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
        Context->Device->getVideoDriver()->draw2DImageBatch(HLineSegment, LinePositions, LineRects, 0, video::SColor(255, 255, 255, 255), true);

        if (State == EOS_GFX)
            Context->Device->getVideoDriver()->draw2DImageBatch(VLineSegment, GfxLinePositions, GfxLineRects, 0, video::SColor(255, 255, 255, 255), true);
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
            static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_GLOW_OFF))->setPressed(true);
            static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_GLOW_LOW))->setPressed(false);
            static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_GLOW_MEDIUM))->setPressed(false);
            static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_GLOW_HIGH))->setPressed(false);
            break;

        case EOI_GLOW_LOW:
            static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_GLOW_OFF))->setPressed(false);
            static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_GLOW_LOW))->setPressed(true);
            static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_GLOW_MEDIUM))->setPressed(false);
            static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_GLOW_HIGH))->setPressed(false);
            break;

        case EOI_GLOW_MEDIUM:
            static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_GLOW_OFF))->setPressed(false);
            static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_GLOW_LOW))->setPressed(false);
            static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_GLOW_MEDIUM))->setPressed(true);
            static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_GLOW_HIGH))->setPressed(false);
            break;

        case EOI_GLOW_HIGH:
            static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_GLOW_OFF))->setPressed(false);
            static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_GLOW_LOW))->setPressed(false);
            static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_GLOW_MEDIUM))->setPressed(false);
            static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_GLOW_HIGH))->setPressed(true);
            break;

        case EOI_FXAA_OFF:
            static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_FXAA_ON))->setPressed(false);
            static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_FXAA_OFF))->setPressed(true);
            break;
        case EOI_FXAA_ON:
            static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_FXAA_ON))->setPressed(true);
            static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_FXAA_OFF))->setPressed(false);
            break;

        case EOI_GFX:
            if (State != EOS_GFX)
            {
                deserialize();
                create_gfx();
            }
            else if (State == EOS_GFX)
                static_cast<gui::IGUIButton*>(MainWindow->getElementFromId(EOI_GFX))->setPressed(true);
            break;

        case EOI_CONTROLS:
            if (State != EOS_CONTROLS)
            {

                create_controls();
            }
            else if (State == EOS_CONTROLS)
                static_cast<gui::IGUIButton*>(MainWindow->getElementFromId(EOI_CONTROLS))->setPressed(true);
            break;

        case EOI_BACK:
            Context->Renderer->getFader()->startFadeOut(1, 0, 1);
            Context->Sets->write();
            MainWindow->remove();
            Context->State = new ObjectStateInit(Context);
            delete this;
            return;
        }

        deserialize();
    }
}


void ObjectStateOptions::create_gui()
{
    MainWindow = addOverlayWindow(Context);

    addButton(core::position2d<s32>(40, 25), core::dimension2d<s32>(150, 40), L"GFX", Context, EOI_GFX, MainWindow)->setIsPushButton(true);
    addButton(core::position2d<s32>(150, 25), core::dimension2d<s32>(270, 40), L"CONTROLS", Context, EOI_CONTROLS, MainWindow)->setIsPushButton(true);
    addButton(core::position2d<s32>(30, 420), core::dimension2d<s32>(128, 32), L"BACK", Context, EOI_BACK, MainWindow);

    // prepare stuff for the horizontal line
    HLineSegment = Context->Device->getVideoDriver()->getTexture("textures/line_h.png");
    for (u32 i = 30*Context->GUIScale.X; i < MainWindow->getAbsoluteClippingRect().LowerRightCorner.X - 30*Context->GUIScale.X; i += HLineSegment->getSize().Width-2)
    {
        LinePositions.push_back(core::position2d<s32>(i, 70*Context->GUIScale.Y));
        LineRects.push_back(core::rect<s32>(core::vector2d<s32>(0, 0), HLineSegment->getSize()));
    }

    PaneWindow = 0;
    create_gfx();
}

void ObjectStateOptions::create_gfx()
{
    State = EOS_GFX;
    static_cast<gui::IGUIButton*>(MainWindow->getElementFromId(EOI_GFX))->setPressed(true);
    static_cast<gui::IGUIButton*>(MainWindow->getElementFromId(EOI_CONTROLS))->setPressed(false);
    if (PaneWindow)
        PaneWindow->remove();
    PaneWindow = addOverlayWindow(Context, 1, 0.80);
    MainWindow->addChild(PaneWindow);

    addText(core::position2d<s32>(0, 40), core::dimension2d<s32>(270, 40), L"GLOW", Context, PaneWindow, gui::EGUIA_LOWERRIGHT);
    addButton(core::position2d<s32>(290, 42), core::dimension2d<s32>(128, 40), L"OFF", Context, EOI_GLOW_OFF, PaneWindow)->setIsPushButton(true);
    addButton(core::position2d<s32>(420, 42), core::dimension2d<s32>(128, 40), L"LOW", Context, EOI_GLOW_LOW, PaneWindow)->setIsPushButton(true);
    addButton(core::position2d<s32>(555, 42), core::dimension2d<s32>(160, 40), L"MEDIUM", Context, EOI_GLOW_MEDIUM, PaneWindow)->setIsPushButton(true);
    addButton(core::position2d<s32>(720, 42), core::dimension2d<s32>(128, 40), L"HIGH", Context, EOI_GLOW_HIGH, PaneWindow)->setIsPushButton(true);


    addText(core::position2d<s32>(0, 160), core::dimension2d<s32>(270, 40), L"ANTI", Context, PaneWindow, gui::EGUIA_LOWERRIGHT);
    addText(core::position2d<s32>(0, 190), core::dimension2d<s32>(270, 40), L"ALIASING", Context, PaneWindow, gui::EGUIA_LOWERRIGHT);
    addButton(core::position2d<s32>(290, 177), core::dimension2d<s32>(128, 40), L"OFF", Context, EOI_FXAA_OFF, PaneWindow)->setIsPushButton(true);
    addButton(core::position2d<s32>(420, 177), core::dimension2d<s32>(128, 40), L"ON", Context, EOI_FXAA_ON, PaneWindow)->setIsPushButton(true);

    serialize();

    // prepare stuff for the vertical line
    VLineSegment = Context->Device->getVideoDriver()->getTexture("textures/line_v.png");
    for (s32 i = PaneWindow->getAbsoluteClippingRect().UpperLeftCorner.Y; i < PaneWindow->getAbsoluteClippingRect().LowerRightCorner.Y; i += VLineSegment->getSize().Height-2)
    {
        GfxLinePositions.push_back(core::position2d<s32>(280*Context->GUIScale.X, i));
        GfxLineRects.push_back(core::rect<s32>(core::vector2d<s32>(0, 0), VLineSegment->getSize()));
    }
}

void ObjectStateOptions::create_controls()
{
    State = EOS_CONTROLS;
    static_cast<gui::IGUIButton*>(MainWindow->getElementFromId(EOI_GFX))->setPressed(false);
    static_cast<gui::IGUIButton*>(MainWindow->getElementFromId(EOI_CONTROLS))->setPressed(true);
    if (PaneWindow)
        PaneWindow->remove();
    PaneWindow = addOverlayWindow(Context, 1, 0.80);
    MainWindow->addChild(PaneWindow);
}
void ObjectStateOptions::serialize()
{
    if (State == EOS_GFX)
    {
        if (!Context->Sets->Glow)
            static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_GLOW_OFF))->setPressed(true);
        else
        {
            switch (Context->Sets->EffectQuality)
            {
            case video::EPQ_OCTOPUS:
                static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_GLOW_LOW))->setPressed(true);
                break;
            case video::EPQ_QUARTER:
                static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_GLOW_MEDIUM))->setPressed(true);
                break;
            case video::EPQ_HALF:
                static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_GLOW_HIGH))->setPressed(true);
                break;
            default:
                break;
            }
        }


        if (Context->Sets->Antialiasing)
            static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_FXAA_ON))->setPressed(true);
        else
            static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_FXAA_OFF))->setPressed(true);
    }
}

void ObjectStateOptions::deserialize()
{
    if (State == EOS_GFX)
    {
        bool oldGlow = Context->Sets->Glow;
        if (static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_GLOW_OFF))->isPressed())
            Context->Sets->Glow = false;
        else
            Context->Sets->Glow = true;

        bool oldEffectQuality = Context->Sets->EffectQuality;
        if (static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_GLOW_LOW))->isPressed())
            Context->Sets->EffectQuality = video::EPQ_OCTOPUS;
        else if (static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_GLOW_MEDIUM))->isPressed())
            Context->Sets->EffectQuality = video::EPQ_QUARTER;
        else if (static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_GLOW_HIGH))->isPressed())
            Context->Sets->EffectQuality = video::EPQ_HALF;

        bool oldFXAA = Context->Sets->Antialiasing;
        if (static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_FXAA_ON))->isPressed())
            Context->Sets->Antialiasing = true;
        else
            Context->Sets->Antialiasing = false;


        if (Context->Sets->Glow != oldGlow || oldEffectQuality != Context->Sets->EffectQuality || Context->Sets->Antialiasing != oldFXAA)
            Context->Renderer->loadPP(true);
    }
}
