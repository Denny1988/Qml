#include "priceDialog.h"
#include "SR.h"
#include "SC.h"

namespace HistoricalChartSpace
{
	PriceDialog::PriceDialog(CommonSettings& commonSettings, PriceSettings& priceSettings, QWidget *parent)
		:QDialog(parent),
		m_commonSettings(commonSettings),
		m_priceSettings(priceSettings)
	{
		m_ui.setupUi(this);
		this->setFixedSize(this->size());
		this->applyStringResource();
		this->initSettings();
	}

	PriceDialog::~PriceDialog()
	{
	}

	void PriceDialog::applyStringResource()
	{
		this->setWindowTitle(SR::get("CommonProperty"));
		m_ui.okButton->setText(SR::get("OK"));
		m_ui.cancelButton->setText(SR::get("Cancel"));
		m_ui.tabWidget->setTabText(0, SR::get("Common"));
		m_ui.tabWidget->setTabText(1, SR::get("Colors"));
		m_ui.candlesticksButton->setText(SR::get("Candlestick"));
		m_ui.barButton->setText(SR::get("Bar"));
		m_ui.lineChartButton->setText(SR::get("Line"));
		m_ui.autoScrollCheckBox->setText(SR::get("AutoScroll"));
		m_ui.chartShiftCheckBox->setText(SR::get("ShiftData"));
		m_ui.scaleLabel->setText(SR::get("Scale"));
		m_ui.showGridCheckBox->setText(SR::get("ShowGrid"));
		m_ui.showLableInfoCheckBox->setText(SR::get("ShowLabelInfo"));
		m_ui.showVolumeCheckBox->setText(SR::get("ShowVolume"));
		m_ui.showBidLineCheckBox->setText(SR::get("ShowBidLine"));
		m_ui.showAskLinecheckBox->setText(SR::get("ShowAskLine"));

		m_ui.backgroundLabel->setText(SR::get("Background"));
		m_ui.foregroundLabel->setText(SR::get("Foreground"));
		m_ui.gridLabel->setText(SR::get("Grid"));
		m_ui.barUpLabel->setText(SR::get("BarUp"));
		m_ui.barDownLabel->setText(SR::get("BarDown"));
		m_ui.bullCandleLabel->setText(SR::get("BullCandle"));
		m_ui.bearCandleLabel->setText(SR::get("BearCandle"));
		m_ui.crossStarLabel->setText(SR::get("CrossStar"));
		m_ui.lineChartLabel->setText(SR::get("LineChart"));
		m_ui.volumeLabel->setText(SR::get(SC::Volume));
		m_ui.bidLineLabel->setText(SR::get("BidLine"));
		m_ui.askLineLabel->setText(SR::get("AskLine"));
		m_ui.openOrderLabel->setText(SR::get("OpenOrder"));
		m_ui.pendingOrderLabel->setText(SR::get("PendingOrder"));

        m_ui.productCodeLabel->setText(SR::get("ProductCode"));
	}

	void PriceDialog::initSettings()
	{
		if (m_commonSettings.m_isAutoScroll)
		{
			m_ui.autoScrollCheckBox->setCheckState(Qt::Checked);
		}

		if (m_commonSettings.m_isShiftData)
		{
			m_ui.chartShiftCheckBox->setCheckState(Qt::Checked);
		}

		if (m_commonSettings.m_isShowGrid)
		{
			m_ui.showGridCheckBox->setCheckState(Qt::Checked);
		}

		if (m_commonSettings.m_isShowLabelInfo)
		{
			m_ui.showLableInfoCheckBox->setCheckState(Qt::Checked);
		}

		if (m_priceSettings.m_isShowBidPriceLine)
		{
			m_ui.showBidLineCheckBox->setCheckState(Qt::Checked);
		}
		if (m_priceSettings.m_isShowAskPriceLine)
		{
			m_ui.showAskLinecheckBox->setCheckState(Qt::Checked);
		}
		if (m_priceSettings.m_isShowVolume)
		{
			m_ui.showVolumeCheckBox->setCheckState(Qt::Checked);
		}

		m_ui.scaleSlider->setRange(zoom_minimal, zoom_maximal);
		m_ui.scaleSlider->setValue(m_commonSettings.m_zoomLevel);

		switch (m_priceSettings.m_drawType)
		{
		case enPriceBar:
			m_ui.barButton->setChecked(true);
			break;
		case enPriceLine:
			m_ui.lineChartButton->setChecked(true);
			break;
		default:
			m_ui.candlesticksButton->setChecked(true);
			break;
		}

		QList<QComboBox*> listCombobox;
		listCombobox << m_ui.backgroundColorBox << m_ui.foregroundColorBox << m_ui.barDownColorBox
			<< m_ui.bearCandleColorBox << m_ui.bullCandleColorBox << m_ui.bidLineColorBox << m_ui.askLineColorBox
			<<m_ui.crossStarColorBox << m_ui.lineChartColorBox << m_ui.volumesColorBox << m_ui.gridColorBox << m_ui.barUpColorBox
            << m_ui.openOrderColorBox << m_ui.pendingOrderColorBox << m_ui.productCodeComboBox;
		foreach(QComboBox* cbb, listCombobox)
		{
			QStringList colorNameList = QColor::colorNames();
			foreach(QString colorName, colorNameList)
			{
				QPixmap pix_color(cbb->size() / 2);
				pix_color.fill(QColor(colorName));
				cbb->addItem(QIcon(pix_color), colorName, QColor(colorName));
				cbb->setIconSize(cbb->size() / 2);
				cbb->setSizeAdjustPolicy(QComboBox::AdjustToContents);
			}
		}

		setComBoxDefaultColor(m_ui.backgroundColorBox, m_commonSettings.m_background);
		setComBoxDefaultColor(m_ui.foregroundColorBox, m_commonSettings.m_foreground);
		setComBoxDefaultColor(m_ui.barDownColorBox, m_priceSettings.m_barDownColor);
		setComBoxDefaultColor(m_ui.bearCandleColorBox, m_priceSettings.m_bearCandleColor);
		setComBoxDefaultColor(m_ui.bullCandleColorBox, m_priceSettings.m_bullCandleColor);
		setComBoxDefaultColor(m_ui.bidLineColorBox, m_priceSettings.m_bidLineColor);
		setComBoxDefaultColor(m_ui.askLineColorBox, m_priceSettings.m_askLineColor);
		setComBoxDefaultColor(m_ui.crossStarColorBox, m_priceSettings.m_crossStarColor);
		setComBoxDefaultColor(m_ui.lineChartColorBox, m_priceSettings.m_lineChartColor);
		setComBoxDefaultColor(m_ui.volumesColorBox, m_priceSettings.m_volumesColor);
		setComBoxDefaultColor(m_ui.gridColorBox, m_commonSettings.m_gridColor);
		setComBoxDefaultColor(m_ui.barUpColorBox, m_priceSettings.m_barUpColor);
		setComBoxDefaultColor(m_ui.openOrderColorBox, m_priceSettings.m_openOrderLineColor);
		setComBoxDefaultColor(m_ui.pendingOrderColorBox, m_priceSettings.m_pendingOrderLineColor);
        setComBoxDefaultColor(m_ui.productCodeComboBox, m_commonSettings.m_productCodeColor);
	}

	void PriceDialog::setComBoxDefaultColor(QComboBox* cbb, const QColor& color)
	{
		if (cbb == NULL) return;
		int i = cbb->findData(color);
		if (i == -1)
		{
			i = 0;
			QPixmap pix_color(cbb->size() / 2);
			pix_color.fill(color);
			cbb->insertItem(i, QIcon(pix_color), "Custom", color);
			cbb->setIconSize(cbb->size() / 2);
			cbb->setSizeAdjustPolicy(QComboBox::AdjustToContents);
		}
		cbb->setCurrentIndex(i);
	}

	//const CommonSettings& PriceDialog::getCommonSettings() const
	//{
	//	return m_commonSettings;
	//}

	//const PriceSettings& PriceDialog::getPriceSettings() const
	//{
	//	return m_priceSettings;
	//}

	void PriceDialog::accept()
	{
        bool colorChange = false;
        if(m_commonSettings.m_background != m_ui.backgroundColorBox->currentData().value<QColor>()
          || m_commonSettings.m_foreground != m_ui.foregroundColorBox->currentData().value<QColor>()
          || m_commonSettings.m_gridColor != m_ui.gridColorBox->currentData().value<QColor>()
          || m_commonSettings.m_productCodeColor != m_ui.productCodeComboBox->currentData().value<QColor>()
          || m_priceSettings.m_barDownColor != m_ui.barDownColorBox->currentData().value<QColor>()
          || m_priceSettings.m_barUpColor != m_ui.barUpColorBox->currentData().value<QColor>()
          || m_priceSettings.m_bearCandleColor != m_ui.bearCandleColorBox->currentData().value<QColor>()
          || m_priceSettings.m_bullCandleColor != m_ui.bullCandleColorBox->currentData().value<QColor>()
          || m_priceSettings.m_bidLineColor != m_ui.bidLineColorBox->currentData().value<QColor>()
          || m_priceSettings.m_askLineColor != m_ui.askLineColorBox->currentData().value<QColor>()
          || m_priceSettings.m_crossStarColor != m_ui.crossStarColorBox->currentData().value<QColor>()
          || m_priceSettings.m_lineChartColor != m_ui.lineChartColorBox->currentData().value<QColor>()
          || m_priceSettings.m_volumesColor != m_ui.volumesColorBox->currentData().value<QColor>()
          || m_priceSettings.m_openOrderLineColor != m_ui.openOrderColorBox->currentData().value<QColor>()
          || m_priceSettings.m_pendingOrderLineColor != m_ui.pendingOrderColorBox->currentData().value<QColor>())
            colorChange = true;

		m_commonSettings.m_background = m_ui.backgroundColorBox->currentData().value<QColor>();
		m_commonSettings.m_foreground = m_ui.foregroundColorBox->currentData().value<QColor>();
		m_commonSettings.m_gridColor = m_ui.gridColorBox->currentData().value<QColor>();
        m_commonSettings.m_productCodeColor = m_ui.productCodeComboBox->currentData().value<QColor>();
		m_commonSettings.m_isAutoScroll = m_ui.autoScrollCheckBox->isChecked();
		m_commonSettings.m_isShiftData = m_ui.chartShiftCheckBox->isChecked();
		m_commonSettings.m_isShowGrid = m_ui.showGridCheckBox->isChecked();
		m_commonSettings.m_isShowLabelInfo = m_ui.showLableInfoCheckBox->isChecked();
		m_commonSettings.m_zoomLevel = getElementZoomRatio();

		m_priceSettings.m_barDownColor = m_ui.barDownColorBox->currentData().value<QColor>();
		m_priceSettings.m_barUpColor = m_ui.barUpColorBox->currentData().value<QColor>();
		m_priceSettings.m_bearCandleColor = m_ui.bearCandleColorBox->currentData().value<QColor>();
		m_priceSettings.m_bullCandleColor = m_ui.bullCandleColorBox->currentData().value<QColor>();
		m_priceSettings.m_drawType = getPriceDrawType();
		m_priceSettings.m_isShowBidPriceLine = m_ui.showBidLineCheckBox->isChecked();
		m_priceSettings.m_isShowAskPriceLine = m_ui.showAskLinecheckBox->isChecked();
		m_priceSettings.m_isShowVolume = m_ui.showVolumeCheckBox->isChecked();
		m_priceSettings.m_bidLineColor = m_ui.bidLineColorBox->currentData().value<QColor>();
		m_priceSettings.m_askLineColor = m_ui.askLineColorBox->currentData().value<QColor>();
		m_priceSettings.m_crossStarColor = m_ui.crossStarColorBox->currentData().value<QColor>();
		m_priceSettings.m_lineChartColor = m_ui.lineChartColorBox->currentData().value<QColor>();
		m_priceSettings.m_volumesColor = m_ui.volumesColorBox->currentData().value<QColor>();
		m_priceSettings.m_openOrderLineColor = m_ui.openOrderColorBox->currentData().value<QColor>();
		m_priceSettings.m_pendingOrderLineColor = m_ui.pendingOrderColorBox->currentData().value<QColor>();

        if(colorChange)
            emit UserSetChartColorSignSignal();

		return QDialog::accept();
	}

	enPriceType PriceDialog::getPriceDrawType() const
	{
		if(m_ui.barButton->isChecked())
		{
			return enPriceBar;
		}
		else if(m_ui.lineChartButton->isChecked())
		{
			return enPriceLine;
		}
		else
		{
			return enPriceCandlesticks;
		}
	}

	ZoomLevel PriceDialog::getElementZoomRatio() const
	{
		int value = m_ui.scaleSlider->value();
		for(int i = zoom_minimal; i < zoom_unknown; i<<=1)
		{
			if(i >= value)
			{
				value = i;
				break;
			}
		}
		return ZoomLevel(value);
	}
}


#include "moc_priceDialog.cpp"
