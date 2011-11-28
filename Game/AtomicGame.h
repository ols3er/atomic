#ifndef __atomic_AtomicGame_h__
#define __atomic_AtomicGame_h__
namespace atomic {

class World;
class AtomicRenderer;

class AtomicGame
{
public:
    static const uint32 MAX_WORLDS = 3;

private:
    World *m_world;

public:
    AtomicGame();
    ~AtomicGame();

    void update();
    void draw();

    // 描画スレッドから呼ばれる
    void drawCallback();


    World* getWorld() { return m_world; }
};

#define atomicGetWorld()    atomicGetGame()->getWorld()


} // namespace atomic
#endif __atomic_AtomicGame_h__
