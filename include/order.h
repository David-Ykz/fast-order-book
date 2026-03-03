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

