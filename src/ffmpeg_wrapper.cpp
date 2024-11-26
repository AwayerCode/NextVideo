extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
#include <libavdevice/avdevice.h>
#include <libavutil/avutil.h>
#include <libavutil/pixdesc.h>
}

#include "ffmpeg_wrapper.h"

FFmpegWrapper::FFmpegWrapper()
{
    // 初始化FFmpeg
#if LIBAVDEVICE_VERSION_INT < AV_VERSION_INT(59, 0, 100)
    avdevice_register_all();
#endif
    avformat_network_init();
}

FFmpegWrapper::~FFmpegWrapper()
{
    // 清理FFmpeg资源
    avformat_network_deinit();
}

bool FFmpegWrapper::analyzeMedia(const std::string& inputPath, MediaInfo& info)
{
    AVFormatContext* formatContext = nullptr;
    if (avformat_open_input(&formatContext, inputPath.c_str(), nullptr, nullptr) < 0) {
        return false;
    }

    if (avformat_find_stream_info(formatContext, nullptr) < 0) {
        avformat_close_input(&formatContext);
        return false;
    }

    // 填充基本信息
    info.formatName = formatContext->iformat->name;
    info.duration = formatContext->duration / (double)AV_TIME_BASE;
    info.fileSize = avio_size(formatContext->pb);
    info.totalBitrate = formatContext->bit_rate;

    // 分析视频流
    for (unsigned int i = 0; i < formatContext->nb_streams; i++) {
        AVStream* stream = formatContext->streams[i];
        if (stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            const AVCodec* codec = avcodec_find_decoder(stream->codecpar->codec_id);
            if (codec) {
                info.videoCodec = codec->name ? codec->name : "unknown";
                info.width = stream->codecpar->width;
                info.height = stream->codecpar->height;
                info.frameRate = av_q2d(stream->avg_frame_rate);
                info.videoBitrate = stream->codecpar->bit_rate;
                info.pixelFormat = av_get_pix_fmt_name((AVPixelFormat)stream->codecpar->format);
                // 获取色彩空间信息
                switch(stream->codecpar->color_space) {
                    case AVCOL_SPC_BT709: info.colorSpace = "BT.709"; break;
                    case AVCOL_SPC_BT470BG: info.colorSpace = "BT.601"; break;
                    default: info.colorSpace = "Unknown";
                }
            }
        }
        else if (stream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            const AVCodec* codec = avcodec_find_decoder(stream->codecpar->codec_id);
            if (codec) {
                info.audioCodec = codec->name ? codec->name : "unknown";
                info.sampleRate = stream->codecpar->sample_rate;
                info.channels = stream->codecpar->ch_layout.nb_channels;
                info.bitDepth = av_get_bytes_per_sample((AVSampleFormat)stream->codecpar->format) * 8;
                info.audioBitrate = stream->codecpar->bit_rate;
            }
        }
    }

    avformat_close_input(&formatContext);
    return true;
}

bool FFmpegWrapper::applyFilter(const std::string& inputPath,
                              const std::string& outputPath,
                              const std::string& filterName,
                              const std::map<std::string, std::string>& params)
{
    // 打开输入文件
    AVFormatContext* inputFormatContext = nullptr;
    if (avformat_open_input(&inputFormatContext, inputPath.c_str(), nullptr, nullptr) < 0) {
        return false;
    }

    // 获取流信息
    if (avformat_find_stream_info(inputFormatContext, nullptr) < 0) {
        avformat_close_input(&inputFormatContext);
        return false;
    }

    // 创建输出格式上下文
    AVFormatContext* outputFormatContext = nullptr;
    avformat_alloc_output_context2(&outputFormatContext, nullptr, nullptr, outputPath.c_str());
    if (!outputFormatContext) {
        avformat_close_input(&inputFormatContext);
        return false;
    }

    // 创建滤镜图
    AVFilterGraph* filterGraph = nullptr;
    AVFilterContext* buffersrcContext = nullptr;
    AVFilterContext* buffersinkContext = nullptr;

    // 设置滤镜图
    if (!setupFilterGraph(&filterGraph, &buffersrcContext, &buffersinkContext, filterName)) {
        avformat_close_input(&inputFormatContext);
        avformat_free_context(outputFormatContext);
        return false;
    }

    // 处理视频帧并写入输出文件
    // ... 这里需要实现实际的滤镜处理逻辑 ...

    // 清理资源
    avfilter_graph_free(&filterGraph);
    avformat_close_input(&inputFormatContext);
    avformat_free_context(outputFormatContext);

    return true;
}

bool FFmpegWrapper::setupFilterGraph(AVFilterGraph** graph,
                                   AVFilterContext** buffersrc_ctx,
                                   AVFilterContext** buffersink_ctx,
                                   const std::string& filterDesc)
{
    int ret;
    AVFilterGraph* filterGraph = avfilter_graph_alloc();
    if (!filterGraph) {
        return false;
    }

    // 创建buffer源和接收器
    const AVFilter* buffersrc = avfilter_get_by_name("buffer");
    const AVFilter* buffersink = avfilter_get_by_name("buffersink");
    if (!buffersrc || !buffersink) {
        avfilter_graph_free(&filterGraph);
        return false;
    }

    // 配置滤镜图
    // ... 这里需要实现具体的滤镜图配置逻辑 ...

    *graph = filterGraph;
    return true;
} 