#include "NetXX.h"
#include "CCLuaEngine.h"
#if 1
    #define PROTO_TYPE 0 //0 sproto, 1 custom
    #define PROTO_TYPE1_HEADER_LEN 0
#else
    #define PROTO_TYPE 1 //0 sproto, 1 custom
    #define PROTO_TYPE1_HEADER_LEN 11
#endif

using namespace cocos2d;
using namespace network;
namespace network{
    

long long millitime()
{
    auto t = std::chrono::steady_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(t).count();
}

// information_ivec

#define _DEFINE_KEYS \
const unsigned char information_key[] = { 0xfc,0x15,0x45,0xc9,0x39,0xc4,0xbf,0xbb,0xb4,0x55,0xe7,0xd7,0x93,0xa3,0x0c,0x3e };

enum ErrorStatus {
    DataOK=0,
    SendSucc = 0,
    ConnectSucc = 0,
    Connecting,
    ConnectFail,
    ConnectTimeout,
    ConnectLose,
    NetWorkStop,
    NetWorkUnreachable,
    SendFailed,
    SendTimeOut,
    RecvFailed,
    PacketError,
    PacketTooLong,
    IllegalPacket,
    OtherError
};

// tcp error message table
static std::string s_tableErrorMsg[] = {
    "OK",
    "Connect Failed!",
    "Connect Timeout!",
    "Send Failed!",
    "Send Timeout!",
    "Receive Failed!",
    "Network Unreachable!",
    "Connection Lost!",
    "Packet Too Long!",
    "Illegal Packet Received!",
    "Unknown Error!!!"
};

enum {
    CID_CONNECTION_ESTABLISHED = 100
};

#define HWP_PACKET_HEADER_LENGTH 2
#define HWP_PACKET_MAX_LENGTH 65535
    
#define SEND_TIME_OUT_SECOND 30
    

NetXX* NetXX::getInstance(void)
{
    return purelib::gc::singleton<NetXX>::instance();
}

NetXX* NetXX::create(void)
{
    auto net = new NetXX();
    if(net)
    {
        net->autorelease();
    }
    return net;
}

NetXX::NetXX()
{
    this->blocking = false;
    responseTimeout = SEND_TIME_OUT_SECOND;
    connecting = false;
    luacallback = 0;
//    async_tcp = std::make_shared<purelib::inet::async_tcp_client>();
    __tcpcli = new purelib::inet::async_tcp_client();
}
NetXX::~NetXX()
{
    //CCLOG(" ~NetXX-------------------%d",1);
    if(__tcpcli)
    {
        delete __tcpcli;
        __tcpcli = nullptr;
    }
    //CCLOG(" ~NetXX-------------------%d",2);
    Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    if(luacallback!=0)
    {
        LuaEngine::getInstance()->removeScriptHandler(luacallback);
    }
    luacallback = 0;
    //CCLOG(" ~NetXX-------------------%d",3);
}

void NetXX::setSendTimeout(long seconds)
{
    responseTimeout = seconds;
}

bool NetXX::init()
{

    CCLOG("NetXX::init......");

    connecting = false;

    pauseReceive = false;
    
    __tcpcli->set_callbacks(
                        NetXX::decode_packet_length,
                        [=](int ec, const char* msg){
                            this->connect_lose_callback_t(ec,msg);
                        },
                        [this](std::vector<char>&& packet) {
                            this->pdu_recv_callback_t(std::move(packet));
                        },
                        [this](const vdcallback_t& f) {
                            this->threadsafe_callbacjk_t(f);
                        });
    
    __tcpcli->set_connect_listener(
                     [this](bool succeed, int error){
                        this->connect_callback_t(succeed,error);
                     }
                    );
    
    Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    Director::getInstance()->getScheduler()->schedule(
                                        schedule_selector(NetXX::dispatch_received_pdu),
                                                                   this,
                                                                   0,
                                                                   false);
    this->setConnectWaitTimeout(500);
    __tcpcli->start_service(); // The tcpcli thread not connect server until wakup called

    return true;
}

void NetXX::dispatch_received_pdu(float dt)
{
    if(pauseReceive)
    {
        return;
    }
    __tcpcli->dispatch_received_pdu();
}

bool NetXX::decode_packet_length(const char* data, size_t datalen, int& len)
{
    if (datalen >= HWP_PACKET_HEADER_LENGTH) {
#if (PROTO_TYPE==1)
        char buf[2]={data[0],data[1]};
        unsigned short t;
        memcpy(&t,buf,2);
        len = t + PROTO_TYPE1_HEADER_LEN;
#else
        char buf[2]={data[1],data[0]};
        unsigned short t;
        memcpy(&t,buf,2); 
        len = t+2;
#endif

        if(len>HWP_PACKET_MAX_LENGTH)
        {
            return false;
        }
        return true;
    }
    else {
        len = -1;
        return true;
    }
}

void NetXX::connect_lose_callback_t(int ec, const char* msg)
{
    CCLOG("Connect lost, Local Error Info, ec:%d, error msg:%s", ec, msg);
    
    this->lastErrorNumber = ec;
    this->blocking = false;
    if (ec == -200)
    {
        //switch need to notify
        CCLOG("  change    connect-------------------");
    }
    else
    {
        //no need notify tcpcli auto reconnect
        this->handleClose();
    }
    notifyLua(0,ConnectLose,"",0);
//    reConnect();
}

void NetXX::pdu_recv_callback_t(std::vector<char>&& packet)
{
    this->dispatch_received_pkg(std::move(packet));
}

void NetXX::threadsafe_callbacjk_t(const vdcallback_t& f)
{
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]{
        f();
    });
}

void NetXX::connect_callback_t(bool succeed, int error)
{
    this->blocking = false;
    //no need to reconnect tcpcli auto cycle connect
    if (succeed) {
        
        if (error == -200) {
            CCLOG(" change  error   listen-------------------");
            return;
        }
        // connect succed, login game server.
        this->tRespExpireThreshold.clear();
        
        CCLOG("NetXX::connect_listener --> Connection Established!");
        //MARK::NTF
        //CONNECT_SUCC
        notifyLua(0,ConnectSucc,"",0);
    }
    else {
        //MARK::NTF
        //CONNENT_FAIL
        notifyLua(0,ConnectFail,"",0);
        
//        reConnect();
    }
}


void NetXX::shutdown()
{ // client shutdown network
	CCLOG("--------shutdown---------");
    this->blocking = false;
    if (__tcpcli->is_connected()) {
        __tcpcli->close(); // will trigger connection lost event.
    }
    else
    {
        handleClose();
    }
    CCLOG("NetXX::shutdown(void)");
    notifyLua(0,NetWorkStop,"",0);

    pauseReceive = false;
}

void NetXX::handleClose()
{ // peer close socket or local close the socket will tirgger this call.
    reset();
}

void NetXX::reset(void)
{
    removeAllExpireCheckers();
}

void NetXX::setEndpoint(const char* address, const char* addressv6, u_short port)
{
    __tcpcli->set_endpoint(address, addressv6, port);
}

void NetXX::startConnect(void)
{
    CCLOG("NetXX::startConnect()...%d",this->blocking);
    if (this->blocking){
        return;
    }
    __tcpcli->notify_connect();
    notifyLua(0,Connecting,"",0);
    pauseReceive = false;
}

void NetXX::reConnect()
{
    CCLOG("NetXX::reConnect()...%d",this->blocking);
    if (this->blocking) {
        return;
    }
    this->blocking = true;
    __tcpcli->notify_connect();
    notifyLua(0,Connecting,"",0);
    pauseReceive = false;
}

//-1: disable auto connect
void NetXX::setConnectWaitTimeout(long milliseconds)
{
    __tcpcli->set_connect_wait_timeout(milliseconds);
}

void  NetXX::setLuaListener(int hLuaFunc)
{
    if(luacallback!=0)
    {
        LuaEngine::getInstance()->removeScriptHandler(luacallback);
    }
    luacallback = hLuaFunc;
}
void  NetXX::delLuaListener()
{
    if(luacallback!=0)
    {
        LuaEngine::getInstance()->removeScriptHandler(luacallback);
    }
    luacallback = 0;
}

void  NetXX::setPauseReceive(bool flag)
{
    pauseReceive = flag;
}

bool NetXX::sendMsg(std::string& msg, int sid)
{
    bool isconn = __tcpcli->is_connected();
    if (!isconn)
    {
        CCLOG("sendMsg:%d no connect fail!",sid);
        dispatchNoRespMessage(sid,SendFailed);
        return false;
    }
    sendMsg_ex(msg, sid);
    return true;
}

void NetXX::encrypt(unsigned char * buffer, unsigned short len, unsigned char key)
{
    for( unsigned short i=0; i<len; ++i)
        buffer[i] = ((buffer[i])^key)+key;
}
    
void NetXX::decrypt(unsigned char * buffer, unsigned short len, unsigned char key)
{
    for( unsigned short i=0; i<len; ++i)
        buffer[i] = (buffer[i]-key)^key;
}
    
bool NetXX::sendMsg_ex(std::string& msg, int sid)
{
    
#if (PROTO_TYPE==1)
    //MARK::HEADER---------------------------------------------
    if(msg.size()<11)
    {
        CCLOG("Send msg lengh<11 !!!");
        return false;
    }
    unsigned char cbKey =   1 + rand() % 254 ;
    auto pmsg = msg.data();
    encrypt((unsigned char*)pmsg+11,msg.size()-11,cbKey);
    msg.erase(5,1);
    msg.insert(5,1,cbKey);
    //--------------------------------------------------------
#else
    char buf[2]={0x00,0x00};
    unsigned short t = msg.size();
    memcpy(buf,&t,0x02);
    msg.insert(0,1,buf[0]);
    msg.insert(0,1,buf[1]);
#endif
    std::vector<char> ob(msg.begin(), msg.end());
    __tcpcli->async_send(std::move(ob), [=](purelib::inet::error_number status){
        // if status not 0, general reson: network error occored.
        //just notify no need to process
        if (status == purelib::inet::ERR_OK) {
            // cocos2d::log("Send status: %d, detail: %s", status, s_tableErrorMsg[status].c_str());
            CCLOG("Send ok sid=%d!!!",sid);
        }
        else {
            CCLOG("Send fail Fatal Error Occured sid=%d error=%d!!!",sid,status);
        }
    });

    if (sid != -1)
    {
        // removeExpireChecker
        if (removeExpireChecker(sid))
        {
            CCLOG("exist checker removed succeed.");
        }
        // createExpireChecker
        createExpireChecker(sid);
    }

    return true;
}

bool  NetXX::isRespExpired(int sid) const
{
    auto expireIter = this->tRespExpireThreshold.find(sid);
    bool needsCheckExpire = (expireIter != this->tRespExpireThreshold.end());

    return needsCheckExpire && ((millitime() - expireIter->second.expireTime) >= (this->responseTimeout * 950)); // 5 seconds.
}

void NetXX::removeAllExpireCheckers(void)
{
    for (auto& checker : tRespExpireThreshold)
    {
        __tcpcli->cancel_timer(checker.second.timer.get());
    }
    tRespExpireThreshold.clear();
}

bool  NetXX::removeExpireChecker(int command_id)
{
    if (command_id != -1) {
        auto expireIter = tRespExpireThreshold.find(command_id);
        if (expireIter != tRespExpireThreshold.end()) {
            expireIter->second.timer->cancel();
            tRespExpireThreshold.erase(command_id); // erase exists checker
            return true;
        }
    }

    return false;
}

void NetXX::createExpireChecker(int sid, const std::function<void()>& expiredCallback)
{
//    CCLOG("settling timeout timer for response sid id:%d", sid);
    // createExpireTimer
    auto t = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::seconds(responseTimeout));
    std::shared_ptr<deadline_timer> timerobj(new deadline_timer(t));
    timerobj->async_wait([sid, this, expiredCallback](bool cancelled){
        if (!cancelled) {
            // timeout, remove timer, but not remove info.
            auto timeoTarget = this->tRespExpireThreshold.find(sid);
            if (timeoTarget != this->tRespExpireThreshold.end())
            { // the respCID is timeout
                CCLOG("The response is timeout, client detected! sid : %d",sid);
                this->dispatchNoRespMessage(sid,SendTimeOut);
                // __tcpcli->removeTimerListener(timeoTarget->second.timer.get());

                // 10 seconds no response, reset network
                if (expiredCallback) {
                    expiredCallback();
                }
                else {
                    this->shutdown();
                }
            }
            else {
                CCLOG("checker not exist!");
            }
        }
    });
    tRespExpireThreshold.insert(std::make_pair(sid, RespExpireInfo{ millitime(), timerobj }));
}

void NetXX::dispatchNoRespMessage(int sid,int state)
{
    if (sid != -1) {
        CCLOG("NetXX::dispatchNoRespMessage ==> sid:%d,%d", sid,state);
        notifyLua(1,state,"",sid);
    }
}

void NetXX::dispatch_received_pkg(std::vector<char>&& packet)
{
    try {
#if (PROTO_TYPE==1)
        //MARK::HEADER---------------------------------------------
        if(packet.size()<PROTO_TYPE1_HEADER_LEN)
        {
            CCLOG("recv packet lengh<11 !!!");
            return;
        }
        removeAllExpireCheckers();
        std::string data(packet.begin(),packet.end());
        auto cbKey = data[5];
        auto pmsg = data.data();
        decrypt((unsigned char*)(pmsg+PROTO_TYPE1_HEADER_LEN),data.size()-PROTO_TYPE1_HEADER_LEN,cbKey);
        notifyLua(1,DataOK,std::move(data),0);
        //---------------------------------------------------------
#else
        removeAllExpireCheckers();
        //decode
        std::string data(packet.begin()+2,packet.end());
        notifyLua(1,DataOK,std::move(data),0);
#endif
    }
    catch (std::logic_error& e)
    {
        CCLOG("decode package exception occured:%s", e.what());
        // __tcpcli->close();
        this->shutdown();
        //MARK::NTF
        //decode error
        notifyLua(1,PacketError,"",0);
        return;
    }
}
static cocos2d::LuaEngine* s_luaEngine = nullptr;
//evt 0:state 1:data
//status
void  NetXX::notifyLua(int evt,int status,std::string&& data,int sid)
{
    if(!luacallback){
        return;
    }
    if(!s_luaEngine)
    {
        s_luaEngine = cocos2d::LuaEngine::getInstance();
    }

    if (s_luaEngine)
    {
        LuaStack *pStack = s_luaEngine->getLuaStack();
        pStack->pushInt(evt);
        pStack->pushInt(status);
        pStack->pushString(data.data(),(int)data.size());
        pStack->pushInt(sid);
        pStack->executeFunctionByHandler(luacallback, 4);
        pStack->clean();
    }
}


void NetXX::logHex(const char* protocol, const char* hex, size_t len)
{
    std::string msg/* = protocol*/;

    // msg.append(":\n");

    int line_feed = 16;
    char svalue[16] = { 0 };
    for (int i = 0; i < len; ++i)
    {
        if (i + 1 != len) {
            if ((i + 1) % line_feed != 0) {
                sprintf(svalue, "%02x  ", (unsigned char)hex[i]);
                msg.append(svalue);
            }
            else {
                sprintf(svalue, "%02x \n", (unsigned char)hex[i]);
                msg.append(svalue);
            }
        }
        else {
            sprintf(svalue, "%02x \n", (unsigned char)hex[i]);
            msg.append(svalue);
        }
    }

    CCLOG("%s,len=%u:\n%s", protocol, len, msg.c_str());
}
    
}

