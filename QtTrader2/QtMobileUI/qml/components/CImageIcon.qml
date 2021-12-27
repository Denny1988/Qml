import QtQuick 2.0
import "."
Item {

    property alias image: cImageIcon;
    property string imageName: "";

    Image {
        id: cImageIcon
        width: parent.width;
        height: parent.height;
        source: "qrc:/images/" + imageName;
        visible: imageName !== "";
    }
}
