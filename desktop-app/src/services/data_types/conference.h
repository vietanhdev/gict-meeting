#ifndef CONFERENCE_H
#define CONFERENCE_H

#include <string>
#include <vector>
#include <mutex>
#include <cstdlib>
#include <opencv2/opencv.hpp>

#include "participant.h"

class Conference {

    private:
        std::mutex global_mutex;
        std::string name;
        std::string secret_key;

        // Server
        std::string server_ip;
        int video_up_port;
        int video_down_port;
        int audio_up_port;
        int audio_down_port;

        // Conference image
        std::mutex conference_frame_mutex;
        cv::Mat conference_frame;

        std::mutex client_cam_frame_mutex;
        cv::Mat client_cam_frame;

        // Client
        unsigned char client_id;
        std::string client_name;
        int client_auth_key;

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
            this->client_id = c.client_id;
            this->client_name = c.client_name;
            this->client_auth_key = c.client_auth_key;
            this->participants = c.participants;
            return *this;
        }

        Conference(Conference &&) = delete;
        Conference & operator=(Conference &&) = delete;

        static auto& instance(){
            static Conference conference;
            return conference;
        }

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
        void setClientId(std::string);
        unsigned char getClientId();
        void setClientName(std::string);
        std::string getClientName();
        void setClientAuthKey(std::string);
        int getClientAuthKey();

        void setConferenceFrame(const cv::Mat &img);
        cv::Mat getConferenceFrame();

        void setClientCamFrame(const cv::Mat &img);
        cv::Mat getClientCamFrame();

        void addParticipant(Participant p);

        int frameSeqID = 0;
        std::vector< std::vector<unsigned char> > frameQueue;

        void enqueueFrame(std::vector<unsigned char> newFrame);
        std::vector<unsigned char> dequeueFrame();

};



#endif
