//
//  FaceSetUpdate.hpp
//  FaceppApiLib
//
//  Created by Li Cheng on 2018/9/30.
//  Copyright 漏 2018骞?Li Cheng. All rights reserved.
//

#ifndef FaceSetUpdate_hpp
#define FaceSetUpdate_hpp

#include <stdio.h>
#include <iostream>

class FaceSetUpdate{
public:
    std::string update(const char *api_key ,const char *api_secret,const char *faceset_token, const char* new_name);
};

#endif /* FaceSetUpdate_hpp */
