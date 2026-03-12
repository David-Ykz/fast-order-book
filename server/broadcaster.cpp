#include "broadcaster.h"
#include <iostream>

void Broadcaster::setupMulticast() {
    multicastSock = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&multicastAddr, 0, sizeof(multicastAddr));
    multicastAddr.sin_family = AF_INET;
    multicastAddr.sin_addr.s_addr = inet_addr("239.0.0.1");
    multicastAddr.sin_port = htons(book->ticker);
}

void Broadcaster::listenAndBroadcast(atomic_bool &running) {
    while (running) {
        FilledOrder filledOrder;
        bool hasData = book->filledOrders.consume(filledOrder);
        if (!hasData) continue;
        cout << "broadcasting" << endl;
        TradeEvent msg{filledOrder.client, filledOrder.price, filledOrder.quantity, book->ticker};
        sendto(multicastSock, &msg, sizeof(msg), 0, (struct sockaddr*)&multicastAddr, sizeof(multicastAddr));
    }
}