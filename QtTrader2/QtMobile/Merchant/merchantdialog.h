#ifndef MERCHANTDIALOG_H
#define MERCHANTDIALOG_H

#include <QWidget>
#include <QEvent>


namespace Ui {
    class MerchantWidget;
}

class MerchantDialog : public QWidget
{
    Q_OBJECT
public:
    explicit MerchantDialog(bool isShowClose = false, QWidget *parent = nullptr);
    bool event(QEvent *event);

signals:
    void setMerchantSignal(QString merchantCode);
    void closeSignal();

public slots:
    void okClickSlot();

private:
    Ui::MerchantWidget* m_ui;

};

#endif // MERCHANTDIALOG_H
