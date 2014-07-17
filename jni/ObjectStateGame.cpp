#include "ObjectStateGame.h"

ObjectStateGame::ObjectStateGame(SContext* cont) : Object(cont),
    DistTravelledText(0)
{
    Name = "ObjectStateGame";
    setPersistent(true);
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    core::rect<s32> distTextPos(20, 430, 200, 450);
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
    Context->ObjManager->getObjectFromName("ObjectEventReceiver")->unregisterObserver(this);

    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_DIED));
}

void ObjectStateGame::onMessage(SMessage msg)
{
    if (msg.Type == EMT_OBJ_POS)
    {
        DT = msg.Position.Z;

        if (DistTravelledText)
        {
            core::stringw distText = L"DT: ";
            distText += DT;
            DistTravelledText->setText(distText.c_str());
        }
    }
    else if (msg.Type == EMT_PLAYER_CRASHED)
    {
        debugLog("player crashed (lol n00b)");
        Context->ObjManager->getObjectFromName("ObjectEventReceiver")->registerObserver(this);
        createGameoverWindow();
        DistTravelledText->remove();
        DistTravelledText = 0;
    }
    else if (msg.Type == EMT_GUI)
    {
        if (msg.GUI.EventType != gui::EGET_BUTTON_CLICKED)
            return;

        if (msg.GUI.Caller->getID() == EGGI_OK)
        {
            Context->ObjManager->clear();
            GameoverWnd->remove();
            new ObjectStateInit(Context);
            delete this;
        }
    }
}

void ObjectStateGame::createGameoverWindow()
{
    GameoverWnd = addOverlayWindow(Context);

    gui::IGUIEnvironment* gui = Context->Device->getGUIEnvironment();
    video::IVideoDriver* video = Context->Device->getVideoDriver();

    addButton(core::position2d<s32>(363, 50), core::dimension2d<s32>(128, 64),
              video->getTexture("gui/game_over.png"), Context, -1, GameoverWnd);

    core::rect<s32> dtTextPos(0, 250, 854, 280);
    scaleGUIRect(dtTextPos, Context->GUIScale);
    gui::IGUIStaticText* dtText = gui->addStaticText((core::stringw("Distance Travelled: ") + core::stringw(DT)).c_str(), dtTextPos, false, true, GameoverWnd);
    dtText->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_CENTER);
    dtText->setOverrideColor(video::SColor(255, 255, 255, 255));

    addButton(core::position2d<s32>(363, 350), core::dimension2d<s32>(128, 64),
              video->getTexture("gui/ok.png"), Context, EGGI_OK, GameoverWnd);
}
