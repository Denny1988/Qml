#ifndef AUTOUPDATEDIALOG_H
#define AUTOUPDATEDIALOG_H

#include <QWidget>


namespace Ui {
    class AutoUpdateWidget;
}

class AutoUpdateDialog : public QWidget
{
    Q_OBJECT
public:
    explicit AutoUpdateDialog(QString currentVersion, QString newVersion, bool isNeedDirectUpdate = false, QWidget *parent = nullptr);

signals:
    void startUpdateSignal();
    void closeSignal();

public slots:
    void updateClickSlot();
    void notUpdateClickSlot();

private:
    Ui::AutoUpdateWidget* m_ui;

};

#endif
