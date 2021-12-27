#ifndef COMMUNICATIONENUM_H
#define COMMUNICATIONENUM_H
#include <QMetaType>

namespace Common
{
	namespace Enum
	{
		namespace ContentType
		{
			enum ContentType
			{
				Xml,
				UnmanageMemory,
				Json,
				KeepAlivePacket
			};
		}

		namespace Packet_FirstByte
		{
			enum Packet_FirstByte
			{
				Price=0x01,
				KeepAlive=0x02,
				KeepAliveSuccess=0x06,
				JsonFormat=0x20
			};
		};

		namespace BeatStatus
		{
			enum BeatStatus
			{
				ConnectionIsBroken,
				SessionTimeout,
				Failed
			};
		}

		namespace ConnectionStatus
		{
			enum ConnectionStatus {			
					Connecting,
					ConnectFailed,
					Connected,
					ConnectionBroken,
					Recovering,
					Recovered,
					ConnectUnstable
				};	
		}
	}
}
Q_DECLARE_METATYPE(Common::Enum::ConnectionStatus::ConnectionStatus);
#endif