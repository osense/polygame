#ifndef RESOURCEPOOL_H
#define RESOURCEPOOL_H

#include "SContext.h"
#include <stack>
#include <irrlicht.h>

using namespace irr;

class LinePool
{
public:

    struct Line
    {
        core::vector3df Start, End;
        scene::IMeshSceneNode* Node;
    };


    LinePool(SContext* cont);
    ~LinePool();

    Line* get();
    void give(Line* elem);
    void prepare(unsigned int count);

private:
    SContext* Context;
    std::stack<Line*> Lines;

    Line* createLine();
};

#endif // RESOURCEPOOL_H
