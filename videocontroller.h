#ifndef VIDEOCONTROLLER_H
#define VIDEOCONTROLLER_H

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/QVariantList>
#include <QtMultimedia/QMediaPlayer>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include "ffmpeg_wrapper.h"

class VideoController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QMediaPlayer* originalPlayer READ originalPlayer CONSTANT)
    Q_PROPERTY(QMediaPlayer* filteredPlayer READ filteredPlayer CONSTANT)
    Q_PROPERTY(bool isPlaying READ isPlaying NOTIFY isPlayingChanged)
    Q_PROPERTY(qint64 position READ position NOTIFY positionChanged)
    Q_PROPERTY(qint64 duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(QString timeText READ timeText NOTIFY positionChanged)
    Q_PROPERTY(QVariantList videoInfo READ videoInfo NOTIFY videoInfoChanged)
    Q_PROPERTY(QVariantList audioInfo READ audioInfo NOTIFY audioInfoChanged)
    Q_PROPERTY(QVariantList containerInfo READ containerInfo NOTIFY containerInfoChanged)

public:
    explicit VideoController(QObject *parent = nullptr);
    ~VideoController();

    QMediaPlayer* originalPlayer() const { return m_originalPlayer; }
    QMediaPlayer* filteredPlayer() const { return m_filteredPlayer; }
    bool isPlaying() const { return m_isPlaying; }
    qint64 position() const;
    qint64 duration() const;
    QString timeText() const;
    QVariantList videoInfo() const { return m_videoInfo; }
    QVariantList audioInfo() const { return m_audioInfo; }
    QVariantList containerInfo() const { return m_containerInfo; }

public slots:
    void loadVideo(const QUrl &url);
    void togglePlayPause();
    void seek(qint64 position);
    void analyzeVideo(const QString &path);
    void filterChanged(const QString &filterName);
    void setFilterParam(const QString &param, const QVariant &value);
    void applyFilter();

signals:
    void isPlayingChanged();
    void positionChanged();
    void durationChanged();
    void videoInfoChanged();
    void audioInfoChanged();
    void containerInfoChanged();

private slots:
    void updatePosition();
    void handleError(QMediaPlayer::Error error);

private:
    void syncPlayers();
    void updateMediaInfo(const MediaInfo &info);

    QMediaPlayer *m_originalPlayer;
    QMediaPlayer *m_filteredPlayer;
    FFmpegWrapper *m_ffmpeg;
    QTimer *m_positionTimer;
    bool m_isPlaying;
    QString m_currentFilter;
    QVariantMap m_filterParams;
    QVariantList m_videoInfo;
    QVariantList m_audioInfo;
    QVariantList m_containerInfo;
};

#endif // VIDEOCONTROLLER_H 