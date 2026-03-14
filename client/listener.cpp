#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include "../server/message.h"
#include <string>
#include <fstream>
#include <random>

using namespace std;

int main(int argc, char* argv[]) {
    uint32_t ticker = stoi(argv[1]);
    int port = htons(ticker);
    const char* group = "239.0.0.1";

    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<> distribution(1000, 10000);
    string filename = "logs/" + to_string(ticker) + "_" + to_string(distribution(generator)) + ".txt";
    ofstream output(filename);

    // create the socket
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    int reuse = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    // bind to port
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = port;

    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Bind failed");
        return 1;
    }

    // join multicast group
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(group);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
        perror("Multicast join failed");
        return 1;
    }

    TradeEvent msg;
    int i = 0;
    while (true) {
        ssize_t len = recvfrom(sock, &msg, sizeof(msg), 0, NULL, NULL);
        // save every other tick since server broadcasts both bid and ask orders
        if (len > 0 && i++ % 2) {
            output << msg.price << " " << msg.quantity << endl;
            output.flush(); 
        }
    }

    return 0;
}