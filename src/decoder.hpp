#ifndef DECODER_HPP
#define DECODER_HPP

// FFmpeg headers
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavutil/error.h>
}

#include <string>
#include <stdexcept>
#include <iostream>

class Decoder {
public:
    /**
     * @brief Constructor for Decoder.
     * 
     * @param inputFile Path to the input video file.
     * 
     * @throws std::runtime_error if initialization fails.
     */
    Decoder(const std::string &inputFile);

    /**
     * @brief Destructor for Decoder.
     * 
     * Frees resources used by the decoder.
     */
    ~Decoder();

    /**
     * @brief Decodes the next video frame from the input file.
     * 
     * @param frame A pointer to an AVFrame where the decoded frame will be stored.
     * 
     * @return true if a frame was successfully decoded, false if end of file is reached.
     * 
     * @throws std::runtime_error if decoding fails.
     */
    bool decodeNextFrame(AVFrame *frame);

    /**
     * @brief Gets the width of the video.
     * 
     * @return The width of the video in pixels.
     */
    int getWidth() const;

    /**
     * @brief Gets the height of the video.
     * 
     * @return The height of the video in pixels.
     */
    int getHeight() const;

    /**
     * @brief Gets the pixel format of the video.
     * 
     * @return The pixel format of the video.
     */
    AVPixelFormat getPixelFormat() const;

private:
    AVFormatContext *formatContext; ///< FFmpeg format context.
    AVCodecContext *codecContext;   ///< FFmpeg codec context.
    int videoStreamIndex;           ///< Index of the video stream.
};

#endif // DECODER_HPP
