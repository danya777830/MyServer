#include "client.h"

int main(int argc, char** argv) {
    Net::Client client("127.0.0.1", "5119");

    std::string ans = client.get();
    printf("ans: %s\n", ans.c_str());
}