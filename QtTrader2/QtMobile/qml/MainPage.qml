import QtQuick 2.0
import QtQuick.Controls 2.3

Rectangle {
    id: mainPage
    //anchors.fill: parent
    color: "green"

    Button {
        width: g_ViewInfo.getScreenWidth()/2
        height: g_ViewInfo.getButtonHeight()
        anchors.centerIn: parent
        text: "主页面-返回按钮"
        //onClicked: myLoader.sourceComponent = loginPage // 切换显示登录页面
        onClicked: {
                    // 销毁关闭主页面
                    mainPage.destroy()
                }
    }
}
