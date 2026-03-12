#include "../book/book.h"
#include "message.h"
#include <arpa/inet.h>

class Broadcaster {
private:
    Book* book;
    int multicastSock;
    struct sockaddr_in multicastAddr;

public:
    Broadcaster(Book *book) : book(book) {}
    ~Broadcaster() {
        close(multicastSock);
    }

    void setupMulticast();
    void listenAndBroadcast(atomic_bool &running);
};