import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import QtQuick.Controls.Material 2.2
import "../components"
import "../../js/Helper.js" as JSHelper

Item {
    property int m_decimal : 2;
    property string m_textLabel: "";
    property double m_maximumValue: 100;
    property double m_minimumValue: 0;
    property double m_value: 50;
    property double m_stepSize: 1;

    property alias mainRectWidth: mainRect.width;
    property alias mainRectHeight: mainRect.height;
    property alias numberSlider : slider

//    Component
//    {
//        id: m_SliderStyle
//        SliderStyle{

//            groove: Rectangle
//            {
//                implicitHeight:8
//                color:"gray"
//                radius:8
//            }

//            handle: Rectangle{
//                anchors.centerIn: parent;
//                color:"lightgray";
//                border.color: "gray";
//                border.width: 2;
//                width: 34;
//                height: 34;
//                radius: 12;
//            }
//        }
//    }

    function changeValue(isUp)
    {
        try{
            var doubleValue = isUp ? slider.value + m_stepSize : slider.value - m_stepSize ;
            if(doubleValue > m_maximumValue){
              doubleValue = m_maximumValue;
            }
            else if(doubleValue < m_minimumValue){
                doubleValue = m_minimumValue;
            }
            cTextField.text = doubleValue.toFixed(m_decimal);
            slider.value = doubleValue;
            m_value = doubleValue;
        }catch(ex){
            console.log("onTextChanged error", ex);
        }
    }

    Rectangle
    {
        id: mainRect
        width: 100 * CDimens.pixelDensity;
        height: 60 * CDimens.pixelDensity;
        anchors.centerIn: parent;
        //color: "#d0d0d0"
        anchors.verticalCenterOffset: 0
        anchors.horizontalCenterOffset: 0

        ColumnLayout {
            id: settingsColumn
            width: parent.width
            height: parent.height
            spacing: 2

            Rectangle{
                width: parent.width ;
                height: parent.height * 0.2;
                CLabel {
                    id: inputLabel
                    text: m_textLabel == "" ?  ("Max:" + m_maximumValue + ",Min:" + m_minimumValue) : m_textLabel;
                }
            }

            Rectangle{
                width: parent.width;
                height: parent.height * 0.25;
                Material.theme: Material.Light
                CTextField {
                    id: cTextField
                    clip: true
                    text: "" + m_value;

//                    validator: DoubleValidator{
//                         id:rdecimal;
//                         decimals: m_decimal;
//                         bottom: m_minimumValue;
//                         top: m_maximumValue ;
//                         notation:DoubleValidator.StandardNotation
//                    }
                    onTextChanged : {
                        try{
                            var doubleValue = parseFloat(cTextField.text);
                            if(doubleValue > m_maximumValue){
                              doubleValue = m_maximumValue;
                            }
                            else if(doubleValue < m_minimumValue){
                                doubleValue = m_minimumValue;
                            }
                            cTextField.text = doubleValue.toFixed(m_decimal);
                            slider.value = doubleValue;
                        }catch(ex){
                            console.log("onTextChanged error", ex);
                        }
                    }
                }
            }

            Rectangle{
                width: parent.width;
                height: parent.height * 0.25;
                Material.theme: Material.Light

                RowLayout {

                    width: parent.width;
                    height: parent.height;
                    Rectangle{
                        width: parent.height;
                        height: parent.height;
                        CIconButton{
                            width: Math.min(10 * CDimens.pixelDensity, parent.width) ;
                            height: Math.min(10 * CDimens.pixelDensity, parent.height) ;
                            anchors.centerIn: parent;
                            text:"-"
                            onClicked:
                            {
                                changeValue(false);
                            }
                        }
                    }

                    Rectangle{
                        width: parent.width - 2 * CDimens.pixelDensity -2 * parent.height;
                        height: parent.height;
                        Slider{
                            id: slider
                            width: parent.width - 2 * CDimens.pixelDensity;
                            height: parent.height - 4;
                            anchors.centerIn: parent;
                            stepSize: m_stepSize;
                            value: m_value;
                            maximumValue: m_maximumValue;
                            minimumValue: m_minimumValue;
                            tickmarksEnabled: true;

                            onValueChanged: {
                               m_value = value;
                               cTextField.text = value.toFixed(m_decimal);
                            }
                            //style: m_SliderStyle
                        }
                    }

                    Rectangle{
                        width: parent.height;
                        height: parent.height;
                        CIconButton{
                            width: Math.min(10 * CDimens.pixelDensity, parent.width) ;
                            height: Math.min(10 * CDimens.pixelDensity, parent.height) ;
                            anchors.centerIn: parent;
                            text:"+"
                            onClicked:
                            {
                                changeValue(true);
                            }
                        }
                    }
                }
            }

        }
    }
}
