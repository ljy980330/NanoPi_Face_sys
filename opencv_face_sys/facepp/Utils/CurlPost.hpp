﻿//
//  CurlPost.hpp
//  FaceppApiLib
//
//  Created by Li Cheng on 2018/9/30.
//  Copyright 漏 2018骞?Li Cheng. All rights reserved.
//

#ifndef CurlPost_hpp
#define CurlPost_hpp

#include <iostream>
#include <map>

using namespace std;

class CurlPost {
    public :
    std::string doPost(const char * URL, map<const char* , const char*> params);
};

#endif /* CurlPost_hpp */
