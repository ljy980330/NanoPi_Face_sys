//
//  FaceppApi.cpp
//  FaceppApiLib
//
//  Created by Li Cheng on 2018/9/30.
//  Copyright Â© 2018å¹?Li Cheng. All rights reserved.
//

#include "FaceppApi.hpp"
#include "Face/FaceDetect.hpp"
//#include "Face/FaceCompare.hpp"
//#include "Face/FaeBeautify.hpp"
//#include "Face/FaceAnalyze.hpp"
#include "Face/FaceGetDetail.hpp"
//#include "Face/FaceSetUserId.hpp"

#include "FaceSet/FaceSetCreate.hpp"
#include "FaceSet/FaceSetAdd.hpp"
#include "FaceSet/FaceSetRemove.hpp"
#include "FaceSet/FaceSetSearch.hpp"
//#include "FaceSet/FaceSetAddFaceAsync.hpp"
//#include "FaceSet/FaceSetRemoceFaceAsync.hpp"
#include "FaceSet/FaceSetDetail.hpp"
#include "FaceSet/FaceSetUpdate.hpp"
//#include "FaceSet/FaceSetDelete.hpp"
//#include "FaceSet/FaceSetList.hpp"
//#include "FaceSet/FaceSetGetTaskStatus.hpp"

#if 0
#include "HumanBody/HumanBodyDetect.hpp"
#include "HumanBody/HumanBodySkeleton.hpp"
#include "HumanBody/HumanBodySegment.hpp"
#include "HumanBody/Guesture.hpp"

#include "OCR/OCRIDCard.hpp"
#include "OCR/OCRDriverLicense.hpp"
#include "OCR/OCRVehicle.hpp"
#include "OCR/OCRBankCard.hpp"

#include "ImagePP/ImageLicenseplate.hpp"
#include "ImagePP/MergeFace.hpp"
#include "ImagePP/RecognizeText.hpp"
#include "ImagePP/SceneAndObject.hpp"
#endif

#include "CurlPost.hpp"
#include <string>
#include <iostream>

FaceppApi::FaceppApi(const char * key, const char * secret){
    this->key = key;
    this->secret = secret;
}

/**
 *
 */
std::string FaceppApi::detect(const char *filePath) {
    FaceDetectApi detectApi = FaceDetectApi();
    return detectApi.detect(key, secret, filePath);
}

std::string FaceppApi::getDetail(const char *face_token) {
    FaceGetDetailApi detailApi = FaceGetDetailApi();
    return detailApi.getDetail(key, secret, face_token);
}

// FaceSet Api ç»?
std::string FaceppApi::createFaceset(const char *name) {
    FaceSetCreateApi createApi = FaceSetCreateApi();
    return createApi.createFaceset(key, secret, name);
}

std::string FaceppApi::addFace(const char *faceset_token, const char* face_tokens) {
    FaceSetAddFaceApi api = FaceSetAddFaceApi();
    return api.addFace(key, secret, faceset_token, face_tokens);
}

std::string FaceppApi::removeFace(const char *api_key, const char *api_secret, const char *faceset_token, const char *face_tokens) {
    FaceSetRemoveApi api = FaceSetRemoveApi();
    return api.removeFace(key, secret, faceset_token, face_tokens);
}

std::string FaceppApi::getFaceSetDetail(const char *faceset_token) {
    FaceSetDetailApi api = FaceSetDetailApi();
    return api.getDetail(key, secret, faceset_token);
}

std::string FaceppApi::search(const char *filePath, const char *faceset_token) {
    FaceSetSearch api = FaceSetSearch();
    return api.search(key, secret, filePath, faceset_token);
}

std::string FaceppApi::updateFaceSet(const char *faceset_token, const char* new_name) {
    FaceSetUpdate api = FaceSetUpdate();
    return api.update(key, secret, faceset_token, new_name);
}

