#-------------------------------------------------
#
# Project created by QtCreator 2019-05-13T13:14:50
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = opencv_face_sys
TEMPLATE = app
CONFIG += c++11

if(contains(DEFINES,LINUX_X86_64)){
    message("compile for Linux X86_64")
    INCLUDEPATH +=/usr/local/include \
        /usr/local/include/opencv \
        /usr/local/include/opencv2 \
        ./facepp/ \
        ./facepp/curl \
        ./facepp/Face \
        ./facepp/FaceSet \
        ./facepp/Utils
    LIBS += /usr/local/lib/libopencv_* \
        /usr/lib/x86_64-linux-gnu/libcurl*.so*
}

if(contains(DEFINES,AARCH64)){
    message("compile for aarch64")
    INCLUDEPATH +=/usr/local/include \
        /usr/local/include/opencv \
        /usr/local/include/opencv2 \
        ./facepp/ \
        ./facepp/curl \
        ./facepp/Face \
        ./facepp/FaceSet \
        ./facepp/Utils
    LIBS += /usr/local/lib/libopencv_* \
        /usr/lib/aarch64-linux-gnu/libcurl*.so*
}

SOURCES += main.cpp\
        mainwidget.cpp \
    welcomewidget.cpp \
    camerwidget.cpp \
    weatherwidget.cpp \
    sysinfo_thread.cpp \
    camerinfo_thread.cpp \
    sysinfowidget.cpp \
    qgraph.cpp \
    camershowwidget.cpp \
    facepp/Face/FaceDetect.cpp \
    facepp/FaceSet/FaceSetAdd.cpp \
    facepp/FaceSet/FaceSetCreate.cpp \
    facepp/FaceSet/FaceSetDetail.cpp \
    facepp/FaceSet/FaceSetRemove.cpp \
    facepp/FaceSet/FaceSetSearch.cpp \
    facepp/FaceSet/FaceSetUpdate.cpp \
    facepp/Utils/CurlPost.cpp \
    facepp/FaceppApi.cpp \
    facefind_thread.cpp

HEADERS  += mainwidget.h \
    includes.h \
    welcomewidget.h \
    camerwidget.h \
    weatherwidget.h \
    sysinfo_thread.h \
    camerinfo_thread.h \
    sysinfowidget.h \
    qgraph.h \
    camershowwidget.h \
    facepp/curl/curl.h \
    facepp/curl/curlver.h \
    facepp/curl/easy.h \
    facepp/curl/mprintf.h \
    facepp/curl/multi.h \
    facepp/curl/stdcheaders.h \
    facepp/curl/system.h \
    facepp/curl/typecheck-gcc.h \
    facepp/Face/FaceDetect.hpp \
    facepp/FaceSet/FaceSetAdd.hpp \
    facepp/FaceSet/FaceSetCreate.hpp \
    facepp/FaceSet/FaceSetDetail.hpp \
    facepp/FaceSet/FaceSetRemove.hpp \
    facepp/FaceSet/FaceSetSearch.hpp \
    facepp/FaceSet/FaceSetUpdate.hpp \
    facepp/Utils/CurlPost.hpp \
    facepp/FaceppApi.hpp \
    facefind_thread.h

FORMS    += mainwidget.ui \
    welcomewidget.ui \
    camerwidget.ui \
    weatherwidget.ui \
    sysinfowidget.ui

RESOURCES +=

DISTFILES += \
    haarcascades/haarcascade_eye.xml \
    haarcascades/haarcascade_eye_tree_eyeglasses.xml \
    haarcascades/haarcascade_frontalcatface.xml \
    haarcascades/haarcascade_frontalcatface_extended.xml \
    haarcascades/haarcascade_frontalface_alt.xml \
    haarcascades/haarcascade_frontalface_alt2.xml \
    haarcascades/haarcascade_frontalface_alt_tree.xml \
    haarcascades/haarcascade_frontalface_default.xml \
    haarcascades/haarcascade_fullbody.xml \
    haarcascades/haarcascade_lefteye_2splits.xml \
    haarcascades/haarcascade_licence_plate_rus_16stages.xml \
    haarcascades/haarcascade_lowerbody.xml \
    haarcascades/haarcascade_profileface.xml \
    haarcascades/haarcascade_righteye_2splits.xml \
    haarcascades/haarcascade_russian_plate_number.xml \
    haarcascades/haarcascade_smile.xml \
    haarcascades/haarcascade_upperbody.xml \
    data/a645d4d0d6045837065633b722e94660 \
    data/dccf66097ecb1c0a5c8accec3ee91c46 \
    data/Key \
    data/name
