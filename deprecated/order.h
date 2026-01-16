using namespace std;
#include <iostream>

static int64_t idCounter = 0;

class Order {
public:
    int64_t id;
    int volume;
    int price;
    
    Order(int volume, int price) {
        this->id = idCounter++;
        this->volume = volume;
        this->price = price;
    }

    void print() {
        cout << "id: " << id << " volume: " << volume << " price: " << price << endl;
    }
};