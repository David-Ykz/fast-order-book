#include "../include/book.h"
#include <iostream>

#define NUM_TESTS 1

struct TestResult {
    uint64_t client;
    uint32_t price;
    uint32_t quantity;
};

struct TestCase {
    Order** orders;
    bool* orderTypes;
    TestResult expectedResult;
};

bool testAddBid() {
    Book book = Book(0);
    Order* order = new Order{0, 0, 100, 100, nullptr, nullptr, nullptr};
    book.addOrder<true>(order);
    bool pass = book.numOrders() == 1;
    book.cleanup();
    return pass;
}

bool testAddAsk() {
    Book book = Book(0);
    Order* order = new Order{0, 0, 100, 100, nullptr, nullptr, nullptr};
    book.addOrder<false>(order);
    bool pass = book.numOrders() == 1;
    book.cleanup();
    return pass;
}

bool testAddEqualBidAsk() {
    Book book = Book(0);
    Order* order1 = new Order{0, 0, 100, 100, nullptr, nullptr, nullptr};
    Order* order2 = new Order{1, 1, 100, 100, nullptr, nullptr, nullptr};
    book.addOrder<true>(order1);
    book.addOrder<false>(order2);
    FilledOrder* filledOrder;
    while (book.pollFilledOrders(filledOrder)) {
        cout << filledOrder << endl;
        delete filledOrder;
    }
    bool pass = book.numOrders() == 0;
    book.cleanup();
    return pass;
}

bool testSmallAskBigBid() {
    Book book = Book(0);
    Order* order1 = new Order{0, 0, 100, 50, nullptr, nullptr, nullptr};
    Order* order2 = new Order{1, 1, 100, 100, nullptr, nullptr, nullptr};
    book.addOrder<false>(order1);
    book.addOrder<true>(order2);
    FilledOrder* filledOrder;
    while (book.pollFilledOrders(filledOrder)) {
        cout << filledOrder << endl;
        delete filledOrder;
    }
    bool pass = book.numOrders() == 1;
    book.cleanup();
    return pass;
}

bool testSmallBidBigAsk() {
    Book book = Book(0);
    Order* order1 = new Order{0, 0, 100, 50, nullptr, nullptr, nullptr};
    Order* order2 = new Order{1, 1, 100, 100, nullptr, nullptr, nullptr};
    book.addOrder<true>(order1);
    book.addOrder<false>(order2);
    FilledOrder* filledOrder;
    while (book.pollFilledOrders(filledOrder)) {
        cout << filledOrder << endl;
        delete filledOrder;
    }
    bool pass = book.numOrders() == 1;
    book.cleanup();
    return pass;
}

bool testNoFillBid() {
    Book book = Book(0);
    Order* order1 = new Order{0, 0, 100, 100, nullptr, nullptr, nullptr};
    Order* order2 = new Order{1, 1, 90, 100, nullptr, nullptr, nullptr};
    book.addOrder<false>(order1);
    book.addOrder<true>(order2);
    bool pass = book.numOrders() == 2;
    book.cleanup();
    return pass;
}

bool testNoFillAsk() {
    Book book = Book(0);
    Order* order1 = new Order{0, 0, 90, 100, nullptr, nullptr, nullptr};
    Order* order2 = new Order{1, 1, 100, 100, nullptr, nullptr, nullptr};
    book.addOrder<true>(order1);
    book.addOrder<false>(order2);
    bool pass = book.numOrders() == 2;
    book.cleanup();
    return pass;
}

bool testFillBid() {
    Book book = Book(0);
    Order* order1 = new Order{0, 0, 90, 100, nullptr, nullptr, nullptr};
    Order* order2 = new Order{1, 1, 100, 100, nullptr, nullptr, nullptr};
    book.addOrder<false>(order1);
    book.addOrder<true>(order2);
    FilledOrder* filledOrder;
    while (book.pollFilledOrders(filledOrder)) {
        cout << filledOrder << endl;
        delete filledOrder;
    }
    bool pass = book.numOrders() == 0;
    book.cleanup();
    return pass;
}

bool testFillAsk() {
    Book book = Book(0);
    Order* order1 = new Order{0, 0, 100, 100, nullptr, nullptr, nullptr};
    Order* order2 = new Order{1, 1, 90, 100, nullptr, nullptr, nullptr};
    book.addOrder<true>(order1);
    book.addOrder<false>(order2);
    FilledOrder* filledOrder;
    while (book.pollFilledOrders(filledOrder)) {
        cout << filledOrder << endl;
        delete filledOrder;
    }
    bool pass = book.numOrders() == 0;
    book.cleanup();
    return pass;
}

bool testAddBids() {
    Book book = Book(0);
    Order* order1 = new Order{0, 0, 90, 50, nullptr, nullptr, nullptr};
    Order* order2 = new Order{1, 1, 100, 100, nullptr, nullptr, nullptr};
    book.addOrder<true>(order1);
    book.addOrder<true>(order2);
    bool pass = book.numOrders() == 2;
    book.cleanup();
    return pass;
}

bool testAddAsks() {
    Book book = Book(0);
    Order* order1 = new Order{0, 0, 100, 50, nullptr, nullptr, nullptr};
    Order* order2 = new Order{1, 1, 90, 100, nullptr, nullptr, nullptr};
    book.addOrder<false>(order1);
    book.addOrder<false>(order2);
    bool pass = book.numOrders() == 2;
    book.cleanup();
    return pass;
}

bool testPartialFill() {
    Book book = Book(0);
    Order* order1 = new Order{0, 0, 100, 100, nullptr, nullptr, nullptr};
    Order* order2 = new Order{1, 1, 100, 100, nullptr, nullptr, nullptr};
    Order* order3 = new Order{2, 0, 110, 200, nullptr, nullptr, nullptr};
    Order* order4 = new Order{3, 2, 110, 150, nullptr, nullptr, nullptr};
    book.addOrder<false>(order1);
    book.addOrder<false>(order2);
    book.addOrder<false>(order3);
    book.addOrder<true>(order4);
    FilledOrder* filledOrder;
    while (book.pollFilledOrders(filledOrder)) {
        cout << filledOrder << endl;
        delete filledOrder;
    }
    bool pass = book.numOrders() == 2;
    book.cleanup();
    return pass;
}

bool testFullFill() {
    Book book = Book(0);
    Order* order1 = new Order{0, 0, 100, 100, nullptr, nullptr, nullptr};
    Order* order2 = new Order{1, 1, 100, 100, nullptr, nullptr, nullptr};
    Order* order3 = new Order{2, 0, 110, 200, nullptr, nullptr, nullptr};
    Order* order4 = new Order{3, 2, 110, 150, nullptr, nullptr, nullptr};
    Order* order5 = new Order{4, 3, 100, 50, nullptr, nullptr, nullptr};
    Order* order6 = new Order{5, 4, 110, 100, nullptr, nullptr, nullptr};
    Order* order7 = new Order{6, 3, 110, 100, nullptr, nullptr, nullptr};
    book.addOrder<false>(order1);
    book.addOrder<false>(order2);
    book.addOrder<false>(order3);
    book.addOrder<true>(order4);
    book.addOrder<true>(order5);
    book.addOrder<true>(order6);
    book.addOrder<true>(order7);
    FilledOrder* filledOrder;
    while (book.pollFilledOrders(filledOrder)) {
        cout << filledOrder << endl;
        delete filledOrder;
    }
    bool pass = book.numOrders() == 0;
    book.cleanup();
    return pass;
}

bool testCascadeFill() {
    Book book = Book(0);
    for (uint32_t i = 0; i < 1000; i++) {
        for (uint32_t j = 1; j <= 1000; j++) {
            Order* order = new Order{0, 0, 100 + i, j, nullptr, nullptr, nullptr};
            book.addOrder<false>(order);
        }
    }
    for (uint32_t i = 0; i < 1000; i++) {
        for (uint32_t j = 1; j <= 1000; j++) {
            Order* order = new Order{0, 0, 100 + i, j, nullptr, nullptr, nullptr};
            book.addOrder<true>(order);
        }
    }
    bool pass = book.numOrders() == 0;
    book.cleanup();
    return pass;
}

bool testBidFill() {
    Book book = Book(0);
    for (uint32_t i = 0; i < 1000; i++) {
        for (uint32_t j = 1; j <= 1000; j++) {
            Order* order = new Order{0, 0, 100 + i, j, nullptr, nullptr, nullptr};
            book.addOrder<false>(order);
        }
    }
    Order* order = new Order{1, 1, 1100, 500500000, nullptr, nullptr, nullptr};
    book.addOrder<true>(order);
    bool pass = book.numOrders() == 0;
    book.cleanup();
    return pass;
}


int main() {
    cout << "Hello World" << endl;
    int passed = 0;
    int total  = 0;

    total++;
    cout << "--------------------" << endl;
    if (testAddBid()) passed++;
    else cout << "FAIL - testAddBid" << endl;

    total++;
    cout << "--------------------" << endl;
    if (testAddAsk()) passed++;
    else cout << "FAIL - testAddAsk" << endl;

    total++;
    cout << "--------------------" << endl;
    if (testAddEqualBidAsk()) passed++;
    else cout << "FAIL - testAddEqualBidAsk" << endl;

    total++;
    cout << "--------------------" << endl;
    if (testSmallAskBigBid()) passed++;
    else cout << "FAIL - testSmallAskBigBid" << endl;

    total++;
    cout << "--------------------" << endl;
    if (testSmallBidBigAsk()) passed++;
    else cout << "FAIL - testSmallBidBigAsk" << endl;

    total++;
    cout << "--------------------" << endl;
    if (testNoFillBid()) passed++;
    else cout << "FAIL - testNoFillBid" << endl;

    total++;
    cout << "--------------------" << endl;
    if (testNoFillAsk()) passed++;
    else cout << "FAIL - testNoFillAsk" << endl;

    total++;
    cout << "--------------------" << endl;
    if (testFillBid()) passed++;
    else cout << "FAIL - testFillBid" << endl;

    total++;
    cout << "--------------------" << endl;
    if (testFillAsk()) passed++;
    else cout << "FAIL - testFillAsk" << endl;

    total++;
    cout << "--------------------" << endl;
    if (testAddBids()) passed++;
    else cout << "FAIL - testAddBids" << endl;

    total++;
    cout << "--------------------" << endl;
    if (testAddAsks()) passed++;
    else cout << "FAIL - testAddAsks" << endl;

    total++;
    cout << "--------------------" << endl;
    if (testPartialFill()) passed++;
    else cout << "FAIL - testPartialFill" << endl;

    total++;
    cout << "--------------------" << endl;
    if (testFullFill()) passed++;
    else cout << "FAIL - testFullFill" << endl;

    total++;
    cout << "--------------------" << endl;
    if (testCascadeFill()) passed++;
    else cout << "FAIL - testCascadeFill" << endl;

    total++;
    cout << "--------------------" << endl;
    if (testBidFill()) passed++;
    else cout << "FAIL - testBidFill" << endl;

    cout << "" << endl;

    cout << "\nPassed " << passed << " / " << total << " tests\n";
    cout << "World Hello" << endl;
    return 0;

}