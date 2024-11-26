#include "videocontroller.h"
#include <QtCore/QTime>
#include <QtCore/QUrl>
#include <QtCore/QDebug>

VideoController::VideoController(QObject *parent)
    : QObject(parent)
    , m_originalPlayer(new QMediaPlayer(this))
    , m_filteredPlayer(new QMediaPlayer(this))
    , m_ffmpeg(new FFmpegWrapper())
    , m_positionTimer(new QTimer(this))
    , m_isPlaying(false)
{
    m_positionTimer->setInterval(50);
    connect(m_positionTimer, &QTimer::timeout, this, &VideoController::updatePosition);
    connect(m_originalPlayer, &QMediaPlayer::errorOccurred,
            this, [this](QMediaPlayer::Error error, const QString &errorString) {
                handleError(error);
            });
}

VideoController::~VideoController()
{
    delete m_ffmpeg;
}

qint64 VideoController::position() const
{
    return m_originalPlayer->position();
}

qint64 VideoController::duration() const
{
    return m_originalPlayer->duration();
}

QString VideoController::timeText() const
{
    qint64 pos = position();
    qint64 dur = duration();
    return QString("%1 / %2")
        .arg(QTime(0, 0).addMSecs(pos).toString("mm:ss"))
        .arg(QTime(0, 0).addMSecs(dur).toString("mm:ss"));
}

void VideoController::loadVideo(const QUrl &url)
{
    m_originalPlayer->setSource(url);
    m_filteredPlayer->setSource(url);
}

void VideoController::togglePlayPause()
{
    if (m_isPlaying) {
        m_originalPlayer->pause();
        m_filteredPlayer->pause();
        m_positionTimer->stop();
    } else {
        m_originalPlayer->play();
        m_filteredPlayer->play();
        m_positionTimer->start();
    }
    m_isPlaying = !m_isPlaying;
    emit isPlayingChanged();
}

void VideoController::seek(qint64 position)
{
    m_originalPlayer->setPosition(position);
    m_filteredPlayer->setPosition(position);
}

void VideoController::analyzeVideo(const QString &path)
{
    MediaInfo info;
    if (m_ffmpeg->analyzeMedia(path.toStdString(), info)) {
        updateMediaInfo(info);
    }
}

void VideoController::filterChanged(const QString &filterName)
{
    m_currentFilter = filterName;
    m_filterParams.clear();
}

void VideoController::setFilterParam(const QString &param, const QVariant &value)
{
    m_filterParams[param] = value;
}

void VideoController::applyFilter()
{
    // 将参数转换为FFmpeg需要的格式
    std::map<std::string, std::string> params;
    for (auto it = m_filterParams.begin(); it != m_filterParams.end(); ++it) {
        params[it.key().toStdString()] = it.value().toString().toStdString();
    }

    QString inputPath = m_originalPlayer->source().toLocalFile();
    QString outputPath = inputPath + "_filtered.mp4";

    if (m_ffmpeg->applyFilter(inputPath.toStdString(), 
                             outputPath.toStdString(),
                             m_currentFilter.toStdString(), 
                             params)) {
        m_filteredPlayer->setSource(QUrl::fromLocalFile(outputPath));
        m_filteredPlayer->setPosition(m_originalPlayer->position());
        if (m_isPlaying) {
            m_filteredPlayer->play();
        }
    }
}

void VideoController::updatePosition()
{
    emit positionChanged();
    syncPlayers();
}

void VideoController::handleError(QMediaPlayer::Error error)
{
    // 处理错误...
}

void VideoController::syncPlayers()
{
    qint64 pos = m_originalPlayer->position();
    if (qAbs(m_filteredPlayer->position() - pos) > 50) {
        m_filteredPlayer->setPosition(pos);
    }
}

void VideoController::updateMediaInfo(const MediaInfo &info)
{
    // 更新视频信息
    m_videoInfo.clear();
    m_videoInfo.append(QVariantMap{
        {"property", "Codec"}, {"value", QString::fromStdString(info.videoCodec)}
    });
    m_videoInfo.append(QVariantMap{
        {"property", "Resolution"}, 
        {"value", QString("%1x%2").arg(info.width).arg(info.height)}
    });
    // ... 添加其他视频信息

    // 更新音频信息
    m_audioInfo.clear();
    m_audioInfo.append(QVariantMap{
        {"property", "Codec"}, {"value", QString::fromStdString(info.audioCodec)}
    });
    // ... 添加其他音频信息

    // 更新容器信息
    m_containerInfo.clear();
    m_containerInfo.append(QVariantMap{
        {"property", "Format"}, {"value", QString::fromStdString(info.formatName)}
    });
    // ... 添加其他容器信息

    emit videoInfoChanged();
    emit audioInfoChanged();
    emit containerInfoChanged();
} 