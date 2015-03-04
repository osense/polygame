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

    if (Context->Device->isAccelerometerActive())
    {
        Context->Device->deactivateAccelerometer();
    }

    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_DIED));
}

void ObjectStateOptions::onMessage(SMessage msg)
{
    if (msg.Type == EMT_KEY)
    {
        if (msg.Key.Code == irr::KEY_ANDROID_BACK)
        {
            Context->Renderer->getFader()->startFadeOut(1, 0, 1);
            Context->Sets->write();
            MainWindow->remove();
            Context->State = new ObjectStateInit(Context);
            delete this;
            return;
        }
    }    
    else if (msg.Type == EMT_GUI && msg.GUI.EventType == gui::EGET_BUTTON_CLICKED)
    {
        s32 callerID = msg.GUI.Caller->getID();

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

        case EOI_CONTROLS_TILT:
            static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_CONTROLS_TOUCH))->setPressed(false);
            static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_CONTROLS_TILT))->setPressed(true);
            controls_onTiltSelected();
            break;

        case EOI_CONTROLS_TOUCH:
            static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_CONTROLS_TOUCH))->setPressed(true);
            static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_CONTROLS_TILT))->setPressed(false);
            controls_onTouchSelected();
            break;

        case EOI_CONTROLS_CALIBRATE:
            new ObjectStateOptionsCalibration(Context);
            break;

        case EOI_SEED_0:
        case EOI_SEED_1:
        case EOI_SEED_2:
        case EOI_SEED_3:
        case EOI_SEED_4:
        case EOI_SEED_5:
        case EOI_SEED_6:
        case EOI_SEED_7:
        case EOI_SEED_8:
        case EOI_SEED_9:
            seed_onDigitPressed(u32(callerID));
            break;

        case EOI_SEED_C:
            static_cast<gui::IGUIEditBox*>(PaneWindow->getElementFromId(EOI_SEED_BOX))->setText(L"0");
            break;

        case EOI_GFX:
            if (State != EOS_GFX)
            {
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

        case EOI_SEED:
            if (State != EOS_SEED)
            {
                create_seed();
            }
            else if (State == EOS_SEED)
                static_cast<gui::IGUIButton*>(MainWindow->getElementFromId(EOI_SEED))->setPressed(true);
        }

        deserialize();
    }
    else if (msg.Type == EMT_ACC)
    {
        if (State == EOS_CONTROLS)
        {
            gui::IGUIElement* bar = PaneWindow->getElementFromId(EOI_CONTROLS_CALIBRATE_BAR);
            if (!bar)
                return;

            gui::IGUIElement* dot = bar->getElementFromId(EOI_CONTROLS_CALIBRATE_BAR_DOT);

            f32 acc = msg.Acc.X;
            const f32 accCutoff = Context->Sets->AccelCutoff;
            clamp(acc, -1 * (accCutoff), accCutoff);
            acc = (acc + accCutoff) / (2 * accCutoff);

            const u32 size = bar->getAbsoluteClippingRect().getWidth() - dot->getAbsoluteClippingRect().getWidth();

            core::position2di dotPos(s32(acc * f32(size)), 1);
            dot->setRelativePosition(dotPos);
        }
    }
}


void ObjectStateOptions::create_gui()
{
    MainWindow = addOverlayWindow(Context);

    addButton(core::position2d<s32>(480, 27), core::dimension2d<s32>(180, 40), L"GFX", Context, EOI_GFX, MainWindow)->setIsPushButton(true);
    addButton(core::position2d<s32>(220, 27), core::dimension2d<s32>(300, 40), L"CONTROLS", Context, EOI_CONTROLS, MainWindow)->setIsPushButton(true);
    //addButton(core::position2d<s32>(370, 27), core::dimension2d<s32>(120, 40), L"SEED", Context, EOI_SEED, MainWindow)->setIsPushButton(true);
    addButton(core::position2d<s32>(30, 27), core::dimension2d<s32>(128, 40), L"BACK", Context, EOI_BACK, MainWindow);

    core::rect<s32> hlRect = core::rect<s32>(30*Context->GUIScale.X, 70*Context->GUIScale.Y, MainWindow->getAbsoluteClippingRect().LowerRightCorner.X - 30*Context->GUIScale.X, 70*Context->GUIScale.Y);
    HorizontalLine = new IGUIElementLine(Context->Device->getGUIEnvironment(), MainWindow, -1, hlRect, Context->Device->getVideoDriver()->getTexture("textures/line_h.png"));

    PaneWindow = addOverlayWindow(Context, 0.8, 1);
    core::rect<s32> vRect = core::rect<s32>(230*Context->GUIScale.X, PaneWindow->getAbsoluteClippingRect().UpperLeftCorner.Y, 280*Context->GUIScale.X, PaneWindow->getAbsoluteClippingRect().LowerRightCorner.Y - 30 * Context->GUIScale.Y);
    VerticalLine = new IGUIElementLine(Context->Device->getGUIEnvironment(), MainWindow, -1, vRect, Context->Device->getVideoDriver()->getTexture("textures/line_v.png"), true);
    VerticalLine->setVisible(false);
    PaneWindow->remove();
    PaneWindow = 0;

    create_gfx();
}

void ObjectStateOptions::create_gfx()
{
    State = EOS_GFX;
    static_cast<gui::IGUIButton*>(MainWindow->getElementFromId(EOI_GFX))->setPressed(true);
    static_cast<gui::IGUIButton*>(MainWindow->getElementFromId(EOI_CONTROLS))->setPressed(false);
    //static_cast<gui::IGUIButton*>(MainWindow->getElementFromId(EOI_SEED))->setPressed(false);
    if (PaneWindow)
        PaneWindow->remove();
    PaneWindow = addOverlayWindow(Context, 0.8, 1);
    MainWindow->addChild(PaneWindow);

    addText(core::position2d<s32>(0, 40), core::dimension2d<s32>(220, 40), L"GLOW", Context, PaneWindow, gui::EGUIA_LOWERRIGHT);
    addButton(core::position2d<s32>(240, 42), core::dimension2d<s32>(128, 40), L"OFF", Context, EOI_GLOW_OFF, PaneWindow)->setIsPushButton(true);
    addButton(core::position2d<s32>(360, 42), core::dimension2d<s32>(128, 40), L"LOW", Context, EOI_GLOW_LOW, PaneWindow)->setIsPushButton(true);
    addButton(core::position2d<s32>(480, 42), core::dimension2d<s32>(180, 40), L"MEDIUM", Context, EOI_GLOW_MEDIUM, PaneWindow)->setIsPushButton(true);
    addButton(core::position2d<s32>(650, 42), core::dimension2d<s32>(128, 40), L"HIGH", Context, EOI_GLOW_HIGH, PaneWindow)->setIsPushButton(true);


    addText(core::position2d<s32>(0, 160), core::dimension2d<s32>(220, 40), L"PLACE", Context, PaneWindow, gui::EGUIA_LOWERRIGHT);
    addText(core::position2d<s32>(0, 190), core::dimension2d<s32>(220, 40), L"HOLDER", Context, PaneWindow, gui::EGUIA_LOWERRIGHT);
    addButton(core::position2d<s32>(240, 177), core::dimension2d<s32>(128, 40), L"OFF", Context, -1, PaneWindow)->setIsPushButton(true);
    addButton(core::position2d<s32>(360, 177), core::dimension2d<s32>(128, 40), L"ON", Context, -1, PaneWindow)->setIsPushButton(true);

    VerticalLine->setVisible(true);
    serialize();
}

void ObjectStateOptions::create_controls()
{
    State = EOS_CONTROLS;
    static_cast<gui::IGUIButton*>(MainWindow->getElementFromId(EOI_GFX))->setPressed(false);
    static_cast<gui::IGUIButton*>(MainWindow->getElementFromId(EOI_CONTROLS))->setPressed(true);
    //static_cast<gui::IGUIButton*>(MainWindow->getElementFromId(EOI_SEED))->setPressed(false);
    if (PaneWindow)
        PaneWindow->remove();
    PaneWindow = addOverlayWindow(Context, 0.8, 1);
    MainWindow->addChild(PaneWindow);

    addText(core::position2d<s32>(0, 40), core::dimension2d<s32>(220, 40), L"TYPE", Context, PaneWindow, gui::EGUIA_LOWERRIGHT);
    addButton(core::position2d<s32>(240, 44), core::dimension2d<s32>(128, 40), L"TILT", Context, EOI_CONTROLS_TILT, PaneWindow)->setIsPushButton(true);
    addButton(core::position2d<s32>(360, 44), core::dimension2d<s32>(128, 40), L"TOUCH", Context, EOI_CONTROLS_TOUCH, PaneWindow)->setIsPushButton(true);

    if (Context->Sets->TouchController)
    {
        controls_onTouchSelected();
    }
    else
    {
        controls_onTiltSelected();
    }

    VerticalLine->setVisible(true);
    serialize();
}

void ObjectStateOptions::create_seed()
{
    State = EOS_SEED;
    static_cast<gui::IGUIButton*>(MainWindow->getElementFromId(EOI_GFX))->setPressed(false);
    static_cast<gui::IGUIButton*>(MainWindow->getElementFromId(EOI_CONTROLS))->setPressed(false);
    static_cast<gui::IGUIButton*>(MainWindow->getElementFromId(EOI_SEED))->setPressed(true);
    if (PaneWindow)
        PaneWindow->remove();
    PaneWindow = addOverlayWindow(Context, 0.8, 1);
    MainWindow->addChild(PaneWindow);

    core::rect<s32> boxRect(core::position2d<s32>(140, 100), core::dimension2d<s32>(200, 40));
    scaleGUIRect(boxRect, Context->GUIScale);
    gui::IGUIEditBox* box = Context->Device->getGUIEnvironment()->addEditBox(L"32768", boxRect, true, PaneWindow, EOI_SEED_BOX);
    box->setOverrideFont(getFont(Context));
    box->setDrawBackground(false);
    box->setTextAlignment(gui::EGUIA_LOWERRIGHT, gui::EGUIA_CENTER);

    addButton(core::position2d<s32>(500, 40), core::dimension2d<s32>(50, 50), L"7", Context, EOI_SEED_7, PaneWindow);
    addButton(core::position2d<s32>(600, 40), core::dimension2d<s32>(50, 50), L"8", Context, EOI_SEED_8, PaneWindow);
    addButton(core::position2d<s32>(700, 40), core::dimension2d<s32>(50, 50), L"9", Context, EOI_SEED_9, PaneWindow);
    addButton(core::position2d<s32>(500, 130), core::dimension2d<s32>(50, 50), L"4", Context, EOI_SEED_4, PaneWindow);
    addButton(core::position2d<s32>(600, 130), core::dimension2d<s32>(50, 50), L"5", Context, EOI_SEED_5, PaneWindow);
    addButton(core::position2d<s32>(700, 130), core::dimension2d<s32>(50, 50), L"6", Context, EOI_SEED_6, PaneWindow);
    addButton(core::position2d<s32>(500, 220), core::dimension2d<s32>(50, 50), L"1", Context, EOI_SEED_1, PaneWindow);
    addButton(core::position2d<s32>(600, 220), core::dimension2d<s32>(50, 50), L"2", Context, EOI_SEED_2, PaneWindow);
    addButton(core::position2d<s32>(700, 220), core::dimension2d<s32>(50, 50), L"3", Context, EOI_SEED_3, PaneWindow);

    addButton(core::position2d<s32>(500, 290), core::dimension2d<s32>(50, 50), L"0", Context, EOI_SEED_0, PaneWindow);
    addButton(core::position2d<s32>(700, 290), core::dimension2d<s32>(50, 50), L"C", Context, EOI_SEED_C, PaneWindow);

    VerticalLine->setVisible(false);
    serialize();
}

void ObjectStateOptions::controls_onTiltSelected()
{
    if (!Context->Device->isAccelerometerActive())
    {
        Context->Device->activateAccelerometer();
    }

    core::rect<s32> boxRect(core::position2d<s32>(260, 190), core::dimension2d<s32>(525, 40));
    scaleGUIRect(boxRect, Context->GUIScale);
    gui::IGUIEditBox* box = Context->Device->getGUIEnvironment()->addEditBox(L"", boxRect, true, PaneWindow, EOI_CONTROLS_CALIBRATE_BAR);
    box->setOverrideFont(getFont(Context));
    box->setDrawBackground(false);
    box->setTextAlignment(gui::EGUIA_LOWERRIGHT, gui::EGUIA_CENTER);


    gui::IGUIElement* dot = addText(core::position2d<s32>(0, 1), core::dimension2d<s32>(40, 40), L"#", Context, box);
    dot->setID(EOI_CONTROLS_CALIBRATE_BAR_DOT);

    addButton(core::position2d<s32>(240, 280), core::dimension2d<s32>(260, 40), L"CALIBRATE", Context, EOI_CONTROLS_CALIBRATE, PaneWindow);
}

void ObjectStateOptions::controls_onTouchSelected()
{
    gui::IGUIElement* elem = 0;

    elem = PaneWindow->getElementFromId(EOI_CONTROLS_CALIBRATE);
    if (elem)
        elem->remove();

    elem = PaneWindow->getElementFromId(EOI_CONTROLS_CALIBRATE_BAR);
    if (elem)
        elem->remove();
}

void ObjectStateOptions::seed_onDigitPressed(u32 digit)
{
    gui::IGUIEditBox* sBox = static_cast<gui::IGUIEditBox*>(PaneWindow->getElementFromId(EOI_SEED_BOX));
    core::stringc text = sBox->getText();

    if (text == L"0")
    {
        if (digit == 0)
        {
            return;
        }
        else
        {
            text = L"";
        }
    }

    text += digit;
    char* eh;
    u32 textI = strtol(text.c_str(), &eh, 10);
    if (textI > MAX_SEED)
    {
        text = "";
        text += MAX_SEED;
    }

    sBox->setText(core::stringw(text).c_str());
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
    }
    else if (State == EOS_CONTROLS)
    {
        if (Context->Sets->TouchController)
        {
            static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_CONTROLS_TOUCH))->setPressed(true);
        }
        else
        {
            static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_CONTROLS_TILT))->setPressed(true);
        }
    }
    else if (State == EOS_SEED)
    {
        core::stringw seedT;
        seedT += Context->Sets->Seed;
        static_cast<gui::IGUIEditBox*>(PaneWindow->getElementFromId(EOI_SEED_BOX))->setText(seedT.c_str());
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

        video::E_POSTPROCESSING_EFFECT_QUALITY oldEffectQuality = Context->Sets->EffectQuality;
        if (static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_GLOW_LOW))->isPressed())
            Context->Sets->EffectQuality = video::EPQ_OCTOPUS;
        else if (static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_GLOW_MEDIUM))->isPressed())
            Context->Sets->EffectQuality = video::EPQ_QUARTER;
        else if (static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_GLOW_HIGH))->isPressed())
            Context->Sets->EffectQuality = video::EPQ_HALF;


        if ((Context->Sets->Glow != oldGlow) || (oldEffectQuality != Context->Sets->EffectQuality))
            Context->Renderer->loadPP(true);
    }
    else if (State == EOS_CONTROLS)
    {
        if (static_cast<gui::IGUIButton*>(PaneWindow->getElementFromId(EOI_CONTROLS_TOUCH))->isPressed())
        {
            Context->Sets->TouchController = true;
        }
        else
        {
            Context->Sets->TouchController = false;
        }
    }
    else if (State == EOS_SEED)
    {
        core::stringc seedT = static_cast<gui::IGUIEditBox*>(PaneWindow->getElementFromId(EOI_SEED_BOX))->getText();
        char* eh;
        Context->Sets->Seed = strtol(seedT.c_str(), &eh, 10);
    }
}
