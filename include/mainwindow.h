#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QFileDialog>
#include <QLabel>
#include <QComboBox>
#include <QStackedWidget>
#include <QSpinBox>
#include <QSlider>
#include <QTimer>
#include <QVideoWidget>
#include <QMediaPlayer>
#include "ffmpeg_wrapper.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAnalyzeVideo();
    void onBrowseFile();
    void onFilterChanged(const QString& filterName);
    void onApplyFilter();
    void onSwitchPage(int index);
    void onPlayPause();
    void onPositionChanged(qint64 position);
    void onDurationChanged(qint64 duration);
    void onSliderMoved(int position);
    void onVideoError(QMediaPlayer::Error error);
    void updateVideoPosition();

private:
    void setupUI();
    void setupAnalyzerPage();
    void setupFilterPage();
    void setupVideoInfoTable();
    void setupAudioInfoTable();
    void setupContainerInfoTable();
    void setupPlaybackControls();
    void syncVideoPlayers();
    
    // 主布局
    QStackedWidget *m_stackedWidget;
    QPushButton *m_switchAnalyzerBtn;
    QPushButton *m_switchFilterBtn;
    
    // 分析页面组件
    QWidget *m_analyzerPage;
    QPushButton *m_browseBtn;
    QPushButton *m_analyzeBtn;
    QLineEdit *m_inputPathEdit;
    QTableWidget *m_videoInfoTable;
    QTableWidget *m_audioInfoTable;
    QTableWidget *m_containerInfoTable;
    
    // 滤镜页面组件
    QWidget *m_filterPage;
    QLabel *m_originalVideoLabel;
    QLabel *m_filteredVideoLabel;
    QComboBox *m_filterSelect;
    QWidget *m_filterParamsWidget;
    QPushButton *m_applyFilterBtn;
    
    FFmpegWrapper *m_ffmpeg;
    
    // 视频播放相关组件
    QMediaPlayer *m_originalPlayer;
    QMediaPlayer *m_filteredPlayer;
    QVideoWidget *m_originalVideo;
    QVideoWidget *m_filteredVideo;
    QPushButton *m_playPauseBtn;
    QSlider *m_progressSlider;
    QLabel *m_timeLabel;
    QTimer *m_positionTimer;
    bool m_isPlaying;
};

#endif // MAINWINDOW_H 