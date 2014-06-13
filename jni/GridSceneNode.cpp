#include "GridSceneNode.h"

irr::scene::GridSceneNode::GridSceneNode(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id) : ISceneNode(parent, mgr, id),
    Thickness(1.0)
{
    //ctor
}

irr::scene::GridSceneNode::~GridSceneNode()
{
    delete Buffer;
}

void irr::scene::GridSceneNode::add(float)
{

}

void irr::scene::GridSceneNode::endRow()
{

}

void irr::scene::GridSceneNode::removeRow()
{

}

void irr::scene::GridSceneNode::render()
{
    irr::video::IVideoDriver* driver = SceneManager->getVideoDriver();
    driver->setTransform(irr::video::ETS_WORLD, AbsoluteTransformation);
    driver->setMaterial(Material);
    driver->drawMeshBuffer(Buffer);
}

void irr::scene::GridSceneNode::OnRegisterSceneNode()
{

}

void irr::scene::GridSceneNode::setMaterialType(irr::video::E_MATERIAL_TYPE tehType)
{
    Material.MaterialType= tehType;
}

const irr::core::aabbox3d<irr::f32>& irr::scene::GridSceneNode::getBoundingBox() const
{
    return Buffer->getBoundingBox();
}

irr::video::SMaterial& irr::scene::GridSceneNode::getMaterial(irr::u32 num)
{
    return Material;
}

irr::u32 irr::scene::GridSceneNode::getMaterialCount() const
{
    return 1;
}
