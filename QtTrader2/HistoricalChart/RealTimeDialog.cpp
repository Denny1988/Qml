#include "RealTimeDialog.h"
#include "SR.h"
#include <QColorDialog>

namespace HistoricalChartSpace
{
	RealTimeDialog::RealTimeDialog(const RealTimePriceSettings& settings, QWidget *parent)
		:QDialog(parent), m_settings(settings)
	{
		m_ui.setupUi(this);
		this->setFixedSize(this->size());
		this->applyStringResource();
		init();
	}
	RealTimeDialog::~RealTimeDialog()
	{
	}

	const RealTimePriceSettings& RealTimeDialog::getSettings() const
	{
		return m_settings;
	}

	void RealTimeDialog::applyStringResource()
	{
		this->setWindowTitle(SR::get("FavoriteSettings"));
		m_ui.okButton->setText(SR::get("OK"));
		m_ui.cancelButton->setText(SR::get("Cancel"));
		m_ui.showGroupBox->setTitle(SR::get("Show"));
		m_ui.gridCheckBox->setText(SR::get("ShowGrid"));
		m_ui.bidCheckBox->setText(SR::get("Bid"));
		m_ui.askCheckBox->setText(SR::get("Ask"));
		m_ui.crosshairCheckBox->setText(SR::get("Crosshair"));

		m_ui.colorsGroupBox->setTitle(SR::get("Colors"));
		m_ui.gridLabel->setText(SR::get("Grid"));
		m_ui.fgLabel->setText(SR::get("Foreground:"));
		m_ui.bgLabel->setText(SR::get("Background:"));
		m_ui.coorBgLabel->setText(SR::get("CoorBackground:"));
		m_ui.bidLabel->setText(SR::get("Bid:"));
		m_ui.askLabel->setText(SR::get("Ask:"));
		m_ui.crosshairLabel->setText(SR::get("Crosshair:"));
	}

	void RealTimeDialog::init()
	{
        //Initialize checkBox
		if (m_settings.m_bDrawGrid)
		{
			m_ui.gridCheckBox->setCheckState(Qt::Checked);
		}

		if (m_settings.m_bDrawAsk)
		{
			m_ui.askCheckBox->setCheckState(Qt::Checked);
		}

		if (m_settings.m_bDrawbid)
		{
			m_ui.bidCheckBox->setCheckState(Qt::Checked);
		}

		if (m_settings.m_bDrawCrosshair)
		{
			m_ui.crosshairCheckBox->setCheckState(Qt::Checked);
		}

        //Initialize color
		QString str = "background-color:";
		m_ui.askColorBT->setStyleSheet(str + m_settings.m_colorAsk.name());
		m_ui.bidColorBT->setStyleSheet(str + m_settings.m_colorBid.name());
		m_ui.crossColorBT->setStyleSheet(str + m_settings.m_colorCrosshair.name());
		m_ui.gridColorBT->setStyleSheet(str + m_settings.m_colorGrid.name());
		m_ui.fgColorBT->setStyleSheet(str + m_settings.m_colorForeground.name());
		m_ui.bgColorBT->setStyleSheet(str + m_settings.m_colorBackground.name());
		m_ui.coorBgColorBT->setStyleSheet(str + m_settings.m_coordinateBackground.name());

		connect(m_ui.askColorBT, SIGNAL(clicked()), SLOT(changeColor()));
		connect(m_ui.bidColorBT, SIGNAL(clicked()), SLOT(changeColor()));
		connect(m_ui.crossColorBT, SIGNAL(clicked()), SLOT(changeColor()));
		connect(m_ui.gridColorBT, SIGNAL(clicked()), SLOT(changeColor()));
		connect(m_ui.fgColorBT, SIGNAL(clicked()), SLOT(changeColor()));
		connect(m_ui.bgColorBT, SIGNAL(clicked()), SLOT(changeColor()));
		connect(m_ui.coorBgColorBT, SIGNAL(clicked()), SLOT(changeColor()));
	}

	void RealTimeDialog::accept()
	{
        bool userSetRealTimeChartColorSign = false;
        const QString& str = "background-color:";
        if(m_settings.m_colorAsk != m_ui.askColorBT->styleSheet().section(str,1).section(';',0,1).trimmed()
                || m_settings.m_colorBid != m_ui.bidColorBT->styleSheet().section(str,1).section(';',0,1).trimmed()
                || m_settings.m_colorCrosshair != m_ui.crossColorBT->styleSheet().section(str,1).section(';',0,1).trimmed()
                || m_settings.m_colorGrid != m_ui.gridColorBT->styleSheet().section(str,1).section(';',0,1).trimmed()
                || m_settings.m_colorForeground != m_ui.fgColorBT->styleSheet().section(str,1).section(';',0,1).trimmed()
                || m_settings.m_colorBackground != m_ui.bgColorBT->styleSheet().section(str, 1).section(';', 0, 1).trimmed()
                || m_settings.m_coordinateBackground != m_ui.coorBgColorBT->styleSheet().section(str, 1).section(';', 0, 1).trimmed())
        {
            userSetRealTimeChartColorSign = true;
        }
		m_settings.m_bDrawAsk = m_ui.askCheckBox->isChecked();
		m_settings.m_bDrawbid = m_ui.bidCheckBox->isChecked();
		m_settings.m_bDrawCrosshair = m_ui.crosshairCheckBox->isChecked();
		m_settings.m_bDrawGrid = m_ui.gridCheckBox->isChecked();

		m_settings.m_colorAsk = m_ui.askColorBT->styleSheet().section(str,1).section(';',0,1).trimmed();
		m_settings.m_colorBid = m_ui.bidColorBT->styleSheet().section(str,1).section(';',0,1).trimmed();
		m_settings.m_colorCrosshair = m_ui.crossColorBT->styleSheet().section(str,1).section(';',0,1).trimmed();
		m_settings.m_colorGrid = m_ui.gridColorBT->styleSheet().section(str,1).section(';',0,1).trimmed();
		m_settings.m_colorForeground = m_ui.fgColorBT->styleSheet().section(str,1).section(';',0,1).trimmed();
		m_settings.m_colorBackground = m_ui.bgColorBT->styleSheet().section(str, 1).section(';', 0, 1).trimmed();
		m_settings.m_coordinateBackground = m_ui.coorBgColorBT->styleSheet().section(str, 1).section(';', 0, 1).trimmed();

        if(userSetRealTimeChartColorSign)
            emit UserSetRealTimeChartColorSignSignal();
		return QDialog::accept();
	}

	void RealTimeDialog::changeColor()
	{
		QPushButton * button = dynamic_cast<QPushButton*>(sender());
		if (button == NULL) return;

		QColor oldColor = button->styleSheet().section("background-color:", 1).section(';', 0, 1).trimmed();
		QColor color = QColorDialog::getColor(oldColor, this);

		if (color.isValid())
		{
			button->setStyleSheet("background-color:" + color.name());
		}
	}
}

#include "moc_RealTimeDialog.cpp"
