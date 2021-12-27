#include "GuidMappingManager.h"

namespace Common
{
	QMutex GuidMappingManager::m_Mutex(QMutex::Recursive);
	GuidMappingManager* GuidMappingManager::_instance = Q_NULLPTR;
	GuidMappingManager* GuidMappingManager::getInstance()
	{
			if (Q_NULLPTR==_instance)
			{
				_instance = new GuidMappingManager;
			}
			return _instance;
	}

	GuidMappingManager::GuidMappingManager()
	{
		
	}

	GuidMappingManager::~GuidMappingManager()
	{

	}

	void GuidMappingManager::addInstrumentGuidMapping(int mappingKey, QUuid id)
	{
		QMutexLocker locker(&GuidMappingManager::m_Mutex);
		if (_instrumentSequenes.contains(mappingKey))
			_instrumentSequenes[mappingKey] = id;
		else
			_instrumentSequenes.insert(mappingKey, id);
	}
	
	QUuid GuidMappingManager::getInstrumentGuidMapping(int mappingKey)
	{
		QMutexLocker locker(&GuidMappingManager::m_Mutex);
		if (_instrumentSequenes.contains(mappingKey))
        {
            return _instrumentSequenes.value(mappingKey);
        }
        return QUuid();
	}

	void GuidMappingManager::addQuotePolicyGuidMapping(int mappingKey, QUuid id)
	{
		if (_quotePolicySequenes.contains(mappingKey))
			_quotePolicySequenes[mappingKey] = id;
		else
			_quotePolicySequenes.insert(mappingKey, id);
	}
	
	QUuid GuidMappingManager::getQuotePolicyGuidMapping(int mappingKey)
	{
		if (_quotePolicySequenes.contains(mappingKey))
        {
            return _quotePolicySequenes.value(mappingKey);
        }
        return QUuid();
	}

	void GuidMappingManager::clear()
	{
		QMutexLocker locker(&GuidMappingManager::m_Mutex);
		_instrumentSequenes.clear();
		_quotePolicySequenes.clear();
	}

	void GuidMappingManager::clearQuotePolicyGuidMapping()
	{
		QMutexLocker locker(&GuidMappingManager::m_Mutex);
		_quotePolicySequenes.clear();
	}

}
