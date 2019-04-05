#ifndef VIDEO_STREAM_SERVICE_H
#define VIDEO_STREAM_SERVICE_H


#include <QObject>
#include <string>
#include <vector>
#include <mutex>
#include <atomic>
#include <thread>
#include <memory>
#include <iostream>

#include "video_frame_protocol.h"
#include "client_socket.h"
#include "video_capture.h"

#include "conference.h"

#include "timer.h"

class VideoStreamService: public QObject {

    Q_OBJECT

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

        std::shared_ptr<std::thread> up_streaming_thread;
        std::shared_ptr<std::thread> down_streaming_thread;

        // Timer
        // Used to control the up streaming framerate
        Timer::time_point_t last_image_up_time; // Last time we sent image to the server
        float frame_up_fps = 15; // Frame rate of image streaming to the server
        

        // Flip image or not. It is applied globally
        std::atomic<bool> flip_cam = true;

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

        // Set to flip camera frame or not. It is applied globally
        void setFlipCamFrame(bool flip);
        bool shouldFlipCamFrame();

    signals:
        void newConferenceFrame();
        void newClientCamFrame();
};

#endif