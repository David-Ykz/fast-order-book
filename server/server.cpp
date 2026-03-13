#include "worker.h"
#include <memory>

int main() {
    unordered_map<uint32_t, thread> exchange;
    for (uint32_t ticker = 8000; ticker <= 8004; ticker += 2) {
        auto workerPointer = make_shared<Worker>(ticker);
        exchange[ticker] = thread([workerPointer, ticker]() mutable {
            workerPointer->pinThreadToCore(ticker - 8000 + 1);
            workerPointer->run(); 
        });
    }

    for (auto& [ticker, workerThread] : exchange) {
        if (workerThread.joinable()) {
            workerThread.join();
        }
    }
    return 0;
}