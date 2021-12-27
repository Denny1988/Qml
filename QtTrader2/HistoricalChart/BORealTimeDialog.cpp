#include "BORealTimeDialog.h"
#include "SR.h"
#include <QColorDialog>
#include <QPushButton>

namespace HistoricalChartSpace
{
	BORealTimeDialog::BORealTimeDialog(const BORealTimePriceSettings& settings, QWidget *parent)
		:QDialog(parent), m_settings(settings)
	{
		m_ui.setupUi(this);
		this->setFixedSize(this->size());
		this->applyStringResource();
		init();
	}
	BORealTimeDialog::~BORealTimeDialog()
	{
	}

	const BORealTimePriceSettings& BORealTimeDialog::getSettings() const
	{
		return m_settings;
	}

	void BORealTimeDialog::applyStringResource()
	{
		this->setWindowTitle(SR::get("FavoriteSettings"));
		m_ui.okButton->setText(SR::get("OK"));
		m_ui.cancelButton->setText(SR::get("Cancel"));
		m_ui.showGroupBox->setTitle(SR::get("Show"));
		m_ui.gridCheckBox->setText(SR::get("ShowGrid"));
		m_ui.barStrokeCheckBox->setText(SR::get("BarStroke"));
		m_ui.crosshairCheckBox->setText(SR::get("Crosshair"));

		m_ui.colorsGroupBox->setTitle(SR::get("Colors"));
		m_ui.gridLabel->setText(SR::get("Grid"));
		m_ui.fgLabel->setText(SR::get("Foreground:"));
		m_ui.bgLabel->setText(SR::get("Background:"));
		m_ui.coorBgLabel->setText(SR::get("CoorBackground:"));
		m_ui.barStrokeLabel->setText(SR::get("BarStroke:"));
		m_ui.crosshairLabel->setText(SR::get("Crosshair:"));
		m_ui.bullishLabel->setText(SR::get("Bullish:"));
		m_ui.bearishLabel->setText(SR::get("Bearish:"));
	}

	void BORealTimeDialog::init()
	{
        //Initialize checkBox
		if (m_settings.m_bDrawGrid)
		{
			m_ui.gridCheckBox->setCheckState(Qt::Checked);
		}

		if (m_settings.m_bDrawBarStroke)
		{
			m_ui.barStrokeCheckBox->setCheckState(Qt::Checked);
		}

		if (m_settings.m_bDrawCrosshair)
		{
			m_ui.crosshairCheckBox->setCheckState(Qt::Checked);
		}

        //Initialize color
		QString str = "background-color:";
		m_ui.barStrokeColorBT->setStyleSheet(str + m_settings.m_colorBarStroke.name());
		m_ui.crossColorBT->setStyleSheet(str + m_settings.m_colorCrosshair.name());
		m_ui.gridColorBT->setStyleSheet(str + m_settings.m_colorGrid.name());
		m_ui.fgColorBT->setStyleSheet(str + m_settings.m_colorForeground.name());
		m_ui.bgColorBT->setStyleSheet(str + m_settings.m_colorBackground.name());
		m_ui.coorBgColorBT->setStyleSheet(str + m_settings.m_coordinateBackground.name());
		m_ui.bullishColorBT->setStyleSheet(str + m_settings.m_colorBullish.name());
		m_ui.bearishColorBT->setStyleSheet(str + m_settings.m_colorBearish.name());

		connect(m_ui.barStrokeColorBT, SIGNAL(clicked()), SLOT(changeColor()));
		connect(m_ui.crossColorBT, SIGNAL(clicked()), SLOT(changeColor()));
		connect(m_ui.gridColorBT, SIGNAL(clicked()), SLOT(changeColor()));
		connect(m_ui.fgColorBT, SIGNAL(clicked()), SLOT(changeColor()));
		connect(m_ui.bgColorBT, SIGNAL(clicked()), SLOT(changeColor()));
		connect(m_ui.coorBgColorBT, SIGNAL(clicked()), SLOT(changeColor()));
		connect(m_ui.bullishColorBT, SIGNAL(clicked()), SLOT(changeColor()));
		connect(m_ui.bearishColorBT, SIGNAL(clicked()), SLOT(changeColor()));
	}

	void BORealTimeDialog::accept()
	{
		m_settings.m_bDrawBarStroke = m_ui.barStrokeCheckBox->isChecked();
		m_settings.m_bDrawCrosshair = m_ui.crosshairCheckBox->isChecked();
		m_settings.m_bDrawGrid = m_ui.gridCheckBox->isChecked();
		const QString& str = "background-color:";
		m_settings.m_colorBarStroke = m_ui.barStrokeColorBT->styleSheet().section(str,1).section(';',0,1).trimmed();
		m_settings.m_colorCrosshair = m_ui.crossColorBT->styleSheet().section(str,1).section(';',0,1).trimmed();
		m_settings.m_colorGrid = m_ui.gridColorBT->styleSheet().section(str,1).section(';',0,1).trimmed();
		m_settings.m_colorForeground = m_ui.fgColorBT->styleSheet().section(str,1).section(';',0,1).trimmed();
		m_settings.m_colorBackground = m_ui.bgColorBT->styleSheet().section(str, 1).section(';', 0, 1).trimmed();
		m_settings.m_coordinateBackground = m_ui.coorBgColorBT->styleSheet().section(str, 1).section(';', 0, 1).trimmed();
		m_settings.m_colorBullish = m_ui.bullishColorBT->styleSheet().section(str,1).section(';',0,1).trimmed();
		m_settings.m_colorBearish = m_ui.bearishColorBT->styleSheet().section(str,1).section(';',0,1).trimmed();
		return QDialog::accept();
	}

	void BORealTimeDialog::changeColor()
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

#include "moc_BORealTimeDialog.cpp"
