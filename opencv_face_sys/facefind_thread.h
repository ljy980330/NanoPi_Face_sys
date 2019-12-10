#ifndef FACEFIND_THREAD_H
#define FACEFIND_THREAD_H

#include <QObject>
#include "includes.h"

class FaceFind_thread : public QThread
{
    Q_OBJECT
public:
    explicit FaceFind_thread(QObject *parent = 0);
    ~FaceFind_thread();

    QString faceImagePath = "./face.jpg";
    const char *APIkey = "jnKErmEcRWpMI1UHB2rNZF10AUxss_8-";
    const char *APIsecre = "1wvLR6qXsPrhQHAwmv3ekjM-JbN8b9hH";
    const char *FaceSetToken = "ce5a1743232bd1763f0b15772e9ce0e0";

    QString apiJsonRet;
    FaceppApi *faceApi;

    struct FaceSearchData faceSearch;

    bool json_analysis(QString str);

signals:
    void face_search_sig(struct FaceSearchData);

protected:
    void run();
};

#endif // FACEFIND_THREAD_H
