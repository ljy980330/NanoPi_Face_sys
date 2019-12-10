//
//  FaceSetUpdate.cpp
//  FaceppApiLib
//
//  Created by Li Cheng on 2018/9/30.
//  Copyright 漏 2018骞?Li Cheng. All rights reserved.
//

#include "FaceSetUpdate.hpp"
#include "CurlPost.hpp"
#include "curl.h"

std::string FaceSetUpdate::update(const char *api_key, const char *api_secret, const char *faceset_token, const char *new_name) {

    const char *URL = "https://api-cn.faceplusplus.com/facepp/v3/faceset/update";
    
    CurlPost curlPost = CurlPost();
    map<const char *, const char *> params;
    params.insert(map<const char *, const char *>::value_type("api_key", api_key));
    params.insert(map<const char *, const char *>::value_type("api_secret", api_secret));
    params.insert(map<const char *, const char *>::value_type("faceset_token", faceset_token));
     params.insert(map<const char *, const char *>::value_type("display_name", new_name));
    return curlPost.doPost(URL, params);
}
