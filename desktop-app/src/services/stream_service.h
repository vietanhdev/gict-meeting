#ifndef STREAM_SERVICE_H
#define STREAM_SERVICE_H

#include <string>
#include <vector>
#include <mutex>
#include <atomic>
#include <thread>
#include <memory>
#include <iostream>

#include "conference.h"

class StreamService {


    public:
        static StreamService& instance(){
            static StreamService stream_service;
            return stream_service;
        }

    private:
        std::atomic<bool> streaming = false;
        Conference conference;

        std::shared_ptr<std::thread> streaming_thread;

    public:
        bool isStreaming();
        void setStreaming(bool streaming);

    private:
        StreamService(); // Disallow instantiation outside of the class.

        static void streamingThread();

    public:

        StreamService(const StreamService&) = delete;
        StreamService& operator=(const StreamService &) = delete;
        StreamService(StreamService &&) = delete;
        StreamService & operator=(StreamService &&) = delete;

        bool startStreaming();
        bool stopStreaming();
};

#endif