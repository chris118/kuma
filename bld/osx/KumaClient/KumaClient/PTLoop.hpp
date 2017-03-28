//
//  PTLoop.hpp
//  KumaClient
//
//  Created by xiaopeng on 2017/3/28.
//  Copyright © 2017年 putao. All rights reserved.
//

#ifndef PTLoop_hpp
#define PTLoop_hpp

#include <stdio.h>
#include <thread>
#include <mutex>
#include <map>
#include <memory>
#include "kmapi.h"

using namespace kuma;

namespace pthttp {
    typedef enum {
        GET,
        POST
    } RequestType;
    
    class PTObject
    {
    public:
        virtual ~PTObject() {}
        virtual int close() = 0;
    };
    
    class PTObjectManager
    {
    public:
        virtual ~PTObjectManager() {}
        virtual void addObject(long conn_id, PTObject* obj) = 0;
        virtual void removeObject(long conn_id) = 0;
        virtual EventLoop* getEventLoop() = 0;
    };
    
    class PTLoopPool;
    class PTLoop : public PTObjectManager
    {
    public:
        PTLoop(PTLoopPool* pool, PollType poll_type = PollType::NONE);
        
        bool init();
        void stop();
        
        void addObject(long conn_id, PTObject* obj) override;
        void removeObject(long conn_id) override;
        EventLoop* getEventLoop() override { return loop_.get(); }
        
    private:
        void cleanup();
        void run();
        
    private:
        typedef std::map<long, PTObject*> ObjectMap;
        
        std::unique_ptr<EventLoop>  loop_;
        PTLoopPool*       pool_;
        std::mutex      obj_mutex_;
        ObjectMap       obj_map_;
        std::thread     thread_;
    };
}

#endif /* PTLoop_hpp */

