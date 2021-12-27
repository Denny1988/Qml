#include "DrawingObjectDialog.h"
#include "SR.h"
#include <QColorDialog>
#include "JulianDate.h"
#include <QDateTimeEdit>
#include <QStandardItem>
#include <QStandardItemModel>
#include "Helper.h"
#include <QFontDatabase>

namespace HistoricalChartSpace
{
	DrawingObjectDialog::DrawingObjectDialog(const DrawingObjectSettings& settings, HistoricalChart *view)
		:QDialog(view->getDialogParent()),
		m_settings(settings),
		m_isTheSameTimeSignal(false),
		m_leftRayCB(NULL),
		m_rightRayCB(NULL),
		m_singleRayCB(NULL),
		m_fillShapeCB(NULL),
		m_pView(view)
	{
		m_ui.setupUi(this);
		this->setFixedSize(this->size());
		this->applyStringResource();
		init();
	}

	DrawingObjectDialog::~DrawingObjectDialog()
	{
	}

	void DrawingObjectDialog::applyStringResource()
	{
		this->setWindowTitle(SR::get("DrawingObjectProperty"));
		m_ui.okButton->setText(SR::get("OK"));
		m_ui.cancelButton->setText(SR::get("Cancel"));
		m_ui.tabWidget->setTabText(0, SR::get("Common"));
		m_ui.tabWidget->setTabText(1, SR::get("Levels"));
		m_ui.tabWidget->setTabText(2, SR::get("Parameter"));
		m_ui.tabWidget->setTabText(3, SR::get("Visualization"));
		m_ui.nameLabel->setText(SR::get("Name"));
		m_ui.descriptionLabel->setText(SR::get("Description"));
		m_ui.styleLabel->setText(SR::get("Style"));
		m_ui.asBackgroundCB->setText(SR::get("AsBackground"));
		m_ui.disableSelectCB->setText(SR::get("DisableSelect"));
		m_ui.addButton->setText(SR::get("Add"));
		m_ui.delButton->setText(SR::get("Delete"));
		m_ui.editButton->setText(SR::get("Edit"));
		m_ui.defaultButton->setText(SR::get("Default"));
		m_ui.allTimeFramesCB->setText(SR::get("AllTimeFrames"));
	}

	void DrawingObjectDialog::init()
	{
		initCommonWidget();
		initLevelsWidget();
		initParameterWidget();
		initVisualizationWidget();
	}
	
	const DrawingObjectSettings& DrawingObjectDialog::getSettings() const
	{
		return m_settings;
	}

	void DrawingObjectDialog::accept()
	{
		do 
		{
			if(m_settings.m_vecDate.size() != m_vecTimeEdit.size()) break;
			if(m_settings.m_vecValue.size() != m_vecLineEdit.size()) break;
			acceptCommonWidget();
			acceptLevelsWidget();
			acceptParameterWidget();
			acceptVisualizationWidget();
			
		} while (false);

		return QDialog::accept();
	}
	
	void DrawingObjectDialog::initCommonWidget()
	{
		m_ui.nameLineEdit->setText(m_settings.m_name);
		m_ui.DesLineEdit->setText(m_settings.m_describe);

		initColorLine(m_ui.colorButton,m_ui.lineTypeCB,m_ui.lineWCB);

		const QString& strBackground = "background-color:";
		m_ui.colorButton->setStyleSheet(strBackground + m_settings.m_colorLine.name());
	
		int indexType = m_ui.lineTypeCB->findData(m_settings.m_intLineType);
		if(indexType != -1)
		{
			m_ui.lineTypeCB->setCurrentIndex(indexType);
		}

		int indexWidth = m_ui.lineWCB->findData(m_settings.m_lfLineWidth);
		if(indexWidth != -1)
		{
			m_ui.lineWCB->setCurrentIndex(indexWidth);
		}

		m_ui.asBackgroundCB->setChecked(m_settings.m_bAsBackground);
		m_ui.disableSelectCB->setChecked(!m_settings.m_bSelectAble);
	}

	void DrawingObjectDialog::initLevelsWidget()
	{
		if (!m_settings.m_bhasLevels)
		{
			m_ui.tabWidget->removeTab(m_ui.tabWidget->indexOf(m_ui.LevelsTab));
			return;
		}

		QMap<double, QString>& mapLevels = m_settings.m_mapLevels;
		QStandardItemModel* model = new QStandardItemModel(m_ui.levelsView);
		model->setHorizontalHeaderLabels(QStringList() << SR::get("Levels") << SR::get("Description"));
		m_ui.levelsView->horizontalHeader()->setStretchLastSection(true);
		m_ui.levelsView->verticalHeader()->setHidden(true);

		foreach(double value, mapLevels.keys())
		{
			QStandardItem* item = new QStandardItem(QString::number(value));
			QStandardItem* itemDsp = new QStandardItem(mapLevels[value]);
			model->appendRow(QList<QStandardItem*>() << item << itemDsp);
		}

		m_ui.levelsView->setModel(model);

		connect(m_ui.addButton, SIGNAL(clicked()), this, SLOT(addRowSlot()));
		connect(m_ui.delButton, SIGNAL(clicked()), this, SLOT(deleteRowSlot()));
		connect(m_ui.editButton, SIGNAL(clicked()), this, SLOT(editRowSlot()));
		connect(m_ui.defaultButton, SIGNAL(clicked()), this, SLOT(defaultRowSlot()));

		connect(m_ui.levelsView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(enableSwitchSlot(const QModelIndex &)));
        // initialization
		m_bButtonState = false;
		m_ui.delButton->setEnabled(false);
		m_ui.editButton->setEnabled(false);

        // Initialize color line type
		initColorLine(m_ui.colorLevelPB, m_ui.lineTypeLevelCBB, m_ui.lineWidthLevelCBB);

        // Specific initialization
		const QString& strBackground = "background-color:";
		m_ui.colorLevelPB->setStyleSheet(strBackground + m_settings.m_levelsColorLine.name());

		int indexType = m_ui.lineTypeLevelCBB->findData(m_settings.m_levelsLineType);
		if (indexType != -1)
		{
			m_ui.lineTypeLevelCBB->setCurrentIndex(indexType);
		}

		int indexWidth = m_ui.lineWidthLevelCBB->findData(m_settings.m_levelsLineWidth);
		if (indexWidth != -1)
		{
			m_ui.lineWidthLevelCBB->setCurrentIndex(indexWidth);
		}
	}

	void DrawingObjectDialog::initParameterWidget()
	{
		QVBoxLayout* verticalLayout = new QVBoxLayout(m_ui.parameterTab);
		int dataSize = m_settings.m_vecDate.size();
		for(int i = 0; i < m_settings.m_vecDate.size(); ++i)
		{
			QString str;
			if (dataSize == 1)
			{
				str = SR::get("Date") + ":";
			}
			else
			{
				str = SR::get("Date") + QString::number(i) + ":";
			}
			QLabel* label = new QLabel(str, m_ui.parameterTab);

			QDateTime dateTime = JulianDate::JDateToQTDate(m_settings.m_vecDate[i]);
			QDateTimeEdit* timeEdit = new QDateTimeEdit(dateTime,m_ui.parameterTab);

			QHBoxLayout* horizentalLayout = new QHBoxLayout(m_ui.parameterTab);
			horizentalLayout->addWidget(label,0);
			horizentalLayout->addWidget(timeEdit,1);
			verticalLayout->addItem(horizentalLayout);
			m_vecTimeEdit.append(timeEdit);
		}
		for (int i = 0; i < m_settings.m_vecValue.size(); ++i)
		{
			QString str;
			if (dataSize == 1)
			{
				str = SR::get("Value") + ":";
			}
			else
			{
				str = SR::get("Value") + QString::number(i) + ":";
			}
			QLabel* label = new QLabel(str, m_ui.parameterTab);

			//str.sprintf("%.03lf", m_settings.m_vecValue[i]);
			str.setNum(m_settings.m_vecValue[i], 'f', m_pView->getDecimalPlaces());
			QLineEdit* lineEdit = new QLineEdit(str, m_ui.parameterTab);
			lineEdit->setValidator(new QDoubleValidator());

			QHBoxLayout* horizentalLayout = new QHBoxLayout(m_ui.parameterTab);
			horizentalLayout->addWidget(label, 0);
			horizentalLayout->addWidget(lineEdit, 1);
			verticalLayout->addItem(horizentalLayout);
			m_vecLineEdit.append(lineEdit);
		}

		if(m_settings.m_intRayFlags & RayShow_Left)
		{
			m_leftRayCB = new QCheckBox(SR::get("LeftRay"),m_ui.parameterTab);
			Qt::CheckState state = m_settings.m_intRayFlags & RayShow_LeftSelected ? Qt::Checked : Qt::Unchecked;
			m_leftRayCB->setCheckState(state);
			QHBoxLayout* horizentalLayout = new QHBoxLayout(m_ui.parameterTab);
			horizentalLayout->addWidget(m_leftRayCB);
			verticalLayout->addItem(horizentalLayout);
		}

		if(m_settings.m_intRayFlags & RayShow_Right)
		{
			m_rightRayCB = new QCheckBox(SR::get("RightRay"),m_ui.parameterTab);
			Qt::CheckState state = m_settings.m_intRayFlags & RayShow_RightSelected ? Qt::Checked : Qt::Unchecked;
			m_rightRayCB->setCheckState(state);
			QHBoxLayout* horizentalLayout = new QHBoxLayout(m_ui.parameterTab);
			horizentalLayout->addWidget(m_rightRayCB);
			verticalLayout->addItem(horizentalLayout);
		}

		if(m_settings.m_intRayFlags & RayShow_Single)
		{
			m_singleRayCB = new QCheckBox(SR::get("Ray"),m_ui.parameterTab);
			Qt::CheckState state = m_settings.m_intRayFlags & RayShow_SingleSelected ? Qt::Checked : Qt::Unchecked;
			m_singleRayCB->setCheckState(state);
			QHBoxLayout* horizentalLayout = new QHBoxLayout(m_ui.parameterTab);
			horizentalLayout->addWidget(m_singleRayCB);
			verticalLayout->addItem(horizentalLayout);
		}

		if(m_settings.m_intFillShape & brushToFillShape_NeedBrush)
		{
			m_fillShapeCB = new QCheckBox(SR::get("Fill"),m_ui.parameterTab);
			Qt::CheckState state = m_settings.m_intFillShape & brushToFillShape_SetBrush ? Qt::Checked :Qt::Unchecked;
			m_fillShapeCB->setCheckState(state);
			QHBoxLayout* horizentalLayout = new QHBoxLayout(m_ui.parameterTab);
			horizentalLayout->addWidget(m_fillShapeCB);
			verticalLayout->addItem(horizentalLayout);
		}

		if (m_settings.m_type == enDrawingObjectText)
		{
			QLabel* label = new QLabel(SR::get("Angleindegrees") + ":", m_ui.parameterTab);
			m_angleEdit = new QLineEdit(QString::number(m_settings.m_angle), m_ui.parameterTab);
			m_angleEdit->setValidator(new QDoubleValidator());

			QHBoxLayout* horizentalLayout = new QHBoxLayout(m_ui.parameterTab);
			horizentalLayout->addWidget(label, 0);
			horizentalLayout->addWidget(m_angleEdit, 1);
			verticalLayout->addItem(horizentalLayout);

			label = new QLabel(SR::get("Font") + ":", m_ui.parameterTab);
			m_fontFamilyComboBox = new QComboBox(m_ui.parameterTab);
			m_fontSizeComboBox = new QComboBox(m_ui.parameterTab);
			QFontDatabase fontDatabase;
			m_fontFamilyComboBox->addItems(fontDatabase.families());
			QList<int> fontSizes = QFontDatabase::standardSizes();
			for (int i = 0; i < fontSizes.count(); i++)
			{
				m_fontSizeComboBox->addItem(QString::number(fontSizes[i]));
			}

			// apply settings
			int index = m_fontFamilyComboBox->findText(m_settings.m_fontFamily);
			if (index < 0) index = 0;
			m_fontFamilyComboBox->setCurrentIndex(index);

			index = m_fontSizeComboBox->findText(QString::number(m_settings.m_fontSize));
			if (index < 0) index = 0;
			m_fontSizeComboBox->setCurrentIndex(index);

			horizentalLayout = new QHBoxLayout(m_ui.parameterTab);
			horizentalLayout->addWidget(label, 0);
			horizentalLayout->addWidget(m_fontFamilyComboBox, 1);
			horizentalLayout->addWidget(m_fontSizeComboBox, 2);
			verticalLayout->addItem(horizentalLayout);
		}
	}

	void DrawingObjectDialog::initVisualizationWidget()
	{
		QStandardItemModel * model = new QStandardItemModel(this);
		QList<showInTime> listParent;
		listParent << enshowInTime_mins << enshowInTime_hours << enshowInTime_daily << enshowInTime_weekily << enShowInTime_monthly;
		foreach(showInTime itemTiem,listParent)
		{
			QStandardItem* itemParent = new QStandardItem(Helper::getShowInTimeText(itemTiem));
			itemParent->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled|Qt::ItemIsTristate);

			Qt::CheckState state = m_settings.m_intFlagsToShow & itemTiem ? Qt::Checked : Qt::Unchecked;
			itemParent->setCheckState(state);
			itemParent->setData(itemTiem); 

			if(itemTiem == enshowInTime_mins)
			{
				for(int i = enShowInTime_1min; i < enshowInTime_mins; i <<= 1)
				{
					QStandardItem* itemSub = new QStandardItem(Helper::getShowInTimeText(showInTime(i)));
					itemSub->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled|Qt::ItemIsTristate);
					state = m_settings.m_intFlagsToShow & i ? Qt::Checked : Qt::Unchecked;
					itemSub->setCheckState(state);
					itemSub->setData(i);
					itemParent->appendRow(itemSub);
				}
			}
			else if(itemTiem == enshowInTime_hours)
			{
				for(int i = enshowInTime_1hour; i < enshowInTime_hours; i <<= 1)
				{
					QStandardItem* itemSub = new QStandardItem(Helper::getShowInTimeText(showInTime(i)));
					itemSub->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled|Qt::ItemIsTristate);
					state = m_settings.m_intFlagsToShow & i ? Qt::Checked : Qt::Unchecked;
					itemSub->setCheckState(state);
					itemSub->setData(i);
					itemParent->appendRow(itemSub);
				}
			}
			model->appendRow(itemParent);
		}

		m_ui.timeView->setModel(model);
		m_ui.timeView->setHeaderHidden(true);
		connect(model,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(handleItemChangeSlot(QStandardItem*)));

        // Associate checkBox
		connect(m_ui.allTimeFramesCB,SIGNAL(stateChanged(int)),this,SLOT(handleTotalCheckStateSlot()));
        // initialization
		Qt::CheckState state = m_settings.m_intFlagsToShow == enshowInTime_all ? Qt::Checked : Qt::Unchecked;
		m_ui.allTimeFramesCB->setCheckState(state);
	}

	void DrawingObjectDialog::acceptCommonWidget()
	{
		m_settings.m_bAsBackground = m_ui.asBackgroundCB->isChecked();
		m_settings.m_bSelectAble = !m_ui.disableSelectCB->isChecked();
		const QString& strBackground = "background-color:";
		m_settings.m_colorLine = m_ui.colorButton->styleSheet().section(strBackground,1).section(';',0,1).trimmed();
		m_settings.m_intLineType = m_ui.lineTypeCB->currentData().toInt();
		m_settings.m_lfLineWidth = m_ui.lineWCB->currentData().toDouble();
		m_settings.m_name = m_ui.nameLineEdit->text();
		m_settings.m_describe = m_ui.DesLineEdit->text();
	}

	void DrawingObjectDialog::acceptLevelsWidget()
	{
		if(!m_settings.m_bhasLevels) return;
		QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(m_ui.levelsView->model());
		if(model == NULL) return;
		m_settings.m_mapLevels.clear();
		
		for(int i = 0; i < model->rowCount(); ++i)
		{
			QStandardItem* valueItem = model->item(i,0);
			QStandardItem* desItem = model->item(i,1);
			
			if(valueItem == NULL || desItem == NULL) continue;

			double value = valueItem->text().toDouble();
			QString descripition = desItem->text(); 
			m_settings.m_mapLevels[value] = descripition;
		}

		const QString& strBackground = "background-color:";
		m_settings.m_levelsColorLine = m_ui.colorLevelPB->styleSheet().section(strBackground,1).section(';',0,1).trimmed();
		m_settings.m_levelsLineType = m_ui.lineTypeLevelCBB->currentData().toInt();
		m_settings.m_levelsLineWidth = m_ui.lineWidthLevelCBB->currentData().toInt();
	}

	void DrawingObjectDialog::acceptParameterWidget()
	{
		for(int i = 0; i < m_settings.m_vecDate.size(); ++ i)
		{
			QDateTimeEdit* timeEdit = m_vecTimeEdit[i];
			m_settings.m_vecDate[i] = JulianDate::QTDateToJDate(timeEdit->dateTime());
		}

		for(int i = 0; i < m_settings.m_vecValue.size(); ++i)
		{
			m_settings.m_vecValue[i] = m_vecLineEdit[i]->text().toDouble();
		}

        // Whether to obtain diffraction attributes
		if(m_settings.m_intRayFlags & RayShow_Left && m_leftRayCB != NULL)
		{
			m_settings.m_intRayFlags &= ~RayShow_LeftSelected;

			if(m_leftRayCB->isChecked())
				m_settings.m_intRayFlags |= RayShow_LeftSelected;
		}

		if(m_settings.m_intRayFlags & RayShow_Right && m_rightRayCB != NULL)
		{
			m_settings.m_intRayFlags &= ~RayShow_RightSelected;

			if(m_rightRayCB->isChecked())
				m_settings.m_intRayFlags |= RayShow_RightSelected;
		}

		if(m_settings.m_intRayFlags & RayShow_Single && m_singleRayCB != NULL)
		{
			m_settings.m_intRayFlags &= ~RayShow_SingleSelected;

			if(m_singleRayCB->isChecked())
				m_settings.m_intRayFlags |= RayShow_SingleSelected;
		}
        //Get fill attributes
		if (m_settings.m_intFillShape & brushToFillShape_NeedBrush && m_fillShapeCB != NULL)
		{
			m_settings.m_intFillShape &= ~brushToFillShape_SetBrush;
			if(m_fillShapeCB->isChecked())
				m_settings.m_intFillShape |= brushToFillShape_SetBrush;
		}

		if (m_settings.m_type == enDrawingObjectText)
		{
			m_settings.m_angle = m_angleEdit->text().toDouble();
			m_settings.m_fontFamily = m_fontFamilyComboBox->currentText();
			m_settings.m_fontSize = m_fontSizeComboBox->currentText().toInt();
		}
	}

	void DrawingObjectDialog::acceptVisualizationWidget()
	{
		if(m_ui.allTimeFramesCB->isChecked())
		{
			m_settings.m_intFlagsToShow = enshowInTime_all;
		}
		else
		{
			m_settings.m_intFlagsToShow = 0;
			QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(m_ui.timeView->model());
			if(model == NULL) return;
	
			for(int i = 0; i < model->rowCount(); ++i)
			{
				QStandardItem* itemParent = model->item(i);
				if(itemParent == NULL) return;

				if(itemParent->rowCount() == 0 && itemParent->checkState() == Qt::Checked)
				{
					m_settings.m_intFlagsToShow |= itemParent->data().toInt();
				}
				else
				{
					for(int j = 0;j < itemParent->rowCount(); ++j)
					{
						if(itemParent->child(j)->checkState() == Qt::Checked)
							m_settings.m_intFlagsToShow |= itemParent->child(j)->data().toInt(); 
					}
				}
			}
		}
	}

	void DrawingObjectDialog::handleTotalCheckStateSlot()
	{
		m_ui.timeView->setEnabled(!m_ui.allTimeFramesCB->isChecked());
	}

	void DrawingObjectDialog::showColorSlot()
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

	void DrawingObjectDialog::handleItemChangeSlot(QStandardItem* item)
	{
        // Add a switch to prevent multiple signalling during processing
		if(m_isTheSameTimeSignal) return;
		m_isTheSameTimeSignal = true;

        // Adding the pending state makes the operation inconvenient, so remove it
		if(item->checkState() == Qt::PartiallyChecked) item->setCheckState(Qt::Checked);

		if(item->hasChildren())
		{
			//if(item->checkState() != Qt::PartiallyChecked)
			//{
			for(int i = 0; i < item->rowCount(); ++i)
			{
				if(item->child(i)->checkState() != item->checkState())
				{
					item->child(i)->setCheckState(item->checkState());
				}
			}
			/*}*/
		}
		else
		{
			QStandardItem* parent = item->parent();
			if(parent != NULL && parent->hasChildren())
			{
				bool flag = true;
				Qt::CheckState state = parent->child(0)->checkState();
				for(int i = 1; i < parent->rowCount(); ++i)
				{
					if(parent->child(i)->checkState() != state)
					{
						flag = false;
						break;
					}
				}

				if(flag)
				{

					if(parent->checkState() != state)
					{
						parent->setCheckState(state);
					}

				}
				else
				{
					parent->setCheckState(Qt::Checked);
				}
				//else
				//{
				//	
				//	if(parent->checkState() != Qt::PartiallyChecked)
				//	{
				//		parent->setCheckState(Qt::PartiallyChecked);
				//	}
				//		
				//}
			}
		}
		m_isTheSameTimeSignal = false;
	}

	void DrawingObjectDialog::addRowSlot()
	{
		QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(m_ui.levelsView->model());
		if(model == NULL) return;
		QStandardItem* itemValue = new QStandardItem();
		QStandardItem* itemDesc = new QStandardItem();
		model->appendRow(QList<QStandardItem*>() << itemValue <<itemDesc);
	}

	void DrawingObjectDialog::deleteRowSlot()
	{
		QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(m_ui.levelsView->model());
		if(model == NULL) return;

		model->removeRow(m_ui.levelsView->currentIndex().row());
		if(model->rowCount() == 0)
		{
			m_ui.delButton->setEnabled(false);
			m_ui.editButton->setEnabled(false);
			m_bButtonState = false;
		}
	}

	void DrawingObjectDialog::editRowSlot()
	{
		QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(m_ui.levelsView->model());
		if(model == NULL) return;
		m_ui.levelsView->edit(m_ui.levelsView->currentIndex());
	}

	void DrawingObjectDialog::defaultRowSlot()
	{
		QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(m_ui.levelsView->model());
		if(model == NULL) return;
		model->clear();
		model->setHorizontalHeaderLabels(QStringList() << "levels" << "Descripition");
		m_ui.levelsView->horizontalHeader()->setStretchLastSection(true);
		QMap<double,QString> mapDefault = Helper::getDefaultLevels(m_settings.m_type);
		foreach(double value,mapDefault.keys())
		{
			QStandardItem* item = new QStandardItem(QString::number(value));
			QStandardItem* itemDsp = new QStandardItem(mapDefault[value]);
			model->appendRow(QList<QStandardItem*>() << item << itemDsp);
		}
		m_ui.delButton->setEnabled(false);
		m_ui.editButton->setEnabled(false);
		m_bButtonState = false;
	}

	void DrawingObjectDialog::enableSwitchSlot(const QModelIndex & index)
	{
		if(!m_bButtonState)
		{
			m_ui.delButton->setEnabled(true);
			m_ui.editButton->setEnabled(true);
			m_bButtonState = true;
		}
	}

	void DrawingObjectDialog::initColorLine(QPushButton* colorPB,QComboBox* lineTypeCB,QComboBox* lineWideCB)
	{
		connect(colorPB,SIGNAL(clicked()),this,SLOT(showColorSlot()));

		lineTypeCB->addItem("SolidLine",(int)Qt::SolidLine);
		lineTypeCB->addItem("DashLine",(int)Qt::DashLine);
		lineTypeCB->addItem("DotLine",(int)Qt::DotLine);
		lineTypeCB->addItem("DashDotline",(int)Qt::DashDotLine);
		lineTypeCB->addItem("DashDotDotLine",(int)Qt::DashDotDotLine);

		lineWideCB->addItem("1.0",1.0);
		lineWideCB->addItem("1.5",1.5);
		lineWideCB->addItem("2.0",2.0);
		lineWideCB->addItem("3.0",3.0);
		lineWideCB->addItem("5.0",5.0);
	}
}

#include "moc_DrawingObjectDialog.cpp"
