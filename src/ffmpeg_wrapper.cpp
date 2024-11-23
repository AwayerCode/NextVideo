#include "ffmpeg_wrapper.h"
#include <sstream>
#include <filesystem>

FFmpegWrapper::FFmpegWrapper() {
    initFFmpeg();
}

FFmpegWrapper::~FFmpegWrapper() {
}

void FFmpegWrapper::initFFmpeg() {
}

bool FFmpegWrapper::analyzeMedia(const std::string& inputPath, MediaInfo& info) {
    AVFormatContext* formatContext = nullptr;
    
    if (avformat_open_input(&formatContext, inputPath.c_str(), nullptr, nullptr) < 0) {
        return false;
    }
    
    if (avformat_find_stream_info(formatContext, nullptr) < 0) {
        avformat_close_input(&formatContext);
        return false;
    }
    
    // Get container info
    info.formatName = formatContext->iformat->long_name;
    info.duration = formatContext->duration / (double)AV_TIME_BASE;
    info.totalBitrate = formatContext->bit_rate;
    
    // Get file size
    std::filesystem::path path(inputPath);
    info.fileSize = std::filesystem::file_size(path);
    
    // Find video and audio streams
    for (unsigned int i = 0; i < formatContext->nb_streams; i++) {
        AVStream* stream = formatContext->streams[i];
        AVCodecParameters* codecParams = stream->codecpar;
        
        if (codecParams->codec_type == AVMEDIA_TYPE_VIDEO) {
            // Video info
            const AVCodec* codec = avcodec_find_decoder(codecParams->codec_id);
            info.videoCodec = codec ? codec->long_name : "Unknown";
            info.width = codecParams->width;
            info.height = codecParams->height;
            info.frameRate = av_q2d(stream->avg_frame_rate);
            info.videoBitrate = codecParams->bit_rate;
            info.pixelFormat = av_get_pix_fmt_name((AVPixelFormat)codecParams->format);
            info.colorSpace = av_color_space_name((AVColorSpace)codecParams->color_space);
            
        } else if (codecParams->codec_type == AVMEDIA_TYPE_AUDIO) {
            // Audio info
            const AVCodec* codec = avcodec_find_decoder(codecParams->codec_id);
            info.audioCodec = codec ? codec->long_name : "Unknown";
            info.sampleRate = codecParams->sample_rate;
            info.channels = codecParams->ch_layout.nb_channels;
            info.bitDepth = av_get_bytes_per_sample((AVSampleFormat)codecParams->format) * 8;
            info.audioBitrate = codecParams->bit_rate;
        }
    }
    
    avformat_close_input(&formatContext);
    return true;
}

bool FFmpegWrapper::applyFilter(const std::string& inputPath,
                              const std::string& outputPath,
                              const std::string& filterName,
                              const std::map<std::string, std::string>& params) {
    AVFormatContext *inputFormatContext = nullptr;
    AVFormatContext *outputFormatContext = nullptr;
    AVFilterGraph *filterGraph = nullptr;
    AVFilterContext *buffersrcContext = nullptr;
    AVFilterContext *buffersinkContext = nullptr;
    int ret;

    // 打开输入文件
    if ((ret = avformat_open_input(&inputFormatContext, inputPath.c_str(), nullptr, nullptr)) < 0) {
        return false;
    }

    // 获取流信息
    if ((ret = avformat_find_stream_info(inputFormatContext, nullptr)) < 0) {
        avformat_close_input(&inputFormatContext);
        return false;
    }

    // 创建输出上下文
    if ((ret = avformat_alloc_output_context2(&outputFormatContext, nullptr, nullptr, outputPath.c_str())) < 0) {
        avformat_close_input(&inputFormatContext);
        return false;
    }

    // 构建滤镜描述字符串
    std::string filterDesc;
    if (filterName == "blur") {
        auto it = params.find("value");
        std::string radius = it != params.end() ? it->second : "5";
        filterDesc = "boxblur=luma_radius=" + radius;
    }
    else if (filterName == "colorbalance") {
        auto rValue = params.find("value1");
        auto gValue = params.find("value3");
        auto bValue = params.find("value5");
        filterDesc = "colorbalance=rs=" + 
                    (rValue != params.end() ? rValue->second : "0") + ":gs=" +
                    (gValue != params.end() ? gValue->second : "0") + ":bs=" +
                    (bValue != params.end() ? bValue->second : "0");
    }
    else if (filterName == "rotate") {
        auto it = params.find("value");
        std::string angle = it != params.end() ? it->second : "0";
        filterDesc = "rotate=" + angle + "*PI/180";
    }
    else if (filterName == "crop") {
        auto widthIt = params.find("value1");
        auto heightIt = params.find("value3");
        std::string width = widthIt != params.end() ? widthIt->second : "iw";
        std::string height = heightIt != params.end() ? heightIt->second : "ih";
        filterDesc = "crop=" + width + ":" + height;
    }
    else {
        avformat_close_input(&inputFormatContext);
        avformat_free_context(outputFormatContext);
        return false;
    }

    // 设置滤镜图
    if (!setupFilterGraph(&filterGraph, &buffersrcContext, &buffersinkContext, filterDesc)) {
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
                                   const std::string& filterDesc) {
    int ret;
    AVFilterGraph *filterGraph = avfilter_graph_alloc();
    if (!filterGraph) {
        return false;
    }

    // 创建buffer源和接收器
    const AVFilter *buffersrc = avfilter_get_by_name("buffer");
    const AVFilter *buffersink = avfilter_get_by_name("buffersink");
    if (!buffersrc || !buffersink) {
        avfilter_graph_free(&filterGraph);
        return false;
    }

    // 创建滤镜图的输入输出节点
    AVFilterInOut *inputs = avfilter_inout_alloc();
    AVFilterInOut *outputs = avfilter_inout_alloc();
    if (!inputs || !outputs) {
        avfilter_graph_free(&filterGraph);
        avfilter_inout_free(&inputs);
        avfilter_inout_free(&outputs);
        return false;
    }

    // 解析滤镜描述字符串
    if ((ret = avfilter_graph_parse_ptr(filterGraph, filterDesc.c_str(),
                                       &inputs, &outputs, nullptr)) < 0) {
        avfilter_graph_free(&filterGraph);
        avfilter_inout_free(&inputs);
        avfilter_inout_free(&outputs);
        return false;
    }

    // 配置滤镜图
    if ((ret = avfilter_graph_config(filterGraph, nullptr)) < 0) {
        avfilter_graph_free(&filterGraph);
        return false;
    }

    *graph = filterGraph;
    return true;
}

std::vector<std::string> FFmpegWrapper::getAvailableFilters() {
    std::vector<std::string> filters;
    void *opaque = nullptr;
    const AVFilter *filter = nullptr;

    while ((filter = av_filter_iterate(&opaque))) {
        filters.push_back(filter->name);
    }

    return filters;
} 