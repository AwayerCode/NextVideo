import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts

Window {
    width: 800
    height: 600
    visible: true
    title: qsTr("PowerDelete")
    color: "#17212b"  // Telegram 深色背景色
    
    Connections {
        target: fileHandler
        function onFileSelected(filePath) {
            filePathField.text = filePath
        }

        function onLogUpdated(log) {
            textArea.append(log)
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        // 上部区域 - 选择文件区
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 120
            color: "#242f3d"  // Telegram 卡片背景色
            radius: 10

            RowLayout {
                anchors.fill: parent
                anchors.margins: 15
                spacing: 15
                
                TextField {
                    id: filePathField
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40
                    placeholderText: "Select Process File..."
                    verticalAlignment: TextInput.AlignVCenter
                    selectByMouse: true
                    font.pixelSize: 14
                    color: "#ffffff"
                    placeholderTextColor: "#8596a7"
                    
                    background: Rectangle {
                        color: "#151d27"
                        radius: 8
                        border.color: parent.activeFocus ? "#5288c1" : "transparent"
                        border.width: 1

                        // 添加聚焦动画
                        Behavior on border.color {
                            ColorAnimation { duration: 150 }
                        }
                    }
                }

                Button {
                    id: selectButton
                    Layout.preferredWidth: 100
                    Layout.preferredHeight: 40
                    text: "Select"
                    onClicked: fileHandler.openFileDialog()
                }

                Button {
                    id: startButton
                    Layout.preferredWidth: 100
                    Layout.preferredHeight: 40
                    text: "Start"
                    highlighted: true
                    onClicked: fileHandler.deleteFile()
                }
            }
        }

        // 下部区域 - 日志显示区
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#242f3d"
            radius: 10
            
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 15
                spacing: 10

                // 标题栏
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40
                    color: "transparent"

                    RowLayout {
                        anchors.fill: parent
                        spacing: 10

                        Text {
                            text: "Operation Logs"
                            color: "#ffffff"
                            font.pixelSize: 16
                            font.bold: true
                        }

                        Item { Layout.fillWidth: true }

                        Text {
                            text: "Clear"
                            color: "#5288c1"
                            font.pixelSize: 14
                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: textArea.clear()
                            }
                        }
                    }
                }

                // 分隔线
                Rectangle {
                    Layout.fillWidth: true
                    height: 1
                    color: "#2f3b4b"
                }

                // 日志区域
                ScrollView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true

                    TextArea {
                        id: textArea
                        readOnly: true
                        wrapMode: Text.WordWrap
                        selectByMouse: true
                        font.family: "Consolas"
                        font.pixelSize: 13
                        color: "#ffffff"
                        placeholderText: "Operation logs will be displayed here..."
                        placeholderTextColor: "#8596a7"
                        background: null

                        // 自定义选择颜色
                        selectionColor: "#5288c1"
                        selectedTextColor: "#ffffff"
                    }
                }
            }
        }
    }
}