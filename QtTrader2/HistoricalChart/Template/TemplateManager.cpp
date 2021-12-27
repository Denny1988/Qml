#include "TemplateManager.h"
#include "SR.h"
#include "historicalchart.h"
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QScopedPointer>
#include <QDebug>
#include "Helper.h"
#include "TemplateSaveDialog.h"


namespace HistoricalChartSpace
{
	TemplateManager::TemplateManager(HistoricalChart* pView, QString& templateFilePath) : m_pView(pView)
	{
        //_ASSERT(!templateFilePath.isNull() && !templateFilePath.isEmpty());
		m_templateFilePath = templateFilePath;
		if (!m_templateFilePath.endsWith('\\') && !m_templateFilePath.endsWith('/'))
		{
			m_templateFilePath += "/";
		}
		QDir dir(m_templateFilePath);
		if (!dir.exists())
		{
			bool success = dir.mkpath(m_templateFilePath);
            //_ASSERT(success);
		}

		m_saveTemplateAction = new QAction(SR::get("SaveTemplate") + "...", pView);
		//m_loadTemplateAction = new QAction(SR::get("LoadTemplate"), pView);
		m_removeTemplateAciton = new QAction(SR::get("RemoveTemplate"), pView);

		m_menu = new QMenu("TemplateMenu", pView);
		m_menu->addAction(m_saveTemplateAction);
		//m_menu->addAction(m_loadTemplateAction);
		m_menu->addAction(m_removeTemplateAciton);
		m_menu->addSeparator();

		m_removeTemplateMenu = new QMenu("removeTemplateMenu", pView);
		m_removeTemplateAciton->setMenu(m_removeTemplateMenu);

		QObject::connect(m_saveTemplateAction, SIGNAL(triggered()), this, SLOT(SaveTemplateSlot()));
		//QObject::connect(m_loadTemplateAction, SIGNAL(triggered()), this, SLOT(LoadTemplateSlot()));
		QObject::connect(m_menu, SIGNAL(aboutToShow()), this, SLOT(AppendTemplateMenuSlot()));
	}

	TemplateManager::~TemplateManager()
	{
	}

	HistoricalChart* TemplateManager::getView()
	{
		return m_pView;
	}

	QMenu* TemplateManager::getMenu()
	{
		return m_menu;
	}

	QString TemplateManager::getDefaultTemplateConfigXml() const
	{
		QString configXml;
		QString fileName = this->getDefaultTemplateFileName();
		if (!(fileName.isNull() || fileName.isEmpty()))
		{
			QFile file(fileName);
			if (file.open(QIODevice::ReadOnly))
			{
				configXml = file.readAll();
				file.close();
			}
		}
		return configXml;
	}

	//void TemplateManager::appendMenuTo(QToolBar* toolbar)
	//{
	//	QAction* templatesAction = new QAction(SR::get("Templates"), toolbar);
	//	templatesAction->setMenu(m_menu);
	//	toolbar->addAction(templatesAction);
	//}

	void TemplateManager::AppendTemplateMenuSlot()
	{
		this->clearTemplateMenu();
		QDir dir(m_templateFilePath, "*.tpl");
		QStringList templateFiles = dir.entryList();
		for (int index = 0; index < templateFiles.size(); index++)
		{
			QString fileName = templateFiles[index];
			fileName.truncate(fileName.lastIndexOf(".tpl", -1, Qt::CaseInsensitive));
			if (fileName.toLower() == "default") continue;

			QAction* applyTemplateAction = new QAction(fileName, m_menu);
			applyTemplateAction->setData(fileName);
			m_menu->addAction(applyTemplateAction);
			QObject::connect(applyTemplateAction, SIGNAL(triggered()), this, SLOT(ApplyTemplateSlot()));
			m_applyTemplateActions.append(applyTemplateAction);

			QAction* removeTemplateAction = new QAction(fileName, m_removeTemplateMenu);
			removeTemplateAction->setData(fileName);
			m_removeTemplateMenu->addAction(removeTemplateAction);
			QObject::connect(removeTemplateAction, SIGNAL(triggered()), this, SLOT(RemoveTemplateSlot()));
		}
	}

	void TemplateManager::clearTemplateMenu()
	{
        foreach (QAction* action, m_applyTemplateActions)
		{
			m_menu->removeAction(action);
		}
		
		m_removeTemplateMenu->clear();
	}

	void TemplateManager::SaveTemplateSlot()
	{
		QScopedPointer<TemplateSaveDialog> dialog(new TemplateSaveDialog(this, m_templateFilePath));
		dialog->exec();
	}

	void TemplateManager::saveTemplate(QString& name, bool asDefault)
	{
		QString fileName = m_templateFilePath + name + ".tpl";
		QFile file(fileName);
		if (file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QString configXmlString = m_pView->getConfigXml();
			file.write(configXmlString.toLatin1().data());
			file.close();
		}

		if (asDefault)
		{
			QFile file(m_templateFilePath + "default.txt");
			if (file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
			{
				file.write(name.toLatin1().data());
			}
		}
	}

	QString TemplateManager::getDefaultTemplateFileName() const
	{
		QString defaultTemplateFileName;
		QFile file(m_templateFilePath + "default.txt");
		if (file.exists())
		{
			if (file.open(QIODevice::ReadOnly))
			{
				QString fileName = file.readAll();
				defaultTemplateFileName = m_templateFilePath + fileName + ".tpl";
			}
		}
		return defaultTemplateFileName;
	}

	bool TemplateManager::isDefaultTemplate(const QString& name) const
	{
		QFile file(m_templateFilePath + "default.txt");
		if (file.exists())
		{
			if (file.open(QIODevice::ReadOnly))
			{
				QString fileName = file.readAll();
				return fileName == name;
			}
		}
		return false;
	}

	void TemplateManager::LoadTemplateSlot()
	{
		QString fileName = QFileDialog::getOpenFileName(m_pView, SR::get("LoadTemplate"), m_templateFilePath, "*.tpl");
		if (!fileName.isNull())
		{
			QString configXmlString;
			QFile file(fileName);
			if (file.open(QIODevice::ReadOnly))
			{
				configXmlString = file.readAll();
				file.close();
			}
			m_pView->applyTemplate(configXmlString);
		}
	}

	void TemplateManager::ApplyTemplateSlot()
	{
		QAction *action = qobject_cast<QAction*>(sender());
		if (action)
		{
			QString fileName = QString("%1%2%3").arg(m_templateFilePath).arg(action->data().toString()).arg(".tpl");
			QFile file(fileName);
			if (file.open(QIODevice::ReadOnly))
			{
				QString templateXml = file.readAll();
				m_pView->applyTemplate(templateXml);
			}
		}
	}

	void TemplateManager::RemoveTemplateSlot()
	{
		QAction *action = qobject_cast<QAction*>(sender());
		if (action)
		{
			QString templateName = action->data().toString();
			QMessageBox messageBox(m_pView->getDialogParent());
			messageBox.setIcon(QMessageBox::Warning);
			messageBox.setWindowTitle(SR::get("Warning"));
			messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
			messageBox.setText(SR::get("ConfirmRemoveTemplate").arg(templateName));

			QAbstractButton* yesButton = messageBox.button(QMessageBox::Yes);
			yesButton->setText(SR::get("Yes"));

			QAbstractButton* noButton = messageBox.button(QMessageBox::No);
			noButton->setText(SR::get("No"));

			messageBox.exec();

			if (messageBox.clickedButton() == yesButton)
			{
				QString fileName = QString("%1%2%3").arg(m_templateFilePath).arg(templateName).arg(".tpl");
				QDir dir(m_templateFilePath);
				dir.remove(fileName);
			}
		}
	}
}

#include "moc_TemplateManager.cpp"
