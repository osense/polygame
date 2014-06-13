#ifndef GAMESTATEMENU_H
#define GAMESTATEMENU_H

#include <irrlicht.h>

#include "Object.h"
#include "ObjectManager.h"

#include "ObjectStateGame.h"
#include "functions.h"

enum E_GUI_ID
{
    EGI_NEWGAME = 0,
    EGI_CONTINUE,
    EGI_LOADGAME,
    EGI_OPTIONS,
    EGI_EXIT,
    EGI_HELP,
    EGI_ABOUT
};

using namespace irr;
using namespace core;

class ObjectStateMenu : public Object
{
    public:
        ObjectStateMenu(SContext* cont);
        virtual ~ObjectStateMenu();

        virtual void onMessage(SMessage msg);

    private:
        gui::IGUIWindow* create_menu();

        scene::ICameraSceneNode* Camera;
};

#endif // GAMESTATEMENU_H
