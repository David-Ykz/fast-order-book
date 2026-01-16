#include "order.h"
#include <queue>

class Limit {
public:
    queue<Order*> orders;
    int price;

    Limit(Order* order) {
        orders.push(order);
        price = order->price;
    }    
};