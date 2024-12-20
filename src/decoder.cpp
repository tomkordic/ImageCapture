#include "Decoder.hpp"

Decoder::Decoder(const std::string &inputFile)
    : formatContext(nullptr), codecContext(nullptr), videoStreamIndex(-1) {
    // Open input file
    if (avformat_open_input(&formatContext, inputFile.c_str(), nullptr, nullptr) < 0) {
        throw std::runtime_error("Failed to open input file: " + inputFile);
    }

    // Retrieve stream information
    if (avformat_find_stream_info(formatContext, nullptr) < 0) {
        avformat_close_input(&formatContext);
        throw std::runtime_error("Failed to retrieve stream information");
    }

    // Find the first video stream
    for (unsigned int i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            break;
        }
    }

    if (videoStreamIndex == -1) {
        avformat_close_input(&formatContext);
        throw std::runtime_error("No video stream found");
    }

    // Get codec parameters and find decoder
    AVCodecParameters *codecParams = formatContext->streams[videoStreamIndex]->codecpar;
    const AVCodec *codec = avcodec_find_decoder(codecParams->codec_id);
    if (!codec) {
        avformat_close_input(&formatContext);
        throw std::runtime_error("Unsupported codec");
    }

    // Allocate and configure codec context
    codecContext = avcodec_alloc_context3(codec);
    if (!codecContext) {
        avformat_close_input(&formatContext);
        throw std::runtime_error("Failed to allocate codec context");
    }

    if (avcodec_parameters_to_context(codecContext, codecParams) < 0) {
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        throw std::runtime_error("Failed to copy codec parameters to codec context");
    }

    // Open codec
    if (avcodec_open2(codecContext, codec, nullptr) < 0) {
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        throw std::runtime_error("Failed to open codec");
    }
}

Decoder::~Decoder() {
    if (codecContext) {
        avcodec_free_context(&codecContext);
    }
    if (formatContext) {
        avformat_close_input(&formatContext);
    }
}

bool Decoder::decodeNextFrame(AVFrame *frame) {
    // TODO: maybe reuse packet
    AVPacket *packet = av_packet_alloc();

    while (av_read_frame(formatContext, packet) >= 0) {
        if (packet->stream_index == videoStreamIndex) {
            if (avcodec_send_packet(codecContext, packet) == 0) {
                if (avcodec_receive_frame(codecContext, frame) == 0) {
                    av_packet_unref(packet);
                    return true;
                }
            }
        }
        av_packet_unref(packet);
    }
    av_packet_free(&packet);
    return false; // End of file or no more frames
}

int Decoder::getWidth() const {
    return codecContext->width;
}

int Decoder::getHeight() const {
    return codecContext->height;
}

AVPixelFormat Decoder::getPixelFormat() const {
    return codecContext->pix_fmt;
}
