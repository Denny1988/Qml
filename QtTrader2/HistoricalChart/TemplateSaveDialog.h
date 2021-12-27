#pragma once

#include <QDialog>
#include <QComboBox>
#include <QCheckBox>

namespace HistoricalChartSpace
{
	class TemplateManager;
	class TemplateSaveDialog : public QDialog
	{
		Q_OBJECT
	public:
		TemplateSaveDialog(TemplateManager* templateManager, QString& templateFilePath);
		~TemplateSaveDialog();

	private slots:
		void save();
		void cancel();
		void ComboBox_currentIndexChanged(const QString& text);

	private:
		void fillComboBox();

	private:
		const QString& m_templateFilePath;
		QComboBox* m_templateComboBox;
		QCheckBox* m_IsDefaultCheckBox;
		TemplateManager* m_templateManager;
	};
}