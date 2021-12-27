#include "Package.h"

namespace Communication
{
    Package::Package(Type type, Format format, QString& sessionId, QString& requestId, QVariant& content)
    {
        this->_type = type;
        this->_format = format;
        this->_sessionId = sessionId;
        this->_requestId = requestId;
        this->_content = content;
    }

//----------------------------------------public method----------------------------------------------------------//
    QSharedPointer<QByteArray> Package::buildRequest(const QString& sessionId, const QString& requestId,
               const QString& methodName, const QVector<Common::Parameter>& parameters)
    {
       QByteArray content = Package::buildRequestContent(methodName, parameters);
       return Package::buildRequest(Request, sessionId, requestId, content);
    }

    QSharedPointer<QByteArray> Package::buildKeepAliveRequest(const QString& sessionId, const QString& requestId)
    {
        return Package::buildRequest(KeepAlive, sessionId, requestId);
    }

    QSharedPointer<Package> Package::from(QSharedPointer<QByteArray> rawData)
    {
       Type packageType = (Type)(rawData->at(0));
       bool isCompressed = (rawData->at(1) & 0x0F) == 0x01;
       Format format = (Format)(rawData->at(1) & 0xF0);
       int sessionLength = rawData->at(2);
       int requestIdLength = rawData->at(3);

       QString session, requestId;
       if(sessionLength > 0)
       {
           session = QString::fromUtf8(rawData->constData() + HEADLENGTH, sessionLength);
       }

       if(requestIdLength > 0)
       {
           requestId = QString::fromUtf8(rawData->constData() + HEADLENGTH + sessionLength, requestIdLength);
       }

       QByteArray contentData;
       int beginIndex = HEADLENGTH + sessionLength + requestIdLength;
       int contentLength = rawData->size() - beginIndex;
       if(contentLength >= 0)
       {
           if(isCompressed)
           {
               //默认长度为压缩后的长度的10倍
               contentData.reserve(contentLength + 4);
               int len = 12 * contentLength;
               contentData.append( (unsigned int)((len >> 24) & 0xFF));
               contentData.append( (unsigned int)((len >> 16) & 0xFF));
               contentData.append( (unsigned int)((len >> 8) & 0xFF));
               contentData.append( (unsigned int)((len >> 0) & 0xFF));
               contentData.append(rawData->constData() + beginIndex, contentLength);
           }
           else
           {
               contentData.reserve(contentLength);
               contentData.append(rawData->constData() + beginIndex, contentLength);
           }
//           if(packageType != Price)
//           {
//               if(packageType == Notify && Common::AppSetting::getInstance()->getWriteLog())
//               {
//                   QString contentStr = contentData;
//                   QLOG_INFO(Common::Constants::LogKeyConstants::Communication) << "content:" <<  contentStr;
//               }
//           }
           QVariant content = Package::parse(packageType, format, isCompressed, contentData);
           return QSharedPointer<Package>(new Package(packageType, format, session, requestId, content));
       }
       else
       {
           QVariant content;
           return QSharedPointer<Package>(new Package(ExceptionPackage, format, session, requestId, content));
       }
    }


//----------------------------------------private method----------------------------------------------------------//
    QVariant Package::parse(Type packageType, Format format, bool isCompressed, const QByteArray& contentData)
    {
        QByteArray data = isCompressed ? qUncompress(contentData) : contentData;

        if(format == Json)
        {
            if(Common::AppSetting::getInstance()->getWriteLog())
                QLOG_INFO(Common::Constants::LogKeyConstants::Communication) << " Package::parse Json content: " << data;
            QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
            return QVariant::fromValue(jsonDoc.object());
        }
        else if(format == XML)
        {
            if(packageType == Price)
            {
                QString contentStr = Decode(data);
                return QVariant::fromValue(contentStr);
            }
            else
            {
                bool isWriteLog = false;
                #ifdef RUNTIME_OS_ANDROID
                    isWriteLog = true;
                #endif
                #ifdef RUNTIME_OS_IPHONE
                    isWriteLog = true;
                #endif
                #ifdef RUNTIME_OS_WIN32
                    if(Common::AppSetting::getInstance()->getWriteLog())
                        isWriteLog = true;
                #endif
                #ifdef RUNTIME_OS_MACX
                    if(Common::AppSetting::getInstance()->getWriteLog())
                        isWriteLog = true;
                #endif
                if(isWriteLog)
                {
                    QString contentStr = data;
                    QLOG_INFO(Common::Constants::LogKeyConstants::Communication)<< " Package::parse XML content:" <<  contentStr;
                }
                QDomDocument doc;
                doc.setContent(data);
                QDomElement contentXml =  doc.documentElement().toElement();

                //QDomElement contentXml = contentXml.childNodes().at(0).toElement();//Get the content in the content_result node
                if(contentXml.nodeName().toLower() == "result")
                {
                    QDomElement content_result = contentXml.firstChildElement("content_result");
                    if(!content_result.isNull())
                    {
                        //Get the content in the content_result node
                        return QVariant::fromValue(content_result);
                    }
                    else
                    {
                       return QVariant::fromValue(contentXml);
                    }
                }
                else if(contentXml.nodeName().toLower() == "commands")
                {
                    return QVariant::fromValue(contentXml.childNodes().at(0).toElement());//Get the content in the content_result node
                }
                else
                {
                    return QVariant::fromValue(contentXml);
                }
            }
        }
        else
        {
            bool isWriteLog = false;
            #ifdef RUNTIME_OS_ANDROID
                isWriteLog = true;
            #endif
            #ifdef RUNTIME_OS_IPHONE
                isWriteLog = true;
            #endif
            #ifdef RUNTIME_OS_WIN32
                if(Common::AppSetting::getInstance()->getWriteLog())
                    isWriteLog = true;
            #endif
            #ifdef RUNTIME_OS_MACX
                if(Common::AppSetting::getInstance()->getWriteLog())
                    isWriteLog = true;
            #endif
            if(isWriteLog)
            {
                QString contentStr = data;
                QLOG_INFO(Common::Constants::LogKeyConstants::Communication)<< "Package::parse format:"<< format << " content:" <<  contentStr;
            }
            return QVariant::fromValue(data);
        }

        return QVariant();
    }

    QSharedPointer<QByteArray> Package::buildRequest(Type type, const QString& sessionId, const QString& requestId, const QByteArray& content)
    {
        QByteArray session = sessionId.toUtf8();
        QByteArray request = requestId.toUtf8();

        QSharedPointer<QByteArray> result = QSharedPointer<QByteArray>(new QByteArray());
        result->reserve(HEADLENGTH  + session.size() + request.size() + content.size());

        result->append(type);
        result->append(Json);
        result->append((char)(session.size()));
        result->append((char)(request.size()));
        int contentsize =  content.size();
        for (int i = 0; i<4;i++)
        {
            //result->append((contentsize >> 8*i & 0xff));
            result->append((contentsize >> 8*i));
        }
        if(session.size() != 0)
            result->append(session);
        if(request.size() != 0)
            result->append(request);
        result->append(content);

        return result;
    }


    QByteArray Package::buildRequestContent(const QString& methodName, const QVector<Common::Parameter>& parameters)
    {
        QJsonObject args;
        for(int i=0; i<parameters.count(); i++)
        {
            QJsonValue value = Package::qVariantToQJsonValue(parameters[i]._parameterValue);
            args.insert(parameters[i]._parameterName, value);
        }

        QJsonObject packet;
        packet.insert(Common::Constants::RequestConstants::MethodNodeName, methodName);
        packet.insert(Common::Constants::RequestConstants::ArgumentsNodeName, args);

        QJsonDocument jsonDoc;
        jsonDoc.setObject(packet);
        return jsonDoc.toJson(QJsonDocument::Compact);
    }

    QJsonValue Package::qVariantToQJsonValue(const QVariant& qv)
    {
        QJsonValue qJsonValue;
        QVariant::Type qvType = qv.type();
        if(qvType==QVariant::String)
        {
            qJsonValue = qv.toString();
        }
        else if(qvType==QVariant::Bool)
        {
            qJsonValue = qv.toBool();
        }
        else if(qvType==QVariant::Int)
        {
            qJsonValue = qv.toInt();
        }
        else if(qvType==QVariant::Double)
        {
            qJsonValue = qv.toDouble();
        }
        else if(qvType==QMetaType::QJsonArray)
        {
            qJsonValue = qv.toJsonArray();
        }
        return qJsonValue;
    }


    QString Package::Decode(QByteArray quotation)
    {
        QString quotationStr;
        foreach (auto data, quotation)
        {
            char value = (char)((data & 0XF0) / 16);
            quotationStr.append(Common::Constants::QuotationConstants::_BitsToChar[value - 1]);
            value = (char)(data & 0X0F);
            if (value != 0) quotationStr.append(Common::Constants::QuotationConstants::_BitsToChar[value - 1]);
        }
        return quotationStr.mid(0, quotationStr.length()-1);
    }




}
