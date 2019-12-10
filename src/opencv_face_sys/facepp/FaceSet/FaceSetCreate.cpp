//
//  FaceSetCreate.cpp
//  FaceppApiLib
//
//  Created by Li Cheng on 2018/9/30.
//  Copyright 漏 2018骞?Li Cheng. All rights reserved.
//b

#include "FaceSetCreate.hpp"
#include "CurlPost.hpp"
#include "curl.h"

std::string FaceSetCreateApi::createFaceset(const char *api_key, const char *api_secret, const char *display_name) {
    
    const char *URL = "https://api-cn.faceplusplus.com/facepp/v3/faceset/create";
    
    CurlPost curlPost = CurlPost();
    map<const char *, const char *> params;
    params.insert(map<const char *, const char *>::value_type("api_key", api_key));
    params.insert(map<const char *, const char *>::value_type("api_secret", api_secret));
    params.insert(map<const char *, const char *>::value_type("display_name", display_name));
    return curlPost.doPost(URL, params);
}
