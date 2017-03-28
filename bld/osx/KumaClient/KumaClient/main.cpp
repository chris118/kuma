//
//  main.cpp
//  KumaClient
//
//  Created by xiaopeng on 2017/3/27.
//  Copyright © 2017年 putao. All rights reserved.
//

#include <iostream>
#include <chrono>
#include <thread>
#include "PTHttpClient.hpp"

int main(int argc, const char * argv[]) {
    
    HttpClient client;
    
    client.Request("http://news-at.zhihu.com/api/4/news/latest");
    
    
    while (true) {
        std::chrono::milliseconds dura( 1000 );
        std::this_thread::sleep_for( dura );
    }
    return 0;
}
