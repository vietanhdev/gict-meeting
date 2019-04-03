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
        struct sockaddr_in client_addr;
        cv::Mat image;

        bool connected = false; // Connected = true means client have connected to the system => client_addr has been set.

    public:
        Participant(const Participant& p) {
            std::lock_guard<std::mutex> lock(g_mutex);
            this->name = p.name;
            this->client_id = p.client_id;
            this->auth_key = p.auth_key;
            this->client_addr = p.client_addr;
            this->connected = p.connected;
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
        void setClientAddress(struct sockaddr_in client_addr) {
            std::lock_guard<std::mutex> lock(g_mutex);
            this->client_addr = client_addr;
            this->connected = true;
        }

        struct sockaddr_in getClientAddress() {
            std::lock_guard<std::mutex> lock(g_mutex);
            return client_addr;
        }

        cv::Mat getImage() {
            std::lock_guard<std::mutex> lock(g_mutex);
            return image.clone();
        }

        void setImage(const cv::Mat & img) {
            std::lock_guard<std::mutex> lock(g_mutex);
            image = img.clone();
        }

        Participant(unsigned char client_id, std::string name, int auth_key);

};

#endif