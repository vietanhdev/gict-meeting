#ifndef VIDEO_STREAM_SERVICE_H
#define VIDEO_STREAM_SERVICE_H

#include <string>
#include <vector>
#include <mutex>
#include <atomic>
#include <thread>
#include <memory>
#include <iostream>

#include "conference.h"


#include <iostream>
#include <string>
#include <vector>

#include "basic_protocol.h"
#include "sender_socket.h"
#include "video_capture.h"


class VideoStreamService {


    public:
        static VideoStreamService& instance(){
            static VideoStreamService stream_service;
            return stream_service;
        }

    private:

        // This flag will pass the start/stop streaming to the worker threads.
        // Set this to "true" to start streaming, otherwise, set it to "false"
        std::atomic<bool> streaming_signal_flag = false; 

        std::atomic<bool> streaming = false; // Current streaming status
        Conference conference;

        std::shared_ptr<std::thread> up_streaming_thread;
        std::shared_ptr<std::thread> down_streaming_thread;

    public:
        bool isStreaming();
        void setStreaming(bool streaming);

    private:
        VideoStreamService(); // Disallow instantiation outside of the class.

        static void upStreamingThread();
        static void downStreamingThread();

    public:

        VideoStreamService(const VideoStreamService&) = delete;
        VideoStreamService& operator=(const VideoStreamService &) = delete;
        VideoStreamService(VideoStreamService &&) = delete;
        VideoStreamService & operator=(VideoStreamService &&) = delete;

        bool startStreaming();
        bool stopStreaming();
};

#endif