#include <cstdint>
#include <ostream>

using namespace std;

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

inline ostream& operator<<(ostream& os, const Order* order) {
    os << "Id: " << order->id;
    os << " | ClientId: " << order->client;
    os << " | Price: " << order->price;
    os << " | Quantity: " << order->quantity;

    return os;
}

struct FilledOrder {
    uint64_t client;
    uint32_t price;
    uint32_t quantity;
};

inline ostream& operator<<(ostream& os, const FilledOrder *filledOrder) {
    os << "ClientId: " << filledOrder->client;
    os << " | Price: " << filledOrder->price;
    os << " | Quantity: " << filledOrder->quantity;

    return os;
}

struct ClientOrder {
    uint64_t client;
    uint32_t price;
    uint32_t quantity;
    bool bidNotAsk;
};

inline ostream& operator<<(ostream& os, const ClientOrder* clientOrder) {
    os << " ClientId: " << clientOrder->client;
    os << " | Price: " << clientOrder->price;
    os << " | Quantity: " << clientOrder->quantity;
    os << " | Bid: " << clientOrder->bidNotAsk;

    return os;
}