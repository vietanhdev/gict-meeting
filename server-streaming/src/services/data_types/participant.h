#ifndef PARTICIPANT_H
#define PARTICIPANT_H

#include <string>
#include <vector>
#include <memory>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <atomic>
#include <mutex>
#include <opencv2/opencv.hpp>


class Participant {

    private:
        std::mutex g_mutex; // Global mutex
        std::string name;
        unsigned char client_id;
        int auth_key;
        struct sockaddr_in client_image_addr;
        struct sockaddr_in client_audio_addr;
        cv::Mat image;

        bool image_connected = false; // image_connected = true means client have connected to the system and requested image stream  => client_addr has been set.

        bool audio_connected = false; // image_connected = true means client have connected to the system and requested audio stream  => client_addr has been set.

    public:

        int frame_width = 640;
        int frame_height = 480;
        int frame_quality = 50;

        Participant(const Participant& p) {
            std::lock_guard<std::mutex> lock(g_mutex);
            this->name = p.name;
            this->client_id = p.client_id;
            this->auth_key = p.auth_key;
            this->client_audio_addr = p.client_audio_addr;
            this->client_image_addr = p.client_image_addr;
            this->image_connected = p.image_connected;
            this->audio_connected = p.audio_connected;
        }

        std::string getName() {
            return name;
        }
        unsigned char getClientId() const {
            return client_id;
        }
        int getAuthKey() const {
            return auth_key;
        }

        void setClientImageAddress(struct sockaddr_in client_addr, int frame_width, int frame_height, int frame_quality) {
            std::lock_guard<std::mutex> lock(g_mutex);
            this->client_image_addr = client_addr;
            this->frame_width = frame_width;
            this->frame_height = frame_height;
            this->frame_quality = frame_quality;
            this->image_connected = true;
        }


        void setClientAudioAddress(struct sockaddr_in client_addr) {
            std::lock_guard<std::mutex> lock(g_mutex);
            this->client_audio_addr = client_addr;
            this->audio_connected = true;
        }

        struct sockaddr_in getClientImageAddress() {
            std::lock_guard<std::mutex> lock(g_mutex);
            return client_image_addr;
        }

        struct sockaddr_in getClientAudioAddress() {
            std::lock_guard<std::mutex> lock(g_mutex);
            return client_audio_addr;
        }

        cv::Mat getImage() {
            std::lock_guard<std::mutex> lock(g_mutex);
            return image.clone();
        }

        void setImage(const cv::Mat & img) {
            std::lock_guard<std::mutex> lock(g_mutex);
            image = img.clone();
        }


        bool isConnectedImage() {
            std::lock_guard<std::mutex> lock(g_mutex);
            return image_connected;
        }

        bool isConnectedAudio() {
            std::lock_guard<std::mutex> lock(g_mutex);
            return audio_connected;
        }

        Participant(unsigned char client_id, std::string name, int auth_key);

};

#endif