import QtQuick
import QtQuick.Controls

CustomButton {
    id: control
    
    property bool highlighted: false
    
    contentItem: Text {
        text: control.text
        font.pixelSize: 14
        color: control.highlighted ? "#ffffff" : "#5288c1"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
    
    background: Rectangle {
        color: control.highlighted ? "#5288c1" : "transparent"
        radius: 8
        border.color: "#5288c1"
        border.width: 1
        
        // 悬停效果
        Rectangle {
            anchors.fill: parent
            radius: 8
            color: control.hovered ? "#5288c120" : "transparent"
        }
    }
}