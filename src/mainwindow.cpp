#include "mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ffmpeg(nullptr)
    , m_originalPlayer(nullptr)
    , m_filteredPlayer(nullptr)
    , m_originalVideo(nullptr)
    , m_filteredVideo(nullptr)
    , m_positionTimer(nullptr)
    , m_isPlaying(false)
    , m_stackedWidget(nullptr)
    , m_analyzerPage(nullptr)
    , m_filterPage(nullptr)
{
    try {
        // 首先创建所有需要的组件
        m_ffmpeg = new FFmpegWrapper();
        
        // 创建视频播放器和视频窗口部件
        m_originalPlayer = new QMediaPlayer(this);
        m_filteredPlayer = new QMediaPlayer(this);
        
        // 创建视频显示窗口
        m_originalVideo = new QVideoWidget(this);
        m_filteredVideo = new QVideoWidget(this);
        
        // 设置视频窗口的属性
        m_originalVideo->setMinimumSize(480, 320);
        m_filteredVideo->setMinimumSize(480, 320);
        m_originalVideo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        m_filteredVideo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        
        // 设置视频输出
        m_originalPlayer->setVideoOutput(m_originalVideo);
        m_filteredPlayer->setVideoOutput(m_filteredVideo);
        
        // 创建定时器
        m_positionTimer = new QTimer(this);
        m_positionTimer->setInterval(50);
        
        // 创建堆叠窗口部件
        m_stackedWidget = new QStackedWidget(this);
        m_analyzerPage = new QWidget(this);
        m_filterPage = new QWidget(this);
        
        // 然后设置UI
        setupUI();
        setWindowTitle("Video Analyzer");
        
        // 连接信号
        connect(m_positionTimer, &QTimer::timeout, this, &MainWindow::updateVideoPosition);
        connect(m_originalPlayer, &QMediaPlayer::positionChanged, this, &MainWindow::onPositionChanged);
        connect(m_originalPlayer, &QMediaPlayer::durationChanged, this, &MainWindow::onDurationChanged);
        connect(m_originalPlayer, &QMediaPlayer::errorOccurred, this, &MainWindow::onVideoError);
        
    } catch (const std::exception& e) {
        qDebug() << "Exception in MainWindow constructor:" << e.what();
        throw;
    }
}

MainWindow::~MainWindow()
{
    // 先停止播放和定时器
    if (m_originalPlayer) {
        m_originalPlayer->stop();
    }
    if (m_filteredPlayer) {
        m_filteredPlayer->stop();
    }
    if (m_positionTimer) {
        m_positionTimer->stop();
    }
    
    // 删除动态分配的对象
    delete m_ffmpeg;
    // Qt的父子关系会自动处理其他对象的删除
}

void MainWindow::setupUI()
{
    // 创建主窗口的中心部件
    auto centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // 创建主布局
    auto mainLayout = new QVBoxLayout(centralWidget);
    
    // 创建页面切换按钮
    auto buttonLayout = new QHBoxLayout();
    m_switchAnalyzerBtn = new QPushButton("Analyzer", this);
    m_switchFilterBtn = new QPushButton("Filter Test", this);
    buttonLayout->addWidget(m_switchAnalyzerBtn);
    buttonLayout->addWidget(m_switchFilterBtn);
    mainLayout->addLayout(buttonLayout);
    
    // 创建堆叠窗口部件
    m_stackedWidget = new QStackedWidget(this);
    mainLayout->addWidget(m_stackedWidget);
    
    // 创建分析页面
    m_analyzerPage = new QWidget(this);
    setupAnalyzerPage();
    m_stackedWidget->addWidget(m_analyzerPage);
    
    // 创建滤镜页面
    m_filterPage = new QWidget(this);
    setupFilterPage();
    m_stackedWidget->addWidget(m_filterPage);
    
    // 连接信号
    connect(m_switchAnalyzerBtn, &QPushButton::clicked, this, [this]() { onSwitchPage(0); });
    connect(m_switchFilterBtn, &QPushButton::clicked, this, [this]() { onSwitchPage(1); });
}

void MainWindow::setupAnalyzerPage()
{
    // 文件选择区域
    auto fileSelectLayout = new QHBoxLayout();
    m_inputPathEdit = new QLineEdit(this);
    m_inputPathEdit->setPlaceholderText("Input Video File Path");
    m_browseBtn = new QPushButton("Browse", this);
    m_analyzeBtn = new QPushButton("Analyze", this);
    
    fileSelectLayout->addWidget(m_inputPathEdit);
    fileSelectLayout->addWidget(m_browseBtn);
    fileSelectLayout->addWidget(m_analyzeBtn);
    
    m_analyzerPage->setLayout(fileSelectLayout);
    
    // 连接信号和槽
    connect(m_browseBtn, &QPushButton::clicked, this, &MainWindow::onBrowseFile);
    connect(m_analyzeBtn, &QPushButton::clicked, this, &MainWindow::onAnalyzeVideo);
}

void MainWindow::setupFilterPage()
{
    auto layout = new QVBoxLayout(m_filterPage);
    
    // 添加文件选择区域
    auto fileSelectLayout = new QHBoxLayout();
    auto inputPathEdit = new QLineEdit(this);
    inputPathEdit->setPlaceholderText("Input Video File Path");
    auto browseBtn = new QPushButton("Browse", this);
    
    fileSelectLayout->addWidget(inputPathEdit);
    fileSelectLayout->addWidget(browseBtn);
    layout->addLayout(fileSelectLayout);
    
    // 创建视频显示区域
    auto videoLayout = new QHBoxLayout();
    
    // 左侧原始视频
    auto leftLayout = new QVBoxLayout();
    leftLayout->addWidget(new QLabel("Original Video", this));
    
    // 设置视频显示区域的属性
    m_originalVideo->setMinimumSize(480, 320);
    m_originalVideo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_originalVideo->setStyleSheet("QVideoWidget { background-color: black; }");
    leftLayout->addWidget(m_originalVideo, 1);
    
    // 右侧过滤后视频
    auto rightLayout = new QVBoxLayout();
    rightLayout->addWidget(new QLabel("Filtered Video", this));
    
    // 设置视频显示区域的属性
    m_filteredVideo->setMinimumSize(480, 320);
    m_filteredVideo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_filteredVideo->setStyleSheet("QVideoWidget { background-color: black; }");
    rightLayout->addWidget(m_filteredVideo, 1);
    
    // 设置视频布局的拉伸因子
    videoLayout->addLayout(leftLayout, 1);
    videoLayout->addLayout(rightLayout, 1);
    layout->addLayout(videoLayout, 1);  // 让视频区域占据大部分空间
    
    // 添加播放控制
    auto playbackLayout = new QHBoxLayout();
    m_playPauseBtn = new QPushButton("Play", this);
    m_progressSlider = new QSlider(Qt::Horizontal, this);
    m_timeLabel = new QLabel("00:00 / 00:00", this);
    
    playbackLayout->addWidget(m_playPauseBtn);
    playbackLayout->addWidget(m_progressSlider);
    playbackLayout->addWidget(m_timeLabel);
    layout->addLayout(playbackLayout);
    
    // 滤镜控制区域
    auto filterControlLayout = new QHBoxLayout();
    m_filterSelect = new QComboBox(this);
    m_filterSelect->addItems({"blur", "sharpen", "edge", "colorbalance", "rotate", "crop"});
    m_filterParamsWidget = new QWidget(this);
    m_filterParamsWidget->setLayout(new QHBoxLayout());
    m_applyFilterBtn = new QPushButton("Apply Filter", this);
    
    filterControlLayout->addWidget(new QLabel("Filter:", this));
    filterControlLayout->addWidget(m_filterSelect);
    filterControlLayout->addWidget(m_filterParamsWidget);
    filterControlLayout->addWidget(m_applyFilterBtn);
    layout->addLayout(filterControlLayout);
    
    // 转换控制区域
    auto convertLayout = new QHBoxLayout();
    auto outputPathEdit = new QLineEdit(this);
    outputPathEdit->setPlaceholderText("Output Video File Path (optional)");
    auto outputBrowseBtn = new QPushButton("Browse", this);
    auto convertBtn = new QPushButton("Convert", this);
    
    convertLayout->addWidget(outputPathEdit);
    convertLayout->addWidget(outputBrowseBtn);
    convertLayout->addWidget(convertBtn);
    layout->addLayout(convertLayout);
    
    // 连接信号
    connect(browseBtn, &QPushButton::clicked, this, [=]() {
        QString filePath = QFileDialog::getOpenFileName(this,
            "Select Input Video", "", "Video Files (*.mp4 *.avi *.mkv *.mov);;All Files (*)");
        if (!filePath.isEmpty()) {
            inputPathEdit->setText(filePath);
            m_originalPlayer->setSource(QUrl::fromLocalFile(filePath));
            m_filteredPlayer->setSource(QUrl::fromLocalFile(filePath));
        }
    });
    
    connect(outputBrowseBtn, &QPushButton::clicked, this, [=]() {
        QString filePath = QFileDialog::getSaveFileName(this,
            "Select Output Video", "", "Video Files (*.mp4);;All Files (*)");
        if (!filePath.isEmpty()) {
            outputPathEdit->setText(filePath);
        }
    });
    
    connect(convertBtn, &QPushButton::clicked, this, [=]() {
        QString inputPath = inputPathEdit->text();
        QString outputPath = outputPathEdit->text();
        
        if (inputPath.isEmpty()) {
            QMessageBox::warning(this, "Error", "Please select input video file");
            return;
        }
        
        if (outputPath.isEmpty()) {
            outputPath = inputPath;
            outputPath.insert(outputPath.lastIndexOf('.'), "_filtered");
            outputPathEdit->setText(outputPath);
        }
        
        // 获取当前滤镜参数
        std::map<std::string, std::string> params;
        QLayout* layout = m_filterParamsWidget->layout();
        if (layout) {
            for (int i = 0; i < layout->count(); i++) {
                QWidget* widget = layout->itemAt(i)->widget();
                if (auto spinBox = qobject_cast<QSpinBox*>(widget)) {
                    params["value"] = std::to_string(spinBox->value());
                }
                else if (auto slider = qobject_cast<QSlider*>(widget)) {
                    params[QString("value%1").arg(i).toStdString()] = std::to_string(slider->value());
                }
            }
        }
        
        // 开始转换
        if (m_ffmpeg->applyFilter(inputPath.toStdString(), 
                                outputPath.toStdString(),
                                m_filterSelect->currentText().toStdString(), 
                                params)) {
            QMessageBox::information(this, "Success", "Video conversion completed");
            m_filteredPlayer->setSource(QUrl::fromLocalFile(outputPath));
        } else {
            QMessageBox::warning(this, "Error", "Failed to convert video");
        }
    });
    
    connect(m_playPauseBtn, &QPushButton::clicked, this, &MainWindow::onPlayPause);
    connect(m_progressSlider, &QSlider::sliderMoved, this, &MainWindow::onSliderMoved);
    connect(m_filterSelect, &QComboBox::currentTextChanged, this, &MainWindow::onFilterChanged);
}

void MainWindow::onPlayPause()
{
    if (!m_isPlaying) {
        m_originalPlayer->play();
        m_filteredPlayer->play();
        m_playPauseBtn->setText("Pause");
        m_positionTimer->start();
    } else {
        m_originalPlayer->pause();
        m_filteredPlayer->pause();
        m_playPauseBtn->setText("Play");
        m_positionTimer->stop();
    }
    m_isPlaying = !m_isPlaying;
}

void MainWindow::onPositionChanged(qint64 position)
{
    if (!m_progressSlider->isSliderDown()) {
        m_progressSlider->setValue(position);
    }
    
    // 更新时间标签
    qint64 duration = m_originalPlayer->duration();
    QString timeStr = QString("%1 / %2")
        .arg(QTime(0, 0).addMSecs(position).toString("mm:ss"))
        .arg(QTime(0, 0).addMSecs(duration).toString("mm:ss"));
    m_timeLabel->setText(timeStr);
}

void MainWindow::onDurationChanged(qint64 duration)
{
    m_progressSlider->setRange(0, duration);
}

void MainWindow::onSliderMoved(int position)
{
    m_originalPlayer->setPosition(position);
    m_filteredPlayer->setPosition(position);
}

void MainWindow::updateVideoPosition()
{
    // 同步两个视频的播放位置
    qint64 pos = m_originalPlayer->position();
    if (qAbs(m_filteredPlayer->position() - pos) > 50) {  // 如果差异超过50ms
        m_filteredPlayer->setPosition(pos);
    }
}

void MainWindow::onVideoError(QMediaPlayer::Error error)
{
    QMessageBox::warning(this, "Error", 
        QString("Video playback error: %1").arg(m_originalPlayer->errorString()));
}

void MainWindow::onSwitchPage(int index)
{
    m_stackedWidget->setCurrentIndex(index);
}

void MainWindow::setupVideoInfoTable()
{
    m_videoInfoTable->setColumnCount(2);
    m_videoInfoTable->setHorizontalHeaderLabels({"Property", "Value"});
    m_videoInfoTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_videoInfoTable->setRowCount(6);
    m_videoInfoTable->setVerticalHeaderLabels({
        "Codec", "Resolution", "Framerate", "Bitrate",
        "Pixel Format", "Color Space"
    });
}

void MainWindow::setupAudioInfoTable()
{
    m_audioInfoTable->setColumnCount(2);
    m_audioInfoTable->setHorizontalHeaderLabels({"Property", "Value"});
    m_audioInfoTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_audioInfoTable->setRowCount(5);
    m_audioInfoTable->setVerticalHeaderLabels({
        "Codec", "Sample Rate", "Channels", "Bit Depth",
        "Bitrate"
    });
}

void MainWindow::setupContainerInfoTable()
{
    m_containerInfoTable->setColumnCount(2);
    m_containerInfoTable->setHorizontalHeaderLabels({"Property", "Value"});
    m_containerInfoTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_containerInfoTable->setRowCount(4);
    m_containerInfoTable->setVerticalHeaderLabels({
        "Format", "Duration", "Size", "Bitrate"
    });
}

void MainWindow::onBrowseFile()
{
    QString filePath = QFileDialog::getOpenFileName(this,
        "Select Video File", "", "Video Files (*.mp4 *.avi *.mkv *.mov);;All Files (*)");
    
    if (!filePath.isEmpty()) {
        m_inputPathEdit->setText(filePath);
        
        // 设置视频源
        m_originalPlayer->setSource(QUrl::fromLocalFile(filePath));
        // 过滤后的视频暂时使用原视频
        m_filteredPlayer->setSource(QUrl::fromLocalFile(filePath));
    }
}

void MainWindow::onAnalyzeVideo()
{
    QString filePath = m_inputPathEdit->text();
    if (filePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select a video file first");
        return;
    }
    
    MediaInfo mediaInfo;
    if (!m_ffmpeg->analyzeMedia(filePath.toStdString(), mediaInfo)) {
        QMessageBox::warning(this, "Error", "Failed to analyze video file");
        return;
    }
    
    // Update video info table
    m_videoInfoTable->setItem(0, 1, new QTableWidgetItem(QString::fromStdString(mediaInfo.videoCodec)));
    m_videoInfoTable->setItem(1, 1, new QTableWidgetItem(QString("%1x%2").arg(mediaInfo.width).arg(mediaInfo.height)));
    m_videoInfoTable->setItem(2, 1, new QTableWidgetItem(QString::number(mediaInfo.frameRate, 'f', 2)));
    m_videoInfoTable->setItem(3, 1, new QTableWidgetItem(QString::number(mediaInfo.videoBitrate / 1000) + " kbps"));
    m_videoInfoTable->setItem(4, 1, new QTableWidgetItem(QString::fromStdString(mediaInfo.pixelFormat)));
    m_videoInfoTable->setItem(5, 1, new QTableWidgetItem(QString::fromStdString(mediaInfo.colorSpace)));
    
    // Update audio info table
    m_audioInfoTable->setItem(0, 1, new QTableWidgetItem(QString::fromStdString(mediaInfo.audioCodec)));
    m_audioInfoTable->setItem(1, 1, new QTableWidgetItem(QString::number(mediaInfo.sampleRate) + " Hz"));
    m_audioInfoTable->setItem(2, 1, new QTableWidgetItem(QString::number(mediaInfo.channels)));
    m_audioInfoTable->setItem(3, 1, new QTableWidgetItem(QString::number(mediaInfo.bitDepth) + " bit"));
    m_audioInfoTable->setItem(4, 1, new QTableWidgetItem(QString::number(mediaInfo.audioBitrate / 1000) + " kbps"));
    
    // Update container info table
    m_containerInfoTable->setItem(0, 1, new QTableWidgetItem(QString::fromStdString(mediaInfo.formatName)));
    m_containerInfoTable->setItem(1, 1, new QTableWidgetItem(QString::number(mediaInfo.duration, 'f', 2) + " seconds"));
    m_containerInfoTable->setItem(2, 1, new QTableWidgetItem(QString::number(mediaInfo.fileSize / (1024 * 1024), 'f', 2) + " MB"));
    m_containerInfoTable->setItem(3, 1, new QTableWidgetItem(QString::number(mediaInfo.totalBitrate / 1000) + " kbps"));
}

void MainWindow::onFilterChanged(const QString& filterName)
{
    // 清除旧的参数控件
    QLayout* layout = m_filterParamsWidget->layout();
    if (!layout) {
        layout = new QHBoxLayout(m_filterParamsWidget);
    }
    
    while (layout->count() > 0) {
        QLayoutItem* item = layout->takeAt(0);
        delete item->widget();
        delete item;
    }
    
    // 根据选择的滤镜添加相应的参数控件
    if (filterName == "blur") {
        layout->addWidget(new QLabel("Radius:", this));
        auto spinBox = new QSpinBox(this);
        spinBox->setRange(1, 20);
        spinBox->setValue(3);
        layout->addWidget(spinBox);
    }
    else if (filterName == "colorbalance") {
        layout->addWidget(new QLabel("R:", this));
        auto rSlider = new QSlider(Qt::Horizontal, this);
        rSlider->setRange(-100, 100);
        layout->addWidget(rSlider);
        
        layout->addWidget(new QLabel("G:", this));
        auto gSlider = new QSlider(Qt::Horizontal, this);
        gSlider->setRange(-100, 100);
        layout->addWidget(gSlider);
        
        layout->addWidget(new QLabel("B:", this));
        auto bSlider = new QSlider(Qt::Horizontal, this);
        bSlider->setRange(-100, 100);
        layout->addWidget(bSlider);
    }
    else if (filterName == "rotate") {
        layout->addWidget(new QLabel("Angle:", this));
        auto spinBox = new QSpinBox(this);
        spinBox->setRange(0, 360);
        spinBox->setValue(0);
        layout->addWidget(spinBox);
    }
    else if (filterName == "crop") {
        layout->addWidget(new QLabel("Width:", this));
        auto widthBox = new QSpinBox(this);
        widthBox->setRange(1, 1920);
        layout->addWidget(widthBox);
        
        layout->addWidget(new QLabel("Height:", this));
        auto heightBox = new QSpinBox(this);
        heightBox->setRange(1, 1080);
        layout->addWidget(heightBox);
    }
}

void MainWindow::onApplyFilter()
{
    // 获取当前选择的滤镜和参数
    QString filterName = m_filterSelect->currentText();
    std::map<std::string, std::string> params;
    
    // 从参数控件中获取值
    QLayout* layout = m_filterParamsWidget->layout();
    if (layout) {
        for (int i = 0; i < layout->count(); i++) {
            QWidget* widget = layout->itemAt(i)->widget();
            if (auto spinBox = qobject_cast<QSpinBox*>(widget)) {
                params["value"] = std::to_string(spinBox->value());
            }
            else if (auto slider = qobject_cast<QSlider*>(widget)) {
                params[QString("value%1").arg(i).toStdString()] = std::to_string(slider->value());
            }
        }
    }
    
    // 应用滤镜
    QString inputPath = m_inputPathEdit->text();
    QString outputPath = inputPath + "_filtered.mp4";
    
    if (m_ffmpeg->applyFilter(inputPath.toStdString(), 
                             outputPath.toStdString(),
                             filterName.toStdString(), 
                             params)) {
        // 更新过滤后的视频播放器
        m_filteredPlayer->setSource(QUrl::fromLocalFile(outputPath));
        m_filteredPlayer->setPosition(m_originalPlayer->position());
        if (m_isPlaying) {
            m_filteredPlayer->play();
        }
    } else {
        QMessageBox::warning(this, "Error", "Failed to apply filter");
    }
} 