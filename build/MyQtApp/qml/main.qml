import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("My Qt App")

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 20

        Label {
            text: backend.getMessage()
            font.pixelSize: 24
            Layout.alignment: Qt.AlignHCenter
        }

        CustomButton {
            Layout.alignment: Qt.AlignHCenter
            onClicked: backend.setMessage("按钮被点击了！")
        }
    }
} 