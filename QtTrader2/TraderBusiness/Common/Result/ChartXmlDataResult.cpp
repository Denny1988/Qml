#include "ChartXmlDataResult.h"

namespace Common
{
	namespace Result
	{
		ChartXmlDataResult::ChartXmlDataResult(QSharedPointer<Communication::AsyncResult> asyncResult, int requestType)
			: InvokeResult(asyncResult), m_requestType(requestType)
		{
		}

		const QDomElement& ChartXmlDataResult::getXmlResult()
		{
			return this->asyncResult->xmlResult();
		}
	}
}

#include "moc_ChartXmlDataResult.cpp"