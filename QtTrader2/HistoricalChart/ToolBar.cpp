#include "Toolbar.h"
#include "SR.h"
#include "historicalchart.h"
#include <QLayout>
#include <QLayoutItem>
#include <QMetaObject>
#include <QDebug>

namespace HistoricalChartSpace
{
	ToolBar::ToolBar(HistoricalChart *pView, bool isBlackTheme)
		: m_pView(pView), QToolBar(pView),
		  m_isBlackTheme(isBlackTheme),
		  m_isMovedByUser(false),
		  m_lastWidget(nullptr),
		  m_chartDestoryed(false)
	{
#if WIN32
		this->setObjectName("ChartToolBar");
#else
		this->setWindowOpacity(1);
		this->setStyleSheet("background-color: #ffffff;");
#endif
	}

	ToolBar::~ToolBar()
	{
	}

	void ToolBar::OnThemeChanged(bool isBlackTheme)
	{
		m_isBlackTheme = isBlackTheme;
		if (!m_isChangeChartToolbarByChangeSystemTheme)
			m_isBlackTheme = false; // After the graphical toolbar is moved to the Trader TitleBar, only the white background Theme is needed.
		if (m_isBlackTheme)
		{
			m_gridSwitchAction->setIcon(getResQIcon("grid_line.png"));
			m_crosshairSwitch->setIcon(getResQIcon("crosshair.png"));
			m_autoScrollSwitch->setIcon(getResQIcon("auto_scroll.png"));
			m_shiftDataSwitch->setIcon(getResQIcon("shift_left.png"));
			m_labelInfoSwitch->setIcon(getResQIcon("info.png"));
			m_quickPlacerSwitch->setIcon(getResQIcon("click_place.png"));
			m_showExecutedOrderSwitch->setIcon(getResQIcon("open_position.png"));
			m_showPendingOrderSwitch->setIcon(getResQIcon("pending_order.png"));
			m_drawTextAction->setIcon(getResQIcon("text.png"));
			m_indicatorsAction->setIcon(getResQIcon("indicators.png"));
			m_drawAction->setIcon(getResQIcon("draw.png"));
			m_templatesAction->setIcon(getResQIcon("template.png"));
			QList<QWidget *> widgets = m_templatesAction->associatedWidgets();
			qDebug() << "Changed to black theme. m_templatesAction->isVisible:" << widgets[1]->isVisible();
			qDebug() << "Changed to black theme. m_templatesAction->isVisibleTo:" << widgets[1]->isVisibleTo(this);
		}
		else
		{
			m_gridSwitchAction->setIcon(getResQIcon("grid_line_w.png"));
			m_crosshairSwitch->setIcon(getResQIcon("crosshair_w.png"));
			m_autoScrollSwitch->setIcon(getResQIcon("auto_scroll_w.png"));
			m_shiftDataSwitch->setIcon(getResQIcon("shift_left_w.png"));
			m_labelInfoSwitch->setIcon(getResQIcon("info_w.png"));
			m_quickPlacerSwitch->setIcon(getResQIcon("click_place_w.png"));
			m_showExecutedOrderSwitch->setIcon(getResQIcon("open_position_w.png"));
			m_showPendingOrderSwitch->setIcon(getResQIcon("pending_order_w.png"));
			m_drawTextAction->setIcon(getResQIcon("text_w.png"));
			m_indicatorsAction->setIcon(getResQIcon("indicators_w.png"));
			m_drawAction->setIcon(getResQIcon("draw_w.png"));
			m_templatesAction->setIcon(getResQIcon("template_w.png"));
			QList<QWidget *> widgets = m_templatesAction->associatedWidgets();
			qDebug() << "Changed to white theme. m_templatesAction->isVisible:" << widgets[1]->isVisible();
			qDebug() << "Changed to white theme. m_templatesAction->isVisibleTo:" << widgets[1]->isVisibleTo(this);
		}
	}

	void ToolBar::createActions()
	{
		m_gridSwitchAction = new QAction(SR::get("ShowGrid"), m_pView);
		m_gridSwitchAction->setCheckable(true);
		QObject::connect(m_gridSwitchAction, SIGNAL(triggered()), m_pView, SLOT(ShowGridSwitchSlot()));

		m_crosshairSwitch = new QAction(SR::get("Crosshair"), m_pView);
		m_crosshairSwitch->setCheckable(true);
		QObject::connect(m_crosshairSwitch, SIGNAL(triggered()), m_pView, SLOT(CrosshairSwitchSlot()));

		m_autoScrollSwitch = new QAction(SR::get("AutoScroll"), m_pView);
		m_autoScrollSwitch->setCheckable(true);
		QObject::connect(m_autoScrollSwitch, SIGNAL(triggered()), m_pView, SLOT(AutoScrollSwitchSlot()));

		m_shiftDataSwitch = new QAction(SR::get("ShiftData"), m_pView);
		m_shiftDataSwitch->setCheckable(true);
		QObject::connect(m_shiftDataSwitch, SIGNAL(triggered()), m_pView, SLOT(ShiftDataSwitchSlot()));

		m_zoomOutAction = new QAction(getResQIcon("zoom_out.png"), SR::get("ZoomOut"), m_pView);
		//m_zoomOutAction->setData(false);
		QObject::connect(m_zoomOutAction, SIGNAL(triggered()), m_pView, SLOT(OnZoomOutSlot()));

		m_zoomInAciton = new QAction(getResQIcon("zoom_in.png"), SR::get("ZoomIn"), m_pView);
		//m_zoomInAciton->setData(true);
		m_zoomInAciton->setCheckable(true);
		QObject::connect(m_zoomInAciton, SIGNAL(triggered()), m_pView, SLOT(OnZoomInSlot()));

		m_labelInfoSwitch = new QAction(SR::get("ShowLabelInfo"), m_pView);
		m_labelInfoSwitch->setCheckable(true);
		QObject::connect(m_labelInfoSwitch, SIGNAL(triggered()), m_pView, SLOT(LabelInfoSwitchSlot()));

		m_quickPlacerSwitch = new QAction(SR::get("QuickPlace"), m_pView);
		m_quickPlacerSwitch->setCheckable(true);
		QObject::connect(m_quickPlacerSwitch, SIGNAL(triggered(bool)), m_pView, SLOT(QuickPlacerSwitchSlot(bool)));

		m_trendIndicatorAction = new QAction(SR::get("TrendIndicators"), m_pView);
		m_oscillatorsIndicatorAction = new QAction(SR::get("OscillatorsIndicators"), m_pView);
		m_volumesIndicatorAction = new QAction(SR::get("VolumesIndicators"), m_pView);
		m_othersIndicatorAction = new QAction(SR::get("Others"), m_pView);

		for (int i = 0; i < enIndicatorTrendEnd; ++i)
		{
			QAction *action = new QAction(IndicatorCore::getIndicatorName((enIndicatorType)i), m_pView);
			if (action == NULL)
				return;
			action->setData(i);
			m_trendIndicatorActions.append(action);
			QObject::connect(action, SIGNAL(triggered()), m_pView, SLOT(AddIndicatorSlot()));
		}
		for (int i = enIndicatorTrendEnd + 1; i < enIndicatorOscillatorsEnd; ++i)
		{
			QAction *action = new QAction(IndicatorCore::getIndicatorName((enIndicatorType)i), m_pView);
			if (action == NULL)
				return;
			action->setData(i);
			m_oscillatorsIndicatorActions.append(action);
			QObject::connect(action, SIGNAL(triggered()), m_pView, SLOT(AddIndicatorSlot()));
		}
		for (int i = enIndicatorOscillatorsEnd + 1; i < enIndicatorVolumesEnd; ++i)
		{
			QAction *action = new QAction(IndicatorCore::getIndicatorName((enIndicatorType)i), m_pView);
			if (action == NULL)
				return;
			action->setData(i);
			m_volumesIndicatorActions.append(action);
			QObject::connect(action, SIGNAL(triggered()), m_pView, SLOT(AddIndicatorSlot()));
		}
		for (int i = enIndicatorVolumesEnd + 1; i < enIndicatorEnd; ++i)
		{
			QAction *action = new QAction(IndicatorCore::getIndicatorName((enIndicatorType)i), m_pView);
			if (action == NULL)
				return;
			action->setData(i);
			m_othersIndicatorActions.append(action);
			QObject::connect(action, SIGNAL(triggered()), m_pView, SLOT(AddIndicatorSlot()));
		}

		m_showExecutedOrderSwitch = new QAction(SR::get("ShowOpenPosition"), m_pView);
		m_showExecutedOrderSwitch->setCheckable(true);
		QObject::connect(m_showExecutedOrderSwitch, SIGNAL(triggered(bool)), m_pView, SLOT(ShowExecutedOrderSwitchSlot(bool)));

		m_showPendingOrderSwitch = new QAction(SR::get("ShowPendingOrder"), m_pView);
		m_showPendingOrderSwitch->setCheckable(true);
		QObject::connect(m_showPendingOrderSwitch, SIGNAL(triggered(bool)), m_pView, SLOT(ShowPendingOrderSwitchSlot(bool)));

		m_drawTextAction = new QAction(SR::get("Text"), m_pView);
		m_drawTextAction->setCheckable(true);
		QObject::connect(m_drawTextAction, SIGNAL(triggered(bool)), m_pView, SLOT(CreateTextSlot(bool)));

		for (int i = 0; i < enDrawingObjectTypeUnknown; ++i)
		{
			QAction *action = new QAction(Helper::getDrawingObjBaseStrInfo((enDrawingObjectType)i), m_pView);
			if (action == NULL)
				return;
			action->setData(i);
			m_listDrawingObjectAction.append(action);
			QObject::connect(action, SIGNAL(triggered()), m_pView, SLOT(CreatorDrawingObjectSlot()));
		}
	}

	int ToolBar::getCurrentWidth()
	{
		return m_markLabel->rect().topRight().x();
	}

	int ToolBar::getCurrentInstrumentIndex()
	{
		return m_instrumentComboBox->currentIndex();
	}
	int ToolBar::getDisplayPeriod()
	{
		return m_periodComboBox->currentData().toInt();
	}

	void ToolBar::mouseReleaseEvent(QMouseEvent *event)
	{
		qDebug() << " ToolBar::mouseReleaseEvent:" << event;
		event->accept();
	}

	void ToolBar::mouseMoveEvent(QMouseEvent *event)
	{
		qDebug() << " ToolBar::mouseMoveEvent:" << event;
		event->accept();
	}
	void ToolBar::mousePressEvent(QMouseEvent *event)
	{
		qDebug() << " ToolBar::mousePressEvent:" << event;
		event->accept();
	}

	/*
	void ToolBar::mousePressEvent(QMouseEvent* event)
	{
		m_startPos = event->pos();
	}

	void ToolBar::mouseMoveEvent(QMouseEvent* event)
	{
		if (event->buttons() & Qt::LeftButton)
		{
			QPoint targetPoint = m_basePoint + event->pos() - m_startPos;

			if (targetPoint.x() < 0) targetPoint.setX(0);
			if (targetPoint.y() < 0) targetPoint.setY(0);

			int maxX = m_pView->width() - this->width();
			int maxY = m_pView->height() - this->height();
			if (targetPoint.x() > maxX) targetPoint.setX(maxX);
			if (targetPoint.y() > maxY) targetPoint.setY(maxY);

			this->move(targetPoint);
			m_basePoint = targetPoint;
			m_isMovedByUser = true;
		}
	}
	*/

	void ToolBar::test()
	{
		////m_toolbar->resize(m_toolbar->width() / 2, m_toolbar->height());
		//this->OnChartResize(width() / 2);
		this->setFloatable(true);
		this->setMovable(true);
		bool lastActionIsVisible = this->widgetForAction(m_templatesAction)->isVisible();
		lastActionIsVisible = this->widgetForAction(m_templatesAction)->isVisibleTo(this);
	}

	void ToolBar::DrawingObjectCreated(enDrawingObjectType lineType)
	{
		if (lineType == enDrawingObjectText)
		{
			m_drawTextAction->setChecked(false);
		}
	}

	void ToolBar::createToolBar(const QList<Instrument> &instruments, const QList<Frequency> &frequencies, TemplateManager *templateManager, bool isChangeChartToolbarByChangeSystemTheme)
	{
		this->m_isChangeChartToolbarByChangeSystemTheme = isChangeChartToolbarByChangeSystemTheme;

		this->createActions();

		m_instrumentComboBox = new QComboBox(m_pView);
		m_quotePolicyComboBox = new QComboBox(m_pView);
		m_frequencyComboBox = new QComboBox(m_pView);
		m_graphTypeComboBox = new QComboBox(m_pView);
		m_periodComboBox = new QComboBox(m_pView);

		this->addWidget(m_instrumentComboBox);
		m_quotePolicyComboBoxAction = this->addWidget(m_quotePolicyComboBox);
		this->addWidget(m_frequencyComboBox);
		this->addWidget(m_graphTypeComboBox);
		this->addWidget(m_periodComboBox);

		foreach (Instrument ins, instruments)
		{
			m_instrumentComboBox->addItem(ins.getDescription());
		}

		foreach (Frequency frequency, frequencies)
		{
			m_frequencyComboBox->addItem(SR::get(frequency.getName()));
		}

		m_graphTypeComboBox->addItem(SR::get("Bar"), enPriceBar);
		m_graphTypeComboBox->addItem(SR::get("Line"), enPriceLine);
		m_graphTypeComboBox->addItem(SR::get("Candlestick"), enPriceCandlesticks);

		this->addActions(QList<QAction *>()
						 << m_gridSwitchAction
						 << m_crosshairSwitch
						 << m_autoScrollSwitch
						 << m_shiftDataSwitch
						 << m_zoomOutAction
						 << m_zoomInAciton
						 << m_labelInfoSwitch
						 << m_quickPlacerSwitch
						 << m_showExecutedOrderSwitch
						 << m_showPendingOrderSwitch
						 << m_drawTextAction);
		this->setAutoFillBackground(true);

		QMenu *menu = new QMenu(SR::get("Indicator"), m_pView);
		menu->addAction(m_trendIndicatorAction);
		menu->addAction(m_oscillatorsIndicatorAction);
		//menu->addAction(m_volumesIndicatorAction);  According to bug 6941, Temporarily do not display volume indicators.
		menu->addAction(m_othersIndicatorAction);

		QMenu *trendIndicatorsMenu = new QMenu(m_pView);
		trendIndicatorsMenu->addActions(m_trendIndicatorActions);
		m_trendIndicatorAction->setMenu(trendIndicatorsMenu);

		QMenu *oscillatorsIndicatorsMenu = new QMenu(m_pView);
		oscillatorsIndicatorsMenu->addActions(m_oscillatorsIndicatorActions);
		m_oscillatorsIndicatorAction->setMenu(oscillatorsIndicatorsMenu);

		QMenu *volumesIndicatorsMenu = new QMenu(m_pView);
		volumesIndicatorsMenu->addActions(m_volumesIndicatorActions);
		m_volumesIndicatorAction->setMenu(volumesIndicatorsMenu);

		QMenu *othersIndicatorsMenu = new QMenu(m_pView);
		othersIndicatorsMenu->addActions(m_othersIndicatorActions);
		m_othersIndicatorAction->setMenu(othersIndicatorsMenu);

		m_indicatorsAction = new QAction(QIcon("Resources/ChartImages/indicators.png"), SR::get("Indicator"), m_pView);
		m_indicatorsAction->setMenu(menu);
		this->addAction(m_indicatorsAction);

		menu = new QMenu(SR::get("DrawingObjects"), m_pView);
		menu->addActions(m_listDrawingObjectAction);
		m_drawAction = new QAction(QIcon("Resources/ChartImages/draw.png"), SR::get("DrawingObjects"), m_pView);
		m_drawAction->setMenu(menu);
		this->addAction(m_drawAction);

		m_templatesAction = new QAction(QIcon("Resources/ChartImages/template.png"), SR::get("Templates"), m_pView);
		m_templatesAction->setMenu(templateManager->getMenu());
		this->addAction(m_templatesAction);

		m_lastWidget = m_templatesAction->associatedWidgets()[1];
		m_lastWidget->installEventFilter(this);

		QWidget::show();
		QWidget::update();
		m_toolbarWidth = width();

		this->OnThemeChanged(m_isBlackTheme);

		m_markLabel = new QLabel("", m_pView);
		m_markLabel->setFixedWidth(1);
		this->addWidget(m_markLabel);
	}
	/*
	void ToolBar::mouseDoubleClickEvent(QMouseEvent *)
	{
		//// adjust toolbar width  -- The code is valid, and the adjustment effect is not satisfactory, so do not use it temporarily.
		//for each (QObject* child in this->children())
		//{
		//	QString className = child->metaObject()->className();
		//	if (className == "QToolBarExtension")
		//	{
		//		QWidget* extersionButton = qobject_cast<QWidget*>(child);
		//		if (extersionButton && extersionButton->isVisible())
		//		{
		//			m_toolbarWidth += 30;
		//		}
		//	}
		//}

		m_isMovedByUser = false;

		QWidget* quickPlacerWidget = m_pView->m_tradeHelper->getQuickPlacer()->getWidget();
		int leftMargin = quickPlacerWidget && quickPlacerWidget->isVisible() ? quickPlacerWidget->pos().x() + quickPlacerWidget->width() + 2 : 0;

		QRect rect;
		int chartWidth = m_pView->width() + 2;
		int x = chartWidth - m_toolbarWidth;
		if (x < 0) x = 0;
		if (x < leftMargin) x = leftMargin;
		rect.setX(x);
		rect.setY(0);
		rect.setWidth(m_toolbarWidth);
		rect.setHeight(this->height());
		this->setGeometry(rect);

		this->OnChartResize(m_pView->width());
	}
	*/

	//void ToolBar::paintEvent(QPaintEvent *event)
	//{
	//	QStyleOption opt;
	//	opt.init(this);
	//	QPainter p(this);
	//	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	//}

	bool ToolBar::eventFilter(QObject *target, QEvent *event)
	{
		if (target == m_lastWidget)
		{
			if (event->type() == QEvent::Show)
			{
				qDebug() << "m_templatesAction Show";
				int width = m_lastWidget->pos().x() + m_lastWidget->width() + 10;
				qDebug() << "old width:" << this->size().width() << ", new width:" << width;
				this->setGeometry(this->pos().x(), this->pos().y(), width, this->size().height());
				m_toolbarWidth = width;
			}
			else if (event->type() == QEvent::Hide)
			{
				qDebug() << "m_templatesAction Hide";
			}
		}
		return QObject::eventFilter(target, event);
	}

	void ToolBar::OnChartResize(int chartWidth)
	{
		int width = this->getCurrentWidth();
		if (width == 0)
			width = m_toolbarWidth;
		width = qMin(chartWidth - 120, width);
		this->resize(width, this->height()); // 120 It is the width of the tool button on the right side of the Trader TitleBar.
		return;

		if (this->isVisibleTo(m_pView))
		{
			bool isAdjusted = true;
			do
			{
				QWidget *quickPlacerWidget = m_pView->m_tradeHelper->getQuickPlacer()->getWidget();
				int leftMargin = quickPlacerWidget && m_pView->getConfig().getCommonSettings().m_isShowQuickPlacer ? quickPlacerWidget->pos().x() + quickPlacerWidget->width() + 2 : 0;

				if (!m_lastWidget->isVisible() && leftMargin + m_toolbarWidth + 10 < m_pView->width())
				{
					m_toolbarWidth += 10;
					isAdjusted = true;
				}
				else
				{
					isAdjusted = false;
				}

				QRect rect = m_pView->rect();
				int x, y;
				if (m_isMovedByUser)
				{
					x = this->x();
				}
				else
				{
					x = chartWidth - m_toolbarWidth;
					if (x < 0)
						x = 0;
				}
				if (x < leftMargin)
					x = leftMargin;
				rect.setX(x);
				rect.setY(this->y());
				rect.setHeight(this->height());

				int space = chartWidth - x;
				rect.setWidth(space > m_toolbarWidth ? m_toolbarWidth : space);
				this->setGeometry(rect);
			} while (!m_lastWidget->isVisible() && isAdjusted);
		}
	}

	void ToolBar::switchVisible()
	{
		this->setVisible(!this->isVisible());
	}

	void ToolBar::resetPeriodComboBox(const Frequency &currentFrequency)
	{
		// update period ComboBox
		/*
		1. get periods (default: unlimited)  unlimited mean no period limit.(Consistent with current behavior)
		2. add to m_periodCombobox
		*/
		QObject::disconnect(m_periodComboBox, SIGNAL(currentIndexChanged(int)), m_pView, SLOT(SetPeriodSlot(int)));
		m_periodComboBox->clear();

		const FrequencyType type = currentFrequency.getType();
		const int unit = currentFrequency.getUint();
		m_periodComboBox->addItem(SR::get("Unlimited"), PeriodType::Unlimited);

		if(type == FrequencyType::Minute) 
		{
			if(unit == 1)
			{
				m_periodComboBox->addItem(SR::get("Today"), PeriodType::Today);
				m_periodComboBox->addItem(SR::get("ThisWeek"), PeriodType::ThisWeek);
				m_periodComboBox->addItem(SR::get("ThisMonth"), PeriodType::ThisMonth);
				m_periodComboBox->addItem(SR::get("Day2"), PeriodType::Day2);
				m_periodComboBox->addItem(SR::get("Day3"), PeriodType::Day3);
				m_periodComboBox->addItem(SR::get("Day4"), PeriodType::Day4);
				m_periodComboBox->addItem(SR::get("Day5"), PeriodType::Day5);
				m_periodComboBox->addItem(SR::get("Day10"), PeriodType::Day10);
				m_periodComboBox->addItem(SR::get("Day15"), PeriodType::Day15);
				m_periodComboBox->addItem(SR::get("Day30"), PeriodType::Day30);
				//m_periodComboBox->addItem(SR::get("Day45"), PeriodType::Day45);
			}
			else if(unit == 5)
			{
				m_periodComboBox->addItem(SR::get("Today"), PeriodType::Today);
				m_periodComboBox->addItem(SR::get("ThisWeek"), PeriodType::ThisWeek);
				m_periodComboBox->addItem(SR::get("ThisMonth"), PeriodType::ThisMonth);
				m_periodComboBox->addItem(SR::get("ThisQuarter"), PeriodType::ThisQuarter);
				m_periodComboBox->addItem(SR::get("Day5"), PeriodType::Day5);
				m_periodComboBox->addItem(SR::get("Day10"), PeriodType::Day10);
				m_periodComboBox->addItem(SR::get("Day15"), PeriodType::Day15);
				m_periodComboBox->addItem(SR::get("Day30"), PeriodType::Day30);
				m_periodComboBox->addItem(SR::get("Day45"), PeriodType::Day45);
				m_periodComboBox->addItem(SR::get("Day60"), PeriodType::Day60);
				m_periodComboBox->addItem(SR::get("Day90"), PeriodType::Day90);
				m_periodComboBox->addItem(SR::get("Day120"), PeriodType::Day120);
				m_periodComboBox->addItem(SR::get("Day180"), PeriodType::Day180);
			}
			else if(unit == 15)
			{
				m_periodComboBox->addItem(SR::get("ThisWeek"), PeriodType::ThisWeek);
				m_periodComboBox->addItem(SR::get("ThisMonth"), PeriodType::ThisMonth);
				m_periodComboBox->addItem(SR::get("ThisQuarter"), PeriodType::ThisQuarter);
				m_periodComboBox->addItem(SR::get("Day15"), PeriodType::Day15);
				m_periodComboBox->addItem(SR::get("Day30"), PeriodType::Day30);
				m_periodComboBox->addItem(SR::get("Day45"), PeriodType::Day45);
				m_periodComboBox->addItem(SR::get("Month2"), PeriodType::Month2);
				m_periodComboBox->addItem(SR::get("Month3"), PeriodType::Month3);
				m_periodComboBox->addItem(SR::get("Month6"), PeriodType::Month6);
				m_periodComboBox->addItem(SR::get("Year1"), PeriodType::Year1);
				m_periodComboBox->addItem(SR::get("Year2"), PeriodType::Year2);
			}
			else if(unit == 30)
			{
				m_periodComboBox->addItem(SR::get("ThisWeek"), PeriodType::ThisWeek);
				m_periodComboBox->addItem(SR::get("ThisMonth"), PeriodType::ThisMonth);
				m_periodComboBox->addItem(SR::get("ThisQuarter"), PeriodType::ThisQuarter);
				m_periodComboBox->addItem(SR::get("Day30"), PeriodType::Day30);
				m_periodComboBox->addItem(SR::get("Day60"), PeriodType::Day60);
				m_periodComboBox->addItem(SR::get("Day90"), PeriodType::Day90);
				m_periodComboBox->addItem(SR::get("Month6"), PeriodType::Month6);
				m_periodComboBox->addItem(SR::get("Year1"), PeriodType::Year1);
				m_periodComboBox->addItem(SR::get("Year2"), PeriodType::Year2);
				m_periodComboBox->addItem(SR::get("Year3"), PeriodType::Year3);
			}
		}
		else if(type == FrequencyType::Hour)
		{
			if(unit == 1)
			{
				m_periodComboBox->addItem(SR::get("ThisMonth"), PeriodType::ThisMonth);
				m_periodComboBox->addItem(SR::get("ThisQuarter"), PeriodType::ThisQuarter);
				m_periodComboBox->addItem(SR::get("Day30"), PeriodType::Day30);
				m_periodComboBox->addItem(SR::get("Day60"), PeriodType::Day60);
				m_periodComboBox->addItem(SR::get("Day90"), PeriodType::Day90);
				m_periodComboBox->addItem(SR::get("Month6"), PeriodType::Month6);
				m_periodComboBox->addItem(SR::get("Year1"), PeriodType::Year1);
				m_periodComboBox->addItem(SR::get("Year2"), PeriodType::Year2);
				m_periodComboBox->addItem(SR::get("Year3"), PeriodType::Year3);
				m_periodComboBox->addItem(SR::get("Year5"), PeriodType::Year5);
			}
			else
			{
				m_periodComboBox->addItem(SR::get("ThisMonth"), PeriodType::ThisMonth);
				m_periodComboBox->addItem(SR::get("ThisQuarter"), PeriodType::ThisQuarter);
				m_periodComboBox->addItem(SR::get("ThisYear"), PeriodType::ThisYear);
				m_periodComboBox->addItem(SR::get("Month6"), PeriodType::Month6);
				m_periodComboBox->addItem(SR::get("Year1"), PeriodType::Year1);
				m_periodComboBox->addItem(SR::get("Year2"), PeriodType::Year2);
				m_periodComboBox->addItem(SR::get("Year3"), PeriodType::Year3);
				m_periodComboBox->addItem(SR::get("Year5"), PeriodType::Year5);
				m_periodComboBox->addItem(SR::get("Year10"), PeriodType::Year10);
			}
		}
		else
		{
			if(type == FrequencyType::Day)
			{
				m_periodComboBox->addItem(SR::get("ThisQuarter"), PeriodType::ThisQuarter);
			}
			m_periodComboBox->addItem(SR::get("ThisYear"), PeriodType::ThisYear);
			m_periodComboBox->addItem(SR::get("Month6"), PeriodType::Month6);
			m_periodComboBox->addItem(SR::get("Year1"), PeriodType::Year1);
			m_periodComboBox->addItem(SR::get("Year2"), PeriodType::Year2);
			m_periodComboBox->addItem(SR::get("Year3"), PeriodType::Year3);
			m_periodComboBox->addItem(SR::get("Year4"), PeriodType::Year4);
			m_periodComboBox->addItem(SR::get("Year5"), PeriodType::Year5);
			m_periodComboBox->addItem(SR::get("Year10"), PeriodType::Year10);
			m_periodComboBox->addItem(SR::get("Year20"), PeriodType::Year20);
		}

		QObject::connect(m_periodComboBox, SIGNAL(currentIndexChanged(int)), m_pView, SLOT(SetPeriodSlot(int)));
	}

	void ToolBar::applySettings(Config &config, const Instrument &currentInstrument, const Frequency &currentFrequency, bool userClickAtProductCode)
	{
		enPriceType type = config.getPriceSettings().m_drawType;
		m_graphTypeComboBox->setCurrentIndex(m_graphTypeComboBox->findData(type));

		if (config.hasConfigData())
		{
			if (userClickAtProductCode)
				m_instrumentComboBox->setCurrentIndex(0);
			else
				m_instrumentComboBox->setCurrentText(currentInstrument.getDescription());
			m_frequencyComboBox->setCurrentText(SR::get(currentFrequency.getName()));
		}

		const CommonSettings &settings = config.getCommonSettings();
		this->updateCommonSettings(settings);
	}

	void ToolBar::updateCommonSettings(const CommonSettings &settings)
	{
		m_gridSwitchAction->setChecked(settings.m_isShowGrid);
		m_autoScrollSwitch->setChecked(settings.m_isAutoScroll);
		m_shiftDataSwitch->setChecked(settings.m_isShiftData);
		m_labelInfoSwitch->setChecked(settings.m_isShowLabelInfo);
		m_crosshairSwitch->setChecked(settings.m_isShowCrosshair);
		m_quickPlacerSwitch->setChecked(settings.m_isShowQuickPlacer);
		m_showExecutedOrderSwitch->setChecked(settings.m_isShowExecutedOrder);
		m_showPendingOrderSwitch->setChecked(settings.m_isShowPendingOrder);
	}

	void ToolBar::makeComboBoxConnections()
	{
		QObject::connect(m_instrumentComboBox, SIGNAL(currentIndexChanged(int)), m_pView, SLOT(SetInstrumentSlot(int)));
		QObject::connect(m_quotePolicyComboBox, SIGNAL(currentIndexChanged(int)), m_pView, SLOT(SetQuotePolicySlot(int)));
		QObject::connect(m_frequencyComboBox, SIGNAL(currentIndexChanged(int)), m_pView, SLOT(SetFrequencySlot(int)));
		QObject::connect(m_graphTypeComboBox, SIGNAL(currentIndexChanged(int)), m_pView, SLOT(SetPriceChartSlot(int)));
		//QObject::connect(m_periodComboBox, SIGNAL(currentIndexChanged(int)), m_pView, SLOT(SetPeriodSlot(int)));
	}

	void ToolBar::updateQuotePolicy(const QList<KeyDescription> &quotePolicies, KeyDescription &currentQuotePolicy)
	{
		QObject::disconnect(m_quotePolicyComboBox, SIGNAL(currentIndexChanged(int)), m_pView, SLOT(SetQuotePolicySlot(int)));

		bool isContainsCurrent = false;
		m_quotePolicyComboBox->clear();
		foreach (KeyDescription policy, quotePolicies)
		{
			m_quotePolicyComboBox->addItem(policy.getDescription());
			if (currentQuotePolicy == policy)
			{
				isContainsCurrent = true;
			}
		}

		if (isContainsCurrent)
		{
			m_quotePolicyComboBox->setCurrentText(currentQuotePolicy.getDescription());
		}
		else
		{
			currentQuotePolicy = quotePolicies[0];
		}

		m_quotePolicyComboBoxAction->setVisible(quotePolicies.size() > 1);

		QObject::connect(m_quotePolicyComboBox, SIGNAL(currentIndexChanged(int)), m_pView, SLOT(SetQuotePolicySlot(int)));
	}

	void ToolBar::changeDrawType(enPriceType drawType)
	{
		if (m_graphTypeComboBox->currentData() != drawType)
		{
			m_graphTypeComboBox->setCurrentIndex(m_graphTypeComboBox->findData(drawType));
		}
	}

	void ToolBar::OnGettingChartData(bool isGettingChartData)
	{
		m_instrumentComboBox->setEnabled(!isGettingChartData);
		m_quotePolicyComboBox->setEnabled(!isGettingChartData);
		m_frequencyComboBox->setEnabled(!isGettingChartData);
		m_periodComboBox->setEnabled(!isGettingChartData);
	}

	void ToolBar::setZoomState(bool isMinimalState, bool isMaximalState)
	{
		if (isMinimalState)
		{
			m_zoomOutAction->setEnabled(false);
		}
		else if (isMaximalState)
		{
			m_zoomInAciton->setEnabled(false);
		}
		else
		{
			if (!m_zoomOutAction->isEnabled())
			{
				m_zoomOutAction->setEnabled(true);
			}
			if (!m_zoomInAciton->isEnabled())
			{
				m_zoomInAciton->setEnabled(true);
			}
		}
	}

	//void ToolBar::changeZoomInStyle(bool isInFixPeriodMode)
	//{
	//	m_zoomInAciton->setCheckable(isInFixPeriodMode);
	//}
	bool ToolBar::isInZoomIn()
	{
		return m_zoomInAciton->isChecked();
	}
	void ToolBar::OnZoomInEnd()
	{
		m_zoomInAciton->setChecked(false);
	}
	QIcon ToolBar::getResQIcon(QString fileName)
	{
#ifdef RUNTIME_OS_MACX
		QString path = Helper::getChartResPath(fileName);
		return QIcon(path);
#else
		return QIcon(QString("%1/%2").arg("Resources/ChartImages", fileName));
#endif
	}
}
