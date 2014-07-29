#include "ObjectStateGame.h"

ObjectStateGame::ObjectStateGame(SContext* cont) : Object(cont),
    PauseWnd(0),
    GameoverWnd(0),
    DistTravelledText(0)
{
    Name = "ObjectStateGame";
    setPersistent(true);
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    Context->ObjManager->getObjectFromName("ObjectEventReceiver")->registerObserver(this);

    core::rect<s32> distTextPos(20, 420, 200, 450);
    scaleGUIRect(distTextPos, Context->GUIScale);
    DistTravelledText = Context->Device->getGUIEnvironment()->addStaticText(L"", distTextPos);
    DistTravelledText->setOverrideColor(video::SColor(255, 255, 255, 255));

    new ObjectGrid(Context);
    new ObjectItemSpawner(Context);
    new ObjectSky(Context);
    (new ObjectPlayer(Context))->registerObserver(this);
}

ObjectStateGame::~ObjectStateGame()
{
    Object *eventRec = Context->ObjManager->getObjectFromName("ObjectEventReceiver");
    if (eventRec)
        eventRec->unregisterObserver(this);

    if (GameoverWnd)
        GameoverWnd->remove();

    if (isPaused())
        setPaused(false);

    if (DistTravelledText)
        DistTravelledText->remove();

    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_DIED));
}

void ObjectStateGame::onMessage(SMessage msg)
{
    if (msg.Type == EMT_OBJ_POS)
    {
        DT = msg.Position.Z;

        if (DistTravelledText)
        {
            core::stringw dtText(floor32(DT*100) / 100.0);
            DistTravelledText->setText(dtText.subString(0, dtText.findLastChar(L"123456789", 9) + 1).c_str());
        }
    }
    else if (msg.Type == EMT_PLAYER_CRASHED)
    {
        debugLog("player crashed (lol n00b)");
        createGameoverWindow();
        DistTravelledText->remove();
        DistTravelledText = 0;
    }
    else if (msg.Type == EMT_GUI)
    {
        if (msg.GUI.EventType == gui::EGET_BACKBUTTON_PRESSED)
        {
            if (!GameoverWnd)
            {
                if (!isPaused())
                {
                    Context->Renderer->getFader()->startFadeOut(0.5);
                    setPaused(true);
                }
                else
                {
                    Context->Renderer->getFader()->startFadeInContinuous();
                    setPaused(false);
                }
            }
        }

        if (msg.GUI.EventType != gui::EGET_BUTTON_CLICKED)
            return;

        if (msg.GUI.Caller->getID() == EGGI_OK)
        {
            Context->ObjManager->clear();
            new ObjectStateInit(Context, false);
            delete this;
        }
    }
}


void ObjectStateGame::setPaused(bool paused)
{
    if (paused)
    {
        Context->TimeScale = 0;
        PauseWnd = addOverlayWindow(Context);

        addButton(core::position2d<s32>(299, 50), core::dimension2d<s32>(256, 64),
              L"PAUSE", Context, -1, PauseWnd);

        addButton(core::position2d<s32>(252, 300), core::dimension2d<s32>(350, 64),
              L"BACK TO MAIN MENU", Context, EGGI_OK, PauseWnd);
    }
    else
    {
        Context->TimeScale = 1;
        PauseWnd->remove();
        PauseWnd = 0;
    }
}

bool ObjectStateGame::isPaused() const
{
    return PauseWnd;
}

void ObjectStateGame::createGameoverWindow()
{
    GameoverWnd = addOverlayWindow(Context);

    gui::IGUIEnvironment* gui = Context->Device->getGUIEnvironment();

    addButton(core::position2d<s32>(299, 50), core::dimension2d<s32>(256, 128),
              L"GAME OVER", Context, -1, GameoverWnd);

    core::rect<s32> dtTextPos(0, 250, 854, 280);
    scaleGUIRect(dtTextPos, Context->GUIScale);
    core::stringw dt(floor32(DT*100) / 100.0);
    dt = dt.subString(0, dt.findLastChar(L"123456789", 9) + 1).c_str();
    gui::IGUIStaticText* dtText = gui->addStaticText((core::stringw("Distance Travelled: ") + dt).c_str(), dtTextPos, false, true, GameoverWnd);
    dtText->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_CENTER);
    //dtText->setOverrideColor(video::SColor(255, 255, 255, 255));

    addButton(core::position2d<s32>(363, 350), core::dimension2d<s32>(128, 64),
              L"O.K.", Context, EGGI_OK, GameoverWnd);
}
