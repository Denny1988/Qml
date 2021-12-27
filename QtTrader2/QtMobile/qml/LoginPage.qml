import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4

Item {
    id: loginPage
    width: g_ViewInfo.getScreenWidth()
    height: g_ViewInfo.getScreenHeight()

    Image {
        id: rocket
        anchors.fill: parent
        width: g_ViewInfo.getScreenWidth()
        height: g_ViewInfo.getScreenHeight()
        fillMode: Image.PreserveAspectCrop
        source: merchantManager.getImagePath("Login.png")
    }


    Rectangle{
        id:login_Header_Rectangle
        x:0
        y:0
        width: g_ViewInfo.getScreenWidth()
        height: g_ViewInfo.getButtonHeight()*1.5
        color: "#bba363"

        Text {
            id: login_Language_Text
            x: g_ViewInfo.getMarginHeight()
            y:0
            width: (g_ViewInfo.getScreenWidth() - 2*(20 * g_ViewInfo.getPixelSizeRatio()))/5
            height: parent.height
            text: qsTr("English")
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            color: "white"

            MouseArea{
                anchors.fill: parent
                onReleased: {
                    console.log("English button login_Language_Text click");
                }
            }
        }

        Button{
            id:login_LanguageSign_Button
            anchors.top: parent.top
            anchors.left: login_Language_Text.right
            width: login_Language_Text.width/2
            height: parent.height
            background: Image {
                fillMode: Image.PreserveAspectFit
                source: merchantManager.getImagePath("ArrowOpenForBlack.png")
            }
            onClicked: {
                console.log("English button login_LanguageSign_Button click");
            }
        }

        Image{
            id:login_Logo_Image
            height: parent.height - 2*g_ViewInfo.getMarginHeight()
            width: login_Logo_Image.height
            anchors.centerIn: parent
            fillMode: Image.PreserveAspectCrop
            source: merchantManager.getImagePath("256.png")
        }

        Button{
            id:login_Setting_Button
            width: g_ViewInfo.getButtonHeight()*0.7
            height: g_ViewInfo.getButtonHeight()*0.7
            x:g_ViewInfo.getScreenWidth() - login_Setting_Button.width - 10
            y:(parent.height - login_Setting_Button.height)/2
            background: Image {
                source: merchantManager.getImagePath("menu.png")
            }
            onClicked: {
                console.log("Setting button login_Setting_Button click");
            }
        }

    }

    Rectangle{
        id:login_Content_Rectangle
        anchors.top: login_Header_Rectangle.bottom
        anchors.leftMargin: 20 * g_ViewInfo.getPixelSizeRatio()
        anchors.rightMargin:  20 * g_ViewInfo.getPixelSizeRatio()
        anchors.left: parent.left
        width: g_ViewInfo.getScreenWidth() - 40*g_ViewInfo.getPixelSizeRatio()
        height: g_ViewInfo.getScreenHeight() - login_Header_Rectangle.height
        color: "transparent"

        Text{
            id:login_SayHello_Text
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            height: g_ViewInfo.getButtonHeight()*2
            //text: g_Languages.Tr("GoodMorning", "Login")
            text: "GoodMorning"
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 20
            color: "white"
        }

        Rectangle {
            id:login_placeholder1_Rectangle
            anchors.top: login_SayHello_Text.bottom
            anchors.left: parent.left
            width: parent.width
            height: 30
        }

        Rectangle {
            id:login_UserName_Rectangle
            visible: true
            anchors.top: login_placeholder1_Rectangle.bottom
            anchors.left: parent.left
            width: parent.width
            height: g_ViewInfo.getButtonHeight()
            antialiasing: true
            gradient: Gradient{
                GradientStop { position: 0.0; color: "#90c6b9" }
                GradientStop { position: 0.55; color: "#6d9b90" }
                GradientStop { position: 0.95; color: "#4f7069" }
                GradientStop { position: 1.0; color: "#4a6b64" }
            }

            Rectangle {
                anchors.fill: parent
                anchors.top: parent.top
                anchors.left: parent.left
                width: parent.width
                height: g_ViewInfo.getButtonHeight() - 3
                color: "white"

                TextEdit{
                    id:login_UserName_TextEdit
                    anchors.centerIn: parent
                    width: parent.width
                    height: parent.height
                    //color: "#ffffff"
                    text: "Dem001"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }

        }


        Rectangle {
            id:login_Password_Rectangle
            visible: true
            anchors.top: login_UserName_Rectangle.bottom
            anchors.left: parent.left
            width: parent.width
            height: g_ViewInfo.getButtonHeight()
            antialiasing: true
            gradient: Gradient{
                GradientStop { position: 0.0; color: "#90c6b9" }
                GradientStop { position: 0.55; color: "#6d9b90" }
                GradientStop { position: 0.95; color: "#4f7069" }
                GradientStop { position: 1.0; color: "#4a6b64" }
            }

            TextEdit{
                id:login_Password_TextEdit
                anchors.top: parent.top
                anchors.left: parent.left
                width: parent.width
                height: g_ViewInfo.getButtonHeight()
                //color: "#ffffff"
                text: "12345678"
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            Button{
                id:login_SavePassword_Button
                anchors.top: login_UserName_TextEdit.bottom
                anchors.right: parent.right
                width: g_ViewInfo.getButtonHeight()*1.5
                height: g_ViewInfo.getButtonHeight()
                //text: g_Languages.Tr("SavePassword", "Login")
                text: "SavePassword"
                background: Rectangle {
                               color: "transparent"
                               }

            }
        }




        Rectangle {
            id:login_placeholder2_Rectangle
            anchors.top: login_Password_Rectangle.bottom
            anchors.left: parent.left
            width: parent.width
            height: 30
        }

        Button {
            id:login_Button
            anchors.top: login_placeholder2_Rectangle.bottom
            anchors.left: parent.left
            width: parent.width
            height: g_ViewInfo.getButtonHeight()
            text: "登录"
            onClicked: {
                        // 隐藏登录页面
                        loginPage.visible = false // 不能销毁，否则下面的"主页面"也会跟随销毁，则后面
                        // 点击"主页面-关闭按钮"，将无法销毁关闭"主页面"

                        // 在主窗口（mainWin）上显示主页面
                        var compMainPage = Qt.createComponent("MainPage.qml")
                        .createObject(mainWin, {x:0, y:0, width:g_ViewInfo.getScreenWidth(), height:g_ViewInfo.getButtonHeight()});
                    }
            //onClicked: myLoader.sourceComponent = mainPage // 切换显示主页面
        }

    }



}
