#include "Instrument.h"

namespace HistoricalChartSpace
{
	KeyDescription::KeyDescription()
	{
		m_key = "";
		m_description = "";
        m_Sequence = 0;
	}

	KeyDescription::KeyDescription(const QString& key, const QString& description)
	{
		m_key = key;
		m_Guid = key;
		m_description = description;
        m_Sequence = 0;
	}

    KeyDescription::KeyDescription(const QString& key, const QString& description, int Sequence)
    {
        m_key = key;
        m_Guid = key;
        m_description = description;
        m_Sequence = Sequence;
    }

	const QString& KeyDescription::getKey() const
	{
		return m_key;
	}

	const QUuid& KeyDescription::getId() const
	{
		return m_Guid;
	}

	const QString& KeyDescription::getDescription() const
	{
		return m_description;
	}

    const int KeyDescription::getSequence() const
    {
        return m_Sequence;
    }

	bool KeyDescription::operator<(const KeyDescription& that) const
	{
        if(this->m_Sequence == that.m_Sequence)
            return this->m_description < that.m_description;
        else
            return this->m_Sequence < that.m_Sequence;
	}

	bool KeyDescription::operator==(const KeyDescription& that) const
	{
		return this->m_key == that.m_key;
	}

	bool KeyDescription::operator!=(const KeyDescription& that) const
	{
		return *this == that ? false : true;
	}
}

