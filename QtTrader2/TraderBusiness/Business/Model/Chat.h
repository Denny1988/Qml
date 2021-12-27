#ifndef BUSINESS_MODEL_CHAT_H
#define BUSINESS_MODEL_CHAT_H
#include "IInstantMessage.h"

namespace Business
{
	namespace Model
	{
		class Chat: public IInstantMessage
		{
		public:
			Chat()
			{
				_isRead = false;
				_canDelete = true;
			}
					
		public:

		};
	}
}
#endif
