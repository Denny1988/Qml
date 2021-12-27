
import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.2
import Qt.labs.settings 1.0
import "palettes"
import "dialogs"
import "."

ApplicationWindow {
///*已弃用，保留代码供参考*/

//    id: cApplicationWindow
//    visible: true
//    width: 480
//    height: 800
//    title: qsTr("Trader App Window")

//    style: ApplicationWindowStyle {
//        background: Rectangle {
//            color: "black"
//        }
//    }

//    // Loading
//    signal loaded()

//    Component.onCompleted:
//        loaded()

//    onLoaded: {
//        // http://doc.qt.io/qt-5/qml-qtquick-window-screen.html
//        // The Screen attached object is valid inside Item or Item derived types, after component completion
//        CDimens.pixelDensity = settings.debugMode ? 6.0 : Screen.logicalPixelDensity;
//        CDimens.initialize(null);
//    }

//    // Settings
//    QtObject {
//        id: settings

//        // configurable
//        property string font: "Ubuntu Mono"
//        property int fontSize: 40
//        property string palette: "Light"
//        property int indentSize: 4
//        property bool debugging: true

//        // internal
//        property bool debugMode: false
//        property double pixelDensity :  6.0
//        property string previousVersion: "0.0.0"
//        property string platformName: Qt.platform.os

//    }

//    property alias settings: settings

//    // Palettes
//    PaletteLoader {
//        id: paletteLoader
//        name: settings.palette
//    }
//    property alias palette: paletteLoader.palette


//    QtObject {
//        id: messageHandler
//        objectName: "messageHandler"
//        signal messageReceived(string message)
//    }

//    property alias messageHandler: messageHandler;

//    //property alias accountModel: accountViewModel;//from cpp

//    signal backPressed()

}
