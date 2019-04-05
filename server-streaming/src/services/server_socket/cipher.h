#ifndef CIPHER_H
#define CIPHER_H

#include <string>
#include <vector>

class Cipher {

    public:

    static void xor_crypt(const std::string &key, std::vector<unsigned char>& data) {
        for (size_t i = 0; i != data.size(); i++)
            data[i] ^= key[ i % key.size() ];
    }

};


#endif

