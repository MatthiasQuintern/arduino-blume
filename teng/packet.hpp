#pragma once
#include <vector>
#include <cstdint>

enum PacketType : uint8_t {
    MESSAGE, STATUS, COMMAND
};

struct Packet {
    public: 
        using size_type = uint16_t;
        Packet(PacketType type, size_type size)
            : type(type), size(size) {
            payload = new uint8_t[size];
        }
        ~Packet() {
            delete[] payload;
        }
        uint8_t type;
        size_type size;
        uint8_t* payload;

};
