import QtQuick 2.0
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.2

import "components"
import "layout"

Item {

    id:placePanelItem
    property alias makeDQOrder : qtMakeOrderModel
    property alias columnLayout: columnLayout

    QtObject {
        id: qtMakeOrderModel
        property string askPrice: "12345";
        property string bidPrice: "67890";
        property string instrumentId: "";
        property string instrumentName: "Instrument Name";
        property string setPrice1: "";
        property string setPrice2: "";
        property double lot: 0;
        property bool isBuy: true;
        property int maxMove: 0;
    }

    Component.onCompleted:
    {
        qtMakeOrderModel.instrumentName = cppMakeOrderModel.getInstrument();
        console.debug("SPlacePanel  onCompleted.");
    }

    Component {
        id: tabViewStyle

        TabViewStyle {

            tabsMovable: true

            tab: Item {
                implicitWidth: 40 * CDimens.pixelDensity
                implicitHeight: 8 * CDimens.pixelDensity

                Text {
                    id: textTitle
                    text: styleData.title
                    font.pointSize: 12
                    anchors.centerIn: parent
                    font.bold: styleData.selected
                    color: styleData.selected ? "#46BAFD" : "#808080"
                }

                Rectangle {
                    id: cHorizontalLine
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: 1
                    color:"#46BAFD"
                    visible: styleData.selected
                }
            }

            frame: Rectangle { color: "steelblue" }
        }
    }


    CToolBar {
        id: toolBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        CLabel {
            id: cInstrumentName
            verticalAlignment: Text.AlignVCenter
            anchors.centerIn: parent
            color: palette.label
            text: qtMakeOrderModel.instrumentName
        }

    }

    ColumnLayout {
        id: columnLayout
        width: parent.width;
        spacing: 2;
        height: (parent.height - toolBar .height );
        anchors.top: toolBar.bottom
        Layout.alignment:Qt.AlignTop

        Rectangle {
            id: rectangleChartImage
            width: parent.width;
            height: parent.height * 0.4;
            clip: true
            Image {
                id: chartImage
                width:parent.width;
                height: parent.height;
                clip: true
                source: "../images/chart_sample2.jpg"
            }
        }

        Rectangle {
            id: rectanglePlace
            width: parent.width;
            height: parent.height*0.6;
            clip: true
            color: "#cccccc"

            TabView {
                id:tabViewPlaceType
                width: parent.width;
                height: parent.height;
                style:tabViewStyle

                Tab {
                    title: qsTr("即市单", "SpotTrade");
                    Rectangle {
                        PlaceDQLayout{
                            width: parent.width;
                            height: parent.height;
                        }
                    }
                }

                Tab {
                    title: qsTr("限价单", "SpotTrade");
                    Rectangle {
                        PlaceLimitLayout {
                            width: parent.width;
                            height: parent.height;
                        }
                    }
                }
            }
        }
    }

    NumberInputModel{
        id: numberInputModel;
        m_mianWindowWidth :  placePanelItem.width;
        m_mianWindowHeight: placePanelItem.height;
        m_max: 100.0;
        m_min: 5.0;
        m_step: 5;
        m_value: 10.0;
        onAboutToHide: {
            inputFinish();
        }
        onClosed: {
            inputFinish();
        }
    }

    property Item inputControler : null;

    signal inputFinish();

    onInputFinish : {
        try{
            if(inputControler != null){
                var inputStr = "" + numberInputModel.m_value;
                if( inputControler.btnText){
                    inputControler.btnText = inputStr;
                } else {
                    inputControler.text = inputStr;
                }

            }
        }catch(ex){
            console.log("onInputFinish:",ex);
        }

    }

    function showInputModel(title,  max,  min,  value,  step)
    {
        numberInputModel.show(title,  max,  min,  value,  step);
    }

}
