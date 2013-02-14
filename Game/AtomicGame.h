﻿#ifndef atomic_Game_AtomicGame_h
#define atomic_Game_AtomicGame_h
#include "Input.h"
#include "AtomicApplication.h"
#include "Game/DebugMenu.h"
#include "Network/LevelEditorServer.h"
#include "Network/InputServer.h"
#include "Network/Protocol.h"


namespace atomic {

class World;
class AtomicRenderer;

class AtomicGame
{
public:
    AtomicGame();
    ~AtomicGame();

    bool readReplayFromFile(const char *path);

    void frameBegin();
    void update(float32 dt);
    void asyncupdateBegin(float32 dt);
    void asyncupdateEnd();
    void draw();
    void frameEnd();

    // 描画スレッドから呼ばれる
    void drawCallback();

    void handleLevelEditorCommands(const LevelEditorCommand &cmd);
    void handleLevelEditorQueries(LevelEditorQuery &cmd);
    void handlePMessages(const PMessage &mes);
    int handleCommandLine(const stl::wstring &command);

    void handleEntitiesQuery(std::string &out);

    const InputState& getIngameInputs() const { return m_input_server->getInput(0); }
    World* getWorld()   { return m_world; }
    SFMT* getRandom();
    uint32 getFrame() const                 { return m_frame; }
    PlayerID getPlayerID() const            { return m_player_id; }

#ifdef atomic_enable_sync_lock
    void dbgLockSyncMethods()               { m_sync_lock=true; }
    void dbgUnlockSyncMethods()             { m_sync_lock=false; }
    bool dbgIsSyncMethodsEnabled() const    { return m_sync_lock; }
#endif // atomic_enable_sync_lock

private:
    IInputServer    *m_input_server;
    World           *m_world;
    SFMT            m_rand;
    uint32          m_frame;
    PlayerID        m_player_id;
    EntitiesQueryContext m_ctx_entities_query;
#ifdef atomic_enable_sync_lock
    bool m_sync_lock;
#endif // atomic_enable_sync_lock
};

#define atomicGetWorld()            atomicGetGame()->getWorld()
#define atomicGetIngameInputs()     atomicGetGame()->getIngameInputs()
#define atomicGetRandom()           atomicGetGame()->getRandom()
#define atomicGetFrame()            atomicGetGame()->getFrame()

#ifdef atomic_enable_sync_lock
#   define  atomicDbgLockSyncMethods()      atomicGetGame()->dbgLockSyncMethods()
#   define  atomicDbgUnlockSyncMethods()    atomicGetGame()->dbgUnlockSyncMethods()
#   define  atomicDbgAssertSyncLock()       istAssert(!atomicGetGame()->dbgIsSyncMethodsEnabled())
#else
#   define  atomicDbgLockSyncMethods()      
#   define  atomicDbgUnlockSyncMethods()    
#   define  atomicDbgAssertSyncLock()       
#endif // atomic_enable_sync_lock

} // namespace atomic
#endif atomic_Game_AtomicGame_h
