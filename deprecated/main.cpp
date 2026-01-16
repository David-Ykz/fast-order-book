#include "book.h"

int main() {
    Book book = Book();
    book.addBid(1, 100);
    book.addBid(2, 150);
    book.addAsk(3, 100);
    cout << "bid: " << book.getBidPrice() << ", ask: " << book.getAskPrice() << endl;

    book.cleanup();
    return 0;
}