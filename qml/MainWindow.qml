import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: mainWindow
    width: 400
    height: 300
    title: qsTr("主窗口 - 测试窗口")
    visible: true

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        Label {
            text: qsTr("这是一个测试窗口")
            font.pixelSize: 16
            Layout.alignment: Qt.AlignHCenter
        }

        Button {
            text: qsTr("测试按钮")
            Layout.alignment: Qt.AlignHCenter
            onClicked: {
                console.log("按钮被点击")
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#f0f0f0"
            
            Text {
                anchors.centerIn: parent
                text: qsTr("测试区域")
                font.pixelSize: 14
            }
        }
    }
} 