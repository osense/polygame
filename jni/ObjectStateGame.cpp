#include "ObjectStateGame.h"

ObjectStateGame::ObjectStateGame(SContext* cont, bool loadSavedGame) : Object(cont),
    PauseWnd(0),
    GameoverWnd(0)
{
    Name = "ObjectStateGame";
    setPersistent(true);
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    Context->ObjManager->getObjectFromName("ObjectEventReceiver")->registerObserver(this);

    new ObjectGrid(Context);
    new ObjectSky(Context);
    new ObjectHUD(Context);
    new ObjectPlayer(Context);
    (new ObjectItemSpawner(Context))->spawnTracers();
    new ObjectTracerRecorder(Context);

    if (loadSavedGame)
        loadGame();

    Context->Renderer->getFader()->setIncludeGUI(false);
    Context->Renderer->getFader()->startFadeIn(1.0, loadSavedGame ? 0.25 : 0.5);

    Context->ObjManager->broadcastMessage(SMessage(this, EMT_WAKE_LOCK));
}

ObjectStateGame::~ObjectStateGame()
{
    Object *eventRec = Context->ObjManager->getObjectFromName("ObjectEventReceiver");
    if (eventRec)
        eventRec->unregisterObserver(this);

    if (GameoverWnd)
        GameoverWnd->remove();

    if (PauseWnd)
        PauseWnd->remove();

    Context->TimeScale = 1;

    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_DIED));
}

void ObjectStateGame::onMessage(SMessage msg)
{
    if (msg.Type == EMT_PLAYER_CRASHED)
    {
        debugLog("player crashed (lol n00b)");
        createGameoverWindow();
    }
    else if (msg.Type == EMT_GUI)
    {
        if (msg.GUI.EventType == gui::EGET_BACKBUTTON_PRESSED)
        {
            if (!GameoverWnd)
                setPaused(!isPaused());
        }

        if (msg.GUI.EventType != gui::EGET_BUTTON_CLICKED)
            return;

        switch (msg.GUI.Caller->getID())
        {
        case EGGI_EXIT_BACK:
        case EGGI_EXIT_GAMEOVER:
            saveGame();
            Context->ObjManager->clear();
            new ObjectStateInit(Context, false);
            delete this;
            return;
        }
    }
#ifdef _IRR_ANDROID_PLATFORM_
    else if (msg.Type == EMT_APPCOMMAND && msg.AppCommand.Cmd == APP_CMD_SAVE_STATE)
    {
        setPaused(true);
        saveGame();
    }
#endif // _IRR_ANDROID_PLATFORM_
}


void ObjectStateGame::setPaused(bool paused)
{
    if (paused && !isPaused())
    {
        if (GameoverWnd)
        {
            return;
        }

        Context->TimeScale = 0;
        PauseWnd = addOverlayWindow(Context);

        addText(core::position2d<s32>(299, 50), core::dimension2d<s32>(256, 64),
              L"PAUSE", Context, PauseWnd);

        addButton(core::position2d<s32>(252, 300), core::dimension2d<s32>(350, 64),
              L"BACK TO MAIN MENU", Context, EGGI_EXIT_BACK, PauseWnd);

        Context->Renderer->getFader()->startFadeOut(0.5);
        Context->ObjManager->broadcastMessage(SMessage(this, EMT_WAKE_UNLOCK));
    }
    else if (!paused && isPaused())
    {
        Context->TimeScale = 1;
        PauseWnd->remove();
        PauseWnd = 0;
        Context->Renderer->getFader()->startFadeInContinuous();
        Context->ObjManager->broadcastMessage(SMessage(this, EMT_WAKE_LOCK));
    }
}

bool ObjectStateGame::isPaused() const
{
    return PauseWnd;
}

void ObjectStateGame::saveGame()
{
    Json::Value root;

    if (isGameover())
    {
        root["can_continue"] = false;
    }
    else
    {
        root["can_continue"] = true;
        root["seed"] = Context->Sets->Seed;

        SMessage msg(this, EMT_SERIALIZE);
        msg.SData.Root = &root;
        Context->ObjManager->broadcastMessage(msg);
    }

    writeJson(Context, root, Context->Sets->SavegamePath);
    debugLog(core::stringc("saved game") + Context->Sets->SavegamePath);
}

void ObjectStateGame::loadGame()
{
    debugLog(core::stringc("loading game") + Context->Sets->SavegamePath);
    Json::Value root = readJson(Context, Context->Sets->SavegamePath);

    if (!root["can_continue"].asBool())
    {
        return;
    }

    Context->Sets->Seed = root.get("seed", 0).asUInt();
    SMessage msg(this, EMT_DESERIALIZE);
    msg.SData.Root = &root;
    Context->ObjManager->broadcastMessage(msg);
}

void ObjectStateGame::createGameoverWindow()
{
    GameoverWnd = addOverlayWindow(Context);

    gui::IGUIEnvironment* gui = Context->Device->getGUIEnvironment();

    addText(core::position2d<s32>(299, 50), core::dimension2d<s32>(256, 128),
              L"GAME OVER", Context, GameoverWnd);

    core::rect<s32> dtTextPos(0, 250, 854, 280);
    scaleGUIRect(dtTextPos, Context->GUIScale);
    f32 DT = static_cast<ObjectPlayer*>(Context->ObjManager->getObjectFromName("ObjectPlayer"))->getDT();
    core::stringw dt(floor32(DT*100) / 100.0);
    dt = dt.subString(0, dt.findLastChar(L"123456789", 9) + 1).c_str();
    gui::IGUIStaticText* dtText = gui->addStaticText((core::stringw("Distance Travelled: ") + dt).c_str(), dtTextPos, false, true, GameoverWnd);
    dtText->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_CENTER);

    addButton(core::position2d<s32>(363, 350), core::dimension2d<s32>(128, 64),
              L"O.K.", Context, EGGI_EXIT_GAMEOVER, GameoverWnd);

    Context->ObjManager->broadcastMessage(SMessage(this, EMT_WAKE_UNLOCK));
}

bool ObjectStateGame::isGameover() const
{
    return GameoverWnd;
}
