import QtQuick 2.0
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.2

import "../components"
Item {


    property alias makeDQOrder : makeDQOrderModel
    property alias rootLayout: columnLayout

    QtObject {
        id: makeDQOrderModel
        property string accountName: "Test002"
        property string askPrice: "12345";
        property string bidPrice: "67890";
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
        spacing: 1;

        Rectangle {
            id: rectangleLotSetting
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

                CHintButton {
                    id: buttonDQLot
                    width: parent.width/2 - 3 * CDimens.pixelDensity;
                    height: parent.height - 3 * CDimens.pixelDensity;
                    Layout.margins: 1 * CDimens.pixelDensity;
                    backgroundColor: "#E0E0E0";
                    btnText:makeDQOrderModel.lot;
                    btnTextColor: "#46BAFD"
                    hint:qsTr("手数","click-Lot")
                    hintColor: "black"
                    onClicked: {
                        inputControler = buttonDQLot;
                        showInputModel("请输入手数",10,1,1.5,0.1);
                    }
                }
            }
        }

        Rectangle {
            id: rectangleBuySell
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
                    id: buttonSell
                    backgroundColor: "#E22018";
                    btnText:makeDQOrderModel.bidPrice ;
                    btnTextColor: "#ffffff"
                    hint:qsTr("卖出","click-Sell")
                    hintColor: "#ffffff"

                }

                CHintButton {
                    width: parent.width/2 - 3 * CDimens.pixelDensity;
                    height: parent.height - 3 * CDimens.pixelDensity;
                    Layout.margins: 1 * CDimens.pixelDensity;
                    id: buttonBuy
                    backgroundColor: "#118E45";
                    btnText:makeDQOrderModel.askPrice;
                    btnTextColor: "#ffffff"
                    hint:qsTr("买入","click-Buy")
                    hintColor: "#ffffff"
                }
            }


        }

        PlaceOpenOrders {
            id: placeOpenOrderLayout
            width: parent.width;
            height: 40 * CDimens.pixelDensity;
            clip: true
        }
    }
}

