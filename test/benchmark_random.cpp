#include "../book/book.h"
#include <iostream>
#include <random>
#include <chrono>

#define NUM_ORDERS 10000000

Order* orders[NUM_ORDERS];
bool bidOrAsk[NUM_ORDERS];

inline uint64_t tick() noexcept {
    return chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
}

void generateRandomOrders(uint32_t startPrice, uint32_t endPrice, uint32_t minQuantity, uint32_t maxQuantity,  int seed1, int seed2, int seed3) {
    mt19937 priceGenerator(seed1);
    mt19937 quantityGenerator(seed2);
    mt19937 boolGenerator(seed3);
    uniform_int_distribution<uint32_t> priceDistribution(startPrice, endPrice);
    uniform_int_distribution<uint32_t> quantityDistribution(minQuantity, maxQuantity);
    uniform_int_distribution<int> boolDistribution(0, 1);

    for (uint64_t i = 0; i < NUM_ORDERS; i++) {
        bool bidNotAsk = boolDistribution(boolGenerator);
        uint32_t price = priceDistribution(priceGenerator);
        uint32_t quantity = quantityDistribution(quantityGenerator);
        Order* order = new Order{i, i, price, quantity};
        orders[i] = order;
        bidOrAsk[i] = bidNotAsk;
    }
}

void executeTrades(Book &book) {
    for (int i = 0; i < NUM_ORDERS; i++) {
        uint64_t orderId = orders[i]->id;
        uint64_t clientId = orders[i]->client;
        uint32_t price = orders[i]->price;
        uint32_t volume = orders[i]->quantity;

        if (bidOrAsk[i]) {
            book.addOrder<true>(orderId, clientId, price, volume);
        } else {
            book.addOrder<false>(orderId, clientId, price, volume);
        }
    }
}



int main() {
    generateRandomOrders(1, 4095, 1, 1000000, 2768464, 3945746, 1057837);
    Book book = Book(0);
    uint64_t startTime = tick();
    executeTrades(book);
    uint64_t endTime = tick();

    // book.printBook();
    // cout << endl;
    cout << endTime - startTime << " ms" << endl;

    book.cleanup();
    for (int i = 0; i < NUM_ORDERS; i++) {
        delete orders[i];
    }

    return 0;

}