/********************************************************************************
** Form generated from reading UI file 'autoupdatewidget.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AUTOUPDATEWIDGET_H
#define UI_AUTOUPDATEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AutoUpdateWidget
{
public:
    QFrame *Content_Frame;
    QVBoxLayout *Content_Layout;
    QWidget *Top_Widget;
    QGridLayout *gridLayout;
    QLabel *versionUpdatePrompt_Label;
    QWidget *Bottom_Widget;
    QHBoxLayout *horizontalLayout;
    QToolButton *NotUpdate_Button;
    QToolButton *Update_Button;

    void setupUi(QWidget *AutoUpdateWidget)
    {
        if (AutoUpdateWidget->objectName().isEmpty())
            AutoUpdateWidget->setObjectName(QStringLiteral("AutoUpdateWidget"));
        AutoUpdateWidget->resize(390, 529);
        Content_Frame = new QFrame(AutoUpdateWidget);
        Content_Frame->setObjectName(QStringLiteral("Content_Frame"));
        Content_Frame->setGeometry(QRect(20, 185, 341, 121));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Content_Frame->sizePolicy().hasHeightForWidth());
        Content_Frame->setSizePolicy(sizePolicy);
        Content_Frame->setStyleSheet(QLatin1String("#Content_Frame{\n"
"background-color: rgb(255, 255, 255);\n"
"border:1px solid #c3c3c3 ;\n"
"border-radius:10px;\n"
"}"));
        Content_Layout = new QVBoxLayout(Content_Frame);
        Content_Layout->setSpacing(0);
        Content_Layout->setObjectName(QStringLiteral("Content_Layout"));
        Content_Layout->setContentsMargins(0, 0, 0, 0);
        Top_Widget = new QWidget(Content_Frame);
        Top_Widget->setObjectName(QStringLiteral("Top_Widget"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(Top_Widget->sizePolicy().hasHeightForWidth());
        Top_Widget->setSizePolicy(sizePolicy1);
        gridLayout = new QGridLayout(Top_Widget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        versionUpdatePrompt_Label = new QLabel(Top_Widget);
        versionUpdatePrompt_Label->setObjectName(QStringLiteral("versionUpdatePrompt_Label"));
        versionUpdatePrompt_Label->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        versionUpdatePrompt_Label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        versionUpdatePrompt_Label->setWordWrap(true);

        gridLayout->addWidget(versionUpdatePrompt_Label, 0, 0, 1, 1);


        Content_Layout->addWidget(Top_Widget);

        Bottom_Widget = new QWidget(Content_Frame);
        Bottom_Widget->setObjectName(QStringLiteral("Bottom_Widget"));
        sizePolicy1.setHeightForWidth(Bottom_Widget->sizePolicy().hasHeightForWidth());
        Bottom_Widget->setSizePolicy(sizePolicy1);
        horizontalLayout = new QHBoxLayout(Bottom_Widget);
        horizontalLayout->setSpacing(10);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetFixedSize);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        NotUpdate_Button = new QToolButton(Bottom_Widget);
        NotUpdate_Button->setObjectName(QStringLiteral("NotUpdate_Button"));
        sizePolicy.setHeightForWidth(NotUpdate_Button->sizePolicy().hasHeightForWidth());
        NotUpdate_Button->setSizePolicy(sizePolicy);
        NotUpdate_Button->setFocusPolicy(Qt::NoFocus);
        NotUpdate_Button->setStyleSheet(QLatin1String("border:1px solid #adadad;\n"
"border-radius: 5px;\n"
"background-color: rgb(255, 255, 255);"));

        horizontalLayout->addWidget(NotUpdate_Button);

        Update_Button = new QToolButton(Bottom_Widget);
        Update_Button->setObjectName(QStringLiteral("Update_Button"));
        sizePolicy.setHeightForWidth(Update_Button->sizePolicy().hasHeightForWidth());
        Update_Button->setSizePolicy(sizePolicy);
        Update_Button->setMinimumSize(QSize(0, 40));
        Update_Button->setFocusPolicy(Qt::NoFocus);
        Update_Button->setStyleSheet(QLatin1String("border:1px solid #adadad;\n"
"border-radius: 5px;\n"
"background-color: rgb(255, 255, 255);"));

        horizontalLayout->addWidget(Update_Button);


        Content_Layout->addWidget(Bottom_Widget);

        Content_Layout->setStretch(0, 1);
        Content_Layout->setStretch(1, 1);

        retranslateUi(AutoUpdateWidget);

        QMetaObject::connectSlotsByName(AutoUpdateWidget);
    } // setupUi

    void retranslateUi(QWidget *AutoUpdateWidget)
    {
        AutoUpdateWidget->setWindowTitle(QApplication::translate("AutoUpdateWidget", "Form", nullptr));
        versionUpdatePrompt_Label->setText(QApplication::translate("AutoUpdateWidget", "\347\231\274\347\217\276\347\211\210\346\234\254: 1.0.1, \347\225\266\345\211\215\347\211\210\346\234\254: 1.0.0, \346\230\257\345\220\246\346\233\264\346\226\260\357\274\237", nullptr));
        NotUpdate_Button->setText(QApplication::translate("AutoUpdateWidget", "\346\232\253\344\270\215\346\233\264\346\226\260", nullptr));
        Update_Button->setText(QApplication::translate("AutoUpdateWidget", "\347\253\213\345\215\263\346\233\264\346\226\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AutoUpdateWidget: public Ui_AutoUpdateWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUTOUPDATEWIDGET_H
