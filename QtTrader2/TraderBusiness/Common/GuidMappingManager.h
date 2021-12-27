#ifndef COMMON_GUIDMAPPINGMANAGER_H
#define COMMON_GUIDMAPPINGMANAGER_H

#include <QMap>
#include <QUuid>
#include <QMutexLocker>

namespace Common
{
	class GuidMappingManager
	{
		private:
			GuidMappingManager();
			~GuidMappingManager();
			
		public:
			static GuidMappingManager*	getInstance();
			void addInstrumentGuidMapping(int mappingKey, QUuid id);
			QUuid getInstrumentGuidMapping(int mappingKey);
			void addQuotePolicyGuidMapping(int mappingKey, QUuid id);
			QUuid getQuotePolicyGuidMapping(int mappingKey);
			void clear();
			void clearQuotePolicyGuidMapping();

			
		private:
			static	GuidMappingManager*	_instance; 
			QMap<int, QUuid> _instrumentSequenes;
			QMap<int, QUuid> _quotePolicySequenes;
			static QMutex  m_Mutex;
	};
}
#endif
