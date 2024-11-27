import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Button {
    id: control
    
    property color primaryColor: "#2196F3"
    property color hoverColor: "#1976D2"
    property color pressColor: "#0D47A1"
    
    contentItem: Text {
        text: control.text
        font.pixelSize: 14
        color: "white"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: 120
        implicitHeight: 40
        color: control.pressed ? pressColor : control.hovered ? hoverColor : primaryColor
        radius: 4
        
        // 添加阴影效果
        layer.enabled: true
        layer.effect: DropShadow {
            transparentBorder: true
            horizontalOffset: 0
            verticalOffset: 2
            radius: 8.0
            samples: 17
            color: "#20000000"
        }
    }

    // 添加鼠标悬停动画
    Behavior on scale {
        NumberAnimation {
            duration: 100
        }
    }

    // 鼠标悬停效果
    HoverHandler {
        onHoveredChanged: {
            if (hovered) {
                control.scale = 1.02
            } else {
                control.scale = 1.0
            }
        }
    }
} 