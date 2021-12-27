#pragma once
#include <QObject>
#include <QAction>
#include <QToolBar>
#include <QMenu>

namespace HistoricalChartSpace
{
	class HistoricalChart;
	class TemplateManager : public QObject
	{
		Q_OBJECT
	public:
		TemplateManager(HistoricalChart* pView, QString& templateFilePath);
		virtual ~TemplateManager();

		HistoricalChart* getView();
		//void appendMenuTo(QToolBar* toolbar);
		QMenu* getMenu();
		QString getDefaultTemplateConfigXml() const;

		void saveTemplate(QString& name, bool asDefault);
		QString getDefaultTemplateFileName() const;
		bool isDefaultTemplate(const QString& name) const;

	private slots:
		void SaveTemplateSlot();
		void LoadTemplateSlot();
		void ApplyTemplateSlot();
		void RemoveTemplateSlot();
		void AppendTemplateMenuSlot();

	private:
		void clearTemplateMenu();

	private:
		HistoricalChart* m_pView;
		QMenu* m_menu;
		QString m_templateFilePath;
		QAction* m_saveTemplateAction;
		QAction* m_loadTemplateAction;
		QAction* m_removeTemplateAciton;
		QMenu* m_removeTemplateMenu;

		QList<QAction*> m_applyTemplateActions;
	};
}