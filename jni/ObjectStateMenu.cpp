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

    Context->Renderer->getFader()->setIncludeGUI(true);
    Context->Renderer->getFader()->startFadeIn();
}

ObjectStateMenu::~ObjectStateMenu()
{
    Context->Renderer->getFader()->setIncludeGUI(false);

    Object* eventRec = Context->ObjManager->getObjectFromName("ObjectEventReceiver");
    if (eventRec)
        eventRec->unregisterObserver(this);

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
    Window = addOverlayWindow(Context);

    // we'll assume a gui for 854x480, our functions will take care of the scaling
    /*addButton(position2d<s32>(363, 100), dimension2d<s32>(128, 64),
              video->getTexture("gui/continue.png"), Context, EGI_CONTINUE, Window);
    addButton(position2d<s32>(363, 200), dimension2d<s32>(128, 64),
              video->getTexture("gui/new_game.png"), Context, EGI_NEWGAME, Window);


    addButton(position2d<s32>(740, 380), dimension2d<s32>(64, 64),
              video->getTexture("gui/exit.png"), Context, EGI_EXIT, Window);
    addButton(position2d<s32>(660, 380), dimension2d<s32>(64, 64),
              video->getTexture("gui/options.png"), Context, EGI_OPTIONS, Window);*/

    gui::IGUIButton* continueBtn = addButton(position2d<s32>(299, 100), dimension2d<s32>(256, 64),
              L"CONTINUE", Context, EGI_CONTINUE, Window);
    addButton(position2d<s32>(299, 200), dimension2d<s32>(256, 64),
              L"NEW GAME", Context, EGI_NEWGAME, Window);

    addButton(position2d<s32>(615, 320), dimension2d<s32>(256, 64),
              L"OPTIONS", Context, EGI_OPTIONS, Window);
    addButton(position2d<s32>(700, 380), dimension2d<s32>(128, 64),
              L"EXIT", Context, EGI_EXIT, Window);

    continueBtn->setEnabled(false);
}
