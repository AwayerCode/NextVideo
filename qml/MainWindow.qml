import "components"
import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: mainWindow
    width: 800
    height: 600
    title: qsTr("主窗口")
    visible: true
    color: "#f5f5f5"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        Rectangle {
            Layout.fillWidth: true
            height: 60
            color: "#2196F3"
            radius: 8

            Label {
                anchors.centerIn: parent
                text: qsTr("欢迎使用")
                font {
                    pixelSize: 24
                    bold: true
                }
                color: "white"
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "white"
            radius: 8
            border.color: "#e0e0e0"
            border.width: 1

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 15

                RowLayout {
                    Layout.alignment: Qt.AlignHCenter
                    spacing: 20

                    CustomButton {
                        text: qsTr("功能按钮 1")
                        width: 150
                        height: 40
                        onClicked: console.log("功能1被点击")
                    }

                    CustomButton {
                        text: qsTr("功能按钮 2")
                        width: 150
                        height: 40
                        onClicked: console.log("功能2被点击")
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: "#fafafa"
                    radius: 4
                    border.color: "#eeeeee"
                    border.width: 1

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 15
                        spacing: 10

                        Label {
                            text: qsTr("内容区域")
                            font.pixelSize: 16
                            color: "#424242"
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            color: "transparent"

                            Text {
                                anchors.centerIn: parent
                                text: qsTr("这里可以放置主要内容")
                                font.pixelSize: 14
                                color: "#757575"
                            }
                        }
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 30
            color: "#f5f5f5"
            border.color: "#e0e0e0"
            border.width: 1
            radius: 4

            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
                text: qsTr("状态: 就绪")
                font.pixelSize: 12
                color: "#616161"
            }
        }
    }
} 