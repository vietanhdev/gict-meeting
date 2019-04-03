#ifndef PARTICIPANT_H
#define PARTICIPANT_H

#include <string>
#include <vector>
#include <memory>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

class Participant {

    public:
        std::string name;
        unsigned char client_id;
        std::shared_ptr<struct sockaddr_in> client_addr;

        Participant(unsigned char client_id, std::string name);

};

#endif