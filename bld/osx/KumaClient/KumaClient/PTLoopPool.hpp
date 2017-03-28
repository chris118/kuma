//
//  PTLoopPool.hpp
//  KumaClient
//
//  Created by xiaopeng on 2017/3/28.
//  Copyright © 2017年 putao. All rights reserved.
//

#ifndef PTLoopPool_hpp
#define PTLoopPool_hpp

#include <stdio.h>
#include <thread>
#include <vector>
#include <map>
#include <atomic>

#include "kmapi.h"

using namespace kuma;

namespace pthttp {
    class PTLoop;
    class PTLoopPool
    {
    public:
        PTLoopPool();
        
        bool init(int count, PollType poll_type = PollType::NONE);
        void stop();
        
        long getConnId() { return ++id_seed_; }
        PTLoop* getNextLoop();
        
    private:
        void cleanup();
        
    private:
        std::vector<PTLoop*> loops_;
        int             next_loop_;
        std::atomic_long id_seed_;
    };
}

#endif /* PTLoopPool_hpp */

