#include "conference.h"



Conference::Conference() {}

Conference::Conference(const Conference& c) {
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
}

void Conference::setName(std::string name) {
    std::lock_guard<std::mutex> lock(global_mutex);
    this->name = name;
}
std::string Conference::getName() {
    std::lock_guard<std::mutex> lock(global_mutex);
    return this->name;
}
void Conference::setSecretKey(std::string secret_key) {
    std::lock_guard<std::mutex> lock(global_mutex);
    this->secret_key = secret_key;
}
std::string Conference::getSecretKey() {
    std::lock_guard<std::mutex> lock(global_mutex);
    return this->secret_key;
}
void Conference::setServerIp(std::string server_ip) {
    std::lock_guard<std::mutex> lock(global_mutex);
    this->server_ip = server_ip;
}
std::string Conference::getServerIp() {
    std::lock_guard<std::mutex> lock(global_mutex);
    return this->server_ip;
}
void Conference::setVideoUpPort(std::string video_up_port) {
    std::lock_guard<std::mutex> lock(global_mutex);
    this->video_up_port = std::atoi (video_up_port.c_str());
}
int Conference::getVideoUpPort() {
    std::lock_guard<std::mutex> lock(global_mutex);
    return this->video_up_port;
}
void Conference::setVideoDownPort(std::string video_down_port) {
    std::lock_guard<std::mutex> lock(global_mutex);
    this->video_down_port = std::atoi (video_down_port.c_str());
}
int Conference::getVideoDownPort() {
    std::lock_guard<std::mutex> lock(global_mutex);
    return this->video_down_port;
}
void Conference::setAudioUpPort(std::string audio_up_port) {
    std::lock_guard<std::mutex> lock(global_mutex);
    this->audio_up_port = std::atoi (audio_up_port.c_str());
}
int Conference::getAudioUpPort() {
    std::lock_guard<std::mutex> lock(global_mutex);
    return this->audio_up_port;
}
void Conference::setAudioDownPort(std::string audio_down_port) {
    std::lock_guard<std::mutex> lock(global_mutex);
    this->audio_down_port = std::atoi (audio_down_port.c_str());
}
int Conference::getAudioDownPort() {
    std::lock_guard<std::mutex> lock(global_mutex);
    return this->audio_down_port;
}
void Conference::setClientId(std::string client_id) {
    std::lock_guard<std::mutex> lock(global_mutex);
    this->client_id = std::atoi (client_id.c_str());
}
unsigned char Conference::getClientId() {
    std::lock_guard<std::mutex> lock(global_mutex);
    return this->client_id;
}
void Conference::setClientName(std::string client_name) {
    std::lock_guard<std::mutex> lock(global_mutex);
    this->client_name = client_name;
}
std::string Conference::getClientName() {
    std::lock_guard<std::mutex> lock(global_mutex);
    return this->client_name;
}
void Conference::setClientAuthKey(std::string client_auth_key) {
    std::lock_guard<std::mutex> lock(global_mutex);
    this->client_auth_key = std::atoi (client_auth_key.c_str());
}
int Conference::getClientAuthKey() {
    std::lock_guard<std::mutex> lock(global_mutex);
    return this->client_auth_key;
}


void Conference::setConferenceFrame(const cv::Mat &img) {
    std::lock_guard<std::mutex> lock(conference_frame_mutex);
    conference_frame = img;
}
cv::Mat Conference::getConferenceFrame() {
    std::lock_guard<std::mutex> lock(conference_frame_mutex);
    return this->conference_frame.clone();
}

void Conference::setClientCamFrame(const cv::Mat &img) {
    std::lock_guard<std::mutex> lock(client_cam_frame_mutex);
    client_cam_frame = img;
}
cv::Mat Conference::getClientCamFrame() {
    std::lock_guard<std::mutex> lock(client_cam_frame_mutex);
    return this->client_cam_frame.clone();
}


void Conference::addParticipant(Participant p) {
    this->participants.push_back(p);
}

void Conference::enqueueFrame(std::vector<unsigned char> newFrame) {
    // If the queue is full, discard the new frame
    if (Conference::instance().frameQueue.size() >= 5) return;
    else if (Conference::instance().frameQueue.size() == 0) Conference::instance().frameQueue.push_back(newFrame);
    else {
        bool inserted = false;

        if ((int)(newFrame[6]) == 0 && (int)(Conference::instance().frameQueue[0][6]) == 255) {
            // Special insert, when 2 frame seq bits has reached 255
            Conference::instance().frameQueue.push_back(newFrame);
        } else {
            for (auto it = Conference::instance().frameQueue.begin(); it != Conference::instance().frameQueue.end(); ++it) {
                if ( (int)( (*it)[6] ) == (int)( newFrame[6] )) {
                    if ( (int)( (*it)[7] ) > (int)( newFrame[7] )) {
                        Conference::instance().frameQueue.insert(it, newFrame);
                        inserted = true;
                        break;
                    }
                } else if ( (int)( (*it)[6] ) > (int)( newFrame[6] )) {
                    Conference::instance().frameQueue.insert(it, newFrame);
                    inserted = true;
                    break;
                }
            }

            if (!inserted) {
                Conference::instance().frameQueue.push_back(newFrame);
            }
        }
    }
}

std::vector<unsigned char> Conference::dequeueFrame() {
    std::vector<unsigned char> firstFrame;

    if (Conference::instance().frameQueue.size() > 0) {
        firstFrame = Conference::instance().frameQueue[0];

        // If the queue has only 1 image, don't delete it
        if (Conference::instance().frameQueue.size() > 1) {
            Conference::instance().frameQueue.erase(Conference::instance().frameQueue.begin());
        }

    }

    return firstFrame;
}
