/********************************************************************************
** Form generated from reading UI file 'merchantwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MERCHANTWIDGET_H
#define UI_MERCHANTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MerchantWidget
{
public:
    QFrame *MW_Content_Frame;
    QVBoxLayout *Content_Layout;
    QWidget *Top_Widget;
    QHBoxLayout *horizontalLayout_2;
    QLabel *MW_Code_Label;
    QLineEdit *MW_Code_LineEdit;
    QWidget *Bottom_Widget;
    QHBoxLayout *horizontalLayout;
    QToolButton *MW_Close_Button;
    QToolButton *MW_Ok_Button;

    void setupUi(QWidget *MerchantWidget)
    {
        if (MerchantWidget->objectName().isEmpty())
            MerchantWidget->setObjectName(QStringLiteral("MerchantWidget"));
        MerchantWidget->resize(390, 529);
        MW_Content_Frame = new QFrame(MerchantWidget);
        MW_Content_Frame->setObjectName(QStringLiteral("MW_Content_Frame"));
        MW_Content_Frame->setGeometry(QRect(20, 185, 350, 160));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MW_Content_Frame->sizePolicy().hasHeightForWidth());
        MW_Content_Frame->setSizePolicy(sizePolicy);
        MW_Content_Frame->setStyleSheet(QLatin1String("#MW_Content_Frame{\n"
"background-color: rgb(255, 255, 255, 150);\n"
"border:0px;\n"
"border-radius:10px;\n"
"}"));
        Content_Layout = new QVBoxLayout(MW_Content_Frame);
        Content_Layout->setSpacing(0);
        Content_Layout->setObjectName(QStringLiteral("Content_Layout"));
        Content_Layout->setContentsMargins(0, 0, 0, 0);
        Top_Widget = new QWidget(MW_Content_Frame);
        Top_Widget->setObjectName(QStringLiteral("Top_Widget"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(Top_Widget->sizePolicy().hasHeightForWidth());
        Top_Widget->setSizePolicy(sizePolicy1);
        horizontalLayout_2 = new QHBoxLayout(Top_Widget);
        horizontalLayout_2->setSpacing(5);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        MW_Code_Label = new QLabel(Top_Widget);
        MW_Code_Label->setObjectName(QStringLiteral("MW_Code_Label"));
        MW_Code_Label->setMinimumSize(QSize(0, 35));
        MW_Code_Label->setStyleSheet(QStringLiteral("background-color: transparent;"));

        horizontalLayout_2->addWidget(MW_Code_Label);

        MW_Code_LineEdit = new QLineEdit(Top_Widget);
        MW_Code_LineEdit->setObjectName(QStringLiteral("MW_Code_LineEdit"));
        sizePolicy1.setHeightForWidth(MW_Code_LineEdit->sizePolicy().hasHeightForWidth());
        MW_Code_LineEdit->setSizePolicy(sizePolicy1);
        MW_Code_LineEdit->setMinimumSize(QSize(0, 35));
        MW_Code_LineEdit->setStyleSheet(QLatin1String("border:1px solid #adadad;\n"
"border-radius: 5px;\n"
"background-color: rgb(255, 255, 255);"));

        horizontalLayout_2->addWidget(MW_Code_LineEdit);


        Content_Layout->addWidget(Top_Widget);

        Bottom_Widget = new QWidget(MW_Content_Frame);
        Bottom_Widget->setObjectName(QStringLiteral("Bottom_Widget"));
        sizePolicy1.setHeightForWidth(Bottom_Widget->sizePolicy().hasHeightForWidth());
        Bottom_Widget->setSizePolicy(sizePolicy1);
        horizontalLayout = new QHBoxLayout(Bottom_Widget);
        horizontalLayout->setSpacing(5);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetFixedSize);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        MW_Close_Button = new QToolButton(Bottom_Widget);
        MW_Close_Button->setObjectName(QStringLiteral("MW_Close_Button"));
        sizePolicy.setHeightForWidth(MW_Close_Button->sizePolicy().hasHeightForWidth());
        MW_Close_Button->setSizePolicy(sizePolicy);
        MW_Close_Button->setStyleSheet(QLatin1String("border:1px solid #adadad;\n"
"border-radius: 5px;\n"
"background-color: rgb(255, 255, 255);"));

        horizontalLayout->addWidget(MW_Close_Button);

        MW_Ok_Button = new QToolButton(Bottom_Widget);
        MW_Ok_Button->setObjectName(QStringLiteral("MW_Ok_Button"));
        sizePolicy.setHeightForWidth(MW_Ok_Button->sizePolicy().hasHeightForWidth());
        MW_Ok_Button->setSizePolicy(sizePolicy);
        MW_Ok_Button->setMinimumSize(QSize(0, 40));
        MW_Ok_Button->setFocusPolicy(Qt::NoFocus);
        MW_Ok_Button->setStyleSheet(QLatin1String("border:1px solid #adadad;\n"
"border-radius: 5px;\n"
"background-color: rgb(255, 255, 255);"));

        horizontalLayout->addWidget(MW_Ok_Button);


        Content_Layout->addWidget(Bottom_Widget);


        retranslateUi(MerchantWidget);

        QMetaObject::connectSlotsByName(MerchantWidget);
    } // setupUi

    void retranslateUi(QWidget *MerchantWidget)
    {
        MerchantWidget->setWindowTitle(QApplication::translate("MerchantWidget", "Form", nullptr));
        MW_Code_Label->setText(QApplication::translate("MerchantWidget", "\350\253\213\350\274\270\345\205\245\345\225\206\345\256\266\347\267\250\350\231\237\357\274\232", nullptr));
        MW_Close_Button->setText(QApplication::translate("MerchantWidget", "\351\227\234\351\226\211", nullptr));
        MW_Ok_Button->setText(QApplication::translate("MerchantWidget", "\347\242\272\350\252\215", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MerchantWidget: public Ui_MerchantWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MERCHANTWIDGET_H
