extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/log.h>
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

    
    std::cout << "Finished processing video." << std::endl;
    return 0;
}