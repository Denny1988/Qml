#pragma  once
#include "historicalchart_global.h"
#include <QString>
#include <QUuid>
namespace HistoricalChartSpace
{
	struct HISTORICALCHART_EXPORT KeyDescription
	{
	public:
		KeyDescription();
		KeyDescription(const QString& key, const QString& description);
        KeyDescription(const QString& key, const QString& description, int Sequence);
		~KeyDescription(){}

		const QString& getKey() const;
		const QUuid& getId() const;
		const QString& getDescription() const;
        const int getSequence() const;
		bool operator==(const KeyDescription& that) const;
		bool operator!=(const KeyDescription& that) const;
		bool operator<(const KeyDescription& that) const;

	private:
		QUuid   m_Guid;
		QString	m_key;
		QString	m_description;
        int m_Sequence;
	};

	struct HISTORICALCHART_EXPORT Instrument : public KeyDescription
	{
	public:
		Instrument() { m_hasVolume = false; }
        Instrument(const QString& key, const QString& description, int decimalPlaces, bool isNormal = true, bool hasVolumn = false, int sequence = 0)
            : KeyDescription(key, description, sequence), m_isNormal(isNormal), m_hasVolume(hasVolumn), m_decimalPlaces(decimalPlaces){};
		~Instrument(){};

		bool isNormal() const { return m_isNormal; }
		bool isHasVolume() const { return m_hasVolume; };
		int getDecimalPlaces() const { return m_decimalPlaces; };
		QString getPriceString(double price) const
		{
			QString format = QString("%.%1f").arg(m_decimalPlaces);
			QString priceString;
			priceString.sprintf(format.toLatin1(), price);
			return priceString;
		}

	private:
		bool m_hasVolume;
		bool m_isNormal;
		int m_decimalPlaces;
	};


    struct HISTORICALCHART_EXPORT MobileMarketQuotation
    {
        public:
            int m_priceTrend;
            bool m_isNormal;
            QString  m_ask;
            QString  m_bid;
            QString  m_open;
            QString  m_high;
            QString  m_low;
            QString  m_prevClose;
            QString  m_netChange;
            bool m_showPriceSuperscript;
            int m_enhancedPriceDigit;
    };
}

