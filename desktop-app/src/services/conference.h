#ifndef CONFERENCE_H
#define CONFERENCE_H

#include <string>
#include <vector>
#include <mutex>

#include "participant.h"

class Conference {

    public:
        std::mutex global_mutex;

        std::string name;
        std::string secret_key;

        // Server
        std::string server_ip;
        std::string video_up_port;
        std::string video_down_port;
        std::string audio_up_port;
        std::string audio_down_port;

        // Client
        std::string client_id;
        std::string client_name;

        std::vector<Participant> participants;

    private:
        Conference(); // Disallow instantiation outside of the class.

    public:

        Conference(const Conference&) = delete;
        Conference& operator=(const Conference &) = delete;
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
        std::string getVideoUpPort();
        void setVideoDownPort(std::string);
        std::string getVideoDownPort();
        void setAudioUpPort(std::string);
        std::string getAudioUpPort();
        void setAudioDownPort(std::string);
        std::string getAudioDownPort();
        void setClientId(std::string);
        std::string getClientId();
        void setClientName(std::string);
        std::string getClientName();

        void addParticipant(Participant p);


};



#endif