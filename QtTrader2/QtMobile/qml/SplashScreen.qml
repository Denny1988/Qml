import QtQuick 2.0

Item {
    id:splashScreen
    width: g_ViewInfo.getScreenWidth()
    height: g_ViewInfo.getScreenHeight()

    Image {
        id: rocket
        anchors.fill: parent
        width: g_ViewInfo.getScreenWidth()
        height: g_ViewInfo.getScreenHeight()
        fillMode: Image.PreserveAspectCrop
        source: "../Resources/Images/background.png"
    }

    AnimatedImage{
            id: animated
            x: (g_ViewInfo.getScreenWidth() - 500)/2
            y: g_ViewInfo.getScreenHeight()-g_ViewInfo.getButtonHeight()-10
 //           width: 500
 //           height: g_ViewInfo.getButtonHeight()/2
            source: "qrc:/Resources/Images/Startup.gif"

    }

}
