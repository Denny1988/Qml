#pragma once
#include "ui_DrawingObjectDialog.h"
#include <QDialog>
#include "Settings.h"
#include "historicalchart.h"
class QDateTimeEdit;
class QStandardItem;
namespace HistoricalChartSpace
{
	class DrawingObjectDialog : public QDialog
	{
		Q_OBJECT
	public:
		DrawingObjectDialog(const DrawingObjectSettings& settings, HistoricalChart *view);
		~DrawingObjectDialog();
		const DrawingObjectSettings& getSettings() const;
		public slots:
			void accept();
			void showColorSlot();
			void handleItemChangeSlot(QStandardItem*);
			void handleTotalCheckStateSlot();
			void addRowSlot();
			void deleteRowSlot();
			void editRowSlot();
			void defaultRowSlot();
			void enableSwitchSlot(const QModelIndex &);
	private:
		void applyStringResource();
		void init();
		void initCommonWidget();
		void initLevelsWidget();
		void initParameterWidget();
		void initVisualizationWidget();

		void acceptCommonWidget();
		void acceptLevelsWidget();
		void acceptParameterWidget();
		void acceptVisualizationWidget();

	private:
		void initColorLine(QPushButton* colorPB,QComboBox* lineTypeCB,QComboBox* lineWideCB);
	private:
		DrawingObjectSettings		m_settings;
		Ui::DrawingObjectDialog		m_ui;
		QVector<QDateTimeEdit*>	m_vecTimeEdit;
		QVector<QLineEdit*>		m_vecLineEdit;
		bool					m_isTheSameTimeSignal;
		QCheckBox*				m_leftRayCB;
		QCheckBox*				m_rightRayCB;
		QCheckBox*				m_singleRayCB;
		QCheckBox*				m_fillShapeCB;
		bool					m_bButtonState;
		HistoricalChart* m_pView;

		QLineEdit* m_angleEdit;
		QComboBox* m_fontFamilyComboBox;
		QComboBox* m_fontSizeComboBox;
	};
}