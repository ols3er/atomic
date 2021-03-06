﻿#include "atmPCH.h"
#include "GameServer.h"
#include "GameClient.h"
#include "Engine/Game/AtomicApplication.h"

namespace atm {

#ifdef atm_enable_GameClient

GameClient * GameClient::s_inst;

void GameClient::initializeInstance()
{
    if(!s_inst) {
        s_inst = new GameClient();
    }
}

void GameClient::finalizeInstance()
{
    if(s_inst) {
        delete s_inst;
        s_inst = nullptr;
    }
}

GameClient* GameClient::getInstance()
{
    return s_inst;
}



GameClient::GameClient()
    : m_stop(false)
    , m_thread(nullptr)
    , m_pid(0)
{
}

GameClient::~GameClient()
{
    shutdown();
}

void GameClient::setEventHandler( const EventHandler &h )
{
    m_handler = h;
}

void GameClient::connect( const char *host, uint16 port )
{
    shutdown();
    m_address = Poco::Net::SocketAddress(host, port);
    m_thread = istNew(ist::FunctorThread<>)( std::bind(&GameClient::messageLoop, this) );
}

void GameClient::close()
{
    m_stop = true;
}

void GameClient::shutdown()
{
    close();
    if(m_thread) {
        m_thread->join();
        delete m_thread;
    }
    m_pid = 0;
    m_stop = false;
}

void GameClient::handleEvent( Event e )
{
    if(m_handler) {
        m_handler(this, e);
    }
}

void GameClient::processReceivingMessage( PMessageCont &cont )
{
    for(size_t i=0; i<cont.size(); ++i) {
        PMessage &mes = cont[i];
        switch(mes.type) {
        case PM_Join:
            {
                auto &m = reinterpret_cast<PMessage_Join&>(mes);
                ClientStates &cs = m_client_states[m.player_id];
                wcscpy(cs.name, m.name);
                cs.pid = m.player_id;
            }
            break;
        case PM_Update:
            {
                auto &m = reinterpret_cast<PMessage_Update&>(mes);
                ClientStates &cs = m_client_states[m.player_id];
                cs.ping = m.ping;
            }
            break;
        case PM_Leave:
            {
                auto &m = reinterpret_cast<PMessage_Leave&>(mes);
                m_client_states.erase(m.player_id);
            }
            break;
        }
    }
}

void GameClient::messageLoop()
{
    ist::Thread::setNameToCurrentThread("GameClient::messageLoop()");
    {
        ist::Mutex::ScopedLock slock(m_mutex_send);
        m_message_send.insert(m_message_send.begin(), PMessage_Join::create(0, atmGetConfig()->name));
    }

    Poco::Net::StreamSocket *sock = nullptr;
    Poco::Net::SocketStream *stream = nullptr;
    try {
        sock = new Poco::Net::StreamSocket(m_address);
        sock->setNoDelay(true);
        sock->setBlocking(true);
        sock->setReceiveTimeout(Poco::Timespan(atm_NetworkTimeout, 0));
        sock->setSendTimeout(Poco::Timespan(atm_NetworkTimeout, 0));
        stream = new Poco::Net::SocketStream(*sock);
    }
    catch(Poco::Exception &) {
        handleEvent(EV_ConnectionFailed);
        goto Cleanup;
    }
    handleEvent(EV_Connected);

    while(!m_stop) {
        try {
            sendMessage(stream);
            recvMessage(stream);
            //// 遅延テスト
            //ist::MiliSleep(100);
        }
        catch(Poco::TimeoutException &) {
            m_stop = true;
            handleEvent(EV_Diconnected);
        }
        catch(Poco::Exception &) {
            m_stop = true;
            handleEvent(EV_Diconnected);
        }
    }

    // todo:
    // stream->write(PM_Leave);
    handleEvent(EV_End);

    sock->shutdown();
Cleanup:
    delete stream;
    delete sock;
    m_stop = false;
}

#else // atm_enable_GameClient
#endif // atm_enable_GameClient

} // namespace atm
