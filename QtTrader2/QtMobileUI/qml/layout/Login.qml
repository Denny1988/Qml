import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2

import "../components"

Item {
    id: loginView

    Component.onCompleted:{
        console. log("Component.onCompleted： " , accountViewModel);
    }

    Rectangle {
        id: backgroundRect
        color: "#EFEEF3";
        anchors.fill: parent

    }

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        spacing: 10
        anchors.topMargin: 10
        anchors.leftMargin : 20
        anchors.rightMargin : 20

        RowLayout {
            anchors.margins: 2
            width: parent.width
            height: 30;
            Layout.maximumHeight: 40
            Layout.minimumHeight: 20

            Label {
                id: labelLoginName
                text: qsTr("账户","LoginName")
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                width: 60
                height: parent.height
            }

            TextField {
                id: textFieldLoginName
                Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
                horizontalAlignment: Text.AlignRight
                width: parent.width - labelLoginName.width -10
                height:parent.height

            }

        }


        RowLayout {
            anchors.margins: 2
            width: parent.width
            height: 30;
            Layout.maximumHeight: 40
            Layout.minimumHeight: 20

            Label {
                id: labelPassword
                text: qsTr("密码","Password")
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                width: 60
                height:parent.height
            }

            TextField {
                id: textFieldPassword
                Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
                horizontalAlignment: Text.AlignRight
                width: parent.width - labelLoginName.width -10
                height:parent.height
            }

        }

        RowLayout {
            anchors.margins: 2
            width: parent.width
            height: 30;
            Layout.maximumHeight: 40
            Layout.minimumHeight: 20

            CheckBox {
                id: checkBoxSaveName
                text: qsTr("记住账户","checkBoxSaveName")
            }

            CheckBox {
                id: checkBoxSavePassword
                text: qsTr("记住密码","checkBoxSavePassword")
            }

        }

        RowLayout {
            anchors.margins: 2
            anchors.leftMargin: 20
            anchors.rightMargin: 20
            width: parent.width
            height: 30;
            Layout.maximumHeight: 40
            Layout.minimumHeight: 20


            Button {
                id: buttonLogin
                text: qsTr("登陆","buttonLogin")
                anchors.fill: parent
                onClicked:
                {
                    console.log("accountViewModel.startLogin("+ textFieldLoginName.text +"," + textFieldPassword.text +")");
                    accountViewModel.startLogin(textFieldLoginName.text, textFieldPassword.text);
                }
            }

        }

        RowLayout {
            anchors.margins: 2
            anchors.leftMargin: 20
            anchors.rightMargin: 20
            width: parent.width
            height: 30;
            Layout.maximumHeight: 40
            Layout.minimumHeight: 20


            Button {
                id: buttonReg
                text: qsTr("注册","buttonReg")
                anchors.fill: parent
            }
        }

        Rectangle{
            width: parent.width
            height: parent.height / 4;
        }


    }

}
