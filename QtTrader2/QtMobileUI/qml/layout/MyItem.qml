import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
//ApplicationWindow for test
ApplicationWindow {
    id: root
    objectName: "window"
    visible: true
    width: 480
    height: 800

    color: "#161616"
    title: "Qt Quick Extras Demo"


    Text {
        id: accountView
        width: 100; height: 100
        text: accountViewModel.banlance    // invokes Message::author() to get this value

        Component.onCompleted: {
            accountViewModel.banlance = 8000.0  // invokes Message::setAuthor()
        }
    }
}
