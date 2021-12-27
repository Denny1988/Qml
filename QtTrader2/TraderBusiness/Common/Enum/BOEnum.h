#ifndef BOENUM_H
#define BOENUM_H

namespace Common
{
	namespace Enum
	{
		namespace BOBetOption
		{
			enum BOBetOption
			{
				Instance = 0,
				IntegralMinute = 1,
				Settle = 2
			};
		}

		namespace BOHitResult
		{
			enum BOHitResult
			{        
				Draw,
				Win,
				Lose,
				//WaitingAndCountDown,
				Waiting,        
				Canceled
			};
		}

	}
}
#endif 
