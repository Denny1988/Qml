#include "PlaceResult.h"

namespace Common
{
	namespace Result
	{
		void PlaceResult::placeResultCompleted()
		{
			emit placeCompleted(this->_id);
		}

		QString PlaceResult::getTransactionError()
		{
			QString transactionError = this->error();
			if(transactionError.isEmpty() || transactionError.isNull() || transactionError=="")
			{
				QDomElement qDomElement = this->asyncResult->xmlResult();
                QDomElement transactionErrorXml = qDomElement.firstChildElement("transactionError");
                if(!transactionErrorXml.isNull())
                {
                    transactionError = transactionErrorXml.text();
                }
                else
                {
                    transactionError = qDomElement.text();
                }

			}
			return transactionError;
		}
	}
}
#include "moc_PlaceResult.cpp"
