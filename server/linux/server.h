#pragma once

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define MAXDATASIZE 10
#define BACKLOG 10

namespace Net {
    class Server {
        private:
            int socket_, client_socket_;
            std::string ip_adress_, port_;
            char buffer_[MAXDATASIZE];
            std::string str;

            void init();
            void receive();
		    void proccess();
		    void my_send();
        public: 
            Server(std::string, std::string);
            Server(std::string);

            void start();
    };
}