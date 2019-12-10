//
//  FaceSetRemove.hpp
//  FaceppApiLib
//
//  Created by Li Cheng on 2018/9/30.
//  Copyright 漏 2018骞?Li Cheng. All rights reserved.
//

#ifndef FaceSetRemove_hpp
#define FaceSetRemove_hpp

#include <stdio.h>
#include <iostream>

class FaceSetRemoveApi {
public:
    std::string removeFace(const char *api_key, const char *api_secret, const char *faceset_token, const char *face_tokens);
};

#endif /* FaceSetRemove_hpp */
