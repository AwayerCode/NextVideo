import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import "components"

Window {
    width: 800
    height: 600
    visible: true
    title: qsTr("DelProcess")
    
    Column {
        anchors.centerIn: parent
        spacing: 1

        Rectangle {
            width: parent.width
            height: 100
            color: "#f0f0f0"

            TextField {
                Layout.fillWidth: true
                placeholderText: "SelectPrecessFile..."
                selectByMouse: true  
            }

            CustomButton {
                Layout.fillWidth: true
                text: "Select"
                onClicked: {
                    console.log("Select")
                }
            }
        }
    }
} 