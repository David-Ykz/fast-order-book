#include "../include/book.h"
#include <iostream>

template void Book::addOrder<true>(Order* order);
template void Book::addOrder<false>(Order* order);
template void Book::addOrder<true>(uint64_t id, uint64_t client, uint32_t price, uint32_t quantity);
template void Book::addOrder<false>(uint64_t id, uint64_t client, uint32_t price, uint32_t quantity);
template void Book::cancelOrder<true>(uint64_t);
template void Book::cancelOrder<false>(uint64_t);

/* Removes the order from the book and deletes it */
void Book::removeOrder(Order* order) {
    Limit* limit = order->limit;
    if (order == limit->head) limit->head = order->next;
    if (order == limit->tail) limit->tail = order->prev;
    if (order->prev != nullptr) order->prev->next = order->next;
    if (order->next != nullptr) order->next->prev = order->prev;

    orders.erase(order->id);

    order->~Order();
    orderPool.free(order);
};

/* Matches a bid and an ask */
void Book::simplifyOrders(Order* bid, Order* ask, uint32_t price) {
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
};

/* Tries to match the order. If the order was not fully matched, it adds the order to the book. */
template <bool bidNotAsk>
void Book::addOrder(Order* order) {
    if constexpr (bidNotAsk) { // Bid order
        while (!askBitset.empty()) {
            // get highest ask
            Limit* askLimit = asks[askBitset.start()];

            // if bid price is less than ask price, we can't match
            if (order->price < askLimit->price) break;

            Order* currAskOrder = askLimit->head;
            while (currAskOrder != nullptr) {
                Order* nextAskOrder = currAskOrder->next;
                // compare and fill the order with the lowest quantity
                simplifyOrders(order, currAskOrder, askLimit->price);
                if (!currAskOrder->quantity) {
                    removeOrder(currAskOrder);
                    if (!askLimit->totalQuantity) {
                        removeAskLimit(askLimit);
                    }
                }
                // order has been matched
                if (!order->quantity) {
                    return;
                }
                currAskOrder = nextAskOrder;
            }
        }
        // done matching, now add order to book
        if (bids[order->price] == nullptr) {
            Limit* bidLimit = NewLimit(order);            
            order->limit = bidLimit;
            bids[order->price] = bidLimit;
            bidBitset.set(order->price);
        } else {
            Limit* bidLimit = bids[order->price];
            bidLimit->tail->next = order;
            order->prev = bidLimit->tail;
            order->limit = bidLimit;
            bidLimit->tail = order;
            bidLimit->totalQuantity += order->quantity;
        }
    } else { // Ask order
        while (!bidBitset.empty()) {
            // get lowest bid
            Limit* bidLimit = bids[bidBitset.end()];
            // if ask price is greater than bid price, we can't match
            if (order->price > bidLimit->price) break;

            Order* currBidOrder = bidLimit->head;
            while (currBidOrder != nullptr) {
                Order* nextBidOrder = currBidOrder->next;
                // compare and fill the order with the lowest quantity
                simplifyOrders(order, currBidOrder, bidLimit->price);
                if (!currBidOrder->quantity) {
                    removeOrder(currBidOrder);
                    if (!bidLimit->totalQuantity) {
                        removeBidLimit(bidLimit);
                    }
                }
                // order has been matched
                if (!order->quantity) {
                    return;
                }
                currBidOrder = nextBidOrder;
            }
        }
        // done matching, now add order to book
        if (asks[order->price] == nullptr) {
            Limit* askLimit = NewLimit(order);
            order->limit = askLimit;
            asks[order->price] = askLimit;
            askBitset.set(order->price);
        } else {
            Limit* askLimit = asks[order->price];
            askLimit->tail->next = order;
            order->prev = askLimit->tail;
            order->limit = askLimit;
            askLimit->tail = order;
            askLimit->totalQuantity += order->quantity;
        }
    }
    orders[order->id] = order;
}

/* Wrapper for addOrder */
template <bool bidNotAsk>
void Book::addOrder(uint64_t id, uint64_t client, uint32_t price, uint32_t quantity) {
    Order* order = NewOrder(id, client, price, quantity);
    addOrder<bidNotAsk>(order);
}

/* Cancels an order and removes it from the book */
template <bool bidNotAsk>
void Book::cancelOrder(uint64_t orderId) {
    Order* order = orders.find(orderId)->second;
    Limit* limit = order->limit;
    removeOrder(order);
    limit->totalQuantity -= order->quantity;
    if (!limit->totalQuantity) {
        if constexpr (bidNotAsk) {
            bids[limit->price] = nullptr;
            bidBitset.unset(limit->price);
        } else {
            asks[limit->price] = nullptr;
            askBitset.unset(limit->price);
        }
    }
}

/* Clears the book, destroys any objects created, and clears the bitsets */
void Book::cleanup() {
    bids.clear();
    asks.clear();
    orders.clear();

    bidBitset.clear();
    askBitset.clear();
}