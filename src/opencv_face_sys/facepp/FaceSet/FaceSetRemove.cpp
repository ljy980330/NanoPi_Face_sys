﻿//
//  FaceSetRemove.cpp
//  FaceppApiLib
//
//  Created by Li Cheng on 2018/9/30.
//  Copyright 漏 2018骞?Li Cheng. All rights reserved.
//

#include "FaceSetRemove.hpp"
#include "CurlPost.hpp"
#include "curl.h"

std::string FaceSetRemoveApi::removeFace(const char *api_key, const char *api_secret, const char *faceset_token, const char *face_tokens) {
    
    const char *URL = "https://api-cn.faceplusplus.com/facepp/v3/faceset/removeface";
    
    CurlPost curlPost = CurlPost();
    map<const char *, const char *> params;
    params.insert(map<const char *, const char *>::value_type("api_key", api_key));
    params.insert(map<const char *, const char *>::value_type("api_secret", api_secret));
    params.insert(map<const char *, const char *>::value_type("face_tokens", face_tokens));
    params.insert(map<const char *, const char *>::value_type("faceset_token", faceset_token));
    return curlPost.doPost(URL, params);
}
