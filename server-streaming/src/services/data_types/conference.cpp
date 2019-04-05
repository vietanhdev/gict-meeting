#include "conference.h"

Conference::Conference() {}

bool requireConfigField(const YAML::Node& config, std::string config_field) {
    if (!config[config_field]) {
        std::cout << "Wrong format configuration file - Missing:" + config_field
                  << std::endl;
        return false;
    }
    return true;
}

void Conference::readFromFile(std::string filepath) {
    // Load config file
    YAML::Node config = YAML::LoadFile(filepath);

    // Check config fields
    if (!requireConfigField(config, "conference_name")) exit(1);
    if (!requireConfigField(config, "secret_key")) exit(1);
    if (!requireConfigField(config, "server_ip")) exit(1);
    if (!requireConfigField(config, "video_up_port")) exit(1);
    if (!requireConfigField(config, "video_down_port")) exit(1);
    if (!requireConfigField(config, "audio_up_port")) exit(1);
    if (!requireConfigField(config, "audio_down_port")) exit(1);

    this->setName(config["conference_name"].as<std::string>());
    this->setSecretKey(config["secret_key"].as<std::string>());
    this->setServerIp(config["server_ip"].as<std::string>());
    this->setVideoUpPort(config["video_up_port"].as<std::string>());
    this->setVideoDownPort(config["video_down_port"].as<std::string>());
    this->setAudioUpPort(config["audio_up_port"].as<std::string>());
    this->setAudioDownPort(config["audio_down_port"].as<std::string>());

    // Init participant list
    participants.clear();
    YAML::Node yaml_participants = config["participants"];
    for (std::size_t i = 0; i < yaml_participants.size(); i++) {
        YAML::Node yaml_participant = yaml_participants[i];
        unsigned char id = yaml_participant["client_id"].as<int>();
        std::string name = yaml_participant["name"].as<std::string>();
        int auth_key = yaml_participant["auth_key"].as<int>();
        participants.push_back(Participant(id, name, auth_key));
    }
}

// Check the auth_key of participants
bool Conference::checkAuth(unsigned char client_id, int auth_key) {
    for (int i = 0; i < participants.size(); ++i) {
        if (static_cast<int>(client_id) == static_cast<int>(participants[i].getClientId())) {
            if (auth_key == participants[i].getAuthKey()) {
                return true;
            }
            break;
        } 
    }
    return false;
}

void Conference::connectClientImage(unsigned char client_id, struct sockaddr_in client_addr) {
    for (int i = 0; i < participants.size(); ++i) {
        if (static_cast<int>(client_id) == static_cast<int>(participants[i].getClientId())) {
            participants[i].setClientImageAddress(client_addr);
            return;
        }
    }
}

void Conference::connectClientAudio(unsigned char client_id, struct sockaddr_in client_addr) {
    for (int i = 0; i < participants.size(); ++i) {
        if (static_cast<int>(client_id) == static_cast<int>(participants[i].getClientId())) {
            participants[i].setClientAudioAddress(client_addr);
            return;
        }
    }
}

void Conference::setImage(unsigned char client_id, const cv::Mat & img) {
    for (int i = 0; i < participants.size(); ++i) {
        if (static_cast<int>(client_id) == static_cast<int>(participants[i].getClientId())) {
            participants[i].setImage(img);
            return;
        } 
    }
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
    this->video_up_port = std::atoi(video_up_port.c_str());
}
int Conference::getVideoUpPort() {
    std::lock_guard<std::mutex> lock(global_mutex);
    return this->video_up_port;
}
void Conference::setVideoDownPort(std::string video_down_port) {
    std::lock_guard<std::mutex> lock(global_mutex);
    this->video_down_port = std::atoi(video_down_port.c_str());
}
int Conference::getVideoDownPort() {
    std::lock_guard<std::mutex> lock(global_mutex);
    return this->video_down_port;
}
void Conference::setAudioUpPort(std::string audio_up_port) {
    std::lock_guard<std::mutex> lock(global_mutex);
    this->audio_up_port = std::atoi(audio_up_port.c_str());
}
int Conference::getAudioUpPort() {
    std::lock_guard<std::mutex> lock(global_mutex);
    return this->audio_up_port;
}
void Conference::setAudioDownPort(std::string audio_down_port) {
    std::lock_guard<std::mutex> lock(global_mutex);
    this->audio_down_port = std::atoi(audio_down_port.c_str());
}
int Conference::getAudioDownPort() {
    std::lock_guard<std::mutex> lock(global_mutex);
    return this->audio_down_port;
}