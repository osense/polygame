#include "LinePool.h"

LinePool::LinePool(SContext* cont)
    :Context(cont)
{

}

LinePool::~LinePool()
{
    while (!Lines.empty())
    {
        delete Lines.top();
        Lines.pop();
    }
}

LinePool::Line* LinePool::get()
{
    if (Lines.empty())
    {
        return createLine();
    }
    else
    {
        Line* elem = Lines.top();
        Lines.pop();

        return elem;
    }
}

void LinePool::give(Line* elem)
{
    Lines.push(elem);
}

void LinePool::prepare(unsigned int count)
{
    while (count-- > 0)
    {
        Lines.push(createLine());
    }
}

LinePool::Line* LinePool::createLine()
{
    Line* nl = new Line;
    scene::ISceneManager* smgr = Context->Device->getSceneManager();
    nl->Node = smgr->addMeshSceneNode(smgr->getMesh("line.obj"));
    nl->Node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);

    return nl;
}
