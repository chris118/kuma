//
//  HttpClient.hpp
//  KumaClient
//
//  Created by xiaopeng on 2017/3/27.
//  Copyright © 2017年 putao. All rights reserved.
//

#ifndef HttpClient_hpp
#define HttpClient_hpp

#include <stdio.h>
#include <string>
#include <thread>

#include "kuma/kmapi.h"

using namespace std;
using namespace kuma;

class HttpClient {

public:
    HttpClient();
    ~HttpClient();
    void Request(const std::string& url);
    int close();
    
    void onData(void* data, size_t len);
    void onSend(KMError err);
    void onClose(KMError err);
    void onHeaderComplete();
    void onRequestComplete();
    
private:
    bool init();
    void run();
    
private:
    std::unique_ptr<EventLoop> loop_;
    HttpRequest http_request_;
    size_t      total_bytes_read_ = 0;
    std::thread     thread_;
};
#endif /* HttpClient_hpp */
