#ifndef OBJECTSTATEGAME_H
#define OBJECTSTATEGAME_H

#include <irrlicht.h>
#include <thread>
#include <chrono>
#include "Object.h"
#include "ObjectManager.h"

#include "functions.h"
#include "ObjectStateInit.h"
#include "ObjectGrid.h"
#include "ObjectItemSpawner.h"
#include "ObjectSky.h"
#include "ObjectHUD.h"
#include "ObjectPlayer.h"
#include "ObjectTracerRecorder.h"
#include "EffectFader.h"

enum E_GAME_GUI_ID
{
    EGGI_EXIT_GAMEOVER = 0,
    EGGI_EXIT_BACK
};

using namespace irr;

class ObjectStateGame : public Object
{
    public:
        ObjectStateGame(SContext* cont, bool loadSavedGame = false);
        virtual ~ObjectStateGame();

        virtual void onMessage(SMessage msg);

    private:
        gui::IGUIWindow* PauseWnd;
        void setPaused(bool paused);
        bool isPaused() const;

        void saveGame();
        void loadGame();

        gui::IGUIWindow* GameoverWnd;
        void createGameoverWindow();
        bool isGameover() const;
};

#endif // OBJECTSTATEGAME_H
