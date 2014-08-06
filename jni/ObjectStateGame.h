#ifndef OBJECTSTATEGAME_H
#define OBJECTSTATEGAME_H

#include <irrlicht.h>
#include "Object.h"
#include "ObjectManager.h"

#include "functions.h"
#include "ObjectStateInit.h"
#include "ObjectGrid.h"
#include "ObjectItemSpawner.h"
#include "ObjectSky.h"
#include "ObjectHUD.h"
#include "ObjectPlayer.h"
#include "EffectFader.h"

enum E_GAME_GUI_ID
{
    EGGI_OK = 0
};

using namespace irr;

class ObjectStateGame : public Object
{
    public:
        ObjectStateGame(SContext* cont);
        virtual ~ObjectStateGame();

        virtual void onMessage(SMessage msg);

    private:
        gui::IGUIWindow* PauseWnd;
        void setPaused(bool paused);
        bool isPaused() const;

        gui::IGUIWindow* GameoverWnd;
        void createGameoverWindow();
};

#endif // OBJECTSTATEGAME_H
