#include "ObjectStateMenu.h"

ObjectStateMenu::ObjectStateMenu(SContext* cont) : Object(cont)
{
    Name = "ObjectStateMenu";
    setPersistent(true);
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    create_menu();

    Object* eventRec = Context->ObjManager->getObjectFromName("ObjectEventReceiver");
    if (eventRec)
        eventRec->registerObserver(this);
    else
        debugLog("FATAL ERROR: no ObjectEventReceiver found");


    Camera = Context->Device->getSceneManager()->addCameraSceneNode();
    Camera->setFarValue(20);
    //Camera->setProjectionMatrix(core::matrix4().buildProjectionMatrixPerspectiveLH(16, 9, 0.1, 10));
    if (!Context->ObjManager->getObjectFromName("ObjectSky"))
        new ObjectSky(Context);
    if (!Context->ObjManager->getObjectFromName("ObjectCinematicCubes"))
        new ObjectCinematicCubes(Context);

    //Context->Renderer->setForceFXAAOff(true);
    Context->Renderer->getFader()->setIncludeGUI(true);
    Context->Renderer->getFader()->startFadeIn();

    std::thread ([&] ()
    {
        if (readJson(Context, Context->Sets->SavegamePath)["can_continue"].asBool())
            Window->getElementFromId(EGI_CONTINUE)->setEnabled(true);
    }).detach();
}

ObjectStateMenu::~ObjectStateMenu()
{
    Object* eventRec = Context->ObjManager->getObjectFromName("ObjectEventReceiver");
    if (eventRec)
        eventRec->unregisterObserver(this);

    Camera->remove();
    Window->remove();

    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_DIED));
}

void ObjectStateMenu::onMessage(SMessage msg)
{
    if (msg.Type == EMT_GUI)
    {
        if (msg.GUI.EventType == gui::EGET_BACKBUTTON_PRESSED)
        {
            Context->Device->closeDevice();
            return;
        }

        s32 callerID = msg.GUI.Caller->getID();
        if (msg.GUI.EventType != gui::EGET_BUTTON_CLICKED)
            return;

        switch (callerID)
        {
        case EGI_CONTINUE:
            debugLog("launching game from save...");
            Context->State = new ObjectStateGame(Context, true);
            delete this;
            break;
        case EGI_NEWGAME:
            debugLog("launching game...");
            Context->ObjManager->clear();
            Context->State = new ObjectStateGame(Context);
            delete this;
            break;
        case EGI_OPTIONS:
            Context->State = new ObjectStateOptions(Context);
            delete this;
            break;
        case EGI_EXIT:
            Context->Device->closeDevice();
            break;
        }
    }
}

void ObjectStateMenu::create_menu()
{
    Window = addOverlayWindow(Context);

    addButton(position2d<s32>(299, 100), dimension2d<s32>(256, 64),
              L"CONTINUE", Context, EGI_CONTINUE, Window)->setEnabled(false);
    addButton(position2d<s32>(299, 200), dimension2d<s32>(256, 64),
              L"NEW GAME", Context, EGI_NEWGAME, Window);

    addButton(position2d<s32>(615, 320), dimension2d<s32>(256, 64),
              L"OPTIONS", Context, EGI_OPTIONS, Window);
    addButton(position2d<s32>(700, 380), dimension2d<s32>(128, 64),
              L"EXIT", Context, EGI_EXIT, Window);
}
