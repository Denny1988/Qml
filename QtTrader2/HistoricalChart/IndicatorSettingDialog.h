#ifndef INDICATORSETTINGDIALOG_H
#define INDICATORSETTINGDIALOG_H

#include <QWidget>
#include <QEventLoop>
#include <QCloseEvent>
#include <QDialog>
#include <QListWidget>
#include <QTableWidget>
#include <QPoint>
#include <QEvent>
#include <QLabel>
#include <QList>
#include <QMap>

#include <QVBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>

#include "Indicator/IndicatorCore.h"
#include <QDebug>


namespace  Ui {
    class IndicatorSetting;
}

namespace HistoricalChartSpace
{
    class ItemWidget : public QLabel
    {
        Q_OBJECT
    public:
        explicit ItemWidget(int row, int column, int indicatorType, QString name, bool isChecked, QWidget *parent = nullptr);
        void setChecked(bool isCheck, bool isSubmitSignal = false);

    public:
        bool event(QEvent *event);

    signals:
        void checkItemSignal(int row, int column, int indicatorType, bool isChecked);

    private:
        void setCheckStyle();

    private:
        bool _isStartMouseMoveing;
        QPoint _mouseMoveStartPoint;
        bool _isChecked;
        int _row;
        int _column;
        int _indicatorType;

    };

    class MainIndicatorListWidget : public QTableWidget
    {
        Q_OBJECT
    public:
        explicit MainIndicatorListWidget(int height, int width, QList<enIndicatorType> showIndicators, bool horizontalScreen, QWidget *parent = nullptr);

    signals:
        void checkIndicatorSignal(int indicatorType, bool isChecked, bool isMainChart);

    public slots:
        void checkItemSlot(int row, int column, int indicatorType, bool isChecked);

    private:
        void addListWidgetItem(int row, int column, QString indicatorName, int indicatorType, bool isChecked = false);
        bool _horizontalScreen;

    };

    class SecondaryIndicatorTableWidget : public QTableWidget
    {
        Q_OBJECT
    public:
        explicit SecondaryIndicatorTableWidget(int height, int width, QList<enIndicatorType> showIndicators, bool horizontalScreen,  QWidget *parent = nullptr);
        void hideIndicator();
        void showIndicator();
        int getCheckIndicatorType();

    signals:
        void checkIndicatorSignal(int indicatorType, bool isChecked, bool isMainChart);

    public slots:
        void checkItemSlot(int row, int column, int indicatorType, bool isChecked);

    private:
        void addTableWidgetItem(int row, int column, QString indicatorName, int indicatorType, bool isChecked = false);

    private:
        QMap<QString, ItemWidget*> _items;
        bool _horizontalScreen;
        int _checkIndicatorType;

    };

    class InputLineEdit : public QLineEdit
    {
    public:
        InputLineEdit(const QString& str,QWidget* parent):QLineEdit(str,parent) {
            this->setStyleSheet("border:1px solid #cfcfcf;border-radius:5px;");
        }
        ~InputLineEdit(){
            qDebug() << " ~InputLineEdit";
        }
        void setDataString(const QString& str) { m_str = str;}
        const QString& getDataString() const {return m_str;}
        void setOldValue(const QString& value) { m_oldValue = value;}
        const QString& getOldValue() const {return m_oldValue;}

    private:
        QString m_str;
        QString m_oldValue;
    };

    class Indicator;
    class HistoricalChart;
    class IndicatorSettingDialog : public QDialog
    {
        Q_OBJECT        
        friend class HistoricalChart;
    public:
        explicit IndicatorSettingDialog(double pixelSizeRatio, HistoricalChart* pView, bool horizontalScreen, QWidget *parent = nullptr);
        ~IndicatorSettingDialog();
        void showDialog();
        bool event(QEvent *event);

    signals:
        void addIndicatorSignal(int indicatorType);

    public slots:
        void closeSlot();
        void changeMainCharTabSlot();
        void changeSecondaryTabSlot();
        void changeSelectChartIndicatorSlot();
        void checkIndicatorSlot(int indicatorType, bool isChecked, bool isMainChart);
        void handleStandColorDialogSlot();
        void editingFinishedSlot();

    private:
        MainIndicatorListWidget* _mainIndicatorListWidget;
        SecondaryIndicatorTableWidget* _secondaryIndicatorTableWidget;
        Ui::IndicatorSetting* m_ui;
        double _pixelSizeRatio;
        HistoricalChart* _pView;
        QList<enIndicatorType> _showIndicatorTypes;
        int _currentIndicatorTypeIndex;
        QMap<enIndicatorType, Indicator*> _showIndicators;
        enIndicatorType _secondaryIndicatorType;
        Indicator* _currentIndicator;
        IndicatorSettings			m_settings;
        QVector<InputLineEdit*>		m_vecIntInputLineEdit;
        QVector<InputLineEdit*>		m_vecDoubleInputLineEdit;
        QVector<QToolButton*>		m_vecColorPushButton;
        QToolButton*				m_okButton;
        bool _isExistLayout;

        QMap<Indicator*, InputLineEdit*> m_MAvecIntInputLineEdit;
        QMap<Indicator*, InputLineEdit*> m_MAvecDoubleInputLineEdit;
        QMap<Indicator*, QToolButton*> m_MAvecColorPushButton;

        bool _horizontalScreen;
    private:
        void generateIndicatorProperty();
        void generateHorizontalScreenIndicatorProperty();
        void updateIndicatorProperty();
        void refreshIndicator();
    };

}
#endif // INDICATORSETTINGDIALOG_H
