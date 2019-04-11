#ifndef AUDIO_STREAM_SERVICE_H
#define AUDIO_STREAM_SERVICE_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <thread>
#include <memory>

#include "conference.h"
#include "audio_frame_protocol.h"
#include "server_socket.h"
#include "dev.h"

class AudioStreamService {

    private:
        std::shared_ptr<ServerSocket> audio_up_socket;
        std::shared_ptr<ServerSocket> audio_down_socket;

    public:
        static AudioStreamService& instance(){
            static AudioStreamService stream_service;
            return stream_service;
        }

        std::shared_ptr<ServerSocket> getAudioUpSocket(){
            if (audio_up_socket == nullptr) {
                Conference &conference = Conference::instance();
                const int port = conference.getAudioUpPort();
                audio_up_socket = std::make_shared<ServerSocket>(port);
                if (!audio_up_socket->bindSocketToListen()) {
                    std::cerr << "Could not bind socket at port: " << conference.getAudioUpPort() << std::endl;
                    exit(1);
                }
            }
            return audio_up_socket;
        }

        std::shared_ptr<ServerSocket> getAudioDownSocket(){
            if (audio_down_socket == nullptr) {
                Conference &conference = Conference::instance();
                const int port = conference.getAudioDownPort();
                audio_down_socket = std::make_shared<ServerSocket>(port);
                if (!audio_down_socket->bindSocketToListen()) {
                    std::cerr << "Could not bind socket at port: " << conference.getAudioDownPort() << std::endl;
                    exit(1);
                }
            }
            return audio_down_socket;
        }

    private:
        AudioStreamService(); // Disallow instantiation outside of the class.

        AudioStreamService(const AudioStreamService&) = delete;
        AudioStreamService& operator=(const AudioStreamService &) = delete;
        AudioStreamService(AudioStreamService &&) = delete;
        AudioStreamService & operator=(AudioStreamService &&) = delete;

    public:
        static void audioUpService();
        static void audioDownServiceListening();
        static void audioDownServiceSending();
        void sendAudio(unsigned char source_client_id, const std::vector<unsigned char> &data);

};

#endif