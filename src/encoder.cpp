#include "encoder.hpp"

Encoder::Encoder(const std::string &outputFile, int width, int height, int fps)
    : codecContext(nullptr), file(nullptr), frameCounter(0) {

  // Find the H.265 encoder
  const AVCodec *codec = avcodec_find_encoder(AV_CODEC_ID_HEVC);
  if (!codec) {
    throw std::runtime_error("H.265 encoder not found");
  }

  // Allocate codec context
  codecContext = avcodec_alloc_context3(codec);
  if (!codecContext) {
    throw std::runtime_error("Failed to allocate codec context");
  }

  // Set encoding parameters
  codecContext->bit_rate = 400000; // Bitrate in bits per second
  codecContext->width = width;
  codecContext->height = height;
  codecContext->time_base = AVRational{1, fps};
  codecContext->framerate = AVRational{fps, 1};
  codecContext->gop_size = 10;    // Group of pictures size
  codecContext->max_b_frames = 1; // Allow one B-frame
  codecContext->pix_fmt = AV_PIX_FMT_YUV420P;

  // Open codec
  if (avcodec_open2(codecContext, codec, nullptr) < 0) {
    throw std::runtime_error("Failed to open codec");
  }

  // Open output file
  file = fopen(outputFile.c_str(), "wb");
  if (!file) {
    throw std::runtime_error("Failed to open output file");
  }
}

Encoder::~Encoder() {
  // Flush the encoder and write any remaining packets
  flushEncoder();

  // Close and cleanup
  if (file) {
    fclose(file);
  }
  if (codecContext) {
    avcodec_free_context(&codecContext);
  }
}

void Encoder::encodeFrame(AVFrame *frame) {
  if (!frame) {
    throw std::invalid_argument("Input frame is null");
  }

  frame->pts = frameCounter++;

  // Send the frame to the encoder
  if (avcodec_send_frame(codecContext, frame) < 0) {
    throw std::runtime_error("Failed to send frame to encoder");
  }

  // Receive encoded packets
  AVPacket packet;
  av_init_packet(&packet);
  packet.data = nullptr;
  packet.size = 0;

  while (avcodec_receive_packet(codecContext, &packet) == 0) {
    fwrite(packet.data, 1, packet.size, file); // Write packet to file
    av_packet_unref(&packet);                  // Free the packet
  }
}

void Encoder::flushEncoder() {
  // Send a null frame to signal the end of the stream
  if (avcodec_send_frame(codecContext, nullptr) >= 0) {
    AVPacket packet;
    av_init_packet(&packet);
    packet.data = nullptr;
    packet.size = 0;

    while (avcodec_receive_packet(codecContext, &packet) == 0) {
      fwrite(packet.data, 1, packet.size, file); // Write packet to file
      av_packet_unref(&packet);                  // Free the packet
    }
  }
}