extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libavutil/log.h>
#include <libswscale/swscale.h>
}

#include "decoder.hpp"
#include "encoder.hpp"

#include <iostream>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <video_file>" << std::endl;
    return -1;
  }

  const char *filename = argv[1];

  // Initialize FFmpeg libraries
  av_log_set_level(AV_LOG_ERROR);

  Decoder dec(filename);
  Encoder enc("./out.h265", 320, 240, 15);

  // Allocate frame
  AVFrame *frame = av_frame_alloc();
  if (!frame) {
    throw std::runtime_error("Failed to allocate frame");
  }

  // Decode frames
  while (dec.decodeNextFrame(frame)) {
    enc.encodeFrame(frame);
  }

  av_frame_free(&frame);

  std::cout << "Finished processing video." << std::endl;
  return 0;
}