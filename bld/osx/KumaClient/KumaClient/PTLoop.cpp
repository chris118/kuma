//
//  PTLoop.cpp
//  KumaClient
//
//  Created by xiaopeng on 2017/3/28.
//  Copyright © 2017年 putao. All rights reserved.
//

#include "PTLoop.hpp"
#include "PTLoopPool.hpp"

using namespace pthttp;

PTLoop::PTLoop(PTLoopPool* pool, PollType poll_type)
: loop_(new EventLoop(poll_type))
, pool_(pool)
, thread_()
{
    
}

void PTLoop::cleanup()
{
    std::lock_guard<std::mutex> lg(obj_mutex_);
    for (auto &kv : obj_map_) {
        kv.second->close();
        delete kv.second;
    }
    obj_map_.clear();
}

bool PTLoop::init()
{
    try {
        thread_ = std::thread([this] {
            run();
        });
    }
    catch(...)
    {
        return false;
    }
    return true;
}

void PTLoop::stop()
{
    //cleanup();
    if(loop_) {
        loop_->async([this] { cleanup(); });
        loop_->stop();
    }
    if(thread_.joinable()) {
        try {
            thread_.join();
        } catch (...) {
            printf("failed to join loop thread\n");
        }
    }
}

void PTLoop::run()
{
    if(!loop_->init()) {
        printf("PTLoop::run, failed to init EventLoop\n");
        return;
    }
    loop_->loop();
}

void PTLoop::addObject(long conn_id, PTObject* obj)
{
    std::lock_guard<std::mutex> lg(obj_mutex_);
    obj_map_.insert(std::make_pair(conn_id, obj));
}

void PTLoop::removeObject(long conn_id)
{
    printf("PTLoop::removeObject, conn_id=%ld\n", conn_id);
    std::lock_guard<std::mutex> lg(obj_mutex_);
    auto it = obj_map_.find(conn_id);
    if(it != obj_map_.end()) {
        delete it->second;
        obj_map_.erase(it);
    }
}
