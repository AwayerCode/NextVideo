import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import "components"

Window {
    width: 800
    height: 600
    visible: true
    title: qsTr("PowerDelete")
    
    Connections {
        target: fileHandler
        function onFileSelected(filePath) {
            filePathField.text = filePath
        }

        function onLogUpdated(log) {
            textArea.append(log)
        }
    }

    // 使用 ColumnLayout 作为主布局
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 0

        // 上部区域 - 选择文件区
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 100
            color: "#f0f0f0"
            radius: 4

            // 使用 RowLayout 来水平排列输入框和按钮
            RowLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 10
                
                TextField {
                    id: filePathField
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40
                    placeholderText: "Select Process File..."
                    selectByMouse: true
                    font.pixelSize: 14
                    background: Rectangle {
                        color: "white"
                        radius: 4
                        border.color: "#dddddd"
                        border.width: 1
                    }
                }

                CustomButton {
                    id: selectButton
                    Layout.preferredWidth: 100
                    Layout.preferredHeight: 40
                    text: "Select"
                    onClicked: {
                        fileHandler.openFileDialog() 
                    }
                }

                CustomButton {
                    id: deleteButton
                    Layout.preferredWidth: 100
                    Layout.preferredHeight: 40
                    text: "Delete"
                    onClicked: {
                        fileHandler.deleteFile()
                    }
                }
            }
        }

        // 分隔线
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 2
            color: "#dddddd"
            Layout.topMargin: 10
            Layout.bottomMargin: 10
        }

        // 下部区域 - 日志显示区
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "white"
            radius: 4
            border.color: "#dddddd"
            border.width: 1

            ScrollView {
                anchors.fill: parent
                anchors.margins: 10
                clip: true

                TextArea {
                    id: textArea
                    readOnly: true
                    wrapMode: Text.WordWrap
                    selectByMouse: true
                    font.family: "Consolas"
                    font.pixelSize: 12
                    placeholderText: "Operation logs will be displayed here..."
                    background: null
                }
            }
        }
    }
}