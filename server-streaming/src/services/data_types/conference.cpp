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
    this->participants = c.participants;
}


bool requireConfigField(const YAML::Node & config, std::string config_field) {
    if (!config[config_field]) {
        std::cout << "Wrong format configuration file - Missing:" + config_field << std::endl;
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
    this->video_up_port = video_up_port;
}
std::string Conference::getVideoUpPort() {
    std::lock_guard<std::mutex> lock(global_mutex);
    return this->video_up_port;
}
void Conference::setVideoDownPort(std::string video_down_port) {
    std::lock_guard<std::mutex> lock(global_mutex);
    this->video_down_port = video_down_port;
}
std::string Conference::getVideoDownPort() {
    std::lock_guard<std::mutex> lock(global_mutex);
    return this->video_down_port;
}
void Conference::setAudioUpPort(std::string audio_up_port) {
    std::lock_guard<std::mutex> lock(global_mutex);
    this->audio_up_port = audio_up_port;
}
std::string Conference::getAudioUpPort() {
    std::lock_guard<std::mutex> lock(global_mutex);
    return this->audio_up_port;
}
void Conference::setAudioDownPort(std::string audio_down_port) {
    std::lock_guard<std::mutex> lock(global_mutex);
    this->audio_down_port = audio_down_port;
}
std::string Conference::getAudioDownPort() {
    std::lock_guard<std::mutex> lock(global_mutex);
    return this->audio_down_port;
}

void Conference::addParticipant(Participant p) {
    this->participants.push_back(p);
}