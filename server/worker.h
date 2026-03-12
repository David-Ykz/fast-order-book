#include "broadcaster.h"
#include "../test/benchmark_latency.h"
#include <thread>


class Worker {
private:
    Book book;
    Broadcaster broadcaster;
    thread broadcasterThread;
    atomic_bool running{true};

public:
    Worker(uint32_t ticker) : book(ticker), broadcaster(&book) {
        broadcaster.setupMulticast();
        broadcasterThread = thread([this]() {
            broadcaster.listenAndBroadcast(ref(running)); 
        });
    }
    ~Worker() {
        running = false;
        if (broadcasterThread.joinable()) {
            broadcasterThread.join();
        }
        book.cleanup();
    }

    void run();
};