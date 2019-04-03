#ifndef VIDEO_STREAM_SERVICE_H
#define VIDEO_STREAM_SERVICE_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <thread>

#include "conference.h"
#include "video_frame_protocol.h"
#include "receiver_socket.h"


class VideoStreamService {

    public:
        static VideoStreamService& instance(){
            static VideoStreamService stream_service;
            return stream_service;
        }

    private:
        VideoStreamService(); // Disallow instantiation outside of the class.

        VideoStreamService(const VideoStreamService&) = delete;
        VideoStreamService& operator=(const VideoStreamService &) = delete;
        VideoStreamService(VideoStreamService &&) = delete;
        VideoStreamService & operator=(VideoStreamService &&) = delete;

    public:
        static void videoUpService();
        static void videoDownService();

};

#endif