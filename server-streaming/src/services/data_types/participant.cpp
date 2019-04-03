#include "participant.h"


Participant::Participant(unsigned char client_id, std::string name) {
    this->client_id = client_id;
    this->name = name;
}