#ifndef FFMPEG_WRAPPER_H
#define FFMPEG_WRAPPER_H

#include <string>
#include <vector>
#include <map>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/pixdesc.h>
#include <libavutil/pixfmt.h>
#include <libavutil/frame.h>
#include <libswscale/swscale.h>
#include <libavfilter/avfilter.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
}

struct MediaInfo {
    // Video
    std::string videoCodec;
    int width;
    int height;
    double frameRate;
    int64_t videoBitrate;
    std::string pixelFormat;
    std::string colorSpace;
    
    // Audio
    std::string audioCodec;
    int sampleRate;
    int channels;
    int bitDepth;
    int64_t audioBitrate;
    
    // Container
    std::string formatName;
    double duration;
    int64_t fileSize;
    int64_t totalBitrate;
    
    // Filter
    struct FilterInfo {
        std::string name;
        std::string description;
        bool isAvailable;
    };
    std::vector<FilterInfo> availableFilters;
    
    // Device
    struct DeviceInfo {
        std::string name;
        std::string type;
        std::string description;
    };
    std::vector<DeviceInfo> availableDevices;
    
    // Codec
    struct CodecInfo {
        std::string name;
        std::string type;
        bool isEncoder;
        bool isDecoder;
        std::vector<std::string> supportedPixelFormats;
        std::vector<std::string> supportedSampleFormats;
    };
    std::vector<CodecInfo> availableCodecs;
};

class FFmpegWrapper {
public:
    FFmpegWrapper();
    ~FFmpegWrapper();

    bool analyzeMedia(const std::string& inputPath, MediaInfo& info);
    
    bool applyFilter(const std::string& inputPath,
                    const std::string& outputPath,
                    const std::string& filterName,
                    const std::map<std::string, std::string>& params);
    
    std::vector<std::string> getAvailableFilters();

private:
    void initFFmpeg();
    bool setupFilterGraph(AVFilterGraph** graph,
                         AVFilterContext** buffersrc_ctx,
                         AVFilterContext** buffersink_ctx,
                         const std::string& filterDesc);
};

#endif // FFMPEG_WRAPPER_H 