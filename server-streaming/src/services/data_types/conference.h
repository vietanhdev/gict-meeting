#ifndef CONFERENCE_H
#define CONFERENCE_H

#include <string>
#include <vector>
#include <mutex>
#include <iostream>
#include <cstdlib>
#include <arpa/inet.h>

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

        // Update client address of a participant
        void connectClientImage(unsigned char client_id, struct sockaddr_in client_addr);

        void connectClientAudio(unsigned char client_id, struct sockaddr_in client_addr);

        // Update client image
        void setImage(unsigned char client_id, const cv::Mat & img);


        std::vector<Participant>& getParticipants() {return participants;}

};



#endif