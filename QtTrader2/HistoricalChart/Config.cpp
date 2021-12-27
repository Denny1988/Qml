#include "Config.h"
#include <QFile>
#include <QTextStream>
#include "../QtFramework/QsLog.h"
#include "ZoomController.h"

const char * const c_key = "key";
const char * const c_value = "value";
const char * const c_data = "data";
const char * const c_type = "type";
const char * const c_zoomLevel = "zoomLevel";
const char * const c_barDistance = "barDistance";
const char * const c_heightRatio = "heightRatio";

namespace HistoricalChartSpace
{
	Config::IndicatorPanelIterator::IndicatorPanelIterator(const Config& config) : m_config(config)
	{
		m_currentIndicatorPanel = config.m_xmlDoc.documentElement().firstChildElement("IndicatorPanel");
	}

	Config::IndicatorPanelIterator::~IndicatorPanelIterator()
	{
	}

    bool Config::IndicatorPanelIterator::getCurrentSettings(double& heightRatio, QVector<IndicatorSettings>& indicatorSettings, QMap<QUuid, QVector<DrawingObjectSettings>>& drawingObjectSettingMaps)
	{
		if (m_currentIndicatorPanel.isNull())
		{
			return false;
		}
		else
		{
            m_config.getPanelSettings(m_currentIndicatorPanel, heightRatio, indicatorSettings, drawingObjectSettingMaps);
			m_currentIndicatorPanel = m_currentIndicatorPanel.nextSiblingElement();
			return true;
		}
	}

	Config::Config(QColor& upColor, QColor& downColor, const QString& configXml)
		:m_hasConfigData(false),
		m_instrument(NULL, NULL, 2),
		m_frequency(Minute),
		m_pricePanelHeightRatio(1.0),
		m_upColor(upColor),
		m_downColor(downColor)
	{
		if (!configXml.isNull() && !configXml.isEmpty())
		{
			this->loadConfigXml(configXml);
		}
	}

	Config::~Config()
	{
	}

	CommonSettings& Config::getCommonSettings()
	{
		return m_comomSettings;
	}

	PriceSettings& Config::getPriceSettings()
	{
		return m_priceSettings;
	}

	void Config::loadConfigXml(const QString& configXml, bool asTemplate /*= false*/)
	{
		try
		{
			if (!m_xmlDoc.setContent(configXml))
			{
                // Set up and down colors according to the incoming parameters when opening the graphics window
				m_hasConfigData = false;
				m_priceSettings.m_bullCandleColor = m_upColor;
				m_priceSettings.m_bearCandleColor = m_downColor;
				m_priceSettings.m_barUpColor = m_upColor;
				m_priceSettings.m_barDownColor = m_downColor;
				return;
			}
			m_hasConfigData = true;

			QDomElement rootElement = m_xmlDoc.documentElement();

            // read CommonSettings,PriceSettings,CurrentInstrument，CurrentFrequency
			QDomElement commonEle = rootElement.firstChildElement("CommonSettings");
			if (!commonEle.isNull())
			{
				m_comomSettings.m_background = commonEle.attribute("background");
				m_comomSettings.m_foreground = commonEle.attribute("foreground");
				m_comomSettings.m_gridColor = commonEle.attribute("gridColor");
                m_comomSettings.m_productCodeColor = commonEle.hasAttribute("productCodeColor") ?
                            commonEle.attribute("productCodeColor") : m_comomSettings.m_foreground;
				m_comomSettings.m_isAutoScroll = commonEle.attribute("isAutoScroll").toInt();
				m_comomSettings.m_isShiftData = commonEle.attribute("isShiftData").toInt();
				m_comomSettings.m_isShowGrid = commonEle.attribute("isShowGrid").toInt();
				m_comomSettings.m_isShowLabelInfo = commonEle.attribute("isShowLableInfo").toInt();
				m_comomSettings.m_zoomLevel = (ZoomLevel)commonEle.attribute(c_zoomLevel).toInt();

				m_comomSettings.m_barDistance = commonEle.hasAttribute(c_barDistance) ? 
					commonEle.attribute(c_barDistance).toDouble() :
					ZoomController::getBarDistance(m_comomSettings.m_zoomLevel);

				m_comomSettings.m_isShowCrosshair = commonEle.attribute("isShowCrosshair").toInt();
				m_comomSettings.m_isShowQuickPlacer = commonEle.attribute("isShowQuickPlacer").toInt();
				m_comomSettings.m_isShowExecutedOrder = commonEle.attribute("isShowExecutedOrder").toInt();
                m_comomSettings.m_isShowPendingOrder = commonEle.attribute("isShowPendingOrder").toInt();

			}

			QDomElement priceEle = rootElement.firstChildElement("PriceSettings");
			if (!priceEle.isNull())
			{
#if defined(RUNTIME_OS_IPHONE) || defined(RUNTIME_OS_ANDROID)
                m_priceSettings.m_bullCandleColor = m_upColor;
                m_priceSettings.m_bearCandleColor = m_downColor;
                m_priceSettings.m_barUpColor = m_upColor;
                m_priceSettings.m_barDownColor = m_downColor;
#else
                m_priceSettings.m_barDownColor = priceEle.attribute("barDownColor");
                m_priceSettings.m_barUpColor = priceEle.attribute("barUpColor");
                m_priceSettings.m_bearCandleColor = priceEle.attribute("bearCandleColor");
                m_priceSettings.m_bullCandleColor = priceEle.attribute("bullCandleColor");
#endif
				m_priceSettings.m_drawType = (enPriceType)priceEle.attribute("drawType").toInt();
				m_priceSettings.m_isShowBidPriceLine = priceEle.attribute("isShowBidPriceLine").toInt();
				m_priceSettings.m_isShowAskPriceLine = priceEle.attribute("isShowAskPriceLine").toInt();
				m_priceSettings.m_isShowVolume = priceEle.attribute("isShowVolume").toInt();
				m_priceSettings.m_bidLineColor = priceEle.attribute("bidLineColor");
				m_priceSettings.m_askLineColor = priceEle.attribute("askLineColor");
				if (priceEle.hasAttribute("crossStarColor"))
					m_priceSettings.m_crossStarColor = priceEle.attribute("crossStarColor");
				if (priceEle.hasAttribute("lineChartColor"))
					m_priceSettings.m_lineChartColor = priceEle.attribute("lineChartColor");
				if (priceEle.hasAttribute("openOrderLineColor"))
					m_priceSettings.m_openOrderLineColor = priceEle.attribute("openOrderLineColor");
				if (priceEle.hasAttribute("pendingOrderLineColor"))
					m_priceSettings.m_pendingOrderLineColor = priceEle.attribute("pendingOrderLineColor");
			}

			if (!asTemplate)
			{
				QDomElement insEle = rootElement.firstChildElement("currentInstrument");
				if (!insEle.isNull())
				{
					QString key = insEle.attribute(c_key);
					QString description = insEle.attribute("description");
					bool hasVolume = insEle.attribute("hasVolume").toInt();
					m_instrument = Instrument(key, description, hasVolume);
                    m_userClickAtProductCode = insEle.attribute("userClickAtProductCode").toInt();
				}
				QDomElement quoEle = rootElement.firstChildElement("currentQuotePolicy");
				if (!quoEle.isNull())
				{
					QString key = quoEle.attribute(c_key);
					QString description = quoEle.attribute("description");
					m_quotePolicy = KeyDescription(key, description);
				}
				QDomElement freEle = rootElement.firstChildElement("currentFrequency");
				if (!freEle.isNull())
				{
					int type = freEle.attribute(c_type).toInt();
					int unit = freEle.attribute("unit").toInt();
					m_frequency = Frequency((FrequencyType)type, unit);
				}
			}
		}
		catch (...)
		{
			QLOG_WARN(Constants::LogFile) << QString("Config::loadConfigXml failed,configXml:\r\n%1").arg(configXml);
		}
	}

	// same to loadConfigXml except loading instrument,quotePolicy,frequency
	bool Config::applyTemplate(QString& configXml)
	{
		try
		{
			// validate configXml before loadConfigXml
			QDomDocument document;
			if (document.setContent(configXml))
			{
				QDomElement rootElement = document.documentElement();
				QDomElement element = rootElement.firstChildElement("CommonSettings");
				if (!element.isNull())
				{
					element = rootElement.firstChildElement("PriceSettings");
					if (!element.isNull())
					{
						this->loadConfigXml(configXml, true);
						return true;
					}
				}
			}
		}
		catch (...)
		{
			QLOG_WARN(Constants::LogFile) << QString("Config::applyTemplate failed,configXml:\r\n%1").arg(configXml);
		}
		return false;
	}

	bool Config::hasConfigData()
	{
		return m_hasConfigData;
	}

	const Instrument& Config::getInstrument() const
	{
		return m_instrument;
	}
	const KeyDescription& Config::getQuotePolicy() const
	{
		return m_quotePolicy;
	}
	const Frequency& Config::getFrequency() const
	{
		return m_frequency;
	}

    void Config::getPricePanelSettings(double& heightRatio, QVector<IndicatorSettings>& indicatorSettings, QMap<QUuid, QVector<DrawingObjectSettings>>& drawingObjectSettingMaps)
	{
		QDomElement xmlElement = m_xmlDoc.documentElement();
		QDomElement pricePanelElement = xmlElement.firstChildElement("PricePanel");
		if (!pricePanelElement.isNull())
		{
            this->getPanelSettings(pricePanelElement, heightRatio, indicatorSettings, drawingObjectSettingMaps);
		}
	}

    void Config::getPanelSettings(QDomElement& panelElement, double& heightRatio, QVector<IndicatorSettings>& indicatorSettings, QMap<QUuid, QVector<DrawingObjectSettings>>& drawingObjectSettingMaps) const
	{
		heightRatio = panelElement.attribute("heightRatio").toDouble();
		// indicators
		QDomElement indicatorsElement = panelElement.firstChildElement("Indicators");
		if (!indicatorsElement.isNull())
		{
			QDomElement indicatorElement = indicatorsElement.firstChildElement("Indicator");
			this->getIndicatorsParameter(indicatorElement, indicatorSettings);
		}
		// DrawingObjects
		QDomElement drawingObjectsElement = panelElement.firstChildElement("DrawingObjects");
		if (!drawingObjectsElement.isNull())
		{            
            QDomElement drawingInstrumentElement = drawingObjectsElement.firstChildElement("Instrument");
            if(!drawingInstrumentElement.isNull())
            {
                this->getDrawingObjectParameter(drawingInstrumentElement, drawingObjectSettingMaps);
            }
            else
            {
                QDomElement drawingElement = drawingObjectsElement.firstChildElement("DrawingObject");
                this->getDrawingObjectParameter(drawingElement, drawingObjectSettingMaps, true);
            }
		}
	}

	void Config::getIndicatorsParameter(QDomElement& indicatorElement, QVector<IndicatorSettings>& vecSettings) const
	{
		while (!indicatorElement.isNull())
		{
			IndicatorSettings settings;
			settings.m_arrayNum = indicatorElement.attribute("arrayNum").toInt();
			settings.m_dependency = (enCPD)indicatorElement.attribute("dependency").toInt();
			settings.m_isInPriceChart = indicatorElement.attribute("isInPriceChart").toInt();
			settings.m_maType = (LocalMAType)indicatorElement.attribute("maType").toInt();
			settings.m_plotNum = indicatorElement.attribute("plotNum").toInt();
			settings.m_title = indicatorElement.attribute("strInfo");
			settings.m_type = (enIndicatorType)indicatorElement.attribute(c_type).toInt();

			QDomElement inputIntEle = indicatorElement.firstChildElement("inputIntData");
			if (!inputIntEle.isNull())
			{
				do
				{
					QString key = inputIntEle.attribute(c_key);
					int value = inputIntEle.attribute(c_value).toInt();
					settings.m_hashInputIntData[key] = value;
					inputIntEle = inputIntEle.nextSiblingElement("inputIntData");
				} while (!inputIntEle.isNull());
			}

			QDomElement inputDoubleEle = indicatorElement.firstChildElement("inputDoubleData");
			if (!inputDoubleEle.isNull())
			{
				do
				{
					QString key = inputDoubleEle.attribute(c_key);
					double  value = inputDoubleEle.attribute(c_value).toDouble();
					settings.m_hashInputDoubleData[key] = value;
					inputDoubleEle = inputDoubleEle.nextSiblingElement("inputDoubleData");
				} while (!inputDoubleEle.isNull());
			}

			QDomElement drawInfoEle = indicatorElement.firstChildElement("drawInfo");
			if (!drawInfoEle.isNull())
			{
				do
				{
					IndicatorLineSettings lineSettings =
					{
						drawInfoEle.attribute("lineName"),
						drawInfoEle.attribute("lineColor"),
						drawInfoEle.attribute("lineWidth").toDouble(),
						(Qt::PenStyle)drawInfoEle.attribute("lineType").toInt(),
						(enIndicatorDrawType)drawInfoEle.attribute("drawType").toInt()
					};
					settings.m_vectLineSettings << lineSettings;
					drawInfoEle = drawInfoEle.nextSiblingElement("drawInfo");
				} while (!drawInfoEle.isNull());
			}

			vecSettings << settings;
			indicatorElement = indicatorElement.nextSiblingElement("Indicator");
		}
	}

    void Config::getDrawingObjectParameter(QDomElement& drawingInstrumentElement, QMap<QUuid, QVector<DrawingObjectSettings>>& drawingObjectSettingMaps, bool isOldElement) const
	{
        if(isOldElement)
        {
            QVector<DrawingObjectSettings> vecSettings;
            while (!drawingInstrumentElement.isNull())
            {
                DrawingObjectSettings setings;
                setings.m_bAsBackground = drawingInstrumentElement.attribute("bAsBackground").toInt();
                setings.m_bhasLevels = drawingInstrumentElement.attribute("bHasLevels").toInt();
                setings.m_bSelectAble = drawingInstrumentElement.attribute("bSelectAble").toInt();
                setings.m_colorLine = drawingInstrumentElement.attribute("colorLine");
                setings.m_intFillShape = drawingInstrumentElement.attribute("intFillShape").toInt();
                setings.m_intFlagsToShow = drawingInstrumentElement.attribute("intFlagsToShow").toInt();
                setings.m_intLineType = drawingInstrumentElement.attribute("intLineType").toInt();
                setings.m_intRayFlags = drawingInstrumentElement.attribute("intRayFlags").toInt();
                setings.m_levelsColorLine = drawingInstrumentElement.attribute("levelsColorLine");
                setings.m_levelsLineType = drawingInstrumentElement.attribute("levelsLineType").toInt();
                setings.m_levelsLineWidth = drawingInstrumentElement.attribute("levelsLineWidth").toDouble();
                setings.m_lfLineWidth = drawingInstrumentElement.attribute("lfLineWidth").toDouble();
                setings.m_describe = drawingInstrumentElement.attribute("describe");
                setings.m_name = drawingInstrumentElement.attribute("name");
                setings.m_angle = drawingInstrumentElement.attribute("angle", "0").toDouble();
                setings.m_fontFamily = drawingInstrumentElement.attribute("fontFamily", "Arial");
                setings.m_fontSize = drawingInstrumentElement.attribute("fontSize", "9").toInt();
                setings.m_type = (enDrawingObjectType)drawingInstrumentElement.attribute(c_type).toInt();

                QDomElement levelsEle = drawingInstrumentElement.firstChildElement("levels");
                if (!levelsEle.isNull())
                {
                    do
                    {
                        setings.m_mapLevels[levelsEle.attribute(c_key).toDouble()] = levelsEle.attribute(c_value);
                        levelsEle = levelsEle.nextSiblingElement("levels");
                    } while (!levelsEle.isNull());
                }
                QDomElement dateEle = drawingInstrumentElement.firstChildElement("date");
                if (!dateEle.isNull())
                {
                    do
                    {
                        setings.m_vecDate << dateEle.attribute(c_data).toDouble();
                        dateEle = dateEle.nextSiblingElement("date");
                    } while (!dateEle.isNull());
                }
                QDomElement valueEle = drawingInstrumentElement.firstChildElement(c_value);
                if (!valueEle.isNull())
                {
                    do
                    {
                        setings.m_vecValue << valueEle.attribute(c_data).toDouble();
                        valueEle = valueEle.nextSiblingElement(c_value);
                    } while (!valueEle.isNull());
                }
                vecSettings << setings;
                drawingInstrumentElement = drawingInstrumentElement.nextSiblingElement("DrawingObject");
            }  
            drawingObjectSettingMaps.insert(m_instrument.getId(), vecSettings);
        }
        else
        {
            while(!drawingInstrumentElement.isNull())
            {
                QVector<DrawingObjectSettings> vecSettings;
                QUuid id = QUuid(drawingInstrumentElement.attribute("id"));                
                QDomElement drawingElement = drawingInstrumentElement.firstChildElement("DrawingObject");
                while (!drawingElement.isNull())
                {
                    DrawingObjectSettings setings;
                    setings.m_bAsBackground = drawingElement.attribute("bAsBackground").toInt();
                    setings.m_bhasLevels = drawingElement.attribute("bHasLevels").toInt();
                    setings.m_bSelectAble = drawingElement.attribute("bSelectAble").toInt();
                    setings.m_colorLine = drawingElement.attribute("colorLine");
                    setings.m_intFillShape = drawingElement.attribute("intFillShape").toInt();
                    setings.m_intFlagsToShow = drawingElement.attribute("intFlagsToShow").toInt();
                    setings.m_intLineType = drawingElement.attribute("intLineType").toInt();
                    setings.m_intRayFlags = drawingElement.attribute("intRayFlags").toInt();
                    setings.m_levelsColorLine = drawingElement.attribute("levelsColorLine");
                    setings.m_levelsLineType = drawingElement.attribute("levelsLineType").toInt();
                    setings.m_levelsLineWidth = drawingElement.attribute("levelsLineWidth").toDouble();
                    setings.m_lfLineWidth = drawingElement.attribute("lfLineWidth").toDouble();
                    setings.m_describe = drawingElement.attribute("describe");
                    setings.m_name = drawingElement.attribute("name");
                    setings.m_angle = drawingElement.attribute("angle", "0").toDouble();
                    setings.m_fontFamily = drawingElement.attribute("fontFamily", "Arial");
                    setings.m_fontSize = drawingElement.attribute("fontSize", "9").toInt();
                    setings.m_type = (enDrawingObjectType)drawingElement.attribute(c_type).toInt();

                    QDomElement levelsEle = drawingElement.firstChildElement("levels");
                    if (!levelsEle.isNull())
                    {
                        do
                        {
                            setings.m_mapLevels[levelsEle.attribute(c_key).toDouble()] = levelsEle.attribute(c_value);
                            levelsEle = levelsEle.nextSiblingElement("levels");
                        } while (!levelsEle.isNull());
                    }

                    QDomElement dateEle = drawingElement.firstChildElement("date");
                    if (!dateEle.isNull())
                    {
                        do
                        {
                            setings.m_vecDate << dateEle.attribute(c_data).toDouble();
                            dateEle = dateEle.nextSiblingElement("date");
                        } while (!dateEle.isNull());
                    }

                    QDomElement valueEle = drawingElement.firstChildElement(c_value);
                    if (!valueEle.isNull())
                    {
                        do
                        {
                            setings.m_vecValue << valueEle.attribute(c_data).toDouble();
                            valueEle = valueEle.nextSiblingElement(c_value);
                        } while (!valueEle.isNull());
                    }

                    vecSettings << setings;

                    drawingElement = drawingElement.nextSiblingElement("DrawingObject");

                }
                drawingInstrumentElement = drawingInstrumentElement.nextSiblingElement("Instrument");
                drawingObjectSettingMaps.insert(id, vecSettings);
            }

        }
    }

	QString Config::getConfigXml()
	{
		//m_xmlDoc.save(out, 3);
		return m_xmlDoc.toString();
	}

    void Config::saveCommonSettings(const Instrument& instrument, KeyDescription& quotePolicy, const Frequency& frequency, bool userClickAtProductCode)
	{
		m_xmlDoc.clear();
		QDomElement rootElement = m_xmlDoc.createElement("HistoricalChart");
		m_xmlDoc.appendChild(rootElement);

		QDomElement commonElement = m_xmlDoc.createElement("CommonSettings");
		commonElement.setAttribute("background", m_comomSettings.m_background.name());
		commonElement.setAttribute("foreground", m_comomSettings.m_foreground.name());
		commonElement.setAttribute("gridColor", m_comomSettings.m_gridColor.name());
        commonElement.setAttribute("productCodeColor", m_comomSettings.m_productCodeColor.name());
		commonElement.setAttribute("isAutoScroll", m_comomSettings.m_isAutoScroll);
		commonElement.setAttribute("isShiftData", m_comomSettings.m_isShiftData);
		commonElement.setAttribute("isShowGrid", m_comomSettings.m_isShowGrid);
		commonElement.setAttribute("isShowLableInfo", m_comomSettings.m_isShowLabelInfo);
		commonElement.setAttribute(c_zoomLevel, m_comomSettings.m_zoomLevel);
		commonElement.setAttribute(c_barDistance, m_comomSettings.m_barDistance);
		commonElement.setAttribute("isShowCrosshair", m_comomSettings.m_isShowCrosshair);
		commonElement.setAttribute("isShowQuickPlacer", m_comomSettings.m_isShowQuickPlacer);
		commonElement.setAttribute("isShowExecutedOrder", m_comomSettings.m_isShowExecutedOrder);
		commonElement.setAttribute("isShowPendingOrder", m_comomSettings.m_isShowPendingOrder);

		QDomElement priceElement = m_xmlDoc.createElement("PriceSettings");
		priceElement.setAttribute("barDownColor", m_priceSettings.m_barDownColor.name());
		priceElement.setAttribute("barUpColor", m_priceSettings.m_barUpColor.name());
		priceElement.setAttribute("bearCandleColor", m_priceSettings.m_bearCandleColor.name());
		priceElement.setAttribute("bullCandleColor", m_priceSettings.m_bullCandleColor.name());
		priceElement.setAttribute("drawType", m_priceSettings.m_drawType);
		priceElement.setAttribute("isShowBidPriceLine", m_priceSettings.m_isShowBidPriceLine);
		priceElement.setAttribute("isShowAskPriceLine", m_priceSettings.m_isShowAskPriceLine);
		priceElement.setAttribute("isShowVolume", m_priceSettings.m_isShowVolume);
		priceElement.setAttribute("bidLineColor", m_priceSettings.m_bidLineColor.name());
		priceElement.setAttribute("askLineColor", m_priceSettings.m_askLineColor.name());
		priceElement.setAttribute("crossStarColor", m_priceSettings.m_crossStarColor.name());
		priceElement.setAttribute("lineChartColor", m_priceSettings.m_lineChartColor.name());
		priceElement.setAttribute("volumesColor", m_priceSettings.m_volumesColor.name());
		priceElement.setAttribute("openOrderLineColor", m_priceSettings.m_openOrderLineColor.name());
		priceElement.setAttribute("pendingOrderLineColor", m_priceSettings.m_pendingOrderLineColor.name());

		QDomElement instrumnetElement = m_xmlDoc.createElement("currentInstrument");
		instrumnetElement.setAttribute(c_key, instrument.getKey());
		instrumnetElement.setAttribute("description", instrument.getDescription());
		instrumnetElement.setAttribute("hasVolume", instrument.isHasVolume());
        instrumnetElement.setAttribute("userClickAtProductCode", userClickAtProductCode);

		QDomElement quotePolicyElement = m_xmlDoc.createElement("currentQuotePolicy");
		quotePolicyElement.setAttribute(c_key, quotePolicy.getKey());
		quotePolicyElement.setAttribute("description", quotePolicy.getDescription());

		QDomElement frequencyElement = m_xmlDoc.createElement("currentFrequency");
		frequencyElement.setAttribute(c_type, frequency.getType());
		frequencyElement.setAttribute("unit", frequency.getUint());

		rootElement.appendChild(commonElement);
		rootElement.appendChild(priceElement);
		rootElement.appendChild(instrumnetElement);
		rootElement.appendChild(quotePolicyElement);
		rootElement.appendChild(frequencyElement);
	}

    void Config::savePanelSettings(enPanelType panelType, double heightRatio, const QVector<Indicator*>& indicators, const QMap<QUuid, QList<BaseDrawing*>>& drawingObjectMaps)
	{
		QDomElement xmlElement = m_xmlDoc.documentElement();
		QDomElement panelElement;
		if (panelType == enPanelPrice)
		{
			panelElement = m_xmlDoc.createElement("PricePanel");
		}
		else
		{
			panelElement = m_xmlDoc.createElement("IndicatorPanel");
		}
		xmlElement.appendChild(panelElement);
		panelElement.setAttribute(c_heightRatio, heightRatio);

		if (indicators.size() > 0)
		{
			QDomElement indicatorsElemnet = m_xmlDoc.createElement("Indicators");
			panelElement.appendChild(indicatorsElemnet);
            foreach (Indicator* indicator, indicators)
			{
				this->saveIndicator(indicatorsElemnet, indicator->getIndicatorSettings());
			}
		}

        if (drawingObjectMaps.size() > 0)
		{
			QDomElement drawingObjectsElemnet = m_xmlDoc.createElement("DrawingObjects");
			panelElement.appendChild(drawingObjectsElemnet);
            QMapIterator<QUuid, QList<BaseDrawing*>> baseDrawingItem(drawingObjectMaps);
            while (baseDrawingItem.hasNext()) {
                baseDrawingItem.next();
                auto drawingObjects = baseDrawingItem.value();
                QDomElement drawingInstrumentElemnet = m_xmlDoc.createElement("Instrument");
                drawingInstrumentElemnet.setAttribute("id", baseDrawingItem.key().toString().mid(1, 36));
                drawingObjectsElemnet.appendChild(drawingInstrumentElemnet);
                foreach (BaseDrawing* drawingObject, drawingObjects)
                {
                    this->saveDrawingObjectSettings(drawingInstrumentElemnet, drawingObject->getSettings());
                }
            }
		}
	}

	void Config::saveIndicator(QDomElement& parentElement, const IndicatorSettings& settings)
	{
		QDomElement indicatorEle = m_xmlDoc.createElement("Indicator");
		parentElement.appendChild(indicatorEle);

		indicatorEle.setAttribute("arrayNum", settings.m_arrayNum);
		indicatorEle.setAttribute("dependency", settings.m_dependency);
		indicatorEle.setAttribute("isInPriceChart", settings.m_isInPriceChart);
		indicatorEle.setAttribute("maType", settings.m_maType);
		indicatorEle.setAttribute("plotNum", settings.m_plotNum);
		indicatorEle.setAttribute("strInfo", settings.m_title);
		indicatorEle.setAttribute(c_type, settings.m_type);

		QHashIterator<QString, int> itInt(settings.m_hashInputIntData);
		while (itInt.hasNext())
		{
			itInt.next();
			QDomElement inputIntEle = m_xmlDoc.createElement("inputIntData");
			inputIntEle.setAttribute(c_key, itInt.key());
			inputIntEle.setAttribute(c_value, QString::number(itInt.value()));
			indicatorEle.appendChild(inputIntEle);
		}

		QHashIterator<QString, double> itDouble(settings.m_hashInputDoubleData);
		while (itDouble.hasNext())
		{
			itDouble.next();
			QDomElement inputDoubleEle = m_xmlDoc.createElement("inputDoubleData");
			inputDoubleEle.setAttribute(c_key, itDouble.key());
			inputDoubleEle.setAttribute(c_value, QString::number(itDouble.value()));
			indicatorEle.appendChild(inputDoubleEle);
		}

		if (!settings.isDataValid()) return;

		for (int i = 0; i < settings.m_plotNum; ++i)
		{
			QDomElement drawInfoEle = m_xmlDoc.createElement("drawInfo");
			drawInfoEle.setAttribute("drawType", settings.m_vectLineSettings[i].m_drawType);
			drawInfoEle.setAttribute("lineColor", settings.m_vectLineSettings[i].m_lineColor.name());
			drawInfoEle.setAttribute("lineName", settings.m_vectLineSettings[i].m_lineName);
			drawInfoEle.setAttribute("lineType", settings.m_vectLineSettings[i].m_lineType);
			drawInfoEle.setAttribute("lineWidth", settings.m_vectLineSettings[i].m_lineWidth);
			indicatorEle.appendChild(drawInfoEle);
		}
	}

	void Config::saveDrawingObjectSettings(QDomElement& parentElement, const DrawingObjectSettings& settings)
	{
		QDomElement drawingObjEle = m_xmlDoc.createElement("DrawingObject");
		parentElement.appendChild(drawingObjEle);

		drawingObjEle.setAttribute("bAsBackground", settings.m_bAsBackground);
		drawingObjEle.setAttribute("bHasLevels", settings.m_bhasLevels);
		drawingObjEle.setAttribute("bSelectAble", settings.m_bSelectAble);
		drawingObjEle.setAttribute("colorLine", settings.m_colorLine.name());
		drawingObjEle.setAttribute("intFillShape", settings.m_intFillShape);
		drawingObjEle.setAttribute("intFlagsToShow", settings.m_intFlagsToShow);
		drawingObjEle.setAttribute("intLineType", settings.m_intLineType);
		drawingObjEle.setAttribute("intRayFlags", settings.m_intRayFlags);
		drawingObjEle.setAttribute("levelsColorLine", settings.m_levelsColorLine.name());
		drawingObjEle.setAttribute("levelsLineType", settings.m_levelsLineType);
		drawingObjEle.setAttribute("levelsLineWidth", settings.m_levelsLineWidth);
		drawingObjEle.setAttribute("lfLineWidth", settings.m_lfLineWidth);
		drawingObjEle.setAttribute("describe", settings.m_describe);
		drawingObjEle.setAttribute("name", settings.m_name);
		drawingObjEle.setAttribute("angle", settings.m_angle);
		drawingObjEle.setAttribute("fontFamily", settings.m_fontFamily);
		drawingObjEle.setAttribute("fontSize", settings.m_fontSize);
		drawingObjEle.setAttribute(c_type, settings.m_type);

		QMapIterator<double, QString> it(settings.m_mapLevels);
		while (it.hasNext())
		{
			it.next();
			QDomElement levelsEle = m_xmlDoc.createElement("levels");
			levelsEle.setAttribute(c_key, it.key());
			levelsEle.setAttribute(c_value, it.value());
			drawingObjEle.appendChild(levelsEle);
		}

		for (int i = 0; i < settings.m_vecDate.size(); ++i)
		{
			QDomElement dateEle = m_xmlDoc.createElement("date");
			dateEle.setAttribute(c_data, settings.m_vecDate[i]);
			drawingObjEle.appendChild(dateEle);
		}
		for (int i = 0; i < settings.m_vecValue.size(); ++i)
		{
			QDomElement valueEle = m_xmlDoc.createElement(c_value);
			valueEle.setAttribute(c_data, settings.m_vecValue[i]);
			drawingObjEle.appendChild(valueEle);
		}
	}
}

