#include "TemplateSaveDialog.h"
#include "SR.h"
#include <QDir>
#include <QLabel>
#include <QPushButton>
#include "Template/TemplateManager.h"
#include "historicalchart.h"

namespace HistoricalChartSpace
{
	TemplateSaveDialog::TemplateSaveDialog(TemplateManager* templateManager, QString& templateFilePath)
		: m_templateManager(templateManager), m_templateFilePath(templateFilePath), QDialog(templateManager->getView())
	{
		this->setWindowTitle(SR::get("SaveTemplate"));
		this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
		this->resize(305, 173);
		QLabel* templateNameLabel = new QLabel(SR::get("TemplateName"), this);
		templateNameLabel->setGeometry(QRect(36, 41, 100, 16));
		m_templateComboBox = new QComboBox(this);
		this->fillComboBox();
		m_templateComboBox->setEditable(true);
		m_templateComboBox->setGeometry(QRect(120, 40, 151, 22));
		connect(m_templateComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(ComboBox_currentIndexChanged(const QString&)));

		m_IsDefaultCheckBox = new QCheckBox(this);
		m_IsDefaultCheckBox->setText(SR::get("AsDefault"));
		m_IsDefaultCheckBox->setGeometry(QRect(120, 70, 120, 18));

		QPushButton* okButton = new QPushButton(SR::get("OK"), this);
		okButton->setGeometry(QRect(25, 120, 75, 31));
		connect(okButton, SIGNAL(clicked()), this, SLOT(save()));

		QPushButton* cancelButton = new QPushButton(SR::get("Cancel"), this);
		cancelButton->setGeometry(QRect(200, 120, 75, 31));
		connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
	}

	TemplateSaveDialog::~TemplateSaveDialog()
	{
	}

	void TemplateSaveDialog::fillComboBox()
	{
		QDir dir(m_templateFilePath, "*.tpl");
		QStringList templateFiles = dir.entryList();
		for (int index = 0; index < templateFiles.size(); index++)
		{
			QString fileName = templateFiles[index];
			fileName.truncate(fileName.lastIndexOf(".tpl", -1, Qt::CaseInsensitive));
			m_templateComboBox->addItem(fileName);
		}
	}

	void TemplateSaveDialog::save()
	{
		QString name = m_templateComboBox->currentText();
		bool asDefault = m_IsDefaultCheckBox->checkState() == Qt::Checked;
		m_templateManager->saveTemplate(name, asDefault);
		this->close();
	}

	void TemplateSaveDialog::cancel()
	{
		this->close();
	}

	void TemplateSaveDialog::ComboBox_currentIndexChanged(const QString& text)
	{
		if (m_templateManager->isDefaultTemplate(text))
		{
			m_IsDefaultCheckBox->setChecked(true);
		}
		else
		{
			m_IsDefaultCheckBox->setChecked(false);
		}
	}
}

#include "moc_TemplateSaveDialog.cpp"
