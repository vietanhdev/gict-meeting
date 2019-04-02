#include "conference.h"



Conference::Conference() {}

void Conference::setName(std::string name) {
    std::lock_guard<std::mutex> lock(global_mutex);
    this->name = name;
}
std::string Conference::getName() {
    std::lock_guard<std::mutex> lock(global_mutex);
    return this->name;
}
void Conference::setSecretKey(std::string) {
    std::lock_guard<std::mutex> lock(global_mutex);
    this->secret_key = secret_key;
}
std::string Conference::getSecretKey() {
    std::lock_guard<std::mutex> lock(global_mutex);
    return this->secret_key;
}
void Conference::setServerIp(std::string) {
    std::lock_guard<std::mutex> lock(global_mutex);
    this->server_ip = server_ip;
}
std::string Conference::getServerIp() {
    std::lock_guard<std::mutex> lock(global_mutex);
    return this->server_ip;
}
void Conference::setVideoUpPort(std::string) {
    std::lock_guard<std::mutex> lock(global_mutex);
    this->video_up_port = video_up_port;
}
std::string Conference::getVideoUpPort() {
    std::lock_guard<std::mutex> lock(global_mutex);
    return this->video_up_port;
}
void Conference::setVideoDownPort(std::string) {
    std::lock_guard<std::mutex> lock(global_mutex);
    this->video_down_port = video_down_port;
}
std::string Conference::getVideoDownPort() {
    std::lock_guard<std::mutex> lock(global_mutex);
    return this->video_down_port;
}
void Conference::setAudioUpPort(std::string) {
    std::lock_guard<std::mutex> lock(global_mutex);
    this->audio_up_port = audio_up_port;
}
std::string Conference::getAudioUpPort() {
    std::lock_guard<std::mutex> lock(global_mutex);
    return this->audio_up_port;
}
void Conference::setAudioDownPort(std::string) {
    std::lock_guard<std::mutex> lock(global_mutex);
    this->audio_down_port = audio_down_port;
}
std::string Conference::getAudioDownPort() {
    std::lock_guard<std::mutex> lock(global_mutex);
    return this->audio_down_port;
}
void Conference::setClientId(std::string) {
    std::lock_guard<std::mutex> lock(global_mutex);
    this->client_id = client_id;
}
std::string Conference::getClientId() {
    std::lock_guard<std::mutex> lock(global_mutex);
    return this->client_id;
}
void Conference::setClientName(std::string) {
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