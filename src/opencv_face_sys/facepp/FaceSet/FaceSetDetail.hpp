//
//  FaceSetDetail.hpp
//  FaceppApiLib
//
//  Created by Li Cheng on 2018/9/30.
//  Copyright 漏 2018骞?Li Cheng. All rights reserved.
//

#ifndef FaceSetDetail_hpp
#define FaceSetDetail_hpp

#include <stdio.h>
#include <iostream>

class FaceSetDetailApi{
public:
    std::string getDetail(const char *api_key ,const char *api_secret,const char *faceset_token);
};

#endif /* FaceSetDetail_hpp */
