#include "worker.h"

void Worker::run() {
    constexpr uint64_t numIterations = 100000;
    constexpr size_t numOrders = 20;

    StockParameters params{1, 0.00015, 0.02, 1};
    GBMGenerator generator(params);

    broadcasterThread = thread([this]() {
        pinThreadToCore(this->book.ticker - 8000 + 2);
        broadcaster.listenAndBroadcast(ref(running)); 
    });

    for (int i = 0; i < numIterations; i++) {
        ClientOrder* orders = generateOrders(generator, 25, 1, 1000, numOrders, 0.5);
        executeTrades(book, orders, numOrders);
        delete[] orders;
    }
}

void Worker::pinThreadToCore(int core) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core, &cpuset);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
}