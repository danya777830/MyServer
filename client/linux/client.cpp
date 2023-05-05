#include "client.h"
#include <cstring>
#include <unistd.h>

Net::Client::Client(std::string ip_adress, std::string port) {
    socket_ = -1;
    ip_adress_ = ip_adress;
    port_ = port;
    buffer_ = new char[MAXDATASIZE];
    init();
}

void Net::Client::init() {
    struct addrinfo hints, *servinfo;
    int rv;
    socket_ = -1;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(ip_adress_.c_str(), port_.c_str(), &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(1);
    }

    // loop through all the results and connect to the first we can
    for(auto i = servinfo; i != NULL; i = i->ai_next) {
        if ((socket_ = socket(i->ai_family, i->ai_socktype, i->ai_protocol)) == -1) {
            printf("find socket: err\n");
            continue;
        }

        if (connect(socket_, i->ai_addr, i->ai_addrlen) == -1) {
            close(socket_);
            printf("client connect: err\n");
            socket_ = -1;
            continue;
        }

        break;
    }

    if (socket_ == -1) {
        printf("find socket: err\n");
        exit(1);
    }
}

std::string Net::Client::get() {
    my_send();
    sleep(5);
    return receive();
}

void Net::Client::my_send() {
    printf("sending...\n");
    if (send(socket_, "Hellow world!" + '\0', 14, 0) == -1) {
        perror("send");
    }
    printf("sending: accept\n");
}

std::string Net::Client::receive() {
    printf("receiving...\n");
    int bytes_cnt = -1;
    std::string it;
    while(bytes_cnt != 0) {
        if ((bytes_cnt = recv(socket_, buffer_, MAXDATASIZE-1, 0)) == -1) {
            perror("recv");
            exit(1);
        }
        for (int i = 0; i < bytes_cnt && i < MAXDATASIZE; ++i) {
            it += buffer_[i];
            if(buffer_[i] == '\0') {
                bytes_cnt = 0;
                break;
            }
        }
    }
    printf("receiving: accept\n");
    return it;
}

Net::Client::~Client() {
    close(socket_);
    delete[] buffer_;
}