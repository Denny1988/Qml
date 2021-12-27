#pragma once
#include "Common.h"
#include "Settings.h"
#include <QDomElement>
#include <QList>
#include "Instrument.h"
#include "Indicator/Indicator.h"
#include "BaseDrawing.h"

namespace HistoricalChartSpace
{
	class HISTORICALCHART_EXPORT Config
	{
	public:
		class IndicatorPanelIterator
		{
		public:
			IndicatorPanelIterator(const Config& config);
			~IndicatorPanelIterator();

            bool getCurrentSettings(double& heightRatio, QVector<IndicatorSettings>& indicatorSettings, QMap<QUuid, QVector<DrawingObjectSettings>>& drawingObjectSettingMaps);

		private:
			const Config& m_config;
			QDomElement m_currentIndicatorPanel;
		};

	public:
		friend class IndicatorPanelIterator;
		Config(QColor& upColor, QColor& downColor, const QString& configXml = QString());
		~Config();

		void loadConfigXml(const QString& configXml, bool asTemplate = false);
		QString getConfigXml();

        bool applyTemplate(QString& configXml);

		CommonSettings& getCommonSettings();
		PriceSettings& getPriceSettings();

		bool hasConfigData();
		const Instrument& getInstrument() const;
		const KeyDescription& getQuotePolicy() const;
		const Frequency&  getFrequency() const;
        void getPricePanelSettings(double& heightRatio, QVector<IndicatorSettings>& indicatorSettings, QMap<QUuid, QVector<DrawingObjectSettings>>& drawingObjectSettingMaps);

        void getPanelSettings(QDomElement& panelElement, double& heightRatio, QVector<IndicatorSettings>& indicatorSettings, QMap<QUuid, QVector<DrawingObjectSettings>>& drawingObjectSettingMaps) const;

		void getIndicatorsParameter(QDomElement& indicatorElement, QVector<IndicatorSettings>& vecSettings) const;
        void getDrawingObjectParameter(QDomElement& drawingElement, QMap<QUuid, QVector<DrawingObjectSettings>>& drawingObjectSettingMaps, bool isOldElement = false) const;
		
        void saveCommonSettings(const Instrument& instrument, KeyDescription& quotePolicy, const Frequency& frequency, bool userClickAtProductCode);
        void savePanelSettings(enPanelType panelType, double heightRatio, const QVector<Indicator*>& indicators, const QMap<QUuid, QList<BaseDrawing*>>& drawingObjectMaps);
        bool getUserClickAtProductCode(){ return m_userClickAtProductCode; }

	private:
		void saveIndicator(QDomElement& parentElement, const IndicatorSettings& settings);
		void saveDrawingObjectSettings(QDomElement& parentElement, const DrawingObjectSettings& settings);

	private:
		CommonSettings				m_comomSettings;
		PriceSettings				m_priceSettings;
		Instrument					m_instrument;
		KeyDescription              m_quotePolicy;
		Frequency					m_frequency;
		double						m_pricePanelHeightRatio;
		bool						m_hasConfigData;
		QDomDocument				m_xmlDoc;
		QColor                      m_upColor;
		QColor                      m_downColor;
        bool m_userClickAtProductCode;
	};
}
