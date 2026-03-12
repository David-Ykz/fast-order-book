#pragma once

#include "order.h"
#include "limit.h"
#include "bitset.h"
#include "buffer.h"

#include <unordered_map>
#include <ostream>
#include <boost/pool/object_pool.hpp>

class Book {
private:
    uint64_t orderIdCounter = 0;

    vector<Limit*> bids;
    vector<Limit*> asks;
    Bitset bidBitset;
    Bitset askBitset;
    unordered_map<uint64_t, Order*> orders;
    boost::pool<> orderPool{sizeof(Order)};
    boost::pool<> limitPool{sizeof(Limit)};

    inline Order* NewOrder(uint64_t id, uint64_t client, uint32_t price, uint32_t quantity) {
        void* mem = orderPool.malloc();
        Order* order = new (mem) Order();
        order->id = id;
        order->client = client;
        order->price = price;
        order->quantity = quantity;
        order->next = nullptr;
        order->prev = nullptr;
        
        return order;
    }
    inline Limit* NewLimit(Order* order) {
        void* limitMem = limitPool.malloc();
        Limit* limit = new (limitMem) Limit();
        limit->head = order;
        limit->tail = order;
        limit->totalQuantity = order->quantity;
        limit->price = order->price;

        return limit;
    }
    inline void removeAskLimit(Limit *askLimit) {
        askBitset.unset(askLimit->price);
        asks[askLimit->price] = nullptr;
        limitPool.free(askLimit);
    }
    inline void removeBidLimit(Limit *bidLimit) {
        bids[bidLimit->price] = nullptr;
        bidBitset.unset(bidLimit->price);
        limitPool.free(bidLimit);
    }

    void removeOrder(Order* order);
    void simplifyOrders(Order* bid, Order* ask, uint32_t price);
    template <bool bidNotAsk>
    void addOrder(Order* order);


public:
    uint32_t ticker;
    Buffer<FilledOrder> filledOrders;

    Book(uint32_t ticker) : ticker(ticker) {
        asks.assign(4096, nullptr);
        bids.assign(4096, nullptr);
    }
    
    inline size_t numOrders() {
        return orders.size();
    }

    template <bool bidNotAsk>
    void addOrder(uint64_t client, uint32_t price, uint32_t quantity);
    template <bool bidNotAsk>
    void cancelOrder(uint64_t orderId);

    void cleanup();
};