#pragma once
#include <QSharedPointer>
#include "InvokeResult.h"
#include "traderbusiness_global.h"
//#include "Common.h"
//#include "AsyncCallback.h"
//
//using namespace HistoricalChartSpace;

namespace Common
{
	namespace Result
	{
		class TRADERBUSINESS_EXPORT ChartXmlDataResult : public InvokeResult
		{
			Q_OBJECT
		public:
			ChartXmlDataResult(QSharedPointer<Communication::AsyncResult> asyncResult, int requestType = -1);

			const QDomElement& getXmlResult();

			int m_requestType;

			QUuid m_resultContentKey;
			/*ChartProperty m_chartProperty;
			QSharedPointer<AsyncCallback> m_callback;*/
		};
	}
}