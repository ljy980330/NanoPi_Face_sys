//
//  FaceSetAdd.hpp
//  FaceppApiLib
//
//  Created by Li Cheng on 2018/9/30.
//  Copyright 漏 2018骞?Li Cheng. All rights reserved.
//

#ifndef FaceSetAdd_hpp
#define FaceSetAdd_hpp

#include <stdio.h>
#include <iostream>

class FaceSetAddFaceApi {
public:
    std::string addFace(const char *api_key ,const char *api_secret,const char *faceset_token, const char* face_tokens);
};

#endif /* FaceSetAdd_hpp */
