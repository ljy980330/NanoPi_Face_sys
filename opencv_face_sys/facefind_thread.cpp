#include "facefind_thread.h"

FaceFind_thread::FaceFind_thread(QObject *parent) : QThread(parent)
{
    faceApi = new FaceppApi(APIkey,APIsecre);
}

FaceFind_thread::~FaceFind_thread()
{

}

void FaceFind_thread::run()
{
    apiJsonRet = QString::fromStdString(faceApi->search(faceImagePath.toLatin1().data(),FaceSetToken));
    if(apiJsonRet == "ERROR")
    {
        qDebug() << "NetWork ERROR!!";
        faceSearch.error_message = "NetWork ERROR";

    }
    else if(json_analysis(apiJsonRet) == false)
    {
        qDebug() << "Error Message: " << faceSearch.error_message;
    }

    emit face_search_sig(faceSearch);
}

bool FaceFind_thread::json_analysis(QString str)
{
    QByteArray block;
    block = str.toLatin1();
    //qDebug() << "接收 : " << block;
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(block, &json_error);

    //qDebug() << "parse_doucment : " << parse_doucment;
    //下面是json格式的解析
    if(json_error.error == QJsonParseError::NoError)
    {
        if(parse_doucment.isObject())
        {
            //开始解析json对象
            QJsonObject jsonObject = parse_doucment.object();
            qDebug() << jsonObject;
            if(jsonObject.contains("error_message"))
            {
                faceSearch.error_message = jsonObject.take("error_message").toString();
                return false;
            }
            else
            {
                faceSearch.error_message.clear();
            }

            if(jsonObject.contains("time_used"))
            {
                faceSearch.time_used = jsonObject.take("time_used").toInt();
            }

            if(jsonObject.contains("request_id"))
            {
                faceSearch.request_id = jsonObject.take("request_id").toString();
            }

            if(jsonObject.contains("thresholds"))
            {
                QJsonValue thresholds_value = jsonObject.take("thresholds");
                QJsonObject thresholds_object = thresholds_value.toObject();
                if(thresholds_object.contains("1e-3"))
                {
                    faceSearch.thresholds.thresholds_3 = thresholds_object.take("1e-3").toDouble();
                }
                if(thresholds_object.contains("1e-4"))
                {
                    faceSearch.thresholds.thresholds_4 = thresholds_object.take("1e-4").toDouble();
                }
                if(thresholds_object.contains("1e-5"))
                {
                    faceSearch.thresholds.thresholds_5 = thresholds_object.take("1e-5").toDouble();
                }
            }
            else
            {
                faceSearch.thresholds.thresholds_3 = 100;
                faceSearch.thresholds.thresholds_4 = 100;
                faceSearch.thresholds.thresholds_5 = 100;
            }

            if(jsonObject.contains("results"))
            {
                QJsonValue results_value = jsonObject.take("results");
                if(results_value.isArray()) //判断他是不是json数组
                {
                    QJsonArray results_array = results_value.toArray();
                    QJsonObject results_object = results_array.at(0).toObject();

                    if(results_object.contains("confidence"))
                    {
                        faceSearch.results.confidence = results_object.take("confidence").toDouble();
                    }
                    else
                    {
                        faceSearch.results.confidence = 0;
                    }

                    if(results_object.contains("face_token"))
                    {
                        faceSearch.results.face_token = results_object.take("face_token").toString();
                    }
                    else
                    {
                        faceSearch.results.face_token.clear();
                    }
                }
            }
            else
            {
                faceSearch.results.confidence = 0;
                faceSearch.results.face_token.clear();
            }
        }
    }
    return true;
}

