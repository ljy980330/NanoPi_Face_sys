//
//  FaceDetect.hpp
//  FaceppApiLib
//
//  Created by Li Cheng on 2018/9/29.
//  Copyright 漏 2018骞?Li Cheng. All rights reserved.
//

#ifndef FaceDetect_hpp
#define FaceDetect_hpp

#include <iostream>

class FaceDetectApi {
public:
    std::string detect(const char *api_key,const char *api_secret,const char * filePath);
};


#endif /* FaceDetect_hpp */
