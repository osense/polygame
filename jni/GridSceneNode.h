#ifndef GRIDSCENENODE_H
#define GRIDSCENENODE_H

#include <ISceneNode.h>
#include <irrlicht.h>

namespace irr
{
namespace scene
{

class GridSceneNode : public ISceneNode
{
public:
    GridSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id);
    ~GridSceneNode();

    void render();
    void OnRegisterSceneNode();
    const core::aabbox3d<f32>& getBoundingBox() const;
    void setMaterialType(video::E_MATERIAL_TYPE tehType);
    video::SMaterial& getMaterial(u32 num);
    u32 getMaterialCount() const;

    void add(float);
    void endRow();
    void removeRow();

protected:
    video::SMaterial Material;
    scene::SMeshBuffer* Buffer;

    float Thickness;
};

}
}

#endif // GRIDSCENENODE_H
