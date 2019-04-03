#include "participant.h"


Participant::Participant(unsigned char client_id, std::string name, int auth_key) {
    this->client_id = client_id;
    this->name = name;
    this->auth_key = auth_key;
}