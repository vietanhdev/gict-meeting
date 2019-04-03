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
    this->client_id = client_id;
}
std::string Conference::getClientId() {
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


void Conference::addParticipant(Participant p) {
    this->participants.push_back(p);
}