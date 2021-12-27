#include "HistoricalChartMobile.h"
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

namespace HistoricalChartSpace
{
    const int HistoricalChartMobile::m_RetrieveBarCount = 500;
	//const int HistoricalChart::m_MaxBarCount = 5000;

    HistoricalChartMobile::HistoricalChartMobile(QWidget *parent, QWidget* dialogParent, HistoricalDataSource* dataSource, QString& templateFilePath, const QString& language, bool isBlackTheme)
		:QGraphicsView(parent),
		m_dataSource(dataSource),
		m_crosshair(nullptr),
		m_instrument(nullptr, nullptr, 2),
		m_frequency(Minute),
		m_drawingObjectType(enDrawingObjectTypeUnknown),
		m_listInstrument(dataSource->getInstrumentList()),
		m_listFrequency(dataSource->getFrequencyList()),
		m_isGettingChartData(true),
		m_charPeriod(Unlimited),
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
		m_endTimeOfAppendDataRequest(0)

	{
        grabGesture(Qt::PanGesture);
        grabGesture(Qt::PinchGesture);
        grabGesture(Qt::SwipeGesture);

        QLOG_INFO("Chart") << "HistoricalChartMobile::HistoricalChartMobile()" << this << QThread::currentThreadId();
		if (dialogParent == nullptr)
		{
			m_dialogParent = this;
		}
		m_calculator = new Calculator(this);
		QString lang = SR::setLanguage(language);
		if (lang != "ENG")
		{
			this->setStyleSheet("font-family:\"宋体\";font-size:9pt");
		}

		m_timer = new QTimer(this);
		connect(m_timer, SIGNAL(timeout()), SLOT(ChartLagCheckSlot()));

		if (m_listInstrument.count() > 0)
		{
			// Instrument 按字母排序
			QList<Instrument>& instruments = const_cast<QList<Instrument>&>(m_listInstrument);
			std::sort(instruments.begin(), instruments.end());
		}

		this->createActions();
		m_toolbar = new ToolBar(this, isBlackTheme);
		m_templateManager = new TemplateManager(this, templateFilePath);
		m_toolbar->createToolBar(m_listInstrument, m_listFrequency, m_templateManager);


		//this->setWindowFlags(Qt::Window);
		//	this->setAttribute(Qt::WA_DeleteOnClose);
		this->setObjectName("ChartObject");

		m_tradeHelper = new TradeHelper(this);
		m_orderLineManager = new OrderLineManager(this);
		m_cursorPosition = new CursorPosition(this);
		m_priceAlertAdapter = new PriceAlertAdapter(this);

		qRegisterMetaType<QSharedPointer<vector<PrimHistoricalData>>>("QSharedPointer<vector<PrimHistoricalData>>");
		connect(this, SIGNAL(DataProcessedSignal(bool, bool, bool)), this, SLOT(DataProcessedSlot(bool, bool, bool)));
	}

    HistoricalChartMobile::~HistoricalChartMobile()
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
	}

    QWidget* HistoricalChartMobile::getDialogParent()
	{
		return m_dialogParent;
	}

    BaseData& HistoricalChartMobile::getBaseData()
	{
		return m_baseData;
	}

    PricePanel* HistoricalChartMobile::getPricePanel()
	{
		return m_pricePanel;
	}

    QGraphicsScene* HistoricalChartMobile::getScene()
	{
		return m_scene;
	}

    PriceAlertAdapter* HistoricalChartMobile::getPriceAlertManager()
	{
		return m_priceAlertAdapter;
	}

    Calculator* HistoricalChartMobile::getCalculator()
	{
		return m_calculator;
	}

    //bool HistoricalChartMobile::isInFixPeriodMode()
	//{
	//	return m_inFixPeriodMode;
	//}

    QString HistoricalChartMobile::getTitle()
	{
		return m_instrument.getDescription() + "," + m_frequency.getName();
	}

    void HistoricalChartMobile::setFrequency(const Frequency& frequency)
	{
		m_frequency = frequency;
		this->setWindowTitle(getTitle());
	}

    const Instrument& HistoricalChartMobile::getInstrument()
	{
		return m_instrument;
	}

    QUuid HistoricalChartMobile::getQuotePolicyId()
	{
		return m_quotePolicy.getKey();
	}

    KeyDescription HistoricalChartMobile::getQuotePolicy()
	{
		return m_quotePolicy;
	}

    int HistoricalChartMobile::getDecimalPlaces()
	{
		return m_instrument.getDecimalPlaces();
	}

    Frequency HistoricalChartMobile::getFrequency()
	{
		return m_frequency;
	}

    void HistoricalChartMobile::setQuickPlacer(QuickPlacer* placer)
	{
		m_tradeHelper->setQuickPlacer(placer);
		m_tradeHelper->showQuickPlacer(m_config.getCommonSettings().m_isShowQuickPlacer);
	}

    void HistoricalChartMobile::setQuickModifier(QuickModifier* modifier)
	{
		m_orderLineManager->setQuickModifier(modifier);
	}

    QString HistoricalChartMobile::getConfigXml()
	{
		m_config.saveCommonSettings(m_instrument, m_quotePolicy, m_frequency);
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

    void HistoricalChartMobile::loadConfigXml(QString& configXml, const QString& language)
	{
		if (configXml.isNull() || configXml.isEmpty())
		{
			QString configXml = m_templateManager->getDefaultTemplateConfigXml();
			m_config.loadConfigXml(configXml);
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
    void HistoricalChartMobile::applyTemplate(QString& configXml)
	{
		if (m_config.applyTemplate(configXml))
		{
			this->initializePanels();
			this->arrangePanels();
		}
	}

    void HistoricalChartMobile::createActions()
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

    void HistoricalChartMobile::initializePanels()
	{
		bool sceneCreated = m_scene != nullptr;
		// 创建场景
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

			// 创建X坐标及网格
			m_XCoorPanel = new XCoorPanel(m_config, m_zoomController);
			if (m_XCoorPanel == NULL) return;
			m_scene->addItem(m_XCoorPanel);

			// 创建主价格窗口
			m_pricePanel = new PricePanel(this, m_config, m_baseData);
			if (m_pricePanel == NULL) return;
			m_zoomController = new ZoomController(*this, m_config, *m_pricePanel->getMainPainterPanel());

			m_scene->addItem(m_pricePanel);

			// 创建信息显示标签
			m_infoLabel = new InfoLabel(SR::get("LoadingData"), this);
			m_infoLabel->setAlignment(Qt::AlignLeft);
		}

		// 设置背景
		const QColor& background = m_config.getCommonSettings().m_background;
		m_scene->setBackgroundBrush(background);

		m_infoLabel->setStyleSheet("color:" + m_config.getCommonSettings().m_foreground.name());
		m_infoLabel->setVisible(m_config.getCommonSettings().m_isShowLabelInfo);

		// 读取指标
		if (m_config.hasConfigData())
		{
			int index = m_listInstrument.indexOf(m_config.getInstrument());
			if (index >= 0)
			{
				m_instrument = m_listInstrument[index];
			}
			else if (m_listInstrument.size() > 0)
			{
				m_instrument = m_listInstrument.at(0);
			}

			m_quotePolicy = m_config.getQuotePolicy();
			m_frequency = m_config.getFrequency();

			Config::IndicatorPanelIterator iterator(m_config);
			double heightRatio;
			QVector<IndicatorSettings> indicatorSettings;
			QVector<DrawingObjectSettings> drawingObjectSettings;
			while (iterator.getCurrentSettings(heightRatio, indicatorSettings, drawingObjectSettings))
			{
				IndicatorPanel* indicatorPanel = new IndicatorPanel(this, m_config, m_baseData);
				indicatorPanel->setHeightRatio(heightRatio);
				indicatorPanel->applySettings(indicatorSettings, drawingObjectSettings);
				m_scene->addItem(indicatorPanel);
				this->addSplitPanel(indicatorPanel);
				m_indicatorPanels << indicatorPanel;
				indicatorSettings.clear();
				drawingObjectSettings.clear();
			}
		}
		else
		{
			if (m_listInstrument.size() > 0)
			{
				m_instrument = m_listInstrument.at(0);
			}
			if (m_listFrequency.size() > 0)
			{
				m_frequency = m_listFrequency.at(0);
			}
		}

		m_toolbar->applySettings(m_config, m_instrument, m_frequency);

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
	}

    void HistoricalChartMobile::UpdateQuotePolicy(int callId, QList<KeyDescription>& quotePolicies)
	{
		if(m_quotePolicesRequestCallId == callId)
		{
			m_quotePolicesRequestCallId = 0;
			if (quotePolicies.count() > 0)
			{
				m_listQuotePolicy.clear();
				m_listQuotePolicy.append(quotePolicies);
				m_toolbar->updateQuotePolicy(m_listQuotePolicy, m_quotePolicy);
				if (m_charPeriod == Unlimited)
				{
					this->requestChartData(true);
				}
				else
				{
					m_baseData.clear();
					this->SetPeriodSlot(m_charPeriod);
				}
			}
			else
			{
				m_requestQuotePolicyTimerId = this->startTimer(1000);
			}
		}
	}

    void HistoricalChartMobile::UpdateHistoricalData(int callId, QSharedPointer<vector<PrimHistoricalData>> vecData)
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
				if (junctionPointTiemSpan >= 0 && junctionPointTiemSpan < allowedTimeSpan)
				{
					QtConcurrent::run([=]()
					{
						{
							QMutexLocker locker(&m_baseData.m_mutex);
							for (int i = vecData->size() - 2; i >= 0; i--)
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
			m_fixedPeriodDataRequestCallId = 0;
			isFixPeriodData = true;
			if (vecData->size() > 0)
			{
				QMutexLocker locker(&m_baseData.m_mutex);
				// 此处可接受数据源多次返回的数据（数据源可能分别从本地缓存或后台获取数据）
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
						m_baseData.joinDatasAtFront(vecData.data());
					}
				}
				// caculate a specify zoomlevel here
				m_zoomController->caculateSpecifyZoomLevel();
			}
		}
		else
		{
			isBaseDataUpdated = false;
		}

		this->DataProcessedSignal(isBaseDataUpdated, isFixPeriodData, isInitialData);
	}

    void HistoricalChartMobile::DataProcessedSlot(bool isBaseDataUpdated, bool isFixPeriodData, bool isInitialData)
	{
		m_infoLabel->clear();
		if (isBaseDataUpdated)
		{
			if (isFixPeriodData || isInitialData)
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
	}

    void HistoricalChartMobile::updateOrderLines()
	{
		m_orderLineManager->updateUI();
	}

    void HistoricalChartMobile::ChartDataNotifySlot(PrimHistoricalData data)
	{
		QMutexLocker locker(&m_baseData.m_mutex);

		const int lastIndex = m_baseData.size() - 1;
		const double lastDate = m_baseData.getDate(lastIndex);
		if (JulianDate::lessThan(lastDate, data.date))
		{
			// 增加新数据
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
			// 更新已有数据
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

    void HistoricalChartMobile::LastBarDataCome(int callId, QSharedPointer<vector<PrimHistoricalData>> datas)
	{
		if (m_lastBarRequestCallId == callId)
		{
			m_lastBarRequestCallId = 0;

			QMutexLocker locker(&m_baseData.m_mutex);
			this->updateLastBars(datas.data());
		}
	}

    void HistoricalChartMobile::updateLastBars(vector<PrimHistoricalData>* datas, bool isAppend /* = false*/)
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

	// 图形滞后检查
    void HistoricalChartMobile::ChartLagCheckSlot()
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
			// 图形滞后，请求数据。
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

    void HistoricalChartMobile::ChangeTradeStateSlot(bool paused)
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

    void HistoricalChartMobile::synchronizeSystemTime()
	{
		m_systemTimeRequestCallId = DataSource::uniqueCallId++;
		QSharedPointer<GetSystemTimeCallback> callback(new GetSystemTimeCallback(m_systemTimeRequestCallId, this));
		m_dataSource->beginGetCurrentTime(callback);
	}

    void HistoricalChartMobile::setSystemTime(int callId, double currentTime)
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
			// 启动图形滞后检测
			this->ChartLagCheckSlot();
		}
	}

    void HistoricalChartMobile::requestLastBars(double beginTime)
	{
		m_lastBarRequestCallId = DataSource::uniqueCallId++;
		QSharedPointer<GetLastBarCallback> callback(new GetLastBarCallback(m_lastBarRequestCallId, this));
		m_dataSource->beginGetLastBars(this->getChartProperty(), beginTime, callback);
	}

    void HistoricalChartMobile::RealTimeDataComeSlot(const PrimRealTimeData& data)
	{
		if (m_pricePanel == NULL || m_isGettingChartData) return;

		double lastDate = m_baseData.getDate(m_baseData.size() - 1);
		double timeSpan = m_frequency.getJDateTimeSpan();
		int multiples = (data.date - lastDate) / timeSpan;
		int duration = 3000;
		if (multiples >= 1 && m_timer->remainingTime() > duration)
		{
			// 实时数据修正获取lastBar的时间点
			this->startTimerWithRandDelay(0);
			qDebug() << "start time 3, interval:" << duration;
		}
		else
		{
			m_baseData.realTimeDataCome(data, timeSpan);
		}
		//m_pricePanel->update(); // 不需要，下面会刷新界面。
		if (m_baseData.size() > 0)
		{
			m_pricePanel->refresh();
			int beginIndex, endIndex;
			beginIndex = endIndex = m_baseData.size() - 1;
			this->calculateIndicatorsData(beginIndex, endIndex);
		}
	}

    void HistoricalChartMobile::startTimerWithRandDelay(int interval)
	{
		if (interval < 0)  // 月线、年线溢出
		{
			interval = 2147483647; // int.MaxValue
		}
		interval += 2000 + 3000 * std::rand() / RAND_MAX;  // 延迟 2秒 + 0-3000毫秒
		m_timer->start(interval);
	}

    void HistoricalChartMobile::test()
	{
		m_toolbar->test();
	}

    void HistoricalChartMobile::OnThemeChanged(bool isBlack)
	{
		m_toolbar->OnThemeChanged(isBlack);
	}

    void HistoricalChartMobile::DrawingObjectCreated(enDrawingObjectType type)
	{
		m_toolbar->DrawingObjectCreated(type);
	}

    void HistoricalChartMobile::resizeEvent(QResizeEvent *event)
	{
		this->arrangePanels();
		// resize toolbar to allow drop down on the right.
		int chartWidth = event->size().width() + 2;
		m_toolbar->OnChartResize(chartWidth);
		return QGraphicsView::resizeEvent(event);
	}

    void HistoricalChartMobile::showGrid()
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

    void HistoricalChartMobile::shiftData()
	{
		if(m_XCoorPanel == NULL || m_pricePanel == NULL) return;

		CommonSettings& settings = m_config.getCommonSettings();
		settings.m_isShiftData = !settings.m_isShiftData;
		m_pricePanel->moveToEnd();
		this->updateChart(false);
	}

	// 因: 1.BaseData, 2.Resize, 3.zoomLevel 变化后，造成Basic Parameter(beginIndex,endIndex,barDistance)改变从而刷新界面：
    void HistoricalChartMobile::updateChart(bool isUpadteEndIndex)
	{
        //_ASSERT(m_pricePanel);
		if (isUpadteEndIndex)
		{
			m_pricePanel->updateEndIndex();
		}
		m_pricePanel->refresh();

		// 更新Indicator Panels
		for (int i = 0; i < m_indicatorPanels.size(); ++i)
		{
			IndicatorPanel* panel = m_indicatorPanels[i];
			if (panel)
			{
				panel->refresh();
			}
		}

		// 更新X坐标
        //_ASSERT(m_XCoorPanel);
		m_XCoorPanel->updateXCoor(m_baseData);
	}

    void HistoricalChartMobile::resizePanelsRect(const QRectF& rect)
	{
        //_ASSERT(m_pricePanel);
		QRectF BaseRect(0, 0, rect.width(), rect.height());
		BaseRect.setHeight(rect.height() - Helper::GetMarginSpaceValue(enMarginXCoordinate));
		double splitHight = Helper::GetMarginSpaceValue(enMarginSplitHight);
		const QRectF splitRect(0, 0, rect.width(), splitHight);

		// 设置价格位置大小
		double heightRatio = m_pricePanel->getHeightRatio();
		double priceHight = BaseRect.height() * heightRatio;
		QRectF priceRect(BaseRect);
		priceRect.setHeight(priceHight);

		m_pricePanel->setRect(priceRect);
		m_pricePanel->setPos(rect.left(), rect.top());
		m_pricePanel->adjustSubItem();
		m_infoLabel->moveToInitialPosition();

		// 设置与价格不同位置的指标
		SplitPanel* split = m_pricePanel->getNextSplitPanel();
		if (split == NULL) return;

		split->setRect(splitRect);
		split->setPos(rect.left(), rect.top() + priceHight);

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

			indicatorPanel->setRect(0, 0, BaseRect.width(), height);
			point.setY(point.y() + splitHight);
			indicatorPanel->setPos(point);
			indicatorPanel->adjustSubItem();

			split = indicatorPanel->getNextSplitPanel();
			if (split == NULL) return;
			split->setRect(splitRect);
			point.setY(point.y() + height);
			split->setPos(point);
		}
	}

    void HistoricalChartMobile::changePriceStyle(enPriceType type)
	{
		if(m_pricePanel)
		{
			PriceSettings& settings = m_config.getPriceSettings();
			settings.m_drawType = type;
			m_pricePanel->changeStyle();
		}
	}

    void HistoricalChartMobile::SetInstrumentSlot(int index)
	{
		if (index < m_listInstrument.size())
		{
			m_instrument = m_listInstrument[index];
			this->setWindowTitle(this->getTitle());
			this->requestQuotePolicies();
			QUuid instrumentId = m_instrument.getKey();
			m_tradeHelper->changeInstrument(instrumentId);
			m_orderLineManager->changeInstrument(instrumentId);
			emit InstrumentChangedSignal(instrumentId);
		}
	}

    void HistoricalChartMobile::SetQuotePolicySlot(int index)
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

    void HistoricalChartMobile::SetFrequencySlot(int index)
	{
		if (index < m_listFrequency.size())
		{
			m_frequency = m_listFrequency[index];
			this->setWindowTitle(this->getTitle());
			this->requestChartData(true);

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

    void HistoricalChartMobile::ShowGridSwitchSlot()
	{
		this->showGrid();
	}

    void HistoricalChartMobile::AutoScrollSwitchSlot()
	{
		CommonSettings& settings = m_config.getCommonSettings();
		settings.m_isAutoScroll = !settings.m_isAutoScroll;
	}

    void HistoricalChartMobile::ShiftDataSwitchSlot()
	{
		this->shiftData();
	}

    void HistoricalChartMobile::SetPriceChartSlot(int index)
	{
		this->changePriceStyle((enPriceType)index);
	}

    void HistoricalChartMobile::SetPeriodSlot(int index)
	{
		if (index < 0) return;
		QDate beginDate = QDate::currentDate();
		int quarter;
		switch (index)
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
		default:
            //_ASSERT(false);
			break;
		}
		m_charPeriod = (DisplayPeriod)index;
		m_inFixPeriodMode = m_charPeriod != Unlimited;

		if (m_charPeriod == Unlimited)
		{
			if (m_baseData.size() < 500)
			{
				this->requestChartData(true);
			}
			m_zoomController->resetZoomLevels();
		}
		else
		{
			double periodBeginJTime = m_dataSource->GetPeriodBeginTime(m_charPeriod);
			if (periodBeginJTime == 0) periodBeginJTime = beginDate.toJulianDay() - 0.5;

			bool shouldRequestData = true;
			double endTime;
			if (m_baseData.size() > 0)
			{
				double beginTimeOfBaseDate = m_baseData.getDate(0);

				if (beginTimeOfBaseDate > 0 && periodBeginJTime >= beginTimeOfBaseDate)
				{
					if (periodBeginJTime > m_baseData.getDate(m_baseData.size() - 1))
					{
						// 只有在系统一段时间没有收到价格的极端情况下会出现这种情况
						QMutexLocker locker(&m_baseData.m_mutex);
						m_baseData.clear();
						m_timer->stop();
					}
					else
					{
						m_baseData.truncateBefore(periodBeginJTime);
						this->calculateIndicatorsData(0, m_baseData.size() - 1);
						m_zoomController->caculateSpecifyZoomLevel();
					}
					
					shouldRequestData = false;
				}
				else
				{
					endTime = beginTimeOfBaseDate;
				}
			}
			else
			{
				endTime = m_systemTime ? m_systemTime->now() : JulianDate::QTDateToJDate(QDateTime::currentDateTime());
			}

			if (shouldRequestData)
			{
				this->setIsGettingChartData(true);
				m_fixedPeriodDataRequestCallId = DataSource::uniqueCallId++;
				QSharedPointer<GetChartDataCallback> callback(new GetChartDataCallback(m_fixedPeriodDataRequestCallId, this));
				m_dataSource->beginGetFixPeriodData(this->getChartProperty(), periodBeginJTime, endTime, callback);
			}
		}
	}

    void HistoricalChartMobile::SetElementScaleSlot()
	{
		QAction *action = qobject_cast<QAction*>(sender());
		if (action)
		{
			bool isZoomin = (enPriceType)action->data().toBool();
			m_zoomController->zoom(isZoomin);
		}
	}

    void HistoricalChartMobile::HideToolBarSlot()
	{
		m_toolbar->switchVisible();
	}

    void HistoricalChartMobile::arrangePanels()
	{
		QRectF rect = getViewPortRect();
		resizePanelsRect(rect);

        //_ASSERT(m_XCoorPanel);
		m_XCoorPanel->setRect(rect);
		m_XCoorPanel->setPos(0, 0);
		this->updateChart(true);
	}

    QRectF HistoricalChartMobile::getViewPortRect()
	{
        ////_ASSERT(m_scene);
		QRectF rect = this->viewport()->rect();
		m_scene->setSceneRect(rect);
		return rect;
	}

    void HistoricalChartMobile::AddIndicatorSlot()
	{
		QAction *action = qobject_cast<QAction*>(sender());
		if (action == NULL) return;

		int indicatorType = action->data().toInt();
		if (indicatorType >= enIndicatorEnd) return;

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
			IndicatorPanel* indicatorPanel = new IndicatorPanel(this, m_config, m_baseData, pIndicator);
			m_scene->addItem(indicatorPanel);

			//// 更新指标数据
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
		this->arrangePanels();
	}

    void HistoricalChartMobile::moveChartPixels(int pixels)
	{
		if (m_baseData.size() > 0 && pixels != 0)
		{
			int bars = (int)pixels / m_baseData.getBarDistance()  * Helper::GetMarginSpaceValue(enMarginMouseMoveScale); //数据偏移

			if (bars == 0)
			{
				bars = pixels > 0 ? 1 : -1;
			}
			this->moveChartBars(bars);
		}
	}

    void HistoricalChartMobile::calculateIndicatorsData(int beginIndex, int endIndex)
	{
		m_pricePanel->calculateIndicators(beginIndex, endIndex);
		
		foreach(IndicatorPanel* panel, m_indicatorPanels)
		{
			panel->calculateIndicators(beginIndex, endIndex);
		}
		m_calculator->addEndTask();
	}

    void HistoricalChartMobile::showCommonMenu(QGraphicsSceneContextMenuEvent* event, bool fromPricePanel)
	{
		QMenu menu(this);
		QAction placeLimitOrderAction("", &menu), placeStopOrderAction("", &menu);
		if (fromPricePanel && m_baseData.size() > 0)
		{
			double price = m_pricePanel->Y2Value(event->pos().y());
			double bid, ask;
			m_baseData.getBidAsk(bid, ask);
            //qDebug() << QString("HistoricalChartMobile::HandleCommonContextMenuSlot pos:[%1,%2], price:[%3]").arg(event->pos().x()).arg(event->pos().y()).arg(price);
			
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

    void HistoricalChartMobile::ShowCommonPropertySlot()
	{
		CommonSettings& commonSettings = m_config.getCommonSettings();
		PriceSettings& priceSettings = m_config.getPriceSettings();
		PriceDialog dialog(commonSettings, priceSettings, m_dialogParent);
		dialog.exec();

		if (dialog.result() == QDialog::Accepted)
		{
			// 设置整体背景色
			if (commonSettings.m_background != m_scene->backgroundBrush().color())
			{
				m_scene->setBackgroundBrush(commonSettings.m_background);

			}
			// 设置价格信息的显示
			m_infoLabel->setStyleSheet("color:" + commonSettings.m_foreground.name());

			// 调整PriceStyle的Combobox
			m_toolbar->changeDrawType(priceSettings.m_drawType);

			m_toolbar->updateCommonSettings(commonSettings);

			//是否显示文本信息
			if (commonSettings.m_isShowLabelInfo != m_infoLabel->isVisible())
			{
				m_infoLabel->setVisible(commonSettings.m_isShowLabelInfo);
			}

			m_zoomController->setZoomLevel(commonSettings.m_zoomLevel);

			// 通知Config发生变化
			//this->refreshChart();

			// 设置价格域
			//m_pricePanel->moveToEnd(true);
			this->updateChart(false);
		}
	}

    void HistoricalChartMobile::RefreshSlot()
	{
		{
			QMutexLocker locker(&m_baseData.m_mutex);
			m_baseData.clear();
		}
		this->requestQuotePolicies();
	}

    void HistoricalChartMobile::ShowIndicatorListSlot()
	{
		IndicatorListDialog dialog(this);
		dialog.exec();
	}

    void HistoricalChartMobile::ShowDrawingObjectListSlot()
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

    void HistoricalChartMobile::removeIndicatorPanel(IndicatorPanel* indicatorPanel)
	{
		int index = m_indicatorPanels.indexOf(indicatorPanel);
		m_indicatorPanels.remove(index);
		// 调整比例
		double itemRatio = indicatorPanel->getHeightRatio();
		double remainRatio = 1.0 - itemRatio;

		if (m_pricePanel == NULL) return;

		double mainPanelRatio = m_pricePanel->getHeightRatio();
		m_pricePanel->setHeightRatio(mainPanelRatio / remainRatio);

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
			//调整他们之间的关系
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

    void HistoricalChartMobile::mouseMoveEvent(QMouseEvent *event)
	{
		if (m_drawingObjectType == enDrawingObjectTypeUnknown)
		{
			if (m_baseData.size() > 0)
			{
				QString info;
				if (m_infoLabel != NULL)
				{
					QPoint viewPoint = event->pos();
					QList<QGraphicsItem*> listItem = this->items(viewPoint);

					foreach(QGraphicsItem* item, listItem)
					{
						ChartPanel* panel = dynamic_cast<ChartPanel*>(item);
						if (panel != NULL)
						{
							QPointF scenePoint = this->mapToScene(viewPoint);
							m_cursorPosition->OnPositionChange(scenePoint, panel);
							m_infoLabel->setText(m_cursorPosition->getInfoText());
							break;
						}
					}
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

    void HistoricalChartMobile::LabelInfoSwitchSlot()
	{
		if(m_infoLabel == NULL) return;
		m_infoLabel->setVisible(!m_infoLabel->isVisible()); 
		CommonSettings& settings = m_config.getCommonSettings();
		settings.m_isShowLabelInfo = m_infoLabel->isVisible();
	}

    //void HistoricalChartMobile::refreshChart()
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

    void HistoricalChartMobile::CrosshairSwitchSlot()
	{
		bool isShowCrosshair = !m_crosshair->isVisible();
		m_crosshair->setVisible(isShowCrosshair);
		m_config.getCommonSettings().m_isShowCrosshair = isShowCrosshair;
	}

    void HistoricalChartMobile::QuickPlacerSwitchSlot(bool checked)
	{
		CommonSettings& settings = m_config.getCommonSettings();
		settings.m_isShowQuickPlacer = checked;
		m_tradeHelper->showQuickPlacer(checked);
	}

    void HistoricalChartMobile::ShowExecutedOrderSwitchSlot(bool checked)
	{
		m_config.getCommonSettings().m_isShowExecutedOrder = checked;
		m_orderLineManager->showOrder(true, checked);
	}

    void HistoricalChartMobile::ShowPendingOrderSwitchSlot(bool checked)
	{
		m_config.getCommonSettings().m_isShowPendingOrder = checked;
		m_orderLineManager->showOrder(false, checked);
	}

    void HistoricalChartMobile::CreatorDrawingObjectSlot()
	{
		QAction* action = dynamic_cast<QAction*>(sender());
		if(action != NULL )
		{
			m_drawingObjectType = (enDrawingObjectType)action->data().toInt();
		}
	}

    void HistoricalChartMobile::CreateTextSlot(bool checked)
	{
		m_drawingObjectType = checked ? enDrawingObjectText : enDrawingObjectTypeUnknown;
	}

    enDrawingObjectType HistoricalChartMobile::getDrawingObjectType()
	{
		enDrawingObjectType currentType = m_drawingObjectType;
		m_drawingObjectType = enDrawingObjectTypeUnknown;
		return currentType;
	}

    Config& HistoricalChartMobile::getConfig()
	{
		return m_config;
	}

    QGraphicsItem* HistoricalChartMobile::getXCoordinate() const
	{
		return m_XCoorPanel;
	}

    void HistoricalChartMobile::addSplitPanel(IndicatorPanel* panel)
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

    void HistoricalChartMobile::keyPressEvent(QKeyEvent *event)
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


    bool HistoricalChartMobile::event(QEvent *event)
    {
        if (event->type() == QEvent::Gesture)
            return gestureEvent(static_cast<QGestureEvent*>(event));
        return QWidget::event(event);
    }

    // 放大/缩小
    void HistoricalChartMobile::wheelEvent(QWheelEvent *event)
    {
        qDebug() << "CProjectionPicture::wheelEvent";
    //    QPoint numPixels = event->pixelDelta();
        QPoint scrallAmount = event->angleDelta();
        if(scrallAmount.y() > 0){
            m_zoomController->zoom(true);
        }
        else if(scrallAmount.y() < 0){
            m_zoomController->zoom(false);
        }
        QWidget::wheelEvent(event);
    }

    bool HistoricalChartMobile::gestureEvent(QGestureEvent *event)
    {
        if (QGesture *pinch = event->gesture(Qt::PinchGesture))
            pinchTriggered(static_cast<QPinchGesture *>(pinch));
        return true;
    }

    void HistoricalChartMobile::pinchTriggered(QPinchGesture *gesture)
    {
        QPinchGesture::ChangeFlags changeFlags = gesture->changeFlags();
        if (changeFlags & QPinchGesture::ScaleFactorChanged) {
            m_currentStepScaleFactor = gesture->totalScaleFactor();
            qDebug() << "currentStepScaleFactor: "<< m_currentStepScaleFactor;
        }
        if (gesture->state() == Qt::GestureFinished) {
            m_scaleFactor *= m_currentStepScaleFactor;
            qDebug()<<"m_scaleFactor:" << m_scaleFactor << "currentStepScaleFactor: "<< m_currentStepScaleFactor;
            m_currentStepScaleFactor = 1;
        }
        update();
    }


    void HistoricalChartMobile::exportData()
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
    void HistoricalChartMobile::moveChartBars(int bars)
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
			this->requestChartData(false, endTime, m_RetrieveBarCount);
            qDebug() << "[HistoricalChartMobile::moveChartBars]Begin reqeustChartData baseData.BeginIndex=" << m_baseData.getBeginIndex();
		}
	}

    void HistoricalChartMobile::requestQuotePolicies()
	{
		m_quotePolicesRequestCallId = DataSource::uniqueCallId++;
		this->setIsGettingChartData(true);
		QSharedPointer<GetQuotePoliciesCallback> callback(new GetQuotePoliciesCallback(m_quotePolicesRequestCallId, this));
		m_dataSource->beginGetQuotePolicies(m_instrument.getKey(), callback);
	}

    void HistoricalChartMobile::requestChartData(bool isRequstInitData, double endTime, int size)
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

    ChartProperty& HistoricalChartMobile::getChartProperty()
	{
		m_chartProperty.instrumentId = m_instrument.getKey();
		m_chartProperty.quotePolicyId = m_quotePolicy.getKey();
		m_chartProperty.frequency = m_frequency;
		return m_chartProperty;
	}

    void HistoricalChartMobile::setIsGettingChartData(bool isChanging)
	{
		m_isGettingChartData = isChanging;
		m_toolbar->OnGettingChartData(m_isGettingChartData);
	}

    void HistoricalChartMobile::timerEvent(QTimerEvent* event)
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
	}

    void HistoricalChartMobile::addOrder(const OrderInfo& order)
	{
		m_orderLineManager->addOrder(order);
	}

    void HistoricalChartMobile::removeOrder(const QUuid& orderId)
	{
		m_orderLineManager->removeOrder(orderId);
	}

    void HistoricalChartMobile::clearOrders()
	{
		m_orderLineManager->clearOrders();
	}
}

#include "moc_HistoricalChartMobile.cpp"
