#include <cstdint>
#include <ostream>

using namespace std;

struct Order;

struct Limit {
    Order* head = nullptr;
    Order* tail = nullptr;
    uint32_t totalQuantity;
    uint32_t price;
};

inline ostream& operator<<(ostream& os, const Limit* limit) {
    os << "Price: " << limit->price;
    os << " | Total quantity: " << limit->totalQuantity;

    return os;
}
