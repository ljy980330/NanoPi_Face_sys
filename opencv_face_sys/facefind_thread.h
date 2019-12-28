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

    QString apiJsonRet;
    FaceppApi *faceApi;

    struct FaceSearchData faceSearch;

    bool json_analysis(QString str);
    struct FaceGetDetailResponse FaceGetDetail(QString json_data);

signals:
    void face_search_sig(struct FaceSearchData);

protected:
    void run();
};

#endif // FACEFIND_THREAD_H
