#ifndef ENCODER_HPP
#define ENCODER_HPP

// FFmpeg headers
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libavformat/avformat.h>
}

#include <string>
#include <stdexcept>
#include <cstdio>

class Encoder {
public:
    /**
     * @brief Constructor for Encoder.
     * 
     * @param outputFile Path to the output file where the encoded bitstream will be saved.
     * @param width Width of the video frames to encode.
     * @param height Height of the video frames to encode.
     * @param fps Frames per second for the video.
     * 
     * @throws std::runtime_error if initialization fails.
     */
    Encoder(const std::string &outputFile, int width, int height, int fps);

    /**
     * @brief Destructor for H265Encoder.
     * 
     * Frees resources and ensures the encoder is properly flushed.
     */
    ~Encoder();

    /**
     * @brief Encodes a video frame and writes it to the output file.
     * 
     * @param frame A pointer to the AVFrame to encode. The frame must have a format of AV_PIX_FMT_YUV420P.
     * 
     * @throws std::invalid_argument if the input frame is null.
     * @throws std::runtime_error if encoding fails.
     */
    void encodeFrame(AVFrame *frame);
    /**
     * @brief Flushes the encoder to ensure all remaining packets are written to the output file.
     */
    void flushEncoder();
private:
    AVCodecContext *codecContext; ///< Pointer to the FFmpeg codec context.
    FILE *file;                   ///< Pointer to the output file.
    int frameCounter;             ///< Counter for frame presentation timestamps (PTS).
};

#endif // ENCODER_HPP
