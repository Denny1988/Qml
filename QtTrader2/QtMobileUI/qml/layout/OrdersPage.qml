/**
  deprecated，TreeView on mobile has same bugs
*/
import QtQuick 2.0
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4
Item {

    property QtObject orderModel: null;

    signal loaded()

    Component.onCompleted: {
        loaded();
    }

    onLoaded: {
        try{
            if(settings.qmlUIMode){
                orderModel = testModel;
            } else {
                if(settings.platformName == "windows" ) {
                       orderModel = orderTreeModel.model();
                } else {
                       orderModel = testModel;//orderTreeModel.model();
                }
            }
            console.log("OrdersPage orderModel: " + orderModel);
        }
        catch(ex){
            console.log("OrdersPage Component.onCompleted:", ex);
        }
    }

    //Model
    ListModel {
       id: testModel
       Component.onCompleted: {
           testModel.append({"group":"A1","code":1,"subNode":[]})
           testModel.append({"group":"A2","code":2,"subNode":[]})
       }
    }

    Component {

        id:  orderheaderDelegate

        Text {
            color: "#ff0000"
            text: styleData.value

            onTextChanged: {
                print(styleData.value)
            }
        }
    }

    Component {

        id:  orderItemDelegate

        Rectangle {
            color: ( styleData.row % 2 == 0 ) ? "green" : "lightblue"
            anchors {left: parent.left; right: parent.right; }
            height: 20

            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                text: styleData.value // this points to the role we defined in the TableViewColumn below; which one depends on which column this delegate is instantiated for.
                color: styleData.textColor
                elide: styleData.elideMode
            }
        }
    }


    TreeView {

        width: parent.width
        height: 300
        model: orderModel

        headerDelegate: orderheaderDelegate

        itemDelegate: orderItemDelegate

        TableViewColumn {
            role: "group"
            title: "GROUP"
            width: 200
        }


        TableViewColumn {
            role: "code"
            title: "CODE"
            width: 200
        }

        TreeViewStyle{

            backgroundColor :"#808080"
        }

    }



}
