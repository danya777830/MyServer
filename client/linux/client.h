#pragma once

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define MAXDATASIZE 100
#define BACKLOG 10

namespace Net {
    class Client {
        private:
            int socket_;
            std::string ip_adress_, port_;
            char* buffer_;

            void init();
            void my_send();
            std::string receive();
        public: 
            Client(std::string, std::string);
            ~Client();

            std::string get();
    };
}