#include "ObjectStateMenu.h"

ObjectStateMenu::ObjectStateMenu(SContext* cont) : Object(cont)
{
    Name = "ObjectStateMenu";
    setPersistent(true);
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    //debugLog(stringc("gui scale ratio is ") + stringc(Context->GUIScale.X) + "x" + stringc(Context->GUIScale.Y));

    create_menu();

    Object* eventRec = Context->ObjManager->getObjectFromName("ObjectEventReceiver");
    if (eventRec)
        eventRec->registerObserver(this);
    else
        debugLog("FATAL ERROR: no ObjectEventReceiver found");
}

ObjectStateMenu::~ObjectStateMenu()
{
    Object* eventRec = Context->ObjManager->getObjectFromName("ObjectEventReceiver");
    if (eventRec)
        eventRec->unregisterObserver(this);

    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_DIED));
}

void ObjectStateMenu::onMessage(SMessage msg)
{
    if (msg.Type == EMT_GUI)
    {
        s32 callerID = msg.GUI.Caller->getID();
        if (msg.GUI.EventType != gui::EGET_BUTTON_CLICKED)
            return;

        switch (callerID)
        {
        case EGI_NEWGAME:
            debugLog("launching game...");
            Context->Device->getGUIEnvironment()->clear();
            Context->ObjManager->clear();
            Camera->remove();
            Context->State = new ObjectStateGame(Context);
            delete this;
            break;
        case EGI_OPTIONS:
            break;
        case EGI_EXIT:
            Context->Device->closeDevice();
            break;
        }
    }
}

gui::IGUIWindow* ObjectStateMenu::create_menu()
{
    gui::IGUIEnvironment* gui = Context->Device->getGUIEnvironment();
    video::IVideoDriver* video = Context->Device->getVideoDriver();
    position2d<s32> screenSize;
    screenSize.X = Context->Device->getVideoDriver()->getScreenSize().Width;
    screenSize.Y = Context->Device->getVideoDriver()->getScreenSize().Height;

    gui::IGUIWindow* wnd = gui->addWindow(rect<s32>(position2d<s32>(0, 0), screenSize));
    wnd->setDraggable(false);
    wnd->setDrawBackground(false);
    wnd->getCloseButton()->remove();

    // we'll assume a gui for 854x480, our functions will take care of the scaling
    /*addButton(rect<s32>(position2d<s32>(363, 100), dimension2d<s32>(128, 64)),
              video->getTexture("gui/continue.png"), SContext, EGI_CONTINUE, wnd);
    addButton(rect<s32>(position2d<s32>(363, 200), dimension2d<s32>(128, 64)),
              video->getTexture("gui/new_game.png"), SContext, EGI_NEWGAME, wnd);
    addButton(rect<s32>(position2d<s32>(363, 280), dimension2d<s32>(128, 64)),
              video->getTexture("gui/load_game.png"), SContext, EGI_LOADGAME, wnd);


    addButton(rect<s32>(position2d<s32>(740, 380), dimension2d<s32>(64, 64)),
              video->getTexture("gui/exit.png"), SContext, EGI_EXIT, wnd);
    addButton(rect<s32>(position2d<s32>(660, 380), dimension2d<s32>(64, 64)),
              video->getTexture("gui/options.png"), SContext, EGI_OPTIONS, wnd);*/

    return wnd;
}
