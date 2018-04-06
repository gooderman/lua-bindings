#ifndef _NetXX_H_
#define _NetXX_H_

#include <stdint.h>
#include <vector>
#include <string>
#include <functional>
#include <unordered_map>

#include "object_pool.h"
#include "deadline_timer.h"
using namespace purelib::inet;
#include "src/async_tcp_client.h"

#include "cocos2d.h"
namespace network {

class NetXX : public cocos2d::Ref
{
public:
    NetXX();
    ~NetXX();

    static NetXX* getInstance(void);

    struct RespExpireInfo
    {
        long long expireTime; // milliseconds
        std::shared_ptr<deadline_timer> timer;
    };

    const std::string& getDeviceId() const { return this->deviceId; }

public:
    bool        init();
    void        shutdown();
    void        setLuaListener(int hLuaFunc);
    void        delLuaListener();
    void        setPauseReceive(bool flag);
    //解码包长度
    static bool decode_packet_length(const char* data, size_t datalen, int& len);
    
private:
    //收发错误，链接中断
    void        connect_lose_callback_t(int ec, const char* msg);
    //收到数据
    void        pdu_recv_callback_t(std::vector<char>&& packet);
    //每个包发送通知
    void        threadsafe_callbacjk_t(const vdcallback_t& f);
    void        connect_callback_t(bool succeed, int error);
    void        dispatch_received_pdu(float dt);

public:
    // set endpoint of server.
    void        setEndpoint(const char* address, const char* addressv6, u_short port);
    void        startConnect(void);
    void        reConnect();
    
    //must>=0, -1 maybe error
    void        setConnectWaitTimeout(long milliseconds = 1000);
    //send timeout
    void        setSendTimeout(long seconds = 30);

    /// send any message
    //sid sessionid
    bool        sendMsg(std::string& msg, int sid = -1);
    bool        sendMsg_ex(std::string& msg, int sid = -1);

private:
    bool        removeExpireChecker(int sid);
    void        createExpireChecker(int sid, const std::function<void()>& expiredCallback=nullptr);
    bool        isRespExpired(int sid) const;
    void        removeAllExpireCheckers(void);
    
    void        logHex(const char* protocol, const char* hex, size_t len);
    
private:
    void        handleClose();
    void        reset(void);
    
    
    void        dispatch_received_pkg(std::vector<char>&& packet);

    void        dispatchNoRespMessage(int sid,int state);
    
    void        notifyLua(int evt,int status,std::string&& data,int sid);
    
    void        encrypt(unsigned char * buffer, unsigned short len, unsigned char key);
    
    void        decrypt(unsigned char * buffer, unsigned short len, unsigned char key);
    

private:
    int         luacallback;
    
    std::string                             deviceId;
    
    std::unordered_map<int, RespExpireInfo>  tRespExpireThreshold;
    
    int                                     responseTimeout;

    bool                                    connecting;

	bool                                    blocking = true;

    bool                                    shutingManually = false;

    int                                     lastErrorNumber = 0;
    //sometime maybe used to delaytime
    bool                                    pauseReceive = false;

};
    
}
#endif

