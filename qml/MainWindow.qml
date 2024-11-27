import QtQuick
import QtQuick.Window
import "components"

Window {
    width: 800
    height: 600
    visible: true
    title: qsTr("最小QML应用")
    
    CustomButton {
        anchors.centerIn: parent
        width: 120
        height: 40
        text: "测试按钮"
    }
} 