import QtQuick 2.7
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.2

import "../components"

Item {

    width: 480
    height: 480


    //instrumentGroupsModel for instrumentGroups listView
    ListModel {
        id: instrumentGroupsModel
        Component.onCompleted: {
            try{

                instrumentGroupsModel.append({"insCode":"USD/CAD", "isExpand":true, "isChecked":false, "lots":"买4 卖5", "price":"123.45/67", "tradePLFloat":"40.00", "subOrders":[]});
                var ins0 = instrumentGroupsModel.get(0);
                ins0.subOrders.append({"insCode":"USD/CAD", orderId:"USDCAD01", isVisible:true, "isChecked":false,  time:"18-06-04", lot:"4", isBuy:true, price:"123.00", "tradePLFloat":"50.00"});
                ins0.subOrders.append({"insCode":"USD/CAD", orderId:"USDCAD02", isVisible:true, "isChecked":false, time:"18-06-04", lot:"5", isBuy:false,price:"123.40", "tradePLFloat":"-10.00"});
                ins0.lots = getInstrumentGroupLotString(ins0);

                for(var i = 1; i< 5; i++){
                    var insCode = "Test0" +i;
                    instrumentGroupsModel.append({"insCode": insCode, "isExpand":true, "isChecked":false, "lots":"", "price":"123.45/6" + i, "tradePLFloat": i + "0.0", "subOrders":[]});
                    var insI = instrumentGroupsModel.get(i);
                    insI.subOrders.append({"insCode": insCode, orderId:"order" + i, isVisible:true, "isChecked":false,  time:"18-06-04", lot:"" + (i), isBuy:true, price:"123.00", "tradePLFloat":i + "0.0"});
                    insI.subOrders.append({"insCode": insCode, orderId:"order" + (i+1), isVisible:true, "isChecked":false,  time:"18-06-14", lot:"" + (i+2), isBuy:false, price:"123.45", "tradePLFloat":"0.00"});
                    insI.subOrders.append({"insCode": insCode, orderId:"order" + (i+2), isVisible:true, "isChecked":false,  time:"18-06-24", lot:"" + (i+1), isBuy:false, price:"123.35", "tradePLFloat": (-i-1) + ".00"});

                    insI.lots =getInstrumentGroupLotString(insI);
                }

            }catch(ex){
                console.log("WorkingOrders.initOrders:",ex);
            }

        }
    }

    signal instrumentExpanded(var insCode, var isExpand);

    signal openOrderChecked(var insCode, var orderId, var isChecked);


    onInstrumentExpanded: {
        console.log("onInstrumentExpanded: insCode =" + insCode +",isExpand = " + isExpand);
        for(var index = 0; index < instrumentGroupsModel.count; index ++){
            var instrument = instrumentGroupsModel.get(index);
            if(instrument.insCode === insCode)
            {
                instrument.isExpand = isExpand;
                for(var j = 0; j < instrument.subOrders.count; j ++){
                    var subOrderJ = instrument.subOrders.get(j);
                    subOrderJ.isVisible = isExpand;
                    console.log("subOrderJ orderId =" + subOrderJ.orderId +",isExpand = " + isExpand);
                }
                return ;
            }
        }
    }

    onOpenOrderChecked: {
        console.log("onOpenOrderChecked: insCode =" + insCode +",orderId = " + orderId + ",isChecked = " + isChecked);
        for(var index = 0; index < instrumentGroupsModel.count; index ++){
            var instrument = instrumentGroupsModel.get(index);
            if(instrument.insCode === insCode)
            {
                instrument.isChecked = isChecked;
                for(var j = 0; j < instrument.subOrders.count; j ++){
                    var subOrder = instrument.subOrders.get(j);
                    if(subOrder.orderId === orderId){
                        subOrder.isChecked = isChecked;
                    }
                }
                instrument.lots = getInstrumentGroupLotString(instrument);
                return ;
            }
        }
    }

    function isPlaceEnable(id){
        return true;
    }

    function getInstrumentGroup(insCode){
        for(var index = 0; index < instrumentGroupsModel.count; index ++){
            var instrument = instrumentGroupsModel.get(index);
            if(instrument.insCode === insCode)
            {
                return instrument;
            }
        }
        return undefined;
    }

    function getSubOrder(insCode, orderId){
        for(var index = 0; index < instrumentGroupsModel.count; index ++){
            var instrument = instrumentGroupsModel.get(index);
            if(instrument.insCode === insCode)
            {
                for(var j = 0; j < instrument.subOrders.count; j ++){
                    var subOrder = instrument.subOrders.get(j);
                    if(subOrder.orderId === orderId){
                        return subOrder;
                    }
                }
            }
        }
        return undefined;
    }

    function getInstrumentGroupLotString(instrument){
        var ret = "";
        var buyLot = 0;
        var sellLot = 0;

        for(var j = 0; j < instrument.subOrders.count; j ++){
            var subOrder = instrument.subOrders.get(j);
            if(instrument.isChecked && subOrder.isChecked ||  !instrument.isChecked){
                if(subOrder.isBuy){
                    buyLot += parseFloat(subOrder.lot);
                } else {
                    sellLot += parseFloat(subOrder.lot);
                }
            }
        }
        if(instrument.isChecked){
            ret += "平仓";
            if(buyLot > 0){
                ret += " B" + buyLot;
            }
            if(sellLot > 0){
                ret += " S" + sellLot;
            }
        } else {
            ret += "买" + buyLot;
            ret += " 卖" + sellLot;
        }

        return ret;
    }

    //subOrdersDelegate of instrumentGroup for subOrder Repeater
    Component {
        id: subOrdersDelegate
        Rectangle {
            id:subOrderRectangle
            width: parent.width;
            height:(isVisible ? CDimens.listLineHeight : 0);
            clip:true;

            Rectangle {
                id: cHorizontalSeparator
                anchors.bottom: parent.bottom
                width: parent.width;
                height: Math.max(1, Math.round(0.3 * CDimens.pixelDensity))
                color: settingsPalette.separator
                visible: isVisible;
            }

            Row {
                id:subOrders
                width: parent.width;
                height: parent.height - cHorizontalSeparator.height;
                z:1
                visible: isVisible;

                CCheckBox {
                    id: subOrderTime
                    width: parent.width * 0.3
                    height: parent.height
                    text: time
                    onCheckedChanged:
                    {
                        openOrderChecked(insCode, orderId, checked);
                    }
                }

                Text {
                    id: subOrderLot
                    width: parent.width * 0.2
                    height: parent.height
                    text: (isBuy ? "B" : "S") + lot
                    color: (isBuy ? "#00d743" : "#ff5959")
                    verticalAlignment: Text.AlignVCenter
                }

                Text {
                    id: subOrderPrice
                    width: parent.width * 0.2
                    height: parent.height
                    text: price
                    verticalAlignment: Text.AlignVCenter
                }

                Text {
                    id: subOrderPLFloat
                    width: (parent.width* 0.3 -10 * CDimens.pixelDensity )
                    height: parent.height
                    text: tradePLFloat
                    color: ( parseFloat(tradePLFloat) > 0  ? "#00d743" : "#ff5959")
                    verticalAlignment: Text.AlignVCenter
                }

                CIcon {
                    id: cDropIcon
                    width: 10 * CDimens.pixelDensity
                    height: parent.height
                    //font.pointSize: 10
                    text: "\u25b7" // ▷ \u25ba
                }
            }

            MouseArea {
                width: parent.width * 0.5;
                height:parent.height;
                anchors.right: parent.right;
                anchors.top: parent.top;
                z:1
                onClicked:
                {
                    console.log("subOrder onClicked begin");
                    stackView.push(Qt.resolvedUrl("./OpenPositionDetail.qml"),
                                    {
                                        instrumentCode:insCode,
                                        containerQML:mainRectangle,
                                        openPosition: getSubOrder(insCode, orderId)
                                    });
                    console.log("subOrder onClicked show insCode = " +  insCode + ", orderId = " +  orderId);
                }
            }
        }
    }

    //instrumentGroupDelegate for instrumentGroup listView
    Component {
        id: instrumentGroupDelegate
        Column {

            width: parent.width
            Rectangle {
                id : groupHeader;
                color:settingsPalette.background
                width: parent.width
                height: CDimens.listLineHeight

                Rectangle {
                    id: cinstrumentGroupSeparator
                    anchors.bottom: parent.bottom
                    width: parent.width;
                    height: Math.max(1, Math.round(0.3 * CDimens.pixelDensity))
                    color: settingsPalette.separator
                    z:4
                }

                Row {
                    width: parent.width
                    height: parent.height
                    z:2
                    CLabel {
                        id: instrumentCode
                        width: parent.width * 0.3
                        height: parent.height
                        text: insCode
                        font.pointSize: 13
                        verticalAlignment: Text.AlignVCenter
                    }

                    Rectangle {
                        id: rectangleLots
                        width: parent.width * 0.2
                        height: parent.height
                        color:  settingsPalette.background
                        CLabel {
                            id: instrumentLots
                            anchors.fill: parent
                            text: lots
                            font.pointSize: 13
                            font.bold: true
                            verticalAlignment: Text.AlignVCenter
                        }

                        MouseArea {
                            anchors.fill: parent
                            onPressed:
                            {
                                console.log("Pressed instrumentLots")
                                parent.color = settingsPalette.button
                            }
                            onReleased:{
                                console.log("Released instrumentLots");
                                parent.color = settingsPalette.background
                            }
                            onClicked:
                            {
                                if(isChecked){
                                    console. log("rowQuotation onClicked ", insCode);
                                    if(isPlaceEnable(insCode)){
                                        if( !settings.qmlUIMode ){
                                            cppMakeOrderModel.setInstrument(insCode)
                                        }
                                        stackView.push(Qt.resolvedUrl("../SPlacePanel.qml"))
                                    } else {
                                        console. log("rowQuotation onClicked() insCode " + insCode + " not active");
                                    }
                                }
                            }
                        }
                    }


                    CLabel {
                        id: instrumentPrice
                        width: parent.width * 0.2
                        height: parent.height
                        text: price
                        font.pointSize: 13
                        verticalAlignment: Text.AlignVCenter
                    }

                    CLabel {
                        id: instrumentPLFloat
                        width: (parent.width * 0.3 -10 * CDimens.pixelDensity )
                        height: parent.height
                        text: tradePLFloat
                        font.pointSize: 13
                        verticalAlignment: Text.AlignVCenter
                    }

                    CIcon {
                        id: cDropIcon
                        width: 10 * CDimens.pixelDensity
                        height: parent.height
                        font.pointSize: 10
                        text: isExpand ?  "\u25bc":"\u25ba" // ▼ ►
                        color: "#F04444"
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    z:1
                    onClicked:
                    {
                        isExpand = !isExpand;
                        //instrumentGroupsModel.setProperty(index, "const", !isExpand );
                        instrumentExpanded(insCode, isExpand);
                        console.log("instrumentHeader onClicked IsExpand = " +  isExpand);
                    }
                }
            }
            Repeater {
                model: subOrders
                delegate: subOrdersDelegate
            }
        }
    }



    Rectangle {
        id:mainRectangle
        width: parent.width;
        height:parent.height;

        Rectangle {
            id:orderHeaderBar
            width: parent.width;
            height: CDimens.headerTitleBarHeight;
            visible: true;
            z: 2 //top to the listview
            Rectangle {
                color: settingsPalette.headerTitleBarBackground
                anchors.fill: parent;
                z:0
            }
            Row {
                z:2
                width: parent.width;
                height: parent.height;

                Text {
                    width: parent.width * 0.3
                    height: parent.height
                    text: qsTr("交易日", "Order-tradeDay")
                    verticalAlignment: Text.AlignVCenter
                    color: settingsPalette.headerTitleBarText
                }

                Text {
                    width: parent.width * 0.2
                    height: parent.height
                    text: qsTr("数量", "Order-lot")
                    verticalAlignment: Text.AlignVCenter
                    color: settingsPalette.headerTitleBarText
                }
                Text {
                    width: parent.width *0.2
                    height: parent.height
                    text: qsTr("价格", "Order-price")
                    verticalAlignment: Text.AlignVCenter
                    color: settingsPalette.headerTitleBarText
                }
                Text {
                    width: parent.width * 0.3
                    height: parent.height
                    text: qsTr("盈亏", "Order-PLFloat")
                    verticalAlignment: Text.AlignVCenter
                    color: settingsPalette.headerTitleBarText
                }
            }
        }

        Rectangle {

           anchors.top: orderHeaderBar.bottom
           width: parent.width;
           height:parent.height - orderHeaderBar.height - CDimens.footerHeight;
           clip: true;
           z: 1;
           //View
           ListView{
               id:instrumentGroupListView
               anchors.fill: parent;
               model:instrumentGroupsModel
               delegate: instrumentGroupDelegate
           }
        }

    }
}
