import QtQuick 2.0
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

import "../components"
Item {

    id: placeLimitLayoutItem

    property alias makeDQOrder : makeDQOrderModel
    property alias rootLayout: columnLayout

    QtObject {
        id: makeDQOrderModel
        property string accountName: "Test002"
        property string askPrice: "123.45";
        property string bidPrice: "123.67";
        property string instrumentId: "";
        property string instrumentName: "";
        property string setPrice1: "";
        property string setPrice2: "";
        property double lot: 0;
        property bool isBuy: true;
        property int maxMove: 0;

    }

    ColumnLayout {
        id: columnLayout
        width: parent.width;
        spacing: 2;

        Rectangle {
            id: rectangleAccountSetting
            width: parent.width;
            height: CDimens.placeHintButtonHeight;
            clip: true

            RowLayout {

                width: parent.width;
                height: parent.height;
                Layout.alignment: Qt.AlignCenter

                CHintButton {
                    width: parent.width/2 - 3 * CDimens.pixelDensity;
                    height: parent.height - 3 * CDimens.pixelDensity;
                    Layout.margins: 1 * CDimens.pixelDensity;
                    id: buttonAccount
                    backgroundColor: "#E0E0E0";
                    btnText:makeDQOrderModel.accountName ;
                    btnTextColor: "#46BAFD"
                    hint:qsTr("账号","click-account")
                    hintColor: "black"

                }

                Rectangle {
                    width: parent.width/2 - 3 * CDimens.pixelDensity;
                    height: parent.height - 3 * CDimens.pixelDensity;
                    Layout.margins: 1 * CDimens.pixelDensity;
                    CSwitcher{
                        id:cSwitcherBuySell
                        width: parent.width;
                        height: parent.height;
                        str1:qsTr("买入","click-Buy")
                        str2:qsTr("卖出","click-Sell")
                        onClicked:{
                            console.log("cSwitcherBuySell Changed index: ", index);
                        }
                    }
                }
            }
        }

        Rectangle {
            id: rectangleLotSetting
            width: parent.width;
            height: CDimens.placeHintButtonHeight;
            clip: true

            RowLayout {

                width: parent.width;
                height: parent.height;
                Layout.alignment: Qt.AlignCenter

                Rectangle {
                    width: parent.width/2 - 3 * CDimens.pixelDensity;
                    height: parent.height - 3 * CDimens.pixelDensity;
                    Layout.margins: 1 * CDimens.pixelDensity;
                    id: rectangleBuySell

                    CSwitcher{
                        id:cSwitcherOpenClose
                        width: parent.width;
                        height: parent.height;
                        str1:qsTr("新单","click-Open")
                        str2:qsTr("平仓","click-Close")
                        onClicked:{
                            console.log("CSwitcher Changed index: ", index);
                        }
                    }
                }

                CHintButton {
                    width: parent.width/2 - 3 * CDimens.pixelDensity;
                    height: parent.height - 3 * CDimens.pixelDensity;
                    Layout.margins: 1 * CDimens.pixelDensity;
                    id: buttonLot
                    backgroundColor: "#E0E0E0";
                    btnText:makeDQOrderModel.lot;
                    btnTextColor: "#46BAFD"
                    hint:qsTr("手数","click-Lot")
                    hintColor: "black"
                    onClicked: {
                        inputControler = buttonLot;
                        showInputModel("请输入手数",20,1,1,1);
                    }
                }
            }
        }

        Rectangle {
            id: rectangleLimitStop
            width: parent.width;
            height: CDimens.placeHintButtonHeight;
            clip: true

            RowLayout {

                width: parent.width;
                height: parent.height;
                Layout.alignment: Qt.AlignCenter

                CHintButton {
                    id: buttonSell
                    width: parent.width/2 - 3 * CDimens.pixelDensity;
                    height: parent.height - 3 * CDimens.pixelDensity;
                    Layout.margins: 1 * CDimens.pixelDensity;
                    backgroundColor: "#E0E0E0";
                    btnText:makeDQOrderModel.bidPrice ;
                    btnTextColor: "#46BAFD"
                    hint:qsTr("限价","click-Limit")
                    hintColor: "black"
                    onClicked: {
                        inputControler = buttonSell;
                        showInputModel("请输入获利价",130.00,123.50,0.05, 124.00);
                    }
                }

                CHintButton {
                    id: buttonBuy
                    width: parent.width/2 - 3 * CDimens.pixelDensity;
                    height: parent.height - 3 * CDimens.pixelDensity;
                    Layout.margins: 1 * CDimens.pixelDensity;
                    backgroundColor: "#E0E0E0";
                    btnText:makeDQOrderModel.askPrice;
                    btnTextColor: "#46BAFD"
                    hint:qsTr("止损","click-Stop")
                    hintColor: "black"
                    onClicked: {
                        inputControler = buttonBuy;
                        showInputModel("请输入止损价",123.00,110.00,0.05, 122.00);
                    }
                }
            }


        }

        Rectangle {
            id: rectangleOpenOrder
            width: parent.width;
            height: 45 * CDimens.pixelDensity;

            PlaceOpenOrders {
                id: placeOpenOrdersLayout
                width: parent.width;
                height: 45 * CDimens.pixelDensity;
                clip: true
            }
        }

        Rectangle {
            id: rectangleSubmit
            width: parent.width;
            height: CDimens.placeHintButtonHeight;

            Button {
                id: placeSubmit
                width: parent.width;
                height: parent.height;
                clip: true
                highlighted: true
                text:qsTr("提交","click-submit")
                Material.accent: Material.Red

            }
        }
    }
}





