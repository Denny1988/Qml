#include "CommunicationConstants.h"

namespace Common
{
	namespace Constants
	{
		const QString ResponseConstants::JsonErrorNodeName = "Error";
		const QString ResponseConstants::XmlErrorNodeName = "error";
		const QString ResponseConstants::InvokeIdNodeName = "InvokeId";
		const QString ResponseConstants::ResultNodeName = "Result";

		const QString RequestConstants::ArgumentsNodeName = "Arguments";
		const QString RequestConstants::InvokeIdNodeName = "InvokeId";
		const QString RequestConstants::MethodNodeName = "Method";
		const QString RequestConstants::AppType = "18";
        const QString RequestConstants::MobileAppType = "22";

	}
}
