import QtQuick 2.4
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Controls.Universal 2.0

Item {
    width: 450
    height: 800

    Rectangle {
        id: rectangle
        x: 0
        y: 0
        width: 450
        height: 80
        color: "#ffffff"

        Rectangle {
            id: rectangle1
            x: 20
            y: 80
            width: 410
            height: 720
            color: "#ffffff"
        }
    }
}
