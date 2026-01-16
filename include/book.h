#include "bitset.h"
#include "buffer.h"
#include <map>
#include <unordered_map>
#include <ostream>

struct Order;
struct Limit;

struct Order {
    uint64_t id;
    uint64_t client;
    uint32_t price;
    uint32_t quantity;

    Order* prev;
    Order* next;
    Limit* limit;
};

struct FilledOrder {
    uint64_t client;
    uint32_t price;
    uint32_t quantity;
};


struct Limit {
    Order* head = nullptr;
    Order* tail = nullptr;
    uint32_t totalQuantity;
    uint32_t price;
};

inline ostream& operator<<(ostream& os, const Order* order) {
    os << "Id: " << order->id;
    os << " | ClientId: " << order->client;
    os << " | Price: " << order->price;
    os << " | Quantity: " << order->quantity;

    return os;
}

inline ostream& operator<<(ostream& os, const FilledOrder* filledOrder) {
    os << "ClientId: " << filledOrder->client;
    os << " | Price: " << filledOrder->price;
    os << " | Quantity: " << filledOrder->quantity;

    return os;
}

inline ostream& operator<<(ostream& os, const Limit* limit) {
    os << "Price: " << limit->price;
    os << " | Total quantity: " << limit->totalQuantity;

    return os;
}

class Book {
private:
    uint32_t ticker;
    map<uint32_t, Limit*> bids;
    map<uint32_t, Limit*> asks;
    Bitset bidBitset;
    Bitset askBitset;
    Buffer<FilledOrder*> filledOrders;
    unordered_map<uint64_t, Order*> orders;

public:
    Book(uint32_t ticker) : ticker(ticker) {}
    
    inline size_t numOrders() {
        return orders.size();
    }

    void removeOrder(Order* order);

    void simplifyOrders(Order* bid, Order* ask, uint32_t price);

    template <bool bidNotAsk>
    void addOrder(Order* order);
    template <bool bidNotAsk>
    void cancelOrder(uint64_t orderId);

    bool pollFilledOrders(FilledOrder* &filledOrder);
    void printBook();
    void cleanup();


};

/* TODO:
    - convert map to array with bitset
    - use a pool of orders instead of deleting
*/