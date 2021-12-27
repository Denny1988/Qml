#include "InvokeResult.h"

namespace Common
{
	namespace Result
	{
		InvokeResult::InvokeResult(QSharedPointer<Communication::AsyncResult> asyncResult)
		{
			this->asyncResult = asyncResult;
			QObject::connect(this->asyncResult.data(), SIGNAL(completed()), this, SIGNAL(completed()));
		}

		InvokeResult::~InvokeResult()
		{
			QObject::disconnect(this->asyncResult.data(), SIGNAL(completed()), this, SIGNAL(completed()));
		}
	}
}
#include "moc_InvokeResult.cpp"