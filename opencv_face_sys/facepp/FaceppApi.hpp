//
//  FaceppApi.hpp
//  FaceppApiLib
//
//  Created by Li Cheng on 2018/9/30.
//  Copyright 漏 2018骞?Li Cheng. All rights reserved.
//

#ifndef FaceppApi_hpp
#define FaceppApi_hpp

#include <stdio.h>
#include <string>
#include <iostream>

class FaceppApi {

    protected:
        const char *key; // Api_key
        const char *secret; // Api_secret
    public:
        FaceppApi(const char * key, const char * secret);
        
        std::string detect(const char * filePath);
        std::string getDetail(const char *face_token);

        std::string createFaceset(const char *name);
        std::string addFace(const char *faceset_token, const char* face_tokens);
        std::string getFaceSetDetail(const char *faceset_token);
        std::string search(const char *filePath, const char *faceset_token);
        std::string removeFace(const char *api_key, const char *api_secret, const char *faceset_token, const char *face_tokens);
        std::string updateFaceSet(const char *faceset_token, const char* new_name);
};

#endif /* FaceppApi_hpp */
