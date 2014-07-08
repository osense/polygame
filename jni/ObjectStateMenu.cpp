#include "ObjectStateMenu.h"

ObjectStateMenu::ObjectStateMenu(SContext* cont) : Object(cont)
{
    Name = "ObjectStateMenu";
    setPersistent(true);
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    debugLog((core::stringc("GUI Scale is ") + core::stringc(Context->GUIScale)).c_str());

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
            Window->remove();
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

void ObjectStateMenu::create_menu()
{
    gui::IGUIEnvironment* gui = Context->Device->getGUIEnvironment();
    video::IVideoDriver* video = Context->Device->getVideoDriver();
    position2d<s32> screenSize;
    screenSize.X = Context->Device->getVideoDriver()->getScreenSize().Width;
    screenSize.Y = Context->Device->getVideoDriver()->getScreenSize().Height;

    Window = gui->addWindow(rect<s32>(position2d<s32>(0, 0), screenSize));
    Window->setDraggable(false);
    Window->setDrawBackground(false);
    Window->getCloseButton()->remove();

    // we'll assume a gui for 854x480, our functions will take care of the scaling
    addButton(position2d<s32>(363, 100), dimension2d<s32>(128, 64),
              video->getTexture("gui/continue.png"), Context, EGI_CONTINUE, Window);
    addButton(position2d<s32>(363, 200), dimension2d<s32>(128, 64),
              video->getTexture("gui/new_game.png"), Context, EGI_NEWGAME, Window);


    addButton(position2d<s32>(740, 380), dimension2d<s32>(64, 64),
              video->getTexture("gui/exit.png"), Context, EGI_EXIT, Window);
    addButton(position2d<s32>(660, 380), dimension2d<s32>(64, 64),
              video->getTexture("gui/options.png"), Context, EGI_OPTIONS, Window);
}
