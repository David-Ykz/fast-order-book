#include "worker.h"

void Worker::run() {
    constexpr uint64_t numIterations = 10000;
    constexpr size_t numOrders = 1000;

    StockParameters params{1, 0.0001, 0.01, 1};
    GBMGenerator generator(params);

    for (int i = 0; i < numIterations; i++) {
        cout << "generating" << endl;
        ClientOrder* orders = generateOrders(generator, 50, 1, 10000, numOrders, 0.5);
        executeTrades(book, orders, numOrders);
        delete[] orders;
    }
}