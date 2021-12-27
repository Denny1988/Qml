import QtQuick 2.7
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0
import "../components"

import "../../js/Helper.js" as JSHelper

Rectangle {

    width: 480
    height: 800
    color: "#F0F0F0";

    signal quotationChanged()

    onQuotationChanged : {
        updateModelData();
        updateListView();
    }

    function isPlaceEnable(id){
        return true;
    }

    function updateModelData(){
        //for test
        try{
            var timeNow = new Date().format("HH:mm:ss");
            for(var index  = 0 ; index < quotationModel.count; index ++ ){
               var qi =  quotationModel.get(index);
                var changeFlag = Math.floor(Math.random()*3)-1;
                var changeValue = parseFloat(qi.price) * (Math.random() * 0.005) * changeFlag;
                qi.price += changeValue;
                qi.quotationTime = timeNow;
                qi.priceChangeFlag = changeFlag;
                qi.quotationAsk = (parseFloat(qi.quotationAsk) + changeValue).toFixed(3);
                qi.quotationBid = (parseFloat(qi.quotationBid) + changeValue).toFixed(3);

            }
        }
        catch(ex)
        {
            console.log("updateModelData()", ex);
        }

    }

    function updateListView(){

    }

    Timer {
            interval: 200; running: true; repeat: true
            onTriggered:
            {
                quotationChanged();
            }
    }

    Component {

        id:  quotationDelegate

        Rectangle {
            id:rowQuotation;

            width: parent.width
            height: 28 * CDimens.pixelDensity;
            clip: true;
            Rectangle {
                id: background
                anchors.fill: parent
                z:0

                LinearGradient {
                    anchors.fill: parent
                    start: Qt.point(0, 0)
                    end: Qt.point(0, parent.height)
                    gradient: Gradient {
                        GradientStop{position: 0.0;color: rowQuotationMouseArea.pressed ? "#e5e5e5": "#FFFFFF"; }
                        GradientStop{position: 0.1; color:rowQuotationMouseArea.pressed ? "#d0d0d0": "#e0e0e0"; }
                        GradientStop{ position: 0.9;color:rowQuotationMouseArea.pressed ? "#e0e0e0": "#FFFFFF"; }
                        GradientStop{position: 1.0;color: rowQuotationMouseArea.pressed ? "#c0c0c0": "#a0a0a0"; }
                    }
                }

            }

            Rectangle {
                id: rectPrice1
                width: parent.width
                height: 20 * CDimens.pixelDensity;
                anchors.top: parent.top;
                anchors.left: parent.left;
                color: "transparent"
                z:1
                Row {
                    id: rowPrice1
                    width: parent.width
                    height: parent.height;
                    z:2
                    Rectangle {
                        width: (parent.width -4 * CDimens.pixelDensity) * 0.4
                        height: parent.height;
                        color: "transparent"
                        Text {
                            width: parent.width
                            height: parent.height * 0.7;
                            anchors.top: parent.top;
                            anchors.left: parent.left;
                            font.pointSize: 16
                            font.bold: true
                            verticalAlignment:Text.AlignVCenter;
                            horizontalAlignment:Text.AlignHCenter;
                            color:"#FF5D27"
                            text: instrumentName
                        }
                        Text {
                            width: parent.width
                            height: parent.height * 0.3;
                            anchors.bottom: parent.bottom;
                            anchors.left: parent.left;
                            font.pointSize: 12
                            verticalAlignment:Text.AlignVCenter;
                            horizontalAlignment:Text.AlignHCenter;
                            color:"#F07040"
                            text: instrumentUnit
                        }

                    }

                    Rectangle {
                        width: (parent.width -4 * CDimens.pixelDensity) * 0.3
                        height: parent.height;
                        anchors.margins: CDimens.pixelDensity
                        color: priceChangeFlag == 0 ? "white" : (priceChangeFlag > 0 ? "#00d743" : "#ff5959")

                        Text {
                            width: parent.width * 0.5
                            height: parent.height * 0.3;
                            anchors.top: parent.top;
                            anchors.left: parent.left;
                            font.pointSize: 12
                            font.bold: true
                            verticalAlignment:Text.AlignVCenter;
                            horizontalAlignment:Text.AlignHCenter;
                            color: priceChangeFlag == 0 ?  "black" : "white"
                            text: qsTr("卖","quotation-sell")
                        }

                        Text {
                            width: parent.width * 0.5
                            height: parent.height * 0.3;
                            anchors.top: parent.top;
                            anchors.right: parent.right;
                            font.pointSize: 20
                            font.bold: true
                            clip:true;
                            verticalAlignment:Text.AlignVCenter;
                            horizontalAlignment:Text.AlignHCenter;
                            color: "yellow"
                            text: priceChangeFlag == 0 ? "" : (priceChangeFlag > 0 ? "\u21e7" : "\u21e9")  //⇧⇩\u21e7
                        }
                        Text {
                            width: parent.width
                            height: parent.height * 0.7;
                            anchors.bottom: parent.bottom;
                            anchors.left: parent.left;
                            font.pointSize: 16
                            font.bold: true
                            verticalAlignment:Text.AlignVCenter;
                            horizontalAlignment:Text.AlignHCenter;
                            color: priceChangeFlag == 0 ?  "black" : "white"
                            text: quotationBid
                        }

                    }

                    Rectangle {
                        width: (parent.width -4 * CDimens.pixelDensity) * 0.3
                        height: parent.height;
                        anchors.margins: CDimens.pixelDensity
                        color: priceChangeFlag == 0 ? "white" : (priceChangeFlag > 0 ? "#00d743" : "#ff5959")

                        Text {
                            width: parent.width * 0.5
                            height: parent.height * 0.3;
                            anchors.top: parent.top;
                            anchors.left: parent.left;
                            font.pointSize: 12
                            font.bold: true
                            verticalAlignment:Text.AlignVCenter;
                            horizontalAlignment:Text.AlignHCenter;
                            color: priceChangeFlag == 0 ?  "black" : "white"
                            text: qsTr("买","quotation-buy")
                        }

                        Text {
                            width: parent.width * 0.5
                            height: parent.height * 0.3;
                            anchors.top: parent.top;
                            anchors.right: parent.right;
                            font.pointSize: 20
                            font.bold: true
                            clip:true;
                            verticalAlignment:Text.AlignVCenter;
                            horizontalAlignment:Text.AlignHCenter;
                            color: "yellow"
                            text: priceChangeFlag == 0 ? "" : (priceChangeFlag > 0 ? "\u21e7" : "\u21e9")  //⇧⇩\u21e7
                        }

                        Text {
                            width: parent.width
                            height: parent.height * 0.7;
                            anchors.bottom: parent.bottom;
                            anchors.left: parent.left;
                            font.pointSize: 16
                            font.bold: true
                            verticalAlignment:Text.AlignVCenter;
                            horizontalAlignment:Text.AlignHCenter;
                            color: priceChangeFlag == 0 ?  "black" : "white"
                            text: quotationAsk
                        }

                    }

                    spacing: 1 * CDimens.pixelDensity;
                }
            }

            Rectangle {
                id: rectPrice2
                width: parent.width
                height: parent.height - rectPrice1.height;
                anchors.bottom: parent.bottom;
                anchors.left: parent.left;
                color: "transparent"
                z:1

                Row {
                    id: row1
                    width: parent.width
                    height: parent.height;
                    z:2
                    Text {
                        width: parent.width * 0.25
                        height: parent.height;
                        text: quotationTime
                        font.pointSize: 12
                        font.bold: true
                        verticalAlignment:Text.AlignVCenter;
                    }

                    Text {
                        width: parent.width  * 0.25
                        height: parent.height;
                        text: qsTr("低/高","LoHi（Bid）")
                        font.pointSize: 12
                        font.bold: true
                        verticalAlignment:Text.AlignVCenter;
                    }

                    Text {
                        width: parent.width * 0.25
                        height: parent.height;
                        text: quotationLow
                        font.pointSize: 12
                        font.bold: true
                        verticalAlignment:Text.AlignVCenter;
                    }

                    Text {
                        width: parent.width * 0.25
                        height: parent.height;
                        text: quotationHigh
                        font.pointSize: 12
                        font.bold: true
                        verticalAlignment:Text.AlignVCenter;
                    }
                }
            }


            MouseArea {
                id: rowQuotationMouseArea
                anchors.fill: parent;

                onClicked: {
                    console. log("rowQuotation onClicked ", instrumentName);
                    if(isPlaceEnable(instrumentId)){
                        if( !settings.qmlUIMode ){
                            cppMakeOrderModel.setInstrument(instrumentName)
                        }
                        stackView.push(Qt.resolvedUrl("../SPlacePanel.qml"))
                    } else {
                        console. log("rowQuotation onClicked() instrumentId " + instrumentId + " not active");
                    }
                }
            }

        }
    }


    ListModel {
        id:  quotationModel
        ListElement {
            instrumentId:"000";
            instrumentName:"现货黄金";
            instrumentCode:"LLGUSD";
            instrumentUnit:"(盎司)"
            price:1226.800;
            quotationTime:"12:00:00";
            quotationHigh:"1250.080";
            quotationLow:"1200.080";
            quotationAsk:"1226.080";
            quotationBid:"1224.080";
            priceChangeFlag:0
        }

        ListElement {
            instrumentId:"001";
            instrumentName:"伦敦白银";
            instrumentCode:"LLSUSD";
            instrumentUnit:"(盎司)"
            price:16.800;
            quotationTime:"12:00:00";
            quotationHigh:"18.080";
            quotationLow:"14.080";
            quotationAsk:"16.280";
            quotationBid:"16.180";
            priceChangeFlag:-1
        }

        ListElement {
            instrumentId:"003";
            instrumentName:"原油期权";
            instrumentCode:"OIL/USD";
            instrumentUnit:"(美元)"
            price:76.800;
            quotationTime:"12:00:00";
            quotationHigh:"90.080";
            quotationLow:"60.080";
            quotationAsk:"78.800";
            quotationBid:"76.800";
            priceChangeFlag:0
        }

        ListElement {
            instrumentId:"005";
            instrumentName:"美元/日元";
            instrumentCode:"USD/JAP";
            instrumentUnit:""
            price:116.800;
            quotationTime:"12:00:00";
            quotationHigh:"150.080";
            quotationLow:"100.080";
            quotationAsk:"116.180";
            quotationBid:"116.080";
            priceChangeFlag:1
        }

        ListElement {
            instrumentId:"007";
            instrumentName:"美元/瑞郎";
            instrumentCode:"USD/CHF";
            instrumentUnit:""
            price:0.950;
            quotationTime:"12:00:00";
            quotationHigh:"1.080";
            quotationLow:"0.880";
            quotationAsk:"0.955";
            quotationBid:"0.950";
            priceChangeFlag:1
        }
    }


    Rectangle {

       width: parent.width;
       height:parent.height - CDimens.footerHeight;
       clip: true;
       z: 1;
       //View
       ListView {
           id: listView
           width: parent.width
           height: parent.height
           visible: true;
           spacing: 2 * CDimens.pixelDensity;
           delegate: quotationDelegate;
           model: quotationModel

       }
    }



}
