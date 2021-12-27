#ifndef BUSINESS_MODEL_NEWS_H
#define BUSINESS_MODEL_NEWS_H
#include "IInstantMessage.h"

namespace Business
{
	namespace Model
	{
		class News: public IInstantMessage
		{
		public:
			News()
			{
				_isRead = false;
				_canDelete = false;
			}

		};
	}
}
#endif
