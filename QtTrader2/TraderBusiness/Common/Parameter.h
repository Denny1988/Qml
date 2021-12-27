#ifndef COMMON_PARAMETER_H
#define COMMON_PARAMETER_H

#include <QVariant>

namespace Common
{
	class Parameter
	{
	public:
		Parameter(QString parameterName, QVariant parameterValue)
		{
			_parameterName = parameterName;
			_parameterValue = parameterValue;
		};
		Parameter(){};
		~Parameter(){};
		
	public:
		QString _parameterName;
		QVariant _parameterValue;
	};
}

#endif

