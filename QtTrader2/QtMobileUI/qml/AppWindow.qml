
import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3

import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1
import QtQuick.Controls.Universal 2.1

import Qt.labs.settings 1.0

import "./components"
import "./components/palettes"
import "./components/dialogs"

ApplicationWindow {

    id: applicationWindow

    visible: true
    width: 480
    height: 800
    title: qsTr("交易客户端","Trader App Window")

    Settings {
        id: qtLibSettings
        category: "ThemeStyle"
        property string style: "Default";//"Default","Universal", "Material"
    }

    // Loading
    signal loaded()

    Component.onCompleted:{

        loaded() //signal

        console.log("ApplicationWindow Component.onCompleted")
    }


    onLoaded: {

        if(settings.platformName == "android"){
            //settings.debugMode = false;
            //settings.debugging = false;
            //settings.qmlUIMode = false;
            settings.font = "Droid Sans"
        } else if(settings.platformName == "ios"){

        } else if(settings.platformName == "windows") {
            settings.font = "times"
        }

        console.log("settings.qmlUIMode = " + settings.qmlUIMode);
        console.log("settings.palette = " + settings.palette);
        console.log("settings.platformName = " + settings.platformName);
        console.log("load palette = " + settingsPalette);

        CDimens.initialize(settings);

    }

    // Settings
    QtObject {
        id: settings

        // internal
        property string platformName: Qt.platform.os
        property bool debugMode: false
        property bool debugging: false
        property bool qmlUIMode: (availableStyles === 0)//not use cpp mode
        property string previousVersion: "0.0.0"


        // configurable
        property string palette: "Base"


        //other
        property string font: "Times New Roman"
        property int fontSize: 40
        property int indentSize: 4

    }

    property alias settings: settings




    // Palettes
    PaletteLoader {
        id: paletteLoader
        name: settings.palette

        onPaletteLoaded: {
            console.log("PaletteLoader onLoaded Begin settings.palette = " + settings.palette);
            if(qtLibSettings && qtLibSettings.style == "Material"){
                settings.palette = "Light"
            }
            else if(qtLibSettings && qtLibSettings.style == "Universal"){
                settings.palette = "Dark"
            }
            console.log("PaletteLoader onLoaded End settings.palette = " + settings.palette);
        }

    }


    property alias settingsPalette: paletteLoader.palette


    QtObject {
        id: messageHandler
        objectName: "messageHandler"
        signal messageReceived(string message)
    }

    property alias messageHandler: messageHandler;

    //property alias accountModel: accountViewModel;//from cpp

    signal backPressed()


    header: ToolBar {
        Material.foreground: "white"

        RowLayout {
            spacing: 20
            anchors.fill: parent

            ToolButton {
                contentItem: Image {
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: stackView.depth > 1 ? "../images/back.png" : "../images/drawer.png"
                }
                onClicked: {
                    if (stackView.depth > 1) {
                        stackView.pop()
                    } else {
                        drawer.open()
                    }
                }
            }

            Label {
                id: titleLabel
                text: "Trader App"
                font.pointSize: 16
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }

            ToolButton {
                contentItem: Image {
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: "../images/menu.png"
                }
                onClicked: optionsMenu.open()

                Menu {
                    id: optionsMenu
                    x: parent.width - width
                    transformOrigin: Menu.TopRight

                    MenuItem {
                        text: "Settings"
                        onTriggered: settingsDialog.open()
                    }
                    MenuItem {
                        text: "About"
                        onTriggered:{
                            stackView.push(Qt.resolvedUrl("screens/ScreenInformation.qml"))
                        }
                    }
                    MenuItem {
                        text: "Exit"
                        onTriggered: {
                            Qt.quit();
                        }
                    }
                }
            }
        }
    }

    Dialog {
        id: settingsDialog
        x: Math.round((applicationWindow.width - width) / 2)
        y: Math.round(applicationWindow.height / 6)
        width: Math.round(Math.min(applicationWindow.width, applicationWindow.height) / 3 * 2)
        modal: true
        focus: true
        title: "Settings"

        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: {
            qtLibSettings.style = styleBox.displayText
            console.log("onAccepted: styleBox.displayText= " + styleBox.displayText);
            settingsDialog.close()
        }
        onRejected: {
            styleBox.currentIndex = styleBox.styleIndex
            settingsDialog.close()
        }

        contentItem: ColumnLayout {
            id: settingsColumn
            spacing: 20

            RowLayout {
                spacing: 10

                Label {
                    text: "Style:"
                }

                ComboBox {
                    id: styleBox
                    property int styleIndex: -1
                    model: availableStyles
                    Component.onCompleted: {
                        console.log("styleBox: availableStyles = " + availableStyles +" qtLibSettings.style = "  + qtLibSettings.style);
                        styleIndex = find(qtLibSettings.style, Qt.MatchFixedString)
                        if (styleIndex !== -1)
                            currentIndex = styleIndex

                    }
                    Layout.fillWidth: true
                }
            }

            Label {
                text: qsTr("需要重启生效","Restart required")
                color: "#e41e25"
                opacity: styleBox.currentIndex !== styleBox.styleIndex ? 1.0 : 0.0
                horizontalAlignment: Label.AlignHCenter
                verticalAlignment: Label.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }

    //
    Drawer {
        id: drawer
        width: Math.min(applicationWindow.width, applicationWindow.height) / 3 * 2
        height: applicationWindow.height
        interactive: stackView.depth === 1

        ListView {
            id: listView

            focus: true
            currentIndex: -1
            anchors.fill: parent

            delegate: ItemDelegate {
                width: parent.width
                text: model.title
                highlighted: ListView.isCurrentItem
                onClicked: {
                    listView.currentIndex = index
                    if(model.source.indexOf("qrc:") === 0 || settings.platformName != "windows"){
                        stackView.push(Qt.resolvedUrl(model.source))
                    } else {
                        stackView.push(model.source)
                    }
                    drawer.close()
                }
            }

            model: ListModel {
                ListElement { title: "TestNumberInput"; source: "./test/TestNumberInput.qml"; }
                ListElement { title: "TestNumberInput2"; source: "qrc:/qml/test/TestNumberInput2.qml"; }
            }

            ScrollIndicator.vertical: ScrollIndicator { }
        }
    }

}
