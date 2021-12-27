/********************************************************************************
** Form generated from reading UI file 'SplashScreen.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SPLASHSCREEN_H
#define UI_SPLASHSCREEN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SplashScreen
{
public:
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QProgressBar *progressBar;

    void setupUi(QWidget *SplashScreen)
    {
        if (SplashScreen->objectName().isEmpty())
            SplashScreen->setObjectName(QStringLiteral("SplashScreen"));
        SplashScreen->resize(480, 640);
        SplashScreen->setStyleSheet(QStringLiteral("border-image: url(:/Resources/Images/backgroud.png);"));
        verticalLayout = new QVBoxLayout(SplashScreen);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(20, 0, 20, 20);
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        progressBar = new QProgressBar(SplashScreen);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setMaximumSize(QSize(16777215, 10));
        progressBar->setStyleSheet(QLatin1String("QProgressBar{border-radius:10px;}\n"
"QProgressBar::chunk:horizontal{border-radius:5px;background:#37ce34;}"));
        progressBar->setMaximum(100);
        progressBar->setValue(20);

        verticalLayout->addWidget(progressBar);


        retranslateUi(SplashScreen);

        QMetaObject::connectSlotsByName(SplashScreen);
    } // setupUi

    void retranslateUi(QWidget *SplashScreen)
    {
        SplashScreen->setWindowTitle(QApplication::translate("SplashScreen", "Form", nullptr));
        progressBar->setFormat(QString());
    } // retranslateUi

};

namespace Ui {
    class SplashScreen: public Ui_SplashScreen {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPLASHSCREEN_H
