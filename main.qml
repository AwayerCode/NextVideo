import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: "AI Chat Application"

    ColumnLayout {
        anchors.fill: parent
        spacing: 10
        
        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            
            ListView {
                id: chatView
                model: chatController.messages
                delegate: Rectangle {
                    width: parent.width
                    height: messageText.height + 20
                    color: model.isUser ? "#e0e0e0" : "#f5f5f5"
                    
                    Text {
                        id: messageText
                        text: model.message
                        wrapMode: Text.WordWrap
                        width: parent.width - 20
                        anchors.centerIn: parent
                    }
                }
            }
        }
        
        RowLayout {
            Layout.fillWidth: true
            Layout.margins: 10
            
            TextField {
                id: messageInput
                Layout.fillWidth: true
                placeholderText: "输入消息..."
                onAccepted: sendButton.clicked()
            }
            
            Button {
                id: sendButton
                text: "发送"
                onClicked: {
                    if (messageInput.text.trim() !== "") {
                        chatController.sendMessage(messageInput.text)
                        messageInput.text = ""
                    }
                }
            }
        }
    }
} 