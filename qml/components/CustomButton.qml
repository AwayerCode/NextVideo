import QtQuick
import QtQuick.Controls

Button {
    id: root
    
    contentItem: Text {
        text: root.text
        color: "white"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
    
    background: Rectangle {
        color: root.pressed ? "#666666" : "#888888"
        radius: 4
    }
} 