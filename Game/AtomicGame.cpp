#include "stdafx.h"
#include "types.h"
#include "AtomicApplication.h"
#include "AtomicGame.h"
#include "Graphics/ResourceManager.h"
#include "Graphics/Renderer.h"
#include "Game/Message.h"
#include "Game/World.h"

namespace atomic {


AtomicGame::AtomicGame()
: m_world(NULL)
{
    MessageRouter::initializeInstance();

    m_world = IST_NEW16(World)();
    m_world->initialize();
}

AtomicGame::~AtomicGame()
{
    IST_SAFE_DELETE(m_world);

    MessageRouter::finalizeInstance();
}


void AtomicGame::update(float32 dt)
{
    m_world->update(dt);
}

void AtomicGame::asyncupdateBegin(float32 dt)
{
    m_world->asyncupdateBegin(dt);
}

void AtomicGame::asyncupdateEnd()
{
    m_world->asyncupdateEnd();
}


void AtomicGame::draw()
{
    // todo: フレームスキップ処理
    atomicKickDraw();
    atomicWaitForDrawComplete();
}

void AtomicGame::drawCallback()
{
    AtomicRenderer::getInstance()->beforeDraw();
    if(m_world) {
        m_world->draw();
    }
    AtomicRenderer::getInstance()->draw();

}



} // namespace atomic