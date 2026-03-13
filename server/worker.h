#include "broadcaster.h"
#include "../test/benchmark_latency.h"
#include <thread>
#include <pthread.h>
#include <sched.h>
#include <iostream>


class Worker {
private:
    Book book;
    Broadcaster broadcaster;
    thread broadcasterThread;
    atomic_bool running{true};

public:
    Worker(uint32_t ticker) : book(ticker), broadcaster(&book) {
        broadcaster.setupMulticast();
    }
    ~Worker() {
        running = false;
        if (broadcasterThread.joinable()) {
            broadcasterThread.join();
        }
        book.cleanup();
    }

    void run();
    void pinThreadToCore(int core);
};