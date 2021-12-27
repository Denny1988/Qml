import QtQuick 2.9
import QtQuick.Window 2.0
import QtQuick.Controls 2.3
//import QtQuick.Layouts 1.3
//import QtQuick.Controls 2.2
//import QtQuick.Controls.Material 2.1
//import QtQuick.Controls.Universal 2.1
//import Qt.labs.settings 1.0

//import "components"
//import "screens"
//import "components/dialogs"
//import "../images"

ApplicationWindow {
    id: mainWin
    width: 500
    height: 600
    visible: true
    title: qsTr("Hello World")


    LoginPage {
            width: g_ViewInfo.getScreenWidth()
            height: g_ViewInfo.getScreenHeight()
            anchors.centerIn: parent
        }

    // 1. Loader加载不同组件，实现切换页面的功能
//   Loader{
//       id:myLoader
//       anchors.fill: parent // 弹出的界面都居中显示
//   }
//   Component.onCompleted: myLoader.sourceComponent = loginPage // 一开始显示登录页面

//   // 2. 登录页面-Component
//   Component{
//       id:loginPage
//       LoginPage {
//           width: g_ViewInfo.getScreenWidth()
//           height: g_ViewInfo.getScreenHeight()
//           anchors.centerIn: parent
//       }
//   }

//   // 3.主页面-Component
//   Component{
//       id:mainPage
//       MainPage {
//           width: g_ViewInfo.getScreenWidth()
//           height: g_ViewInfo.getScreenHeight()
//           anchors.centerIn: parent
//       }
//   }
}
