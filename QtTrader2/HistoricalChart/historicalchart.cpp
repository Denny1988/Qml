#include "historicalchart.h"
#include "SR.h"
#include <QDebug>
#include "XCoorPanel.h"
#include "IndicatorPanel.h"
#include "Helper.h"
#include "ChartPanel.h"
#include "JulianDate.h"
#include "PricePanel.h"
#include <QSettings>
#include <QMouseEvent>
#include <QApplication>
#include <ctime>
#include <HistoricalDataSource.h>
#include <QToolBar>
#include <QMenu>
#include <QFont>
#include <QVariant>
#include "SplitPanel.h"
#include <QGraphicsSceneContextMenuEvent>
#include "priceDialog.h"
#include "Crosshair.h"
#include "ToolBar.h"
#include "../QtFramework/QsLog.h"
#include "Calculator.h"
#include "IndicatorListDialog.h"
#include "DrawingObjectListDialog.h"
#include <QMetaType>
#include <QtConcurrentRun>
#include "MainPainterPanel.h"
#if defined (RUNTIME_OS_IPHONE) || defined (RUNTIME_OS_ANDROID)
#include "IndicatorSettingDialog.h"
#include "IndicatorSettingPanel.h"
#endif

namespace HistoricalChartSpace
{
    const int HistoricalChart::m_RetrieveBarCount = 500;
    HistoricalChart::HistoricalChart(QWidget *parent, QWidget* dialogParent, HistoricalDataSource* dataSource, QString& templateFilePath, const QString& language, QColor& upColor, QColor& downColor,
                                     bool isBlackTheme, bool isChangeChartToolbarByChangeSystemTheme, QString merchantPath, double pixelSizeRatio, bool horizontalScreen)
		:QGraphicsView(parent),
		m_dataSource(dataSource),
		m_crosshair(nullptr),
		m_instrument(nullptr, nullptr, 2),
		m_frequency(Minute),
		m_drawingObjectType(enDrawingObjectTypeUnknown),
		m_listInstrument(dataSource->getInstrumentList()),
		m_listFrequency(dataSource->getFrequencyList()),
		m_isGettingChartData(true),
		m_chartPeriodType(Unlimited),
		m_tradePaused(false),
		m_systemTime(nullptr),
		m_zoomController(nullptr),
		m_scene(nullptr),
		m_XCoorPanel(nullptr),
		m_pricePanel(nullptr),
		m_templateManager(nullptr),
		m_inFixPeriodMode(false),
		m_tradeHelper(nullptr),
		m_orderLineManager(nullptr),
		m_priceAlertAdapter(nullptr),
		m_calculator(nullptr),
		m_dialogParent(dialogParent),
		m_quotePolicesRequestCallId(0),
		m_initDataRequestCallId(0),
		m_lastBarRequestCallId(0),
		m_fixedPeriodDataRequestCallId(0),
		m_systemTimeRequestCallId(0),
		m_appendDataRequestCallId(0),
		m_endTimeOfAppendDataRequest(0),
		m_language(language),
        m_config(upColor, downColor),
        m_merchantPath(merchantPath),
        m_userClickAtProductCode(false)
	{
        #if defined (RUNTIME_OS_IPHONE) || defined (RUNTIME_OS_ANDROID)
                grabGesture(Qt::PanGesture);
                grabGesture(Qt::PinchGesture);
                m_pixelSizeRatio = pixelSizeRatio;
                Helper::m_pixelSizeRatio = pixelSizeRatio;
                m_horizontalScreen = horizontalScreen;
        #endif


		QLOG_INFO("Chart") << "HistoricalChart::HistoricalChart()" << this << QThread::currentThreadId();
		if (dialogParent == nullptr)
		{
			m_dialogParent = this;
		}
		m_calculator = new Calculator(this);

        QString lang;
        #if defined (RUNTIME_OS_IPHONE) || defined (RUNTIME_OS_ANDROID)
        lang = SR::setLanguage(language, QString("%1/ChartSR").arg(merchantPath));
        if (lang != "ENG")
            this->setStyleSheet("font-family:\"ËÎÌו\";font-size:9pt");
        else
            this->setStyleSheet("font-size:9pt");
        #endif
        #if defined (RUNTIME_OS_WIN32) || defined (RUNTIME_OS_MACX)
        lang = SR::setLanguage(language, "");
        if (lang != "ENG")
        {
            this->setStyleSheet("font-family:\"ËÎÌו\";font-size:9pt");
        }
        #endif

		m_timer = new QTimer(this);
		connect(m_timer, SIGNAL(timeout()), SLOT(ChartLagCheckSlot()));

		if (m_listInstrument.count() > 0)
		{
            // Instrument Alphabetically
			QList<Instrument>& instruments = const_cast<QList<Instrument>&>(m_listInstrument);
			std::sort(instruments.begin(), instruments.end());
		}

		this->createActions();
		m_toolbar = new ToolBar(this, isBlackTheme);
		m_templateManager = new TemplateManager(this, templateFilePath);
        m_toolbar->createToolBar(m_listInstrument, m_listFrequency, m_templateManager, isChangeChartToolbarByChangeSystemTheme);

		//this->setWindowFlags(Qt::Window);
		//	this->setAttribute(Qt::WA_DeleteOnClose);
		this->setObjectName("ChartObject");

		m_tradeHelper = new TradeHelper(this);
		m_orderLineManager = new OrderLineManager(this);
		m_cursorPosition = new CursorPosition(this);
		m_priceAlertAdapter = new PriceAlertAdapter(this);

		qRegisterMetaType<QSharedPointer<vector<PrimHistoricalData>>>("QSharedPointer<vector<PrimHistoricalData>>");
		connect(this, SIGNAL(DataProcessedSignal(bool, bool, bool)), this, SLOT(DataProcessedSlot(bool, bool, bool)));

        #ifdef RUNTIME_OS_IPHONE
        m_toolbar->setVisible(false);
        m_toolbar->setFixedSize(0,0);
        #endif
        #ifdef RUNTIME_OS_ANDROID
        m_toolbar->setVisible(false);
        m_toolbar->setFixedSize(0,0);
        #endif

	}

	HistoricalChart::~HistoricalChart()
	{
		if (m_systemTime)
		{
			delete m_systemTime;
			m_systemTime = nullptr;
		}
		if (m_zoomController)
		{
			delete m_zoomController;
			m_zoomController = nullptr;
		}
		if (m_templateManager)
		{
			delete m_templateManager;
			m_templateManager = nullptr;
		}
		if (m_tradeHelper)
		{
			delete m_tradeHelper;
		}
		if (m_orderLineManager)
		{
			delete m_orderLineManager;
		}
		if (m_priceAlertAdapter)
		{
			delete m_priceAlertAdapter;
		}
		if (m_calculator)
		{
			m_calculator->stop();
			delete m_calculator;
		}
		if (m_XCoorPanel)
		{
			delete m_XCoorPanel;
		}

        #if defined (RUNTIME_OS_IPHONE) || defined (RUNTIME_OS_ANDROID)
        if (m_mainIndicatorSettingPanel)
        {
            delete m_mainIndicatorSettingPanel;
        }
        if (m_secondaryIndicatorSettingPanel)
        {
            delete m_secondaryIndicatorSettingPanel;
        }
        #endif
		m_toolbar->m_chartDestoryed = true;
	}

	QWidget* HistoricalChart::getDialogParent()
	{
		return m_dialogParent;
	}

	BaseData& HistoricalChart::getBaseData()
	{
		return m_baseData;
	}

	PricePanel* HistoricalChart::getPricePanel()
	{
		return m_pricePanel;
	}

	QGraphicsScene* HistoricalChart::getScene()
	{
		return m_scene;
	}

	PriceAlertAdapter* HistoricalChart::getPriceAlertManager()
	{
		return m_priceAlertAdapter;
	}

	Calculator* HistoricalChart::getCalculator()
	{
		return m_calculator;
	}

	bool HistoricalChart::isInFixPeriodMode()
	{
		return m_inFixPeriodMode;
	}

	QString HistoricalChart::getTitle()
	{
		return m_instrument.getDescription() + "," + m_frequency.getName();
	}

	void HistoricalChart::setFrequency(const Frequency& frequency)
	{
		m_frequency = frequency;
		this->setWindowTitle(getTitle());
	}

	const Instrument& HistoricalChart::getInstrument()
	{
		return m_instrument;
	}

	QUuid HistoricalChart::getQuotePolicyId()
	{
		return m_quotePolicy.getKey();
	}

	KeyDescription HistoricalChart::getQuotePolicy()
	{
		return m_quotePolicy;
	}

	int HistoricalChart::getDecimalPlaces()
	{
		return m_instrument.getDecimalPlaces();
	}

	Frequency HistoricalChart::getFrequency()
	{
		return m_frequency;
	}

	void HistoricalChart::setQuickPlacer(QuickPlacer* placer)
	{
		m_tradeHelper->setQuickPlacer(placer);
		m_tradeHelper->showQuickPlacer(m_config.getCommonSettings().m_isShowQuickPlacer);
	}

	void HistoricalChart::setQuickModifier(QuickModifier* modifier)
	{
		m_orderLineManager->setQuickModifier(modifier);
	}

	QString HistoricalChart::getConfigXml()
	{
        m_config.saveCommonSettings(m_instrument, m_quotePolicy, m_frequency, m_userClickAtProductCode);
		m_pricePanel->saveSettings();
        foreach (IndicatorPanel* pannel, m_indicatorPanels)
		{
			if (pannel != NULL)
			{
				pannel->saveSettings();
			}
		}
		return m_config.getConfigXml();
	}

	void HistoricalChart::loadConfigXml(QString& configXml)
    {
		if (configXml.isNull() || configXml.isEmpty())
		{
			QString xml = m_templateManager->getDefaultTemplateConfigXml();
			m_config.loadConfigXml(xml);
		}
		else
		{
			m_config.loadConfigXml(configXml);
        }
        this->initializePanels();
        this->arrangePanels();
        m_dataSource->bindView(this);
        this->requestQuotePolicies();
		this->setWindowTitle(this->getTitle());
	}

	// same to loadConfigXml except loading instrument,quotePolicy,frequency
	void HistoricalChart::applyTemplate(QString& configXml)
	{
		if (m_config.applyTemplate(configXml))
		{
			this->initializePanels();
			this->arrangePanels();
		}
	}

	void HistoricalChart::createActions()
	{
		m_toolbarSwitchAction = new QAction(SR::get("ToolBarSwitch"), this);
		connect(m_toolbarSwitchAction, SIGNAL(triggered()), SLOT(HideToolBarSlot()));

		m_commonPropertyAction = new QAction(SR::get("CommonProperty"), this);
		connect(m_commonPropertyAction, SIGNAL(triggered()), SLOT(ShowCommonPropertySlot()));

		m_refreshAction = new QAction(SR::get("Refresh"), this);
		connect(m_refreshAction, SIGNAL(triggered()), SLOT(RefreshSlot()));

		m_indicatorListAction = new QAction(SR::get("IndicatorList"), this);
		connect(m_indicatorListAction, SIGNAL(triggered()), SLOT(ShowIndicatorListSlot()));

		m_drawingObjectListAction = new QAction(SR::get("DrawingObjectList"), this);
		connect(m_drawingObjectListAction, SIGNAL(triggered()), SLOT(ShowDrawingObjectListSlot()));
	}

	void HistoricalChart::initializePanels()
    {
        int listInstrumentIndex = 1; // for C++
#if defined (RUNTIME_OS_IPHONE) || defined (RUNTIME_OS_ANDROID)
        listInstrumentIndex = 0;
#endif
        if (m_config.hasConfigData())
        {
            auto saveInstrument = m_config.getInstrument();
            m_userClickAtProductCode = m_config.getUserClickAtProductCode();
            if(m_userClickAtProductCode)
            {
                foreach(Instrument ins, m_listInstrument)
                {
                    if(ins.getId() == saveInstrument.getId())
                    {
                        m_instrument = ins;
                        break;
                    }
                }
                if(m_instrument.getId().isNull())
                {
                     m_instrument = m_listInstrument.at(listInstrumentIndex);
                }
            }
            else
            {
                if(!saveInstrument.getId().isNull())
                {
                    int index = m_listInstrument.indexOf(saveInstrument);
                    if (index >= 0)
                    {
                        m_instrument = m_listInstrument[index];
                    }
                    else if (m_listInstrument.size() > 0)
                    {
                        m_instrument = m_listInstrument.at(listInstrumentIndex);
                    }
                }
                else
                {
                     m_userClickAtProductCode = true;
                     m_instrument = m_listInstrument.at(listInstrumentIndex);
                }
            }
        }
        else
        {
            if (m_listInstrument.size() > 0)
            {
                m_instrument = m_listInstrument.at(listInstrumentIndex);
            }
            if (m_listFrequency.size() > 0)
            {
                m_frequency = m_listFrequency.at(0);
            }
        }
		bool sceneCreated = m_scene != nullptr;
		// Create scene
		if (sceneCreated)
        {
			// clear indicator pannels
            foreach (IndicatorPanel* panel, m_indicatorPanels)
			{
				this->removeIndicatorPanel(panel);
			}
            m_pricePanel->applyConfig();
		}
		else
		{
			m_scene = new QGraphicsScene(this);
			if (m_scene == NULL) return;
			this->setScene(m_scene);
			m_crosshair = new Crosshair(this, m_cursorPosition);

			// Create X coordinate and grid
			m_XCoorPanel = new XCoorPanel(m_config, m_zoomController);
			if (m_XCoorPanel == NULL) return;
			m_scene->addItem(m_XCoorPanel);

			// Create the main price window
			m_pricePanel = new PricePanel(this, m_config, m_baseData);
			if (m_pricePanel == NULL) return;
			m_zoomController = new ZoomController(*this, m_config, *m_pricePanel->getMainPainterPanel());
			m_scene->addItem(m_pricePanel);

      #if defined (RUNTIME_OS_IPHONE) || defined (RUNTIME_OS_ANDROID)
            QMap<QString, enIndicatorType> mainIndicators;
            foreach(Indicator* indicatorItem, m_pricePanel->getIndicators())
            {
                QString shorthandName = IndicatorCore::getIndicatorShorthandName(indicatorItem->getIndicatorSettings().m_type);
                if(!mainIndicators.contains(shorthandName))
                    mainIndicators.insert(shorthandName, indicatorItem->getIndicatorSettings().m_type);
            }
            this->m_mainIndicatorSettingPanel = new IndicatorSettingPanel(this->m_pixelSizeRatio ,this->m_merchantPath, mainIndicators, m_listFrequency, this->m_horizontalScreen);
            if (m_mainIndicatorSettingPanel == NULL) return;
            connect(m_mainIndicatorSettingPanel, SIGNAL(ShowIndicatorSettingDialogSignal()), this, SLOT(ShowIndicatorSettingDialogSlot()), Qt::QueuedConnection);
            connect(m_mainIndicatorSettingPanel, SIGNAL(ChangeIndicatorSignal()), this, SLOT(ChangeMainIndicatorInfoSlot()), Qt::QueuedConnection);
            connect(m_mainIndicatorSettingPanel, SIGNAL(SetFrequencySignal(int)), this, SLOT(SetFrequencySlot(int)));
            m_scene->addItem(m_mainIndicatorSettingPanel);
      #endif

			// Create information display label
            m_infoLabel = new InfoLabel("",this);
			m_infoLabel->setAlignment(Qt::AlignLeft);
        }

		// Set background
		const QColor& background = m_config.getCommonSettings().m_background;
		m_scene->setBackgroundBrush(background);

		m_infoLabel->setStyleSheet("color:" + m_config.getCommonSettings().m_foreground.name());
		m_infoLabel->setVisible(m_config.getCommonSettings().m_isShowLabelInfo);
qDebug() << " HistoricalChart::initializePanels 5";



		// Read indicator
		if (m_config.hasConfigData())
        {            
            m_quotePolicy = m_config.getQuotePolicy();
#ifdef RUNTIME_OS_MACX
            QUuid quotePolicyId = m_quotePolicy.getId();
            qDebug() <<"m_config.getQuotePolicy() = " << quotePolicyId;
#endif
            m_frequency = m_config.getFrequency();
			Config::IndicatorPanelIterator iterator(m_config);
			double heightRatio;
            QVector<IndicatorSettings> indicatorSettings;
            QMap<QUuid, QVector<DrawingObjectSettings>> drawingObjectSettingMaps;
            while (iterator.getCurrentSettings(heightRatio, indicatorSettings, drawingObjectSettingMaps))
            {
				IndicatorPanel* indicatorPanel = new IndicatorPanel(this, m_config, m_baseData);
				indicatorPanel->setHeightRatio(heightRatio);
                indicatorPanel->applySettings(indicatorSettings, drawingObjectSettingMaps);
				m_scene->addItem(indicatorPanel);
				this->addSplitPanel(indicatorPanel);
				m_indicatorPanels << indicatorPanel;
				indicatorSettings.clear();
                drawingObjectSettingMaps.clear();
            }
            qDebug() << " HistoricalChart::initializePanels 6";
#if defined (RUNTIME_OS_IPHONE) || defined (RUNTIME_OS_ANDROID)
            qDebug() << "m_indicatorPanels.count:" << m_indicatorPanels.count();
            enIndicatorType indicator = m_indicatorPanels.count() == 0 ?
                        enIndicatorMACD :
                        m_indicatorPanels.first()->getIndicators().first()->getIndicatorSettings().m_type;
            qDebug() << " enIndicatorType:" << indicator;
            this->m_secondaryIndicatorSettingPanel = new IndicatorSettingPanel(this->m_pixelSizeRatio ,this->m_merchantPath, indicator, this->m_horizontalScreen);
            if (m_secondaryIndicatorSettingPanel == NULL) return;
            connect(m_secondaryIndicatorSettingPanel, SIGNAL(ShowIndicatorSettingDialogSignal()), this, SLOT(ShowIndicatorSettingDialogSlot()), Qt::QueuedConnection);
            connect(m_secondaryIndicatorSettingPanel, SIGNAL(ChangeIndicatorSignal()), this, SLOT(ChangeSecondaryIndicatorInfoSlot()), Qt::QueuedConnection);
            m_scene->addItem(m_secondaryIndicatorSettingPanel);
#endif
		}

#if defined (RUNTIME_OS_IPHONE) || defined (RUNTIME_OS_ANDROID)
        this->m_mainIndicatorSettingPanel->setFrequency(m_frequency);        
        m_infoLabel->setVisible(false);
#endif
        m_toolbar->applySettings(m_config, m_instrument, m_frequency, m_userClickAtProductCode);
		if (!sceneCreated)
		{
			// create connections for comboboxes
			m_toolbar->makeComboBoxConnections();
			m_toolbar->resetPeriodComboBox(m_frequency);
        }
		CommonSettings settings = m_config.getCommonSettings();
		m_crosshair->setVisible(settings.m_isShowCrosshair);
		m_tradeHelper->showQuickPlacer(settings.m_isShowQuickPlacer);
		m_orderLineManager->showOrder(true, settings.m_isShowExecutedOrder);
        m_orderLineManager->showOrder(false, settings.m_isShowPendingOrder);

        //this->ClickInstrumentSwitchChartSlot(m_instrument.getId());
	}

    #if defined (RUNTIME_OS_IPHONE) || defined (RUNTIME_OS_ANDROID)
    void HistoricalChart::showInfoPanel(bool isShowLabelInfo)
    {
        m_infoLabel->setVisible(isShowLabelInfo);
    }

    void HistoricalChart::showCrossHair(bool isCrossHair)
    {
        m_crosshair->setVisible(isCrossHair);
    }

    void HistoricalChart::setDialogParent(QWidget* dialogParent)
    {
        m_dialogParent = dialogParent;

    }
    #endif

	void HistoricalChart::UpdateQuotePolicy(int callId, QList<KeyDescription>& quotePolicies)
	{
		if(m_quotePolicesRequestCallId == callId)
		{
			m_quotePolicesRequestCallId = 0;
			if (quotePolicies.count() > 0)
			{
				m_listQuotePolicy.clear();
				m_listQuotePolicy.append(quotePolicies);
				m_toolbar->updateQuotePolicy(m_listQuotePolicy, m_quotePolicy);
				if (m_chartPeriodType == Unlimited)
				{
					this->requestChartData(true);
					m_zoomController->resetZoomLevels();
				}
				else
				{
					m_baseData.clear();
					this->SetPeriodSlot(m_chartPeriodType);
				}
			}
			else
			{
				m_requestQuotePolicyTimerId = this->startTimer(1000);
			}
		}
	}

	void HistoricalChart::UpdateHistoricalData(int callId, QSharedPointer<vector<PrimHistoricalData>> vecData)
	{
		bool isBaseDataUpdated = vecData->size() > 0;
		bool isFixPeriodData = false;
		bool isInitialData = false;
		if (m_initDataRequestCallId == callId)
		{
			m_initDataRequestCallId = 0;
			isInitialData = true;
			if (vecData->size() > 0)
			{
				this->killTimer(m_requestInitDataTimerId);
				QMutexLocker locker(&m_baseData.m_mutex);
				m_baseData.clear();
				for (size_t i = 0; i < vecData->size(); ++i)
				{
					m_baseData.appendData((*vecData)[i]);
				}
			}
		}
		else if (m_appendDataRequestCallId == callId)
		{
			m_appendDataRequestCallId = 0;
			m_endTimeOfAppendDataRequest = 0;
			// append market data on the left ( only add data on the left( performance issue ) ? OR keep the bars count less then m_MaxBarCount ?
			if (vecData->size() > 0)
			{
				// junction point analysis and make connection
				double junctionPointTiemSpan = m_baseData.getDate(0) - (*vecData)[vecData->size() - 1].date;
				double allowedTimeSpan = m_frequency.getJDateTimeSpan() * 0.5;
				//if (junctionPointTiemSpan >= 0 && junctionPointTiemSpan < allowedTimeSpan)
				if (junctionPointTiemSpan >= 0)
				{
					QtConcurrent::run([=]()
					{
						{
							QMutexLocker locker(&m_baseData.m_mutex);
							// AuxService may return that the last time of data less then requestEndTime's, so set endIndex to :vecData->size() - 1 to accept all data:
							int endIndex = junctionPointTiemSpan < allowedTimeSpan ? vecData->size() - 2 : vecData->size() - 1;
							for (int i = endIndex; i >= 0; i--)
							{
								this->m_baseData.insertData((*vecData)[i]);
							}
							int insertDataCount = vecData->size() - 1;
							if (insertDataCount > 0)
							{
								// adjust beginIndex and endIndex
								this->m_baseData.setBeginIndex(this->m_baseData.getBeginIndex() + insertDataCount);
								this->m_baseData.setEndIndex(this->m_baseData.getEndIndex() + insertDataCount);
							}
						}
						this->DataProcessedSignal(true, false, false);
					});
					return;
				}
				else
				{
					isBaseDataUpdated = false;
					qDebug() << "Request endTime:" << JulianDate::FromJDateToString(m_baseData.getDate(0));
					qDebug() << "Return data endTime:" << JulianDate::FromJDateToString((*vecData)[vecData->size() - 1].date);
				}
			}
		}
		else if (m_fixedPeriodDataRequestCallId == callId)
		{
			QLOG_INFO("Chart.log") << "HistoricalChart UpdateHistoricalData fixedPeriodData got, dataSize:" << vecData->size()
				<< "callId:" << callId;

			isFixPeriodData = true;
			if (vecData->size() > 0)
			{
				QMutexLocker locker(&m_baseData.m_mutex);
				// Data returned by the data source multiple times can be accepted here (the data source may obtain data from the local cache or the background respectively)
				if (m_baseData.size() == 0)
				{
					for (int i = 0; i < vecData->size(); i++)
					{
						m_baseData.appendData(vecData->at(i));
					}
				}
				else
				{
					if (vecData->size() > 1)
					{
                        //_ASSERT(vecData->at(vecData->size() - 1).date == m_baseData.getDate(0));
						if (vecData->at(vecData->size() - 1).date <= m_baseData.getDate(0))
						{
							m_baseData.joinDatasAtFront(vecData.data());
						}
						else if (vecData->at(vecData->size() - 1).date > m_baseData.getDate(m_baseData.size() - 1))
						{
							m_baseData.joinDatasAtEnd(vecData.data());
						}
					}
				}
				// caculate a specify zoomlevel here
				m_zoomController->zoomAll();
				m_toolbar->setZoomState(true, false);
			}
		}
		else
		{
			isBaseDataUpdated = false;
		}

		this->DataProcessedSignal(isBaseDataUpdated, isFixPeriodData, isInitialData);
	}

	void HistoricalChart::DataProcessedSlot(bool isBaseDataUpdated, bool isFixPeriodData, bool isInitialData)
    {
		if (isBaseDataUpdated)
		{
			if (isInitialData)
			{
				m_pricePanel->moveToEnd();
			}
			this->calculateIndicatorsData(0, m_baseData.size() - 1);
			this->updateChart(isFixPeriodData || isInitialData);

			if (isFixPeriodData || isInitialData)
			{
				this->synchronizeSystemTime();
			}
		}

		this->setIsGettingChartData(false);

#if defined (RUNTIME_OS_IPHONE) || defined (RUNTIME_OS_ANDROID)
//        if(isInitialData && m_mainIndicatorSettingPanel->isNullIndicatorInfoLabel())
//        {
//            m_InitIndicatorSettingPanelTimerId = this->startTimer(1000);
//        }
#endif
        int bars = m_pricePanel->getSpareBar();
        if(bars > 0)
        {
            int pixels = (int)bars * m_baseData.getBarDistance()  * Helper::GetMarginSpaceValue(enMarginMouseMoveScale); //Data offset
            moveChartPixels(pixels);
        }

		QLOG_INFO("Chart.log") << "HistoricalChart DataProcessedSlot UI updated";
	}

	void HistoricalChart::updateOrderLines()
	{
		m_orderLineManager->updateUI();
	}

	void HistoricalChart::ChartDataNotifySlot(PrimHistoricalData data)
	{
		QMutexLocker locker(&m_baseData.m_mutex);

		const int lastIndex = m_baseData.size() - 1;
		const double lastDate = m_baseData.getDate(lastIndex);
		if (JulianDate::lessThan(lastDate, data.date))
		{
			// Add new data
			double timeSpan = m_frequency.getJDateTimeSpan();
			if (lastDate + timeSpan * 1.5 < data.date)
			{
				// request gap data once.
				this->requestLastBars(lastDate);
			}
			else
			{
				vector<PrimHistoricalData> lastBars;
				lastBars.push_back(data);
				this->updateLastBars(&lastBars, true);
			}
		}
		else
		{
			// Update existing data
			int count = 0;
			for (int index = lastIndex; index > 0; index--)
			{
				double chartDataDate = m_baseData.getDate(index);
				if (JulianDate::isEqual(data.date, chartDataDate))
				{
					vector<PrimHistoricalData> lastBars;
					lastBars.push_back(data);
					this->updateLastBars(&lastBars);
					break;
				}
				if (++count > 30) break;
			}
		}
	}
	void HistoricalChart::ChangeUpDownColorSlot(QColor& upColor, QColor& downColor)
	{
		PriceSettings& priceSettings = m_config.getPriceSettings();
		priceSettings.m_bullCandleColor = upColor;
		priceSettings.m_bearCandleColor = downColor;
		priceSettings.m_barUpColor = upColor;
		priceSettings.m_barDownColor = downColor;
	}

    void HistoricalChart::ZoomSlot(bool isZoomIn)
    {
        m_zoomController->zoom(isZoomIn);
        int bars = m_pricePanel->getSpareBar();
        if(bars > 0)
        {
            int pixels = (int)bars * m_baseData.getBarDistance()  * Helper::GetMarginSpaceValue(enMarginMouseMoveScale); //Data offset
            moveChartPixels(pixels);
        }
    }

    void HistoricalChart::ClickInstrumentSwitchChartSlot(QUuid instrumentId)
    {
        if(m_userClickAtProductCode)
        {
            foreach(Instrument instrument, m_listInstrument)
            {
                if(instrument.getId() == instrumentId)
                {
                    m_instrument = instrument;
                    this->setWindowTitle(this->getTitle());
                    this->requestQuotePolicies();
                    QUuid instrumentId = m_instrument.getKey();
                    m_tradeHelper->changeInstrument(instrumentId);
                    m_orderLineManager->changeInstrument(instrumentId);
                    emit InstrumentChangedSignal(instrumentId);                    
                    #if defined (RUNTIME_OS_WIN32) || defined (RUNTIME_OS_MACX)
                            this->setInfoPanelContent();
                    #endif

                }
            }
        }
    }

#if defined (RUNTIME_OS_IPHONE) || defined (RUNTIME_OS_ANDROID)
    void HistoricalChart::ShowIndicatorSettingDialogSlot()
    {
        IndicatorSettingDialog* indicatorSettingDialog = new IndicatorSettingDialog(m_pixelSizeRatio, this, this->m_horizontalScreen, m_dialogParent);
        connect(indicatorSettingDialog, SIGNAL(addIndicatorSignal(int)), this, SLOT(AddIndicatorSlot(int)));
        indicatorSettingDialog->showDialog();
    }

    void HistoricalChart::ChangeMainIndicatorInfoSlot()
    {
        QPointF point = m_cursorPosition->getScenePos();
        this->calIndicatorInfoValue(point);
    }

    void HistoricalChart::ChangeSecondaryIndicatorInfoSlot()
    {
        if(m_indicatorPanels.count() > 0)
        {
            QPointF point = m_cursorPosition->getScenePos();
            this->calIndicatorInfoValue(point);
        }
    }

    void HistoricalChart::initIndicatorSettingPanelSlot()
    {
        if(m_mainIndicatorSettingPanel->isNullIndicatorInfoLabel())
        {
            QPointF viewPoint = QPointF(m_pricePanel->rect().bottomRight().x() - Helper::GetMarginSpaceValue(enMarginYCoordinate) - 50, (30 * this->m_pixelSizeRatio) + 100);
            qDebug() << "HistoricalChart::DataProcessedSlot: viewPoint:" << viewPoint;
            this->calIndicatorInfoValue(viewPoint);
        }
    }
#endif



	void HistoricalChart::LastBarDataCome(int callId, QSharedPointer<vector<PrimHistoricalData>> datas)
	{
		if (m_lastBarRequestCallId == callId)
		{
			m_lastBarRequestCallId = 0;

			QMutexLocker locker(&m_baseData.m_mutex);
			this->updateLastBars(datas.data());
		}
	}

	void HistoricalChart::updateLastBars(vector<PrimHistoricalData>* datas, bool isAppend /* = false*/)
	{
		bool dataAppened = isAppend;
		bool dataUpdated = false;

		PrimHistoricalData data = datas->at(0);
		//qDebug()
		//	<< " open:" << data.open << " close:" << data.close << " high:" << data.high << " low:" << data.low << ", isAppend:" << isAppend << " , data_size:" << datas->size()
		//	<< "Begin update Notify data.date:" << JulianDate::FromJDateToString(data.date);

		if (isAppend) 
		{
			for (int i = 0; i < datas->size(); i++)
			{
				m_baseData.appendData(datas->at(i));
			}
		}
		else
		{
			const int maxDataIndex = m_baseData.size() - 1;
			const double& serverJointDate = datas->at(0).date;
			int jointIndex = maxDataIndex;
			bool foundJoint = false;

			// find jointIndex from last 30 data items in m_baseData
			for (int i = 0; i < 30; i++)
			{
				jointIndex -= i;
				if (jointIndex > 0)
				{
					if (JulianDate::isEqual(m_baseData.getDate(jointIndex), serverJointDate))
					{
						foundJoint = true;
						break;
					}
				}
				else break;
			}

			if (foundJoint)
			{
				for (int i = 0; i < datas->size(); i++)
				{
					int index = jointIndex + i;
					if (index <= maxDataIndex)
					{
						//qDebug()
						//	<< " open:" << m_baseData.getOpens()[index] << " close:" << m_baseData.getCloses()[index]
						//	<< " high:" << m_baseData.getHighs()[index] << " low:" << m_baseData.getLows()[index]
						//	<< " old data";

						m_baseData.update(index, datas->at(i));
					}
					else
					{
						m_baseData.appendData(datas->at(i));
					}
				}

				dataAppened = m_baseData.size() > maxDataIndex + 1;
				dataUpdated = true;
			}
		}

		if (dataAppened || dataUpdated)
		{
			// extend all indicators DataBuffer first (to insure indicator painted before indicator data calculated)
			if (dataAppened)
			{
				m_pricePanel->extendIndicatorDataBuffer();
				foreach(IndicatorPanel* panel, m_indicatorPanels)
				{
					panel->extendIndicatorDataBuffer();
				}
			}

			// calculate indicator data
			int endIndex = m_baseData.size() - 1;
			int beginIndex = endIndex - (datas->size() - 1);
			this->calculateIndicatorsData(beginIndex, endIndex);

			if (dataAppened && m_config.getCommonSettings().m_isAutoScroll)
			{
				m_pricePanel->moveToEnd();
			}
			this->updateChart(!m_config.getCommonSettings().m_isAutoScroll);
		}
	}

	// Graphic lag check
	void HistoricalChart::ChartLagCheckSlot()
	{
		double now = m_systemTime->now();
		double dataLastTime = m_baseData.getDate(m_baseData.size() - 1);
		double chartLeadingSpan = dataLastTime + m_frequency.getJDateTimeSpan() - now;
		if (chartLeadingSpan > 0)
		{
			// set new interval for last bar checking.
			this->startTimerWithRandDelay(chartLeadingSpan * 24 * 60 * 60 * 1000);
		}
		else
		{
			// Graphics lag, request data.
			int index = m_baseData.size() - 2;
			if (index < 0) index = 0;
			double lastDataDate = m_baseData.getDate(index);
			if (lastDataDate > 0)
			{
				this->requestLastBars(lastDataDate);
				int interval = m_frequency.getTimeSpanInSecond() * 1000;
				this->startTimerWithRandDelay(interval);
			}
		}
	}

	void HistoricalChart::ChangeTradeStateSlot(bool paused)
	{
		m_tradePaused = paused;
		if (m_tradePaused)
		{
			m_timer->stop();
		}
		else
		{
			this->synchronizeSystemTime();
		}
	}

	void HistoricalChart::synchronizeSystemTime()
	{
		m_systemTimeRequestCallId = DataSource::uniqueCallId++;
		QSharedPointer<GetSystemTimeCallback> callback(new GetSystemTimeCallback(m_systemTimeRequestCallId, this));
		m_dataSource->beginGetCurrentTime(callback);
	}

	void HistoricalChart::setSystemTime(int callId, double currentTime)
	{
		if (m_systemTime)
		{
			delete m_systemTime;
			m_systemTime = NULL;
		}
		m_systemTime = new SystemTime(currentTime);

		if (m_systemTimeRequestCallId == callId)
		{
			m_systemTimeRequestCallId = 0;
			// Start graphics lag detection
			this->ChartLagCheckSlot();
		}
	}

	void HistoricalChart::requestLastBars(double beginTime)
	{
		m_lastBarRequestCallId = DataSource::uniqueCallId++;
		QSharedPointer<GetLastBarCallback> callback(new GetLastBarCallback(m_lastBarRequestCallId, this));
		m_dataSource->beginGetLastBars(this->getChartProperty(), beginTime, callback);
	}

	void HistoricalChart::RealTimeDataComeSlot(const PrimRealTimeData& data)
	{
		if (m_pricePanel == NULL || m_isGettingChartData) return;

		double lastDate = m_baseData.getDate(m_baseData.size() - 1);
		double timeSpan = m_frequency.getJDateTimeSpan();
		int multiples = (data.date - lastDate) / timeSpan;
		int duration = 3000;
		if (multiples >= 1 && m_timer->remainingTime() > duration)
		{
			// Real-time data correction to obtain the time point of lastBar
			this->startTimerWithRandDelay(0);
			qDebug() << "start time 3, interval:" << duration;
		}
		else
		{
			m_baseData.realTimeDataCome(data, timeSpan);
		}
		//m_pricePanel->update(); // No, the interface will be refreshed below.
		if (m_baseData.size() > 0)
		{
			m_pricePanel->refresh();
			int beginIndex, endIndex;
			beginIndex = endIndex = m_baseData.size() - 1;
			this->calculateIndicatorsData(beginIndex, endIndex);
		}
	}

	void HistoricalChart::startTimerWithRandDelay(int interval)
	{
		if (interval < 0)  // Monthly and annual lines overflow
		{
			interval = 2147483647; // int.MaxValue
		}
		interval += 2000 + 3000 * std::rand() / RAND_MAX;  // Delay 2 seconds + 0-3000 milliseconds
		m_timer->start(interval);
        //qDebug() << " HistoricalChart::startTimerWithRandDelay interval:" << interval;
	}

    void HistoricalChart::setInfoPanelContent()
    {
        QList<QGraphicsItem*> listItem = this->items(this->m_lastViewPoint);
        foreach(QGraphicsItem* item, listItem)
        {
            ChartPanel* panel = dynamic_cast<ChartPanel*>(item);
            if (panel != NULL)
            {
                QPointF scenePoint = this->mapToScene(this->m_lastViewPoint);
                m_cursorPosition->OnPositionChange(scenePoint, panel);

                int lines;
                QString infoText = QString("<br><span style=\"color:%1;font-size:24px;font-weight:bold;\">%2</span><br>").arg(
                            m_config.getCommonSettings().m_productCodeColor.name(),
                            m_instrument.getDescription());
                infoText.append(m_cursorPosition->getInfoText(lines));
                m_infoLabel->changeText(infoText, lines + 4);
                break;
            }
        }
    }

	void HistoricalChart::test()
	{
		m_toolbar->test();
	}

	ToolBar* HistoricalChart::getToolbar()
	{
		return m_toolbar;
	}

	void HistoricalChart::OnThemeChanged(bool isBlack)
	{
		m_toolbar->OnThemeChanged(isBlack);
	}

    void HistoricalChart::OnThemeChanged(bool isBlack, bool userSetChartColorSign, QColor bgColor, QColor fontColor)
    {
        m_toolbar->OnThemeChanged(isBlack);
        if(!userSetChartColorSign)
        {
            CommonSettings& commonSettings = m_config.getCommonSettings();
            commonSettings.m_background = bgColor;
            commonSettings.m_foreground = fontColor;
            commonSettings.m_productCodeColor = fontColor;
            // Set the overall background color
            if (commonSettings.m_background != m_scene->backgroundBrush().color())
                m_scene->setBackgroundBrush(commonSettings.m_background);
            // Set the display of price information
            m_infoLabel->setStyleSheet("color:" + commonSettings.m_foreground.name());
            m_toolbar->updateCommonSettings(commonSettings);
#if defined(RUNTIME_OS_IPHONE) || defined(RUNTIME_OS_ANDROID)
            if (m_mainIndicatorSettingPanel)
            {
                m_mainIndicatorSettingPanel->changeStyle(bgColor, fontColor);
            }
            if (m_secondaryIndicatorSettingPanel)
            {
                 m_secondaryIndicatorSettingPanel->changeStyle(bgColor, fontColor);
            }

#endif
            this->setInfoPanelContent();

            this->updateChart(false);
        }
    }

	void HistoricalChart::DrawingObjectCreated(enDrawingObjectType type)
	{
		m_toolbar->DrawingObjectCreated(type);
	}

	void HistoricalChart::resizeEvent(QResizeEvent *event)
	{
		this->arrangePanels();
		// resize toolbar to allow drop down on the right.
		int chartWidth = event->size().width() + 2;
		m_toolbar->OnChartResize(chartWidth);

		if (m_inFixPeriodMode)
		{
			// if in fixed period mode, caculate a specify zoomlevel
			m_zoomController->zoomCurrentRagne();
		}

        int bars = m_pricePanel->getSpareBar();
        if(bars > 0)
        {
            int pixels = (int)bars * m_baseData.getBarDistance()  * Helper::GetMarginSpaceValue(enMarginMouseMoveScale); //Data offset
            moveChartPixels(pixels);
        }
		return QGraphicsView::resizeEvent(event);
	}

	void HistoricalChart::showGrid()
	{
		if(m_XCoorPanel == NULL || m_pricePanel == NULL) return;

		CommonSettings& settings = m_config.getCommonSettings();
		settings.m_isShowGrid = !settings.m_isShowGrid;

		if(m_pricePanel != NULL)
		{
			m_pricePanel->refresh();
		}

		foreach(IndicatorPanel* panel, m_indicatorPanels)
		{
			if (panel != NULL)
			{
				panel->refresh();
			}
		}
	}

	void HistoricalChart::shiftData()
	{
		if(m_XCoorPanel == NULL || m_pricePanel == NULL) return;

		CommonSettings& settings = m_config.getCommonSettings();
		settings.m_isShiftData = !settings.m_isShiftData;
		m_pricePanel->moveToEnd();
		this->updateChart(false);
	}

    // Cause: After 1.BaseData, 2.Resize, 3.zoomLevel changes, the Basic Parameter(beginIndex,endIndex,barDistance) changes to refresh the interface:
	void HistoricalChart::updateChart(bool isUpadteEndIndex)
	{
        //_ASSERT(m_pricePanel);
		if (isUpadteEndIndex)
		{
			m_pricePanel->updateEndIndex();
		}
		m_pricePanel->refresh();

		// update Indicator Panels
		for (int i = 0; i < m_indicatorPanels.size(); ++i)
		{
			IndicatorPanel* panel = m_indicatorPanels[i];
			if (panel)
			{
				panel->refresh();
			}
		}

		// Update X coordinate
        //_ASSERT(m_XCoorPanel);
		m_XCoorPanel->updateXCoor(m_baseData);
    }

	void HistoricalChart::resizePanelsRect(const QRectF& rect)
	{
        qDebug() << " HistoricalChart::resizePanelsRect rect left:" << rect.left() << " top:" << rect.top() << " width:" << rect.width() << " height:" << rect.height() ;
        //_ASSERT(m_pricePanel);
		QRectF BaseRect(0, 0, rect.width(), rect.height());        
		BaseRect.setHeight(rect.height() - Helper::GetMarginSpaceValue(enMarginXCoordinate));
        qDebug() << " HistoricalChart::resizePanelsRect BaseRect left:" << BaseRect.left() << " top:" << BaseRect.top() << " width:" << BaseRect.width() << " height:" << BaseRect.height() ;

        double mainIndicatorSettingPanelheight = 0.0;
        #if defined (RUNTIME_OS_IPHONE) || defined (RUNTIME_OS_ANDROID)
            mainIndicatorSettingPanelheight = m_pixelSizeRatio * 31;
            QRectF mainIndicatorSettingPanelRect(BaseRect);
            mainIndicatorSettingPanelRect.setHeight(mainIndicatorSettingPanelheight);

            m_mainIndicatorSettingPanel->setRect(mainIndicatorSettingPanelRect);
            m_mainIndicatorSettingPanel->setPos(rect.left(), rect.top());
        #endif
        double splitHight = Helper::GetMarginSpaceValue(enMarginSplitHight) - mainIndicatorSettingPanelheight;
        const QRectF splitRect(0, 0, rect.width(), splitHight);


		// Set price position size
		double heightRatio = m_pricePanel->getHeightRatio();
                double priceHight = BaseRect.height() * heightRatio - (m_indicatorPanels.count() > 0 ? 0 : mainIndicatorSettingPanelheight);
		QRectF priceRect(BaseRect);
		priceRect.setHeight(priceHight);
        qDebug() << " HistoricalChart::resizePanelsRect priceRect:" << priceRect;

		m_pricePanel->setRect(priceRect);
        m_pricePanel->setPos(rect.left(), rect.top() + mainIndicatorSettingPanelheight);
		m_pricePanel->adjustSubItem();
		m_infoLabel->moveToInitialPosition();


        double secondaryIndicatorSettingPanelheight = 0.0;
        #if defined (RUNTIME_OS_IPHONE) || defined (RUNTIME_OS_ANDROID)
            if(m_indicatorPanels.count() > 0)
            {
                m_secondaryIndicatorSettingPanel->setVisible(true);
                secondaryIndicatorSettingPanelheight = m_pixelSizeRatio * 31;
                QRectF secondaryIndicatorSettingPanelRect(BaseRect);
                secondaryIndicatorSettingPanelRect.setHeight(secondaryIndicatorSettingPanelheight);

                m_secondaryIndicatorSettingPanel->setRect(secondaryIndicatorSettingPanelRect);
                m_secondaryIndicatorSettingPanel->setPos(rect.left(), rect.top() + priceHight + mainIndicatorSettingPanelheight + 2);
            }
            else
            {
                if(m_secondaryIndicatorSettingPanel)
                    m_secondaryIndicatorSettingPanel->setVisible(false);
            }
        #endif

#if defined (RUNTIME_OS_IPHONE) || defined (RUNTIME_OS_ANDROID)
        int newChartFullScreenY = rect.top() + priceHight + mainIndicatorSettingPanelheight -
               (m_indicatorPanels.count() == 0 ? (30 * this->m_pixelSizeRatio) : 0);
        if(m_ChartFullScreenY != newChartFullScreenY)
        {
            m_ChartFullScreenY = newChartFullScreenY;
            emit moveChartFullScreenSignal(m_ChartFullScreenY);
        }
#endif
		// Set indicators that are different from the price
		SplitPanel* split = m_pricePanel->getNextSplitPanel();
		if (split == NULL) return;

		split->setRect(splitRect);
        split->setPos(rect.left(), rect.top() + priceHight + mainIndicatorSettingPanelheight + secondaryIndicatorSettingPanelheight);

        #if defined (RUNTIME_OS_IPHONE) || defined (RUNTIME_OS_ANDROID)
            split->setVisible(false);
        #endif

		for (int i = 0; i < m_indicatorPanels.size(); ++i)
		{
			IndicatorPanel* indicatorPanel = m_indicatorPanels[i];
			if (indicatorPanel == NULL || split->getNextPanel() != indicatorPanel) return;
			double ratio = indicatorPanel->getHeightRatio();
			double height = BaseRect.height() * ratio - splitHight;
			QPointF point = split->pos();

			if (point.y() + height + splitHight > BaseRect.height() + rect.top())
			{
                height = BaseRect.height() + rect.top() - point.y() - splitHight;
			}

            indicatorPanel->setRect(0, 0, BaseRect.width(), height - secondaryIndicatorSettingPanelheight);
            qDebug() << " HistoricalChart::resizePanelsRect indicatorPanel Rect:" << indicatorPanel->rect();

            point.setY(point.y() + splitHight + secondaryIndicatorSettingPanelheight);
			indicatorPanel->setPos(point);
			indicatorPanel->adjustSubItem();

			split = indicatorPanel->getNextSplitPanel();
			if (split == NULL) return;
			split->setRect(splitRect);
			point.setY(point.y() + height);
			split->setPos(point);
        }
    }

	void HistoricalChart::changePriceStyle(enPriceType type)
	{
		if(m_pricePanel)
		{
			PriceSettings& settings = m_config.getPriceSettings();
			settings.m_drawType = type;
			m_pricePanel->changeStyle();
		}
	}

	void HistoricalChart::SetInstrumentSlot(int index)
	{
		if (index < m_listInstrument.size())
		{
            auto instrument = m_listInstrument[index];
            if(!instrument.getId().isNull())
            {
                m_instrument = instrument;
                m_userClickAtProductCode = false;
                this->setWindowTitle(this->getTitle());
                this->requestQuotePolicies();
                QUuid instrumentId = m_instrument.getKey();
                m_tradeHelper->changeInstrument(instrumentId);
                m_orderLineManager->changeInstrument(instrumentId);
                emit InstrumentChangedSignal(instrumentId);
                this->setInfoPanelContent();
            }
            else
                m_userClickAtProductCode = true;
		}
	}

	void HistoricalChart::SetQuotePolicySlot(int index)
	{
		if (index < m_listQuotePolicy.size())
		{
            m_quotePolicy = m_listQuotePolicy[index];
			this->requestChartData(true);
			QUuid quotePolicyId = m_quotePolicy.getKey();
			m_tradeHelper->changeQuotePolicy(quotePolicyId);
			m_orderLineManager->changeQuotePolicy(quotePolicyId);
			emit QuotePolicyChangedSignal(quotePolicyId);
		}
    }

	void HistoricalChart::SetFrequencySlot(int index)
	{
		if (index < m_listFrequency.size())
		{
			m_frequency = m_listFrequency[index];
			this->changeFrequency(m_frequency);
		}
	}

	void HistoricalChart::ShowGridSwitchSlot()
	{
		this->showGrid();
	}

	void HistoricalChart::AutoScrollSwitchSlot()
	{
		CommonSettings& settings = m_config.getCommonSettings();
		settings.m_isAutoScroll = !settings.m_isAutoScroll;
	}

	void HistoricalChart::ShiftDataSwitchSlot()
	{
		this->shiftData();
	}

	void HistoricalChart::SetPriceChartSlot(int index)
	{
		this->changePriceStyle((enPriceType)index);
	}

	void HistoricalChart::SetPeriodSlot(int index)
	{
		if (index < 0) return;

		QDate beginDate = m_systemTime ? JulianDate::JDateToQTDate(m_systemTime->now()).date() : QDate::currentDate();
		int quarter;
		bool isDays = false;
		m_chartPeriodType = (PeriodType)m_toolbar->getDisplayPeriod();
		switch (m_chartPeriodType)
		{
		case Unlimited:
			break;
		case Today:
			break;
		case ThisWeek:
			beginDate = beginDate.addDays(1 - beginDate.dayOfWeek());
			break;
		case ThisMonth:
			beginDate.setDate(beginDate.year(), beginDate.month(), 1);
			break;
		case ThisQuarter:
			quarter = (beginDate.month() + 2) / 3;
			beginDate.setDate(beginDate.year(), (quarter - 1) * 3 + 1, 1);
			break;
		case ThisYear:
			beginDate.setDate(beginDate.year(), 1, 1);
			break;
		case Day2:
		case Day3:
		case Day4:
		case Day5:
		case Day10:
		case Day15:
		case Day30:
		case Day45:
		case Day60:
		case Day90:
		case Day120:
		case Day180:
			isDays = true;
			break;
		case Month1:
			beginDate = beginDate.addMonths(-1);
			break;
		case Month2:
			beginDate = beginDate.addMonths(-2);
			break;
		case Month3:
			beginDate = beginDate.addMonths(-3);
			break;
		case Month6:
			beginDate = beginDate.addMonths(-6);
			break;
		case Year1:
			beginDate = beginDate.addYears(-1);
			break;
		case Year2:
			beginDate = beginDate.addYears(-2);
			break;
		case Year3:
			beginDate = beginDate.addYears(-3);
			break;
		case Year4:
			beginDate = beginDate.addYears(-4);
			break;
		case Year5:
			beginDate = beginDate.addYears(-5);
			break;
		case Year10:
			beginDate = beginDate.addYears(-10);
			break;
		case Year20:
			beginDate = beginDate.addYears(-20);
			break;
		default:
			//_ASSERT(false);
			break;
		}
		
		m_inFixPeriodMode = m_chartPeriodType != PeriodType::Unlimited;

		//m_toolbar->changeZoomInStyle(m_inFixPeriodMode);

		if (m_chartPeriodType == PeriodType::Unlimited)
		{
			if (m_baseData.size() < 500)
			{
				this->requestChartData(true);
				m_zoomController->resetZoomLevels();
			}
			else
			{
				m_zoomController->resetZoomLevels();
				m_pricePanel->moveToEnd();
			}
		}
		else
		{
			m_zoomController->resetZoomHistory();

			double periodBeginTime = 0;
			bool hasBeginTime = true;
			if (m_chartPeriodType <= PeriodType::ThisYear)
			{
				periodBeginTime = m_dataSource->GetPeriodBeginTime(m_chartPeriodType);
				if (periodBeginTime == 0) periodBeginTime = beginDate.toJulianDay() - 0.5;
			}
			else if (isDays)
			{
				periodBeginTime = m_dataSource->GetPeriodBeginTime(m_chartPeriodType, m_chartProperty.instrumentId);
				hasBeginTime = periodBeginTime != 0;
				if (!hasBeginTime)
				{
					m_baseData.clear();
				}
			}
			else
			{
				periodBeginTime = beginDate.toJulianDay() - 0.5;
			}

			bool shouldRequestData = true;
			double endTime = m_systemTime ? m_systemTime->now() : JulianDate::QTDateToJDate(QDateTime::currentDateTime());
			if (hasBeginTime && m_baseData.size() > 0)
			{
				double beginTimeOfBaseDate = m_baseData.getDate(0);

				if (beginTimeOfBaseDate > 0 && periodBeginTime >= beginTimeOfBaseDate)
				{
					if (periodBeginTime > m_baseData.getDate(m_baseData.size() - 1))
					{
						// This happens only in extreme cases where the system has not received the price for a period of time
						QMutexLocker locker(&m_baseData.m_mutex);
						m_baseData.clear();
						m_timer->stop();
						QLOG_INFO("Chart") << "HistoricalChart::SetPeriodSlot reqeust beginTime Great then last data's, request cancelled. periodBeginTime:" << periodBeginTime;
					}
					else
					{
						m_baseData.truncateBefore(periodBeginTime);
						this->calculateIndicatorsData(0, m_baseData.size() - 1);
						m_zoomController->zoomAll();
					}
					
					shouldRequestData = false;
				}
				else
				{
					endTime = beginTimeOfBaseDate;
				}
			}

			if (shouldRequestData)
			{
				this->setIsGettingChartData(true);
				m_fixedPeriodDataRequestCallId = DataSource::uniqueCallId++;
				QSharedPointer<GetChartDataCallback> callback(new GetChartDataCallback(m_fixedPeriodDataRequestCallId, this));

				if (isDays && !hasBeginTime)
				{
					m_dataSource->beginGetFixPeriodData(this->getChartProperty(), m_chartPeriodType, callback);
				}
				else
				{
					m_dataSource->beginGetFixPeriodData(this->getChartProperty(), periodBeginTime, endTime, callback);
				}
			}
		}
	}

	// ZoomIn: call from chartPanel
	void HistoricalChart::showSpecifyRange(double startTime, double endTime)
	{
		if (endTime > startTime && m_baseData.getRangeDataCount(startTime, endTime) > 5)
		{
			//this->calculateIndicatorsData(0, m_baseData.size() - 1);

			int beginIndex = m_baseData.timeToIndex(startTime);
			int endIndex = m_baseData.timeToIndex(endTime);

			if (m_inFixPeriodMode)
			{
				m_zoomController->zoomRagne(beginIndex, endIndex);
			}
			else
			{
				m_zoomController->zoomRagneForUnlimited(beginIndex, endIndex);
			}
			m_toolbar->OnZoomInEnd();
			m_toolbar->setZoomState(false, false);
			((MainPainterPanel*)m_pricePanel->getMainPainterPanel())->showNormalCoursor();
			this->updateChart(false);
		}
	}

	// call from toolbar
	void HistoricalChart::OnZoomInSlot()
	{
		//m_zoomController->zoom(true);
		if (m_toolbar->isInZoomIn())
		{
			((MainPainterPanel*)m_pricePanel->getMainPainterPanel())->showZoomInCoursor();
		}
		else
		{
			((MainPainterPanel*)m_pricePanel->getMainPainterPanel())->showNormalCoursor();
		}
	}
	// call from toolbar
	void HistoricalChart::OnZoomOutSlot()
	{
		bool isZoomed = m_zoomController->zoomPrevRagne();
		if (!isZoomed)
		{
			m_zoomController->zoom(false);
		}

		if (!m_inFixPeriodMode)
		{
			if (m_zoomController->isSmallestLevel())
			{
				m_toolbar->setZoomState(true, false);
			}
		}

		if(isZoomed)
			this->updateChart(false);
	}

	void HistoricalChart::HideToolBarSlot()
	{
		m_toolbar->switchVisible();
	}

	void HistoricalChart::arrangePanels()
	{
        QRectF rect = getViewPortRect();
        qDebug() << " HistoricalChart::arrangePanels rect:" << rect;
        resizePanelsRect(rect);
        qDebug() << " HistoricalChart::arrangePanels rect2:" << rect;
        //_ASSERT(m_XCoorPanel);
        m_XCoorPanel->setRect(rect);
        m_XCoorPanel->setPos(0, 0);

		this->updateChart(true);
	}

	QRectF HistoricalChart::getViewPortRect()
	{
        ////_ASSERT(m_scene);
		QRectF rect = this->viewport()->rect();
		m_scene->setSceneRect(rect);
		return rect;
	}

	void HistoricalChart::AddIndicatorSlot()
	{
		QAction *action = qobject_cast<QAction*>(sender());
		if (action == NULL) return;

		int indicatorType = action->data().toInt();
		if (indicatorType >= enIndicatorEnd) return;
        this->AddIndicatorSlot(indicatorType);
	}

    void HistoricalChart::AddIndicatorSlot(int indicatorType)
    {
        qDebug() << " AddIndicatorSlot:" << indicatorType;
		/*
        IndicatorSettings indicatorSettings;
        indicatorSettings.m_type = (enIndicatorType)indicatorType;
        Indicator* pIndicator = IndicatorCore::CreateIndicator(m_baseData, indicatorSettings, true, m_calculator);
        m_calculator->addEndTask();
        if (pIndicator->getIndicatorSettings().m_isInPriceChart)
        {
            m_pricePanel->addIndicator(pIndicator);
        }
        else
        {
            #if defined(RUNTIME_OS_IPHONE) || defined(RUNTIME_OS_ANDROID)
                if(m_indicatorPanels.count() > 0)
                {
                    this->removeIndicatorPanel(m_indicatorPanels[0]);
                }
            #endif

            IndicatorPanel* indicatorPanel = new IndicatorPanel(this, m_config, m_baseData, pIndicator);
            m_scene->addItem(indicatorPanel);

            //// Update indicator data
            int size = m_indicatorPanels.size();
            double ratio = 1.0 / (size + 2);
            double change = 1.0 - ratio;
            double heightRatio = m_pricePanel->getHeightRatio();
            m_pricePanel->setHeightRatio(change * heightRatio);
            heightRatio = ratio;
            indicatorPanel->setHeightRatio(ratio);

            for (int j = 0; j < size; ++j)
            {
                if (m_indicatorPanels[j] == NULL) continue;
                double panelHeightRatio = m_indicatorPanels[j]->getHeightRatio();
                m_indicatorPanels[j]->setHeightRatio(panelHeightRatio * change);
            }

            addSplitPanel(indicatorPanel);
            m_indicatorPanels << indicatorPanel;
        }
        this->arrangePanels();*/
        this->AddIndicator(indicatorType);
    }

	void HistoricalChart::moveChartPixels(int pixels)
	{
		if (m_baseData.size() > 0 && pixels != 0)
		{
			int bars = (int)pixels / m_baseData.getBarDistance()  * Helper::GetMarginSpaceValue(enMarginMouseMoveScale); //Data offset

			if (bars == 0)
			{
				bars = pixels > 0 ? 1 : -1;
			}
			this->moveChartBars(bars);
		}
	}

	void HistoricalChart::calculateIndicatorsData(int beginIndex, int endIndex)
	{
		m_pricePanel->calculateIndicators(beginIndex, endIndex);
		
		foreach(IndicatorPanel* panel, m_indicatorPanels)
		{
			panel->calculateIndicators(beginIndex, endIndex);
		}
		m_calculator->addEndTask();
	}

	void HistoricalChart::showCommonMenu(QGraphicsSceneContextMenuEvent* event, bool fromPricePanel)
	{
		QMenu menu(this);
		QAction placeLimitOrderAction("", &menu), placeStopOrderAction("", &menu);
		if (fromPricePanel && m_baseData.size() > 0)
		{
			double price = m_pricePanel->Y2Value(event->pos().y());
			double bid, ask;
			m_baseData.getBidAsk(bid, ask);
			//qDebug() << QString("HistoricalChart::HandleCommonContextMenuSlot pos:[%1,%2], price:[%3]").arg(event->pos().x()).arg(event->pos().y()).arg(price);
			
			m_tradeHelper->appendQuickTradeMenu(menu, price, bid, ask);
		}
		menu.addAction(m_toolbarSwitchAction);
		menu.addAction(m_commonPropertyAction);
		menu.addAction(m_refreshAction);
		QAction templatesAction(SR::get("Templates"), &menu);
		templatesAction.setMenu(m_templateManager->getMenu());
		menu.addAction(&templatesAction);
		menu.addAction(m_indicatorListAction);
		menu.addAction(m_drawingObjectListAction);
		menu.exec(event->screenPos());
	}

//    void HistoricalChart::showCommonMenu(QTapAndHoldGesture* event, bool fromPricePanel)
//    {
//        QMenu menu(this);
//        QAction placeLimitOrderAction("", &menu), placeStopOrderAction("", &menu);
//        if (fromPricePanel && m_baseData.size() > 0)
//        {
//            double price = m_pricePanel->Y2Value(event->position().y());
//            double bid, ask;
//            m_baseData.getBidAsk(bid, ask);
//            //qDebug() << QString("HistoricalChart::HandleCommonContextMenuSlot pos:[%1,%2], price:[%3]").arg(event->pos().x()).arg(event->pos().y()).arg(price);

//            m_tradeHelper->appendQuickTradeMenu(menu, price, bid, ask);
//        }
//        menu.addAction(m_toolbarSwitchAction);
//        menu.addAction(m_commonPropertyAction);
//        menu.addAction(m_refreshAction);
//        QAction templatesAction(SR::get("Templates"), &menu);
//        templatesAction.setMenu(m_templateManager->getMenu());
//        menu.addAction(&templatesAction);
//        menu.addAction(m_indicatorListAction);
//        menu.addAction(m_drawingObjectListAction);
//        menu.exec(event->position());
//    }

	void HistoricalChart::ShowCommonPropertySlot()
	{
		CommonSettings& commonSettings = m_config.getCommonSettings();
		PriceSettings& priceSettings = m_config.getPriceSettings();
		PriceDialog dialog(commonSettings, priceSettings, m_dialogParent);
        connect(&dialog, SIGNAL(UserSetChartColorSignSignal()), this, SIGNAL(UserSetChartColorSignSignal()));
		dialog.exec();

		if (dialog.result() == QDialog::Accepted)
		{
			// Set the overall background color
			if (commonSettings.m_background != m_scene->backgroundBrush().color())
			{
				m_scene->setBackgroundBrush(commonSettings.m_background);

			}
			// Set the display of price information
			m_infoLabel->setStyleSheet("color:" + commonSettings.m_foreground.name());

			// Adjust PriceStyle Combobox
			m_toolbar->changeDrawType(priceSettings.m_drawType);

			m_toolbar->updateCommonSettings(commonSettings);

			//Whether to display text information
			if (commonSettings.m_isShowLabelInfo != m_infoLabel->isVisible())
			{
				m_infoLabel->setVisible(commonSettings.m_isShowLabelInfo);
			}

			m_zoomController->setZoomLevel(commonSettings.m_zoomLevel);

			// Notify Config of changes
			//this->refreshChart();

			// Set price field
			//m_pricePanel->moveToEnd(true);
            this->setInfoPanelContent();
			this->updateChart(false);
            emit UserSetChartColorSignSignal();
		}
	}

	void HistoricalChart::RefreshSlot()
	{
		{
			QMutexLocker locker(&m_baseData.m_mutex);
			m_baseData.clear();
		}
		this->requestQuotePolicies();
	}

	void HistoricalChart::ShowIndicatorListSlot()
	{
		IndicatorListDialog dialog(this);
		dialog.exec();
	}

	void HistoricalChart::ShowDrawingObjectListSlot()
	{
		QList<BaseDrawing*> drawingObjects;
		const QList<BaseDrawing*>& drawings = m_pricePanel->getDrawingObjects();
		int panelIndex = 0;
        foreach (BaseDrawing* drawing, drawings)
		{
			DrawingObjectSettings& settings = const_cast<DrawingObjectSettings&>(drawing->getSettings());
			settings.m_panelIndex = panelIndex;
		}
		drawingObjects << drawings;

		for (int i = 0; i < m_indicatorPanels.count(); i++)
		{
			panelIndex = i + 1;
			const QList<BaseDrawing*>& drawings = m_indicatorPanels[i]->getDrawingObjects();
            foreach (BaseDrawing* drawing, drawings)
			{
				DrawingObjectSettings& settings = const_cast<DrawingObjectSettings&>(drawing->getSettings());
				settings.m_panelIndex = panelIndex;
			}
			drawingObjects << drawings;
		}

		DrawingObjectListDialog dialog(drawingObjects, this);
		dialog.exec();
	}

	void HistoricalChart::removeIndicatorPanel(IndicatorPanel* indicatorPanel)
	{
		int index = m_indicatorPanels.indexOf(indicatorPanel);
		m_indicatorPanels.remove(index);
		// Adjust the ratio
		double itemRatio = indicatorPanel->getHeightRatio();
		double remainRatio = 1.0 - itemRatio;

		if (m_pricePanel == NULL) return;


        #if defined (RUNTIME_OS_IPHONE) || defined (RUNTIME_OS_ANDROID)
            m_pricePanel->setHeightRatio(1.0);
        #else
            double mainPanelRatio = m_pricePanel->getHeightRatio();
            m_pricePanel->setHeightRatio(mainPanelRatio / remainRatio);
        #endif
		foreach(IndicatorPanel* pannel, m_indicatorPanels)
		{
			if (pannel != NULL)
			{
				double oldRatio = pannel->getHeightRatio();
				pannel->setHeightRatio(oldRatio / remainRatio);
			}
		}

		SplitPanel* split = indicatorPanel->getPreSplitPanel();
		if (split != NULL)
		{
			//Adjust their relationship
			ChartPanel* preChart = split->getPrePanel();
			if (preChart != NULL)
			{
				SplitPanel* nextSplit = indicatorPanel->getNextSplitPanel();
				preChart->setNextSplitPanel(nextSplit);
				if (nextSplit != NULL)
				{
					nextSplit->setPrePanel(preChart);
				}
			}
			delete split;
		}
		delete indicatorPanel;

		arrangePanels();
	}

	void HistoricalChart::handleMousePressEvent(QGraphicsSceneMouseEvent *event)
	{
		if (m_crosshair != NULL && m_crosshair->isVisible())
		{
			m_crosshair->update();
		}
    }

	void HistoricalChart::mouseMoveEvent(QMouseEvent *event)
	{
		if (m_drawingObjectType == enDrawingObjectTypeUnknown)
		{
			if (m_baseData.size() > 0)
            {
                if (m_infoLabel != NULL)
				{
                    this->m_lastViewPoint = event->pos();
                   this->setInfoPanelContent();
				}

				if (m_crosshair != NULL && m_crosshair->isVisible())
				{
					//m_crosshair->setInfoText(info);
					m_crosshair->update();
				}
			}
			QGraphicsView::mouseMoveEvent(event);
		}
	}

    Indicator* HistoricalChart::AddIndicator(int indicatorType)
    {
        qDebug() << " AddIndicator:" << indicatorType;
        IndicatorSettings indicatorSettings;
        indicatorSettings.m_type = (enIndicatorType)indicatorType;
        Indicator* pIndicator = IndicatorCore::CreateIndicator(m_baseData, indicatorSettings, true, m_calculator);
        m_calculator->addEndTask();
        if (pIndicator->getIndicatorSettings().m_isInPriceChart)
        {
            m_pricePanel->addIndicator(pIndicator);
        }
        else
        {
         #if defined (RUNTIME_OS_IPHONE) || defined (RUNTIME_OS_ANDROID)
             if(m_indicatorPanels.count() > 0)
             {
                 this->removeIndicatorPanel(m_indicatorPanels[0]);
             }
         #endif

            IndicatorPanel* indicatorPanel = new IndicatorPanel(this, m_config, m_baseData, pIndicator);
            m_scene->addItem(indicatorPanel);

            //// Update indicator data
            int size = m_indicatorPanels.size();
            double ratio = 1.0 / (size + 2);
            double change = 1.0 - ratio;
            double heightRatio = m_pricePanel->getHeightRatio();
            m_pricePanel->setHeightRatio(change * heightRatio);
            heightRatio = ratio;
            indicatorPanel->setHeightRatio(ratio);

            for (int j = 0; j < size; ++j)
            {
                if (m_indicatorPanels[j] == NULL) continue;
                double panelHeightRatio = m_indicatorPanels[j]->getHeightRatio();
                m_indicatorPanels[j]->setHeightRatio(panelHeightRatio * change);
            }
            addSplitPanel(indicatorPanel);
            m_indicatorPanels << indicatorPanel;

            #if defined (RUNTIME_OS_IPHONE) || defined (RUNTIME_OS_ANDROID)
                this->m_secondaryIndicatorSettingPanel->changeMainIndicator(indicatorSettings.m_type);
            #endif
        }
        this->arrangePanels();
        return pIndicator;
    }

#if defined (RUNTIME_OS_IPHONE) || defined (RUNTIME_OS_ANDROID)
    void HistoricalChart::AddIndicatorToIndicatorSettingPanel(bool isMainIndicator, enIndicatorType indicatorType)
    {
        if(isMainIndicator)
        {
            this->m_mainIndicatorSettingPanel->addMainIndicator(indicatorType);
        }
        else
        {
            this->m_secondaryIndicatorSettingPanel->changeMainIndicator(indicatorType);
        }
    }

    void HistoricalChart::RemoveIndicatorToIndicatorSettingPanel(bool isMainIndicator, enIndicatorType indicatorType)
    {
        if(isMainIndicator)
        {
            this->m_mainIndicatorSettingPanel->removeMainIndicator(indicatorType);
        }
        else
        {
            this->m_secondaryIndicatorSettingPanel->removeMainIndicator(indicatorType);
        }
    }

#endif

    void HistoricalChart::changeFrequency(const Frequency& frequency)
    {
		if (m_listFrequency.contains(frequency))
		{
			m_frequency = frequency;
			this->setWindowTitle(this->getTitle());

			{
				QMutexLocker locker(&m_baseData.m_mutex);
				m_baseData.clear();
			}
			this->requestChartData(true);
			m_inFixPeriodMode = false;
			m_zoomController->resetZoomLevels();

			m_pricePanel->updateAllDrawingObjectVisible();

			foreach(IndicatorPanel* panel, m_indicatorPanels)
			{
				if (panel != NULL)
				{
					panel->updateAllDrawingObjectVisible();
				}
			}
			m_toolbar->resetPeriodComboBox(m_frequency);
		}
    }

	void HistoricalChart::LabelInfoSwitchSlot()
	{
		if(m_infoLabel == NULL) return;
		m_infoLabel->setVisible(!m_infoLabel->isVisible()); 
		CommonSettings& settings = m_config.getCommonSettings();
		settings.m_isShowLabelInfo = m_infoLabel->isVisible();
	}

	//void HistoricalChart::refreshChart()
	//{
	//	return;
	//	if(m_pricePanel != NULL)
	//	{
	//		m_pricePanel->refresh();
	//	}

	//	foreach(IndicatorPanel* panel,m_indicatorPanels)
	//	{
	//		if(panel != NULL)
	//		{
	//			panel->refresh();
	//			SplitPanel* split = panel->getPreSplitPanel();
	//			if(split != NULL)
	//			{
	//				split->update();
	//			}
	//		}
	//	}

	//	if(m_XCoorPanel != NULL)
	//	{
	//		m_XCoorPanel->update();
	//	}
	//}

	void HistoricalChart::CrosshairSwitchSlot()
	{
		bool isShowCrosshair = !m_crosshair->isVisible();
		m_crosshair->setVisible(isShowCrosshair);
		m_config.getCommonSettings().m_isShowCrosshair = isShowCrosshair;
	}

	void HistoricalChart::QuickPlacerSwitchSlot(bool checked)
	{        
		CommonSettings& settings = m_config.getCommonSettings();
		settings.m_isShowQuickPlacer = checked;
		m_tradeHelper->showQuickPlacer(checked);
	}

	void HistoricalChart::ShowExecutedOrderSwitchSlot(bool checked)
	{
		m_config.getCommonSettings().m_isShowExecutedOrder = checked;
		m_orderLineManager->showOrder(true, checked);
	}

	void HistoricalChart::ShowPendingOrderSwitchSlot(bool checked)
	{
		m_config.getCommonSettings().m_isShowPendingOrder = checked;
		m_orderLineManager->showOrder(false, checked);
	}

	void HistoricalChart::CreatorDrawingObjectSlot()
	{
		QAction* action = dynamic_cast<QAction*>(sender());
		if(action != NULL )
		{
			m_drawingObjectType = (enDrawingObjectType)action->data().toInt();
		}
	}

	void HistoricalChart::CreateTextSlot(bool checked)
	{
		m_drawingObjectType = checked ? enDrawingObjectText : enDrawingObjectTypeUnknown;
	}

	enDrawingObjectType HistoricalChart::getDrawingObjectType()
	{
		enDrawingObjectType currentType = m_drawingObjectType;
		m_drawingObjectType = enDrawingObjectTypeUnknown;
		return currentType;
	}

	Config& HistoricalChart::getConfig()
	{
		return m_config;
	}

	QGraphicsItem* HistoricalChart::getXCoordinate() const
	{
		return m_XCoorPanel;
	}

	void HistoricalChart::addSplitPanel(IndicatorPanel* panel)
	{
		int size = m_indicatorPanels.size();
		SplitPanel* split = new SplitPanel(m_config);
		if(split == NULL) return;

		if(size == 0 )
		{
			m_pricePanel->setNextSplitPanel(split);
			split->setPrePanel(m_pricePanel);
			panel->setPreSplitPanel(split);
		}
		else
		{
			ChartPanel* prePanel = m_indicatorPanels.last();
			if(prePanel == NULL) return;
			prePanel->setNextSplitPanel(split);
			split->setPrePanel(prePanel);
			panel->setPreSplitPanel(split);
		}

		split->setNextPanel(panel);
		m_scene->addItem(split);
	}

	void HistoricalChart::keyPressEvent(QKeyEvent *event)
	{
		Qt::Key key = (Qt::Key)event->key();
		switch (key)
		{
		case Qt::Key_F5:
			this->updateChart(false);
			break;
		case Qt::Key_Left:
			moveChartBars(1);
			break;
		case Qt::Key_Right:
			moveChartBars(-1);
			break;
		case Qt::Key_PageUp:
			moveChartBars(m_baseData.getEndIndex() - m_baseData.getBeginIndex());
			break;
		case Qt::Key_PageDown:
			moveChartBars(m_baseData.getBeginIndex() - m_baseData.getEndIndex());
			break;
		case Qt::Key_Home:
			m_pricePanel->moveToHome();
			this->updateChart(false);
			break;
		case Qt::Key_End:
			m_pricePanel->moveToEnd();
			this->updateChart(false);
			break;
		case Qt::Key_Plus:
			m_zoomController->zoom(true);
			break;
		case Qt::Key_Minus:
			if (event->modifiers() == (Qt::ControlModifier | Qt::AltModifier | Qt::KeypadModifier))
			{
				this->exportData();
			}
			else
			{
				m_zoomController->zoom(false);
			}
			break;
		default:
				break;
		}
		return QGraphicsView::keyPressEvent(event);
	}

	void HistoricalChart::exportData()
	{
		// 1. write header
		QString data;
		data.append("Time,Open,High,Low,Close,Volume");
		qDebug() << data;

		// 2. write last 10 data items
		QMutexLocker locker(&m_baseData.m_mutex);
		for (int i = m_baseData.size(), j = 0; i > 0 && j < 10; i--, j++)
		{
			// base data
			data.clear();
			data.append(QString("\r\n%1,%2,%3,%4,%5,%6")
				.arg(JulianDate::FromJDateToString(m_baseData.getDate(i)))
				.arg(m_baseData.getOpens()[i])
				.arg(m_baseData.getHighs()[i])
				.arg(m_baseData.getLows()[i])
				.arg(m_baseData.getCloses()[i])
				.arg(m_baseData.getVolumes()[i]));

			qDebug() << data;
		}
	}

	/* bars>0 move right, bars<0 move left */
	void HistoricalChart::moveChartBars(int bars)
	{
		if (m_isGettingChartData || bars == 0) return;
		if (m_pricePanel->moveChart(bars))
		{
			this->updateChart(false);
			m_priceAlertAdapter->updateUI();
		}

		if (m_baseData.getBeginIndex() < 500 && !m_inFixPeriodMode)
		{
			double endTime = m_baseData.getDate(0);
			qDebug() << "[HistoricalChart::moveChartBars]Begin reqeustChartData baseData.BeginIndex=" << m_baseData.getBeginIndex() << " endTime:" << endTime;
			this->requestChartData(false, endTime, m_RetrieveBarCount);
			qDebug() << "[HistoricalChart::moveChartBars]Begin reqeustChartData end";
		}
	}

	void HistoricalChart::requestQuotePolicies()
	{
		m_quotePolicesRequestCallId = DataSource::uniqueCallId++;
		this->setIsGettingChartData(true);
		QSharedPointer<GetQuotePoliciesCallback> callback(new GetQuotePoliciesCallback(m_quotePolicesRequestCallId, this));
		m_dataSource->beginGetQuotePolicies(m_instrument.getKey(), callback);
	}

	void HistoricalChart::requestChartData(bool isRequstInitData, double endTime, int size)
	{
		int callId = DataSource::uniqueCallId++;
		QSharedPointer<GetChartDataCallback> callback(new GetChartDataCallback(callId, this));
		if (isRequstInitData)
		{
			{
				QMutexLocker locker(&m_baseData.m_mutex);
				m_baseData.clear();
			}
			this->updateChart(false);

			m_initDataRequestCallId = callId;
			this->setIsGettingChartData(true);
			m_dataSource->beginGetInitData(this->getChartProperty(), callback);
			m_requestInitDataTimerId = this->startTimer(30000);
		}
		else
		{
			if (m_endTimeOfAppendDataRequest != endTime)
			{
				m_appendDataRequestCallId = callId;
				m_endTimeOfAppendDataRequest = endTime;
				QtConcurrent::run([=]()
				{
					this->m_dataSource->beginGetHistoricalData(this->getChartProperty(), endTime, size, callback);
				});
			}
		}
	}

	ChartProperty& HistoricalChart::getChartProperty()
	{
		m_chartProperty.instrumentId = m_instrument.getKey();
		m_chartProperty.quotePolicyId = m_quotePolicy.getKey();
		m_chartProperty.frequency = m_frequency;
		return m_chartProperty;
	}

	void HistoricalChart::setIsGettingChartData(bool isChanging)
	{
		m_isGettingChartData = isChanging;
		m_toolbar->OnGettingChartData(m_isGettingChartData);
	}

	void HistoricalChart::timerEvent(QTimerEvent* event)
	{
		if (event->timerId() == m_requestQuotePolicyTimerId)
		{
			this->killTimer(m_requestQuotePolicyTimerId);
			this->RefreshSlot();
		}
		else if (event->timerId() == m_requestInitDataTimerId)
		{
			this->killTimer(m_requestInitDataTimerId);
			this->requestChartData(true);
		}
#if defined (RUNTIME_OS_IPHONE) || defined (RUNTIME_OS_ANDROID)
//        else if (event->timerId() == m_InitIndicatorSettingPanelTimerId)
//        {
//            this->killTimer(m_InitIndicatorSettingPanelTimerId);
//            this->initIndicatorSettingPanelSlot();
//        }
#endif
	}

	void HistoricalChart::addOrder(const OrderInfo& order)
	{
		m_orderLineManager->addOrder(order);
	}

	void HistoricalChart::removeOrder(const QUuid& orderId)
	{
		m_orderLineManager->removeOrder(orderId);
	}

	void HistoricalChart::clearOrders()
	{
		m_orderLineManager->clearOrders();
	}

#if defined(RUNTIME_OS_IPHONE) || defined(RUNTIME_OS_ANDROID)
    //------------mobile----------//
    bool HistoricalChart::event(QEvent *event)
    {
        if (event->type() == QEvent::Gesture)
            return gestureEvent(static_cast<QGestureEvent*>(event));
        return QGraphicsView::event(event);
    }

    bool HistoricalChart::gestureEvent(QGestureEvent *event)
    {
        if (QGesture *pinch = event->gesture(Qt::PinchGesture))
            pinchTriggered(static_cast<QPinchGesture *>(pinch));
        return true;
    }

    void HistoricalChart::pinchTriggered(QPinchGesture *gesture)
    {
        QPinchGesture::ChangeFlags changeFlags = gesture->changeFlags();
        if (changeFlags & QPinchGesture::ScaleFactorChanged) {
            qreal stepScaleFactor = gesture->totalScaleFactor();
            if(qAbs(m_currentStepScaleFactor - stepScaleFactor) > 0.00001)
            {
                this->m_isZoomIn = m_currentStepScaleFactor < stepScaleFactor;
                m_currentStepScaleFactor = stepScaleFactor;
                qDebug() << "HistoricalChart::pinchTriggered m_currentStepScaleFactor:" << stepScaleFactor << " isZoomIn:" << this->m_isZoomIn;
            }
        }
        if (gesture->state() == Qt::GestureFinished) {
            qDebug() << "HistoricalChart::pinchTriggered  GestureFinished-------------";
            m_currentStepScaleFactor = 1;
            m_zoomController->zoom(this->m_isZoomIn);
        }
        update();
    }

    void HistoricalChart::calIndicatorInfoValue(QPointF& scenePoint)
    {
        int settingPanelHeight = 30 * this->m_pixelSizeRatio;
        QPointF bottomRight = m_pricePanel->rect().bottomRight();
        int panelWidth = m_pricePanel->getPanelWidth();
        if(panelWidth == 0)
            panelWidth = bottomRight.x() -  Helper::GetMarginSpaceValue(enMarginYCoordinate);
        if(scenePoint.x() >= 0 && scenePoint.x() <= panelWidth)
        {
            if(settingPanelHeight <= scenePoint.y() && scenePoint.y()<= (bottomRight.y() + settingPanelHeight))
            {
                QString shorthandName = m_mainIndicatorSettingPanel->getCurrentIndicatorName();
                if(shorthandName != "Null")
                {
                    m_cursorPosition->OnPositionChange(scenePoint, m_pricePanel);
                    QString infoText = m_cursorPosition->getIndicatorInfoText(IndicatorCore::getIndicatorTypeByShorthandName(shorthandName));
                    m_mainIndicatorSettingPanel->setIndicatorInfoValue(infoText);
                }

                if(m_indicatorPanels.count() > 0)
                {
                    QPointF indicatorBottomRight = m_indicatorPanels[0]->rect().bottomRight();
                    scenePoint = QPointF(scenePoint.x(), bottomRight.y() + 2*settingPanelHeight + indicatorBottomRight.y()/2);
                    m_cursorPosition->OnPositionChange(scenePoint, m_indicatorPanels[0]);
                    QString shorthandName = m_secondaryIndicatorSettingPanel->getCurrentIndicatorName();
                    QString infoText = m_cursorPosition->getIndicatorInfoText(IndicatorCore::getIndicatorTypeByShorthandName(shorthandName));
                    m_secondaryIndicatorSettingPanel->setIndicatorInfoValue(infoText);
                }
            }
            else
            {
                if(m_indicatorPanels.count() > 0)
                {
                    QPointF indicatorBottomRight = m_indicatorPanels[0]->rect().bottomRight();
                    int startTopY = settingPanelHeight + bottomRight.y() + settingPanelHeight;
                    if(startTopY <= scenePoint.y() && scenePoint.y()<= (startTopY + indicatorBottomRight.y()))
                    {
                        m_cursorPosition->OnPositionChange(scenePoint, m_indicatorPanels[0]);
                        QString shorthandName = m_secondaryIndicatorSettingPanel->getCurrentIndicatorName();
                        QString infoText = m_cursorPosition->getIndicatorInfoText(IndicatorCore::getIndicatorTypeByShorthandName(shorthandName));
                        m_secondaryIndicatorSettingPanel->setIndicatorInfoValue(infoText);


                        scenePoint = QPointF(scenePoint.x(), settingPanelHeight + bottomRight.y()/2);
                        shorthandName = m_mainIndicatorSettingPanel->getCurrentIndicatorName();
                        if(shorthandName != "Null")
                        {
                            m_cursorPosition->OnPositionChange(scenePoint, m_pricePanel);
                            QString infoText = m_cursorPosition->getIndicatorInfoText(IndicatorCore::getIndicatorTypeByShorthandName(shorthandName));
                            m_mainIndicatorSettingPanel->setIndicatorInfoValue(infoText);
                        }
                    }
                }
            }
        }
    }

    void HistoricalChart::mousePressEvent(QMouseEvent *event)
    {
        if (m_drawingObjectType == enDrawingObjectTypeUnknown)
        {
            if (m_baseData.size() > 0)
            {
                QPoint viewPoint = event->pos();                
                QPointF scenePoint = this->mapToScene(viewPoint);
                this->calIndicatorInfoValue(scenePoint);
            }
            QGraphicsView::mousePressEvent(event);
        }
    }

#endif
}

#include "moc_historicalchart.cpp"
