#include "../include/book.h"
#include <iostream>
template void Book::addOrder<true>(Order*);
template void Book::addOrder<false>(Order*);
template void Book::cancelOrder<true>(uint64_t);
template void Book::cancelOrder<false>(uint64_t);


void Book::removeOrder(Order* order) {
    Limit* limit = order->limit;
    if (order == limit->head) limit->head = order->next;
    if (order == limit->tail) limit->tail = order->prev;
    if (order->prev != nullptr) order->prev->next = order->next;
    if (order->next != nullptr) order->next->prev = order->prev;

    orders.erase(order->id);
    delete order;
};


void Book::simplifyOrders(Order* bid, Order* ask, uint32_t price) {
    FilledOrder *filledBid;
    FilledOrder *filledAsk;
    uint32_t quantity;
    if (bid->quantity > ask->quantity) {
        quantity = ask->quantity;
        bid->quantity -= quantity;
        ask->quantity = 0;
    } else if (bid->quantity < ask->quantity) {
        quantity = bid->quantity;
        ask->quantity -= quantity;
        bid->quantity = 0;
    } else {
        quantity = ask->quantity;
        ask->quantity = 0;
        bid->quantity = 0;
    }

    if (bid->limit != nullptr) bid->limit->totalQuantity -= quantity;
    if (ask->limit != nullptr) ask->limit->totalQuantity -= quantity;
    

    filledBid = new FilledOrder{bid->client, price, quantity};
    filledAsk = new FilledOrder{ask->client, price, quantity};
    filledOrders.produce(filledBid);
    filledOrders.produce(filledAsk);
};


template <bool bidNotAsk>
void Book::addOrder(Order* order) {
    if constexpr (bidNotAsk) { // Bid order
        while (!asks.empty()) {
            // get highest ask
            auto minAsk = asks.begin();
            // if bid price is less than ask price, we can't match
            if (order->price < minAsk->first) break;

            Limit* askLimit = minAsk->second;
            Order* currAskOrder = askLimit->head;

            while (currAskOrder != nullptr) {
                Order* nextAskOrder = currAskOrder->next;
                // compare and fill the order with the lowest quantity
                simplifyOrders(order, currAskOrder, askLimit->price);
                if (!currAskOrder->quantity) {
                    removeOrder(currAskOrder);
                    if (!askLimit->totalQuantity) {
                        asks.erase(askLimit->price);
                        delete askLimit;
                    }
                }
                if (!order->quantity) {
                    delete order;
                    return;
                }
                currAskOrder = nextAskOrder;
            }
            // we filled as many orders as we can, now we add the bid to the book
            // Limit* bidLimit = new Limit{order, order, order->quantity, order->price};
            // order->limit = bidLimit;
            // bids[order->price] = bidLimit;
            // orders[order->id] = order;
            // return;
        }
        auto it = bids.find(order->price);
        if (it == bids.end()) {
            Limit* bidLimit = new Limit{order, order, order->quantity, order->price};
            order->limit = bidLimit;
            bids[order->price] = bidLimit;
        } else {
            Limit* bidLimit = it->second;
            bidLimit->tail->next = order;
            order->prev = bidLimit->tail;
            order->limit = bidLimit;
            bidLimit->tail = order;
            bidLimit->totalQuantity += order->quantity;
        }
    } else { // Ask order
        while (!bids.empty()) {
            // get lowest bid
            auto maxBid = --bids.end();
            // if ask price is greater than bid price, we can't match
            if (order->price > maxBid->first) break;

            Limit* bidLimit = maxBid->second;
            Order* currBidOrder = bidLimit->head;
            while (currBidOrder != nullptr) {
                Order* nextBidOrder = currBidOrder->next;
                // compare and fill the order with the lowest quantity
                simplifyOrders(order, currBidOrder, bidLimit->price);
                if (!currBidOrder->quantity) {
                    removeOrder(currBidOrder);
                    if (!bidLimit->totalQuantity) {
                        bids.erase(bidLimit->price);
                        delete bidLimit;
                    }
                }
                if (!order->quantity) {
                    delete order;
                    return;
                }
                currBidOrder = nextBidOrder;
            }
            // we filled as many orders as we can, now we add the ask to the book
            // Limit* askLimit = new Limit{order, order, order->quantity, order->price};
            // order->limit = askLimit;
            // asks[order->price] = askLimit;
            // orders[order->id] = order;
            // return;
        }
        auto it = asks.find(order->price);
        if (it == asks.end()) {
            Limit* askLimit = new Limit{order, order, order->quantity, order->price};
            order->limit = askLimit;
            asks[order->price] = askLimit;
        } else {
            Limit* askLimit = it->second;
            askLimit->tail->next = order;
            order->prev = askLimit->tail;
            order->limit = askLimit;
            askLimit->tail = order;
            askLimit->totalQuantity += order->quantity;
        }
    }
    orders[order->id] = order;
}

template <bool bidNotAsk>
void Book::cancelOrder(uint64_t orderId) {
    Order* order = orders.find(orderId)->second;
    Limit* limit = order->limit;
    removeOrder(order);
    limit->totalQuantity -= order->quantity;
    if (!limit->totalQuantity) {
        if constexpr (bidNotAsk) {
            bids.erase(limit->price);
        } else {
            asks.erase(limit->price);
        }
    }
}

bool Book::pollFilledOrders(FilledOrder* &filledOrder) {
    return filledOrders.consume(filledOrder);
}


void Book::printBook() {
    cout << "---------- Bids ----------" << endl;
    for (auto it = bids.begin(); it != bids.end(); it++) {
        cout << it->second << endl;
    }

    cout << "---------- Asks ----------" << endl;
    for (auto it = asks.rbegin(); it != asks.rend(); it++) {
        cout << it->second << endl;
    }
}

void Book::cleanup() {
    for (auto it = orders.begin(); it != orders.end(); ) {
        delete it->second;
        it = orders.erase(it);
    }

    for (auto it = bids.begin(); it != bids.end(); ) {
        delete it->second;
        it = bids.erase(it);
    }

    for (auto it = asks.begin(); it != asks.end(); ) {
        delete it->second;
        it = asks.erase(it);
    }
}