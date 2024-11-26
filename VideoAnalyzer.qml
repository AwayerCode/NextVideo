import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia
import QtQuick.Dialogs
import QtQml.WorkerScript

ApplicationWindow {
    visible: true
    width: 1200
    height: 800
    title: "Video Analyzer"

    StackLayout {
        id: stackLayout
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        // 分析器页面
        Item {
            ColumnLayout {
                anchors.fill: parent
                spacing: 10
                margins: 10

                // 文件选择区域
                RowLayout {
                    TextField {
                        id: inputPathField
                        Layout.fillWidth: true
                        placeholderText: "Input Video File Path"
                    }
                    Button {
                        text: "Browse"
                        onClicked: fileDialog.open()
                    }
                    Button {
                        text: "Analyze"
                        onClicked: videoController.analyzeVideo(inputPathField.text)
                    }
                }

                // 信息显示区域
                TabBar {
                    id: infoTabBar
                    Layout.fillWidth: true
                    TabButton { text: "Video Info" }
                    TabButton { text: "Audio Info" }
                    TabButton { text: "Container Info" }
                }

                StackLayout {
                    currentIndex: infoTabBar.currentIndex
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    // 视频信息表格
                    TableView {
                        id: videoInfoTable
                        model: videoController.videoInfo
                        delegate: Rectangle {
                            implicitWidth: 200
                            implicitHeight: 40
                            border.width: 1
                            Text {
                                anchors.centerIn: parent
                                text: display
                            }
                        }
                    }

                    // 音频信息表格
                    TableView {
                        id: audioInfoTable
                        model: videoController.audioInfo
                        delegate: Rectangle {
                            implicitWidth: 200
                            implicitHeight: 40
                            border.width: 1
                            Text {
                                anchors.centerIn: parent
                                text: display
                            }
                        }
                    }

                    // 容器信息表格
                    TableView {
                        id: containerInfoTable
                        model: videoController.containerInfo
                        delegate: Rectangle {
                            implicitWidth: 200
                            implicitHeight: 40
                            border.width: 1
                            Text {
                                anchors.centerIn: parent
                                text: display
                            }
                        }
                    }
                }
            }
        }

        // 滤镜页面
        Item {
            ColumnLayout {
                anchors.fill: parent
                spacing: 10
                margins: 10

                // 视频显示区域
                RowLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    // 原始视频
                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Label { text: "Original Video" }
                        VideoOutput {
                            id: originalVideo
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            source: videoController.originalPlayer
                        }
                    }

                    // 过滤后视频
                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Label { text: "Filtered Video" }
                        VideoOutput {
                            id: filteredVideo
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            source: videoController.filteredPlayer
                        }
                    }
                }

                // 播放控制
                RowLayout {
                    Layout.fillWidth: true
                    Button {
                        id: playPauseButton
                        text: videoController.isPlaying ? "Pause" : "Play"
                        onClicked: videoController.togglePlayPause()
                    }
                    Slider {
                        id: progressSlider
                        Layout.fillWidth: true
                        from: 0
                        to: videoController.duration
                        value: videoController.position
                        onMoved: videoController.seek(value)
                    }
                    Label {
                        text: videoController.timeText
                    }
                }

                // 滤镜控制
                RowLayout {
                    Layout.fillWidth: true
                    Label { text: "Filter:" }
                    ComboBox {
                        id: filterSelect
                        model: ["blur", "sharpen", "edge", "colorbalance", "rotate", "crop"]
                        onCurrentTextChanged: videoController.filterChanged(currentText)
                    }
                    
                    // 滤镜参数区域
                    Loader {
                        id: filterParamsLoader
                        Layout.fillWidth: true
                        sourceComponent: {
                            switch(filterSelect.currentText) {
                                case "blur": return blurParams
                                case "colorbalance": return colorBalanceParams
                                case "rotate": return rotateParams
                                case "crop": return cropParams
                                default: return null
                            }
                        }
                    }

                    Button {
                        text: "Apply Filter"
                        onClicked: videoController.applyFilter()
                    }
                }
            }
        }
    }

    // 顶部标签栏
    header: TabBar {
        id: tabBar
        TabButton { text: "Analyzer" }
        TabButton { text: "Filter Test" }
    }

    // 滤镜参数组件
    Component {
        id: blurParams
        RowLayout {
            Label { text: "Radius:" }
            SpinBox {
                from: 1
                to: 20
                value: 3
                onValueChanged: videoController.setFilterParam("radius", value)
            }
        }
    }

    Component {
        id: colorBalanceParams
        RowLayout {
            Label { text: "R:" }
            Slider {
                from: -100
                to: 100
                onValueChanged: videoController.setFilterParam("red", value)
            }
            Label { text: "G:" }
            Slider {
                from: -100
                to: 100
                onValueChanged: videoController.setFilterParam("green", value)
            }
            Label { text: "B:" }
            Slider {
                from: -100
                to: 100
                onValueChanged: videoController.setFilterParam("blue", value)
            }
        }
    }

    Component {
        id: rotateParams
        RowLayout {
            Label { text: "Angle:" }
            SpinBox {
                from: 0
                to: 360
                onValueChanged: videoController.setFilterParam("angle", value)
            }
        }
    }

    Component {
        id: cropParams
        RowLayout {
            Label { text: "Width:" }
            SpinBox {
                from: 1
                to: 1920
                onValueChanged: videoController.setFilterParam("width", value)
            }
            Label { text: "Height:" }
            SpinBox {
                from: 1
                to: 1080
                onValueChanged: videoController.setFilterParam("height", value)
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: "Select Video File"
        nameFilters: ["Video files (*.mp4 *.avi *.mkv *.mov)", "All files (*)"]
        onAccepted: {
            inputPathField.text = fileDialog.fileUrl
            videoController.loadVideo(fileDialog.fileUrl)
        }
    }
} 