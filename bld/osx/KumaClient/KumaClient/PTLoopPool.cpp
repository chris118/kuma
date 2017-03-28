//
//  PTLoopPool.cpp
//  KumaClient
//
//  Created by xiaopeng on 2017/3/28.
//  Copyright © 2017年 putao. All rights reserved.
//

#include "PTLoopPool.hpp"
#include "PTLoop.hpp"
#include <string>

using namespace pthttp;


PTLoopPool::PTLoopPool()
: next_loop_(0)
//, id_seed_(0)
{
    id_seed_ = 0;
}

void PTLoopPool::cleanup()
{
    for (auto loop : loops_) {
        loop->stop();
        delete loop;
    }
    loops_.clear();
}

PTLoop* PTLoopPool::getNextLoop()
{
    PTLoop* loop = loops_[next_loop_];
    if(++next_loop_ >= loops_.size()) {
        next_loop_ = 0;
    }
    return loop;
}

bool PTLoopPool::init(int count, PollType poll_type)
{
    for (int i=0; i < count; ++i) {
        PTLoop* l = new PTLoop(this, poll_type);
        loops_.push_back(l);
        l->init();
    }
    return true;
}

void PTLoopPool::stop()
{
    cleanup();
}
