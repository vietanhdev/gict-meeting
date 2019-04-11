#ifndef VIDEO_STREAM_SERVICE_H
#define VIDEO_STREAM_SERVICE_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <thread>
#include <memory>

#include "conference.h"
#include "video_frame_protocol.h"
#include "server_socket.h"
#include "dev.h"


#include "timer.h"

class VideoStreamService {

    private:
        std::shared_ptr<ServerSocket> video_up_socket;
        std::shared_ptr<ServerSocket> video_down_socket;

        // Timer
        // Used to control the down streaming framerate
        Timer::time_point_t last_image_up_time; // Last time we sent image to all client
        float frame_up_fps = 15; // Frame rate of image streaming to clients

    public:
        static VideoStreamService& instance(){
            static VideoStreamService stream_service;
            return stream_service;
        }

        std::shared_ptr<ServerSocket> getVideoUpSocket(){
            if (video_up_socket == nullptr) {
                Conference &conference = Conference::instance();
                const int port = conference.getVideoUpPort();
                video_up_socket = std::make_shared<ServerSocket>(port);
                if (!video_up_socket->bindSocketToListen()) {
                    std::cerr << "Could not bind socket at port: " << conference.getVideoUpPort() << std::endl;
                    exit(1);
                }
            }
            return video_up_socket;
        }

        std::shared_ptr<ServerSocket> getVideoDownSocket(){
            if (video_down_socket == nullptr) {
                Conference &conference = Conference::instance();
                const int port = conference.getVideoDownPort();
                video_down_socket = std::make_shared<ServerSocket>(port);
                if (!video_down_socket->bindSocketToListen()) {
                    std::cerr << "Could not bind socket at port: " << conference.getVideoDownPort() << std::endl;
                    exit(1);
                }
            }
            return video_down_socket;
        }

    private:
        VideoStreamService(); // Disallow instantiation outside of the class.

        VideoStreamService(const VideoStreamService&) = delete;
        VideoStreamService& operator=(const VideoStreamService &) = delete;
        VideoStreamService(VideoStreamService &&) = delete;
        VideoStreamService & operator=(VideoStreamService &&) = delete;

    public:
        static void videoUpService();
        static void videoDownServiceListening();
        static void videoDownServiceSending();

};

#endif