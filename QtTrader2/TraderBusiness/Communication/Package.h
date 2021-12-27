#ifndef COMMUNICATION_Package_H
#define COMMUNICATION_Package_H

#include <QSharedPointer>
#include <QByteArray>
#include <QString>
#include <QVector>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDomDocument>
#include <QXmlSimpleReader>
#include <QXmlStreamReader>

#include "Common/Parameter.h"
#include "Common/Constants/CommunicationConstants.h"
#include "Common/LogHelper.h"
#include "Common/AppSetting.h"
#include "Common/Constants/QuotationConstants.h"

#define HEADLENGTH 8
/*
一、包头：
2、总共8个字节
2、第一个字节存放包的类型，包括Request、Response、Price、Notify、KeepAlive，其值分别为1，2，3，4，5
3、第二个字节存放包内容部分的数据格式，本字节一到四位用来标识包内容是否压缩，值为1，表示包内容有压缩，压缩算法为ZIP
五到八位为包内容的格式，包括Json、XML、Plain，其值分别为1，2，3
4、第三个字节代表session的长度，
5、第四个字节代表RequestId的长度
6、第五到第八字节代表内容的长度，不包括包头及session和RequestId的长度

二、包体：
依次包括如下几部分
session（若有）
RequestId(若有)
内容
*/

Q_DECLARE_METATYPE(QDomElement)

namespace Communication
{
	class Package
	{
	public:
		enum Type
		{
			Request = 0x01,
			Response = 0x02,
			Price = 0x03,
			Notify = 0x04,
			KeepAlive = 0x05,
			KeepAliveSuccessed = 0x06,
			KeepAliveFailed = 0x07,
			ExceptionPackage = 0x08
		};

		enum Format
		{
			Json = 0x10,
			XML = 0x20,
			Plain = 0x30
		};


        Package(Type type, Format format, QString& sessionId, QString& requestId, QVariant& content);
        ~Package() { }
        static QSharedPointer<QByteArray> buildRequest(const QString& sessionId, const QString& requestId,
            const QString& methodName, const QVector<Common::Parameter>& parameters);
        static QSharedPointer<QByteArray> buildKeepAliveRequest(const QString& sessionId, const QString& requestId);
        static QSharedPointer<Package> from(QSharedPointer<QByteArray> rawData);

    public:
        const Type type() { return this->_type; }
        const Format format() { return this->_format; }
        const QString& sessionId() { return this->_sessionId; }
        const QString& requestId() { return this->_requestId; }
        const QVariant& content() { return this->_content; }


	private:
        static QVariant parse(Type packageType, Format format, bool isCompressed, const QByteArray& contentData);
		static QSharedPointer<QByteArray> buildRequest(Type type, const QString& sessionId, const QString& requestId, 
            const QByteArray& content = QByteArray());
        static QByteArray buildRequestContent(const QString& methodName, const QVector<Common::Parameter>& parameters);
        static QJsonValue qVariantToQJsonValue(const QVariant& qv);
        static QString Decode(QByteArray quotation);

    private:
		QString _sessionId;
		QString _requestId;
		Type _type;
		Format _format;
		QVariant _content;
	};
}
#endif
