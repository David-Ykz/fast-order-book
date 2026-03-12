#pragma once
#include "../book/book.h"
#include <iostream>
#include <random>
#include <chrono>
#include "stock_generator.h"

inline uint64_t tick() noexcept {
    return chrono::duration_cast<chrono::nanoseconds>(chrono::system_clock::now().time_since_epoch()).count();
}

ClientOrder* generateOrders(GBMGenerator& gbmGenerator, uint32_t spreadWidth, uint32_t minQuantity, uint32_t maxQuantity, size_t numOrders, double bidNotAskTendency);
void executeTrades(Book &book, ClientOrder* orders, size_t numOrders);
double test();