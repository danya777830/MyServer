#include "server.h"
#include <cstring>

Net::Server::Server(std::string string, std::string port) {
    
}

Net::Server::Server(std::string port) {
    ip_adress_ = "-1";
    port_ = port;
    socket_ = -1;
}

void Net::Server::init() {
    struct addrinfo hints, *server_info;
    int rv;
    int yes = 1;
    socket_ = -1;

    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (ip_adress_ == "-1") {
        hints.ai_flags = AI_PASSIVE;
        if ((rv = getaddrinfo(NULL, port_.c_str(), &hints, &server_info)) != 0) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
            exit(1);
        }
    }
    else {
        if ((rv = getaddrinfo(ip_adress_.c_str(), port_.c_str(), &hints, &server_info)) != 0) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
            exit(1);
        }
    }
    printf("find socket: done\n");

    for (auto i = server_info; i != NULL; i = server_info->ai_next) {
        if ((socket_ = socket(i->ai_family, i->ai_socktype, i->ai_protocol)) == -1) {
            printf("find socket: err\n");
            continue;
        }

        if (setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
            printf("setsockopt: err\n");
            continue;
        }

        if (bind(socket_, i->ai_addr, i->ai_addrlen) == -1) {
            close(socket_);
            printf("bind: err\n");
            continue;
        }

        break;
    }

    if(socket_ == -1) {
        printf("socket init: err\n");
        exit(1);
    }

    if (listen(socket_, BACKLOG) == -1) {
        printf("listen: err\n");
        exit(1);
    }

    printf("socket init: done\n");
    printf("server: waiting for connections...\n");
}



void Net::Server::start() {
    init();

    while(1) {
        receive();
        proccess();
        my_send();
    }
}

void Net::Server::receive() {
    struct sockaddr client_addr;
    socklen_t addr_size = sizeof client_addr;

    if ((client_socket_ = accept(socket_, &client_addr, &addr_size)) == -1) {
        perror("accept: err\n");
        exit(1);
    }

    printf("server connected\n");

    int bytes_cnt = -1;
    str = "";
    while(bytes_cnt != 0) {
        if ((bytes_cnt = recv(client_socket_, &buffer_, MAXDATASIZE, 0)) == -1) {
            printf("recv: err\n");
            exit(1);
        }

        for (int i = 0; i < bytes_cnt; ++i) {
            if(buffer_[i] == '\0') {
                bytes_cnt = 0;
                break;
            }
            str += buffer_[i];
        }
    }

    printf("server get data: %s\n", str.c_str());
}

void Net::Server::proccess() {
    str += "myaaay";

    printf("proccess end: done\n\0");
}

void Net::Server::my_send() {
    const char* mes = str.c_str();
    const char* i = mes;
    while(i != mes + str.size()) {
        int char_cnt;
        if ((char_cnt = send(client_socket_, i, (mes + str.size() - i), 0)) == -1) {
            perror("sending");
            exit(1);
        }
        i += char_cnt;
    }
    delete[] mes;
    close(client_socket_);
    printf("sending: accept");
}