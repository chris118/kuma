//
//  HttpClient.cpp
//  KumaClient
//
//  Created by xiaopeng on 2017/3/27.
//  Copyright © 2017年 putao. All rights reserved.
//

#include "PTHttpClient.hpp"

HttpClient::HttpClient()
:loop_(new EventLoop(PollType::NONE))
,http_request_(loop_.get(), "HTTP/1.1")
,thread_()
{
    init();
}

HttpClient::~HttpClient(){
    printf("~HttpClient()\n");
}


bool HttpClient::init()
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

void HttpClient::run()
{
    if(!loop_->init()) {
        printf("TestLoop::run, failed to init EventLoop\n");
        return;
    }
    loop_->loop();
}

void HttpClient::Request(const std::string& url){
    loop_->async([=] {
        http_request_.setDataCallback([this] (void* data, size_t len) { onData(data, len); });
        http_request_.setWriteCallback([this] (KMError err) { onSend(err); });
        http_request_.setErrorCallback([this] (KMError err) { onClose(err); });
        http_request_.setHeaderCompleteCallback([this] { onHeaderComplete(); });
        http_request_.setResponseCompleteCallback([this] { onRequestComplete(); });
        //    if (url.find("/testdata") != std::string::npos) {
        //        http_request_.addHeader("Content-Length", 128*1024*1024);
        //        http_request_.sendRequest("POST", url.c_str());
        //    } else {
        //        http_request_.sendRequest("GET", url.c_str());
        //    }
        http_request_.sendRequest("GET", url.c_str());
    });
}

int HttpClient::close()
{
    http_request_.close();
    return 0;
}

void HttpClient::onData(void* data, size_t len)
{
    printf("%s", data);
    total_bytes_read_ += len;
    //printf("HttpClient_%ld::onData, len=%zu, total=%zu\n", conn_id_, len, total_bytes_read_);
}

void HttpClient::onSend(KMError err)
{
    uint8_t buf[16*1024];
    memset(buf, 'a', sizeof(buf));
    while (true) {
        int ret = http_request_.sendData(buf, sizeof(buf));
        if (ret < 0) {
            break;
        } else if (ret < sizeof(buf)) {
            // should buffer remain data
            break;
        }
    }
}

void HttpClient::onClose(KMError err)
{
    //printf("HttpClient_%ld::onClose, err=%d\n", conn_id_, err);
    http_request_.close();
}

void HttpClient::onHeaderComplete()
{
    //printf("HttpClient_%ld::onHeaderComplete\n", conn_id_);
}

void HttpClient::onRequestComplete()
{
    static std::atomic_int req_count{0};
    //printf("HttpClient_%ld::onRequestComplete, total=%zu, count=%d\n", conn_id_, total_bytes_read_, ++req_count);
    http_request_.close();
}
