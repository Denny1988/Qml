#include "IndicatorDialog.h"
#include "SR.h"
#include "SC.h"
#include "Indicator/Indicator.h"
#include <QDebug>

namespace HistoricalChartSpace
{

	void IndicatorDialog::ShowDialog(Indicator* indicator, QWidget* parent)
	{
		const IndicatorSettings oldSettings = indicator->getIndicatorSettings();
		IndicatorDialog dlg(indicator, parent);
		dlg.exec();
		if (dlg.result() == QDialog::Accepted)
		{
            // Change the current indicator
			const IndicatorSettings& settings = dlg.getIndicatorSettings();

            // Save indicator configuration information
			indicator->setIndicatorSettings(settings);

            // Determine whether to recalculate
			bool bNeedCalulate = settings.m_dependency != oldSettings.m_dependency || settings.m_maType != oldSettings.m_maType;
			if (!bNeedCalulate)
			{
				QHash<QString, int>::const_iterator it = settings.m_hashInputIntData.begin();
				for (it; it != settings.m_hashInputIntData.end(); ++it)
				{
					if (it.value() != oldSettings.m_hashInputIntData[it.key()])
					{
						bNeedCalulate = true;
						break;
					}
				}

				if (!bNeedCalulate)
				{
					QHash<QString, double>::const_iterator it2 = settings.m_hashInputDoubleData.begin();
					for (it2; it2 != settings.m_hashInputDoubleData.end(); ++it2)
					{
						if (it2.value() != oldSettings.m_hashInputDoubleData[it2.key()])
						{
							bNeedCalulate = true;
							break;
						}
					}
				}
			}

			if (bNeedCalulate)
			{
				indicator->getCalculator()->beginCalculateAll(indicator);
			}
			indicator->getCalculator()->addEndTask();   // always call addEndTask() to refresh UI.
		}
	}

	IndicatorDialog::IndicatorDialog(Indicator* indicator, QWidget *parent) :QDialog(parent), m_indicator(indicator)
	{
		m_settings = indicator->getIndicatorSettings();
		m_CPDCombox = NULL;
		m_maTypeCombox = NULL;
		init();
	}

	IndicatorDialog::~IndicatorDialog()
	{
	}

	void IndicatorDialog::init()
	{
		this->setWindowTitle(m_settings.m_title);

		QVBoxLayout* verticalLayout = new QVBoxLayout(this);
		 
		// CPD
		if(m_settings.m_dependency != enCPDUnknown)
		{
			QLabel* label = new QLabel(SR::get("Dependency") + ": ", this);
			m_CPDCombox = new QComboBox(this);
            // Arranged in order of enum
            m_CPDCombox->addItems(QStringList()
                                  << SR::get(SC::Open)
                                  << SR::get("ClosePrice")
                                  << SR::get(SC::High)
                                  << SR::get(SC::Low)
                                  << SR::get("Median(HL/2)")
                                  << SR::get("Typical(HLC/3)")
                                  << SR::get("WeightedClose(HLCC/4)"));
			m_CPDCombox->setCurrentIndex(m_settings.m_dependency);
			label->setBuddy(m_CPDCombox);

			QHBoxLayout* horizentalLayout = new QHBoxLayout(this);
			horizentalLayout->addWidget(label);
			horizentalLayout->addWidget(m_CPDCombox);
			horizentalLayout->setStretch(0,0);
			horizentalLayout->setStretch(1,0);
			verticalLayout->addItem(horizentalLayout);
		}

		// input Parameter
        //int type parameter
		{
			QHash<QString,int>::const_iterator it = m_settings.m_hashInputIntData.begin();
			for(it; it != m_settings.m_hashInputIntData.end(); ++it)
			{
				QLabel* label = new QLabel(SR::get(it.key()) + ": ", this);
				InputLineEdit* subLineEdit = new InputLineEdit(QString::number(it.value()),this);
				subLineEdit->setDataString(it.key());
				int min, max;
				m_indicator->getParameterRange(it.key(), min, max);
                subLineEdit->setValidator(new QIntValidator(min, max, this));
                subLineEdit->setOldValue(QString::number(it.value()));
                QObject::connect(subLineEdit, &InputLineEdit::textChanged, this, [=] (const QString & value) {
                        int newValue = value.toInt();
                        if(newValue<min || newValue>max)
                        {
                            newValue = subLineEdit->getOldValue().toInt();
                            subLineEdit->setOldValue(QString::number(newValue));
                            subLineEdit->setText(QString::number(newValue));
                        }
                        else
                            subLineEdit->setOldValue(QString::number(newValue));
                    });
				label->setBuddy(subLineEdit);
				QHBoxLayout* horizentalLayout = new QHBoxLayout(this);
				horizentalLayout->addWidget(label);
				horizentalLayout->addWidget(subLineEdit);
				verticalLayout->addItem(horizentalLayout);
				m_vecIntInputLineEdit.append(subLineEdit);	
			}
		}

        // double type parameter
		{
			QHash<QString,double>::const_iterator it = m_settings.m_hashInputDoubleData.begin();
			for(it; it != m_settings.m_hashInputDoubleData.end(); ++it)
            {
                QLabel* label = new QLabel(SR::get(it.key()) + ": ",this);
				InputLineEdit* subLineEdit = new InputLineEdit(QString::number(it.value()),this);
				subLineEdit->setDataString(it.key());
				double min, max, decimals;
				m_indicator->getParameterRange(it.key(), min, max, decimals);
                subLineEdit->setValidator(new QDoubleValidator(min, max, decimals, this));
				label->setBuddy(subLineEdit);
				QHBoxLayout* horizentalLayout = new QHBoxLayout(this);
				horizentalLayout->addWidget(label);
				horizentalLayout->addWidget(subLineEdit);
				verticalLayout->addItem(horizentalLayout);
				m_vecDoubleInputLineEdit.append(subLineEdit);	
			}
		}



		// MATYPE
		if(m_settings.m_maType != LocalMAType_Unknown)
		{
			QLabel* label = new QLabel(SR::get("MAType") + ": ", this);
			m_maTypeCombox = new QComboBox(this);
            // Arranged in order of enum
#ifdef TA_Lib
			m_maTypeCombox->addItems(QStringList() << "Simple Moving Average" << "Exponential Moving Average" << "Weighted Moving Average" << "DEMA"
				<< "Time Series Moving Average" << "Triangular Moving Average" << "KAMA" << "MAMA" << "T3MA");
			m_maTypeCombox->setCurrentIndex(m_settings.m_maType);
#else
			m_maTypeCombox->addItems(QStringList()
                << SR::get("SimpleMovingAverage")
                << SR::get("ExponentialMovingAverage")
                << SR::get("TimeSeriesMovingAverage")
                << SR::get("VariableMovingAverage")
                << SR::get("TriangularMovingAverage")
                << SR::get("WeightedMovingAverage")
				);
			m_maTypeCombox->setCurrentIndex(m_settings.m_maType - 1);
#endif
			label->setBuddy(m_maTypeCombox);

			QHBoxLayout* horizentalLayout = new QHBoxLayout(this);
			horizentalLayout->addWidget(label);
			horizentalLayout->addWidget(m_maTypeCombox);
			horizentalLayout->setStretch(0,0);
			horizentalLayout->setStretch(1,0);
			verticalLayout->addItem(horizentalLayout);
		}

		// color£¬lineWidth,lineType
		if (m_settings.isDataValid())
		{
			const QString& strBackground = "background-color:";
			for (int i = 0; i < m_settings.m_plotNum; ++i)
			{
				QString str = m_settings.m_vectLineSettings[i].m_lineName + " " + SR::get("Style") + ": ";
				QLabel* label = new QLabel(str,this);	
				QPushButton* colorButton = new QPushButton(this);
			
				colorButton->setStyleSheet(strBackground + m_settings.m_vectLineSettings[i].m_lineColor.name());
				connect(colorButton,SIGNAL(clicked()),this,SLOT(handleStandColorDialogSlot()));
			
				QComboBox*	 lineTypeCBB = new QComboBox(this);
			
                lineTypeCBB->addItem(SR::get("SolidLine"),(int)Qt::SolidLine);
                lineTypeCBB->addItem(SR::get("DashLine"),(int)Qt::DashLine);
                lineTypeCBB->addItem(SR::get("DotLine"),(int)Qt::DotLine);
                lineTypeCBB->addItem(SR::get("DashDotLine"),(int)Qt::DashDotLine);
                lineTypeCBB->addItem(SR::get("DashDotDotLine"),(int)Qt::DashDotDotLine);
				int indexType = lineTypeCBB->findData((int)m_settings.m_vectLineSettings[i].m_lineType);
				if(indexType != -1)
				{
					lineTypeCBB->setCurrentIndex(indexType);
				}

				QComboBox*	 lineWidthCBB = new QComboBox(this);

				lineWidthCBB->addItem("1.0",1.0);
				lineWidthCBB->addItem("1.5",1.5);
				lineWidthCBB->addItem("2.0",2.0);
				lineWidthCBB->addItem("3.0",3.0);
				lineWidthCBB->addItem("5.0",5.0);

				int indexWidth = lineWidthCBB->findData(m_settings.m_vectLineSettings[i].m_lineWidth);
				if(indexWidth != -1)
				{
					lineWidthCBB->setCurrentIndex(indexWidth);
				}
				

				QHBoxLayout* horizentalLayout = new QHBoxLayout(this);
				horizentalLayout->addWidget(label);
				horizentalLayout->addWidget(colorButton);
				horizentalLayout->addWidget(lineTypeCBB);
				horizentalLayout->addWidget(lineWidthCBB);
				verticalLayout->addItem(horizentalLayout);

				m_vecColorPushButton.push_back(colorButton);
				m_vecLineTypeCBB.push_back(lineTypeCBB);
				m_vecLineWidthCBB.push_back(lineWidthCBB);
			
			}
		}

		// ok,cancel
		{
			QSpacerItem* spacerItem = new QSpacerItem(231, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);
			m_okButton = new QPushButton(SR::get("OK"),this);
			m_cancelButton = new QPushButton(SR::get("Cancel"), this);

			connect(m_okButton,SIGNAL(clicked()),this,SLOT(accept()));
			connect(m_cancelButton,SIGNAL(clicked()),this,SLOT(reject()));

			QHBoxLayout* horizentalLayout = new QHBoxLayout(this);
			horizentalLayout->addItem(spacerItem);
			horizentalLayout->addWidget(m_okButton);
			horizentalLayout->addWidget(m_cancelButton);
			verticalLayout->addItem(horizentalLayout);
		}


		this->setLayout(verticalLayout);

	}

	void IndicatorDialog::accept()
	{
		if(m_CPDCombox != NULL)
		{
			m_settings.m_dependency = (enCPD)m_CPDCombox->currentIndex();
		}

		if(m_maTypeCombox != NULL)
		{
#ifdef TA_Lib
			m_settings.m_maType = (LocalMAType) m_maTypeCombox->currentIndex();
#else
			m_settings.m_maType = (LocalMAType)(m_maTypeCombox->currentIndex() + 1);
#endif
		}

		for(int i = 0; i < m_vecIntInputLineEdit.size(); ++i)
		{
			InputLineEdit* subLineEdit = m_vecIntInputLineEdit[i];
			if(subLineEdit == NULL) continue;

			m_settings.m_hashInputIntData[subLineEdit->getDataString()] = subLineEdit->text().toInt();
		}

	
		for(int i = 0; i < m_vecDoubleInputLineEdit.size(); ++i)
		{
			InputLineEdit* subLineEdit = m_vecDoubleInputLineEdit[i];
			if(subLineEdit == NULL) continue;

			m_settings.m_hashInputDoubleData[subLineEdit->getDataString()] = subLineEdit->text().toDouble();
		}

		if (m_settings.isDataValid() &&
			m_vecColorPushButton.size() == m_settings.m_plotNum &&
			m_vecLineTypeCBB.size() == m_settings.m_plotNum &&
			m_vecLineWidthCBB.size() == m_settings.m_plotNum)
		{
			const QString& strBackground = "background-color:";
			for (int i = 0; i < m_settings.m_plotNum; ++i)
			{
				m_settings.m_vectLineSettings[i].m_lineColor = m_vecColorPushButton[i]->styleSheet().section(strBackground, 1).section(';', 0, 1).trimmed();
				m_settings.m_vectLineSettings[i].m_lineType = (Qt::PenStyle)m_vecLineTypeCBB[i]->currentData().toInt();
				m_settings.m_vectLineSettings[i].m_lineWidth = m_vecLineWidthCBB[i]->currentData().toDouble();
			}
		}

		m_settings.updateTitle();
		return QDialog::accept();
	}

	void IndicatorDialog::handleStandColorDialogSlot()
	{
		QPushButton * button = dynamic_cast<QPushButton*>(sender());
		if(button == NULL) return;

		const QString& strBackground = "background-color:";
		QColor oldColor = button->styleSheet().section(strBackground,1).section(';',0,1).trimmed();

		QColor color = QColorDialog::getColor(oldColor,this);

		if(color.isValid())
		{
		
			button->setStyleSheet(strBackground + color.name());

		}
	}

	const IndicatorSettings& IndicatorDialog::getIndicatorSettings() const
	{
		return m_settings;
	}


}

#include "moc_IndicatorDialog.cpp"
