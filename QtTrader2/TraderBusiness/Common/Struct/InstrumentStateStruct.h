#ifndef INSTRUMENTSTATESTRUCT_H
#define INSTRUMENTSTATESTRUCT_H

namespace Common
{
	namespace Struct
	{
		struct InstrumentStateStruct
		{
			bool  _fastTrade;
			bool  _closeByFIFO;
			QString _lot;
			double _movepips;
		};
	}
}
#endif