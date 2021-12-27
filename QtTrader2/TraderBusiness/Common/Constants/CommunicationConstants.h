#ifndef COMMUNICATIONCONSTANTS_H
#define COMMUNICATIONCONSTANTS_H

#include <QString>

namespace Common
{
	namespace Constants
	{
		class ResponseConstants
		{
		public:
			static const  QString JsonErrorNodeName;
			static const  QString XmlErrorNodeName;
			static const  QString ResultNodeName;
			static const  QString InvokeIdNodeName;
		};

		class RequestConstants
		{
		public:
			static const QString  ArgumentsNodeName;
			static const QString  InvokeIdNodeName;
			static const QString MethodNodeName;

			static const QString AppType;
            static const QString MobileAppType;
		};
	}
}
#endif
