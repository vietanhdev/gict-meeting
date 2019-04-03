#ifndef CONFERENCE_H
#define CONFERENCE_H

#include <string>
#include <vector>
#include <mutex>
#include <iostream>
#include <cstdlib>

#include "participant.h"
#include "yaml-cpp/yaml.h"

class Conference {

    public:
        std::mutex global_mutex;

        std::string name;
        std::string secret_key;

        // Server
        std::string server_ip;
        int video_up_port;
        int video_down_port;
        int audio_up_port;
        int audio_down_port;

        std::vector<Participant> participants;

    public:
        Conference();
        Conference(const Conference&);
        Conference& operator=(const Conference& c) {
            this->name = c.name;
            this->secret_key = c.secret_key;
            this->server_ip = c.server_ip;
            this->video_up_port = c.video_up_port;
            this->video_down_port = c.video_down_port;
            this->audio_up_port = c.audio_up_port;
            this->audio_down_port = c.audio_down_port;
            this->participants = c.participants;
            return *this;
        }

        Conference(Conference &&) = delete;
        Conference & operator=(Conference &&) = delete;

        static auto& instance(){
            static Conference conference;
            return conference;
        }


        void readFromFile(std::string);

        void setName(std::string);
        std::string getName();
        void setSecretKey(std::string);
        std::string getSecretKey();
        void setServerIp(std::string);
        std::string getServerIp();
        void setVideoUpPort(std::string);
        int getVideoUpPort();
        void setVideoDownPort(std::string);
        int getVideoDownPort();
        void setAudioUpPort(std::string);
        int getAudioUpPort();
        void setAudioDownPort(std::string);
        int getAudioDownPort();

        // Check the auth_key of participants
        // Return true: correct id and auth_key
        // Otherwise, return false
        bool checkAuth(unsigned char client_id, int auth_key);

};



#endif