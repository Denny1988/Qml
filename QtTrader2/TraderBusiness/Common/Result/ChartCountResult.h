#pragma once

#include "InvokeResult.h"
#include "traderbusiness_global.h"
//#include "Common.h"

//using namespace HistoricalChartSpace;

namespace Common
{
	namespace Result
	{
		class TRADERBUSINESS_EXPORT ChartCountResult : public InvokeResult
		{
			Q_OBJECT
		public:
			ChartCountResult(QSharedPointer<Communication::AsyncResult> asyncResult);

			// "<Result>  <content_result>536</content_result></Result>
			int getCount();
			QString getLastDate();
			bool isCompleted(){ return m_isCompleted; }

			int	m_callId;
			
			QUuid m_resultContentKey;
			//ChartProperty m_chartProperty;
			

		private:
			bool m_isCompleted;
		};
	}
}
