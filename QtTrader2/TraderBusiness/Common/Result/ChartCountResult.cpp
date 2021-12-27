#include "ChartCountResult.h"

namespace Common
{
	namespace Result
	{
		ChartCountResult::ChartCountResult(QSharedPointer<Communication::AsyncResult> asyncResult)
			: InvokeResult(asyncResult), m_isCompleted(false)
		{
			QObject::connect(this, &InvokeResult::completed, [=](){ this->m_isCompleted = true; });
		}

		// "<Result>  <content_result>536</content_result></Result>
		int ChartCountResult::getCount()
		{
			QDomElement xmlResult = this->asyncResult->xmlResult();
			QString content = xmlResult.text();
			QString returnValue = content.split(',').at(0);
			return returnValue.toInt();
		}

		QString ChartCountResult::getLastDate()
		{
			QDomElement xmlResult = this->asyncResult->xmlResult();
			QString content = xmlResult.text();
			QString returnValue = content.split(',').at(1);
			return returnValue;
		}
	}
}

#include "moc_ChartCountResult.cpp"