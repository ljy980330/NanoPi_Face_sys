//
//  FaceSetSearch.cpp
//  FaceppApiLib
//
//  Created by Li Cheng on 2018/9/30.
//  Copyright 漏 2018骞?Li Cheng. All rights reserved.
//

#include "FaceSetSearch.hpp"
#include "CurlPost.hpp"
#include "curl.h"

std::string FaceSetSearch::search(const char *api_key, const char *api_secret, const char *filePath, const char *faceset_token) {
    
    const char *URL = "https://api-cn.faceplusplus.com/facepp/v3/search";
    
    if(NULL == filePath) {
        fprintf(stderr, "\n\n-------璇锋眰澶辫触-------\n %s \n\n", "file path can not be empty !");
        return "ERROR";
    }
    
    CurlPost curlPost = CurlPost();
    map<const char *, const char *> params;
    params.insert(map<const char *, const char *>::value_type("api_key", api_key));
    params.insert(map<const char *, const char *>::value_type("api_secret", api_secret));
    params.insert(map<const char *, const char *>::value_type("image_file", filePath));
    params.insert(map<const char *, const char *>::value_type("faceset_token", faceset_token));
    return curlPost.doPost(URL, params);
}

