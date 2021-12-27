#pragma once
#include <QDialog>
#include "Indicator/IndicatorCore.h"
#include <QVector>
#include <QtWidgets/QtWidgets>
namespace HistoricalChartSpace
{
	class Indicator;
	class InputLineEdit;
	class IndicatorDialog : public QDialog
	{
		Q_OBJECT
	public:
		static void ShowDialog(Indicator* indicator, QWidget* parent);

	public:
		IndicatorDialog(Indicator* indicator, QWidget *parent = 0);
		~IndicatorDialog();
		const IndicatorSettings& getIndicatorSettings() const;
		public slots:
			void accept();
			void handleStandColorDialogSlot();
	private:
		void init();
		
	private:
		Indicator* m_indicator;
		IndicatorSettings			m_settings;
		QComboBox*					m_CPDCombox;
		QComboBox*					m_maTypeCombox;
		QVector<InputLineEdit*>		m_vecIntInputLineEdit;
		QVector<InputLineEdit*>		m_vecDoubleInputLineEdit;
		QVector<QPushButton*>		m_vecColorPushButton;
		QVector<QComboBox*>			m_vecLineWidthCBB;
		QVector<QComboBox*>			m_vecLineTypeCBB;
		QPushButton*				m_okButton;
		QPushButton*				m_cancelButton;
		 
	};


	class InputLineEdit : public QLineEdit
	{
	public:
		InputLineEdit(const QString& str,QWidget* parent):QLineEdit(str,parent){}
		void setDataString(const QString& str) { m_str = str;}
		const QString& getDataString() const {return m_str;}        
        void setOldValue(const QString& value) { m_oldValue = value;}
        const QString& getOldValue() const {return m_oldValue;}
	private:
		QString m_str;
        QString m_oldValue;
	};

}
